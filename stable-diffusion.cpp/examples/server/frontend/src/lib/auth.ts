import { reactive } from "vue";

import { type MessageKey, t } from "../i18n";
import en from "../i18n/locales/en";
import { setUnauthorizedHandler, withBase } from "./api";

// Sign-in state for servers started with --auth-file. With sign-in disabled (or an older server
// without the endpoint) `enabled` stays false and the UI behaves exactly as before.
export type AuthRole = "admin" | "user";

export interface AuthUser {
    name: string;
    role: AuthRole;
}

export interface ManagedUser extends AuthUser {
    created: number;
}

export const auth = reactive({
    checked: false,
    enabled: false,
    setupRequired: false,
    user: null as AuthUser | null,
    sessionExpired: false,
});

// Machine-readable error from the server (`code` matches the i18n keys "auth.error.<code>").
export class AuthError extends Error {
    constructor(public code: string, public status: number, public retryAfter = 0) {
        super(code);
    }
}

// Localised message for an error thrown by the calls below.
export function authErrorText(error: unknown): string {
    if (error instanceof AuthError) {
        const key = `auth.error.${error.code}` as MessageKey;
        if (key in en) {
            return t(key, { seconds: error.retryAfter });
        }
        return t("auth.error.generic", { message: error.code });
    }
    return t("auth.error.generic", { message: error instanceof Error ? error.message : String(error) });
}

async function call<T>(method: string, path: string, body?: unknown): Promise<T> {
    const response = await fetch(withBase("", path), {
        method,
        credentials: "same-origin",
        headers: body === undefined ? undefined : { "Content-Type": "application/json" },
        body: body === undefined ? undefined : JSON.stringify(body),
    });
    let payload: any = null;
    try {
        payload = await response.json();
    } catch {
        payload = null;
    }
    if (!response.ok) {
        throw new AuthError(String(payload?.error || `http_${response.status}`), response.status, Number(payload?.retry_after || 0));
    }
    return payload as T;
}

export async function loadAuthStatus(): Promise<void> {
    try {
        const status = await call<{ enabled: boolean; setup_required: boolean; user: AuthUser | null }>("GET", "/sdcpp/v1/auth/status");
        auth.enabled = Boolean(status.enabled);
        auth.setupRequired = Boolean(status.setup_required);
        auth.user = status.user;
    } catch {
        auth.enabled = false;
    } finally {
        auth.checked = true;
    }
}

function signedIn(user: AuthUser): void {
    auth.user = user;
    auth.setupRequired = false;
    auth.sessionExpired = false;
}

export async function signIn(username: string, password: string, remember: boolean): Promise<void> {
    signedIn((await call<{ user: AuthUser }>("POST", "/sdcpp/v1/auth/login", { username, password, remember })).user);
}

export async function createAdministrator(username: string, password: string): Promise<void> {
    signedIn((await call<{ user: AuthUser }>("POST", "/sdcpp/v1/auth/setup", { username, password })).user);
}

export async function signOut(): Promise<void> {
    try {
        await call("POST", "/sdcpp/v1/auth/logout");
    } finally {
        auth.user = null;
        auth.sessionExpired = false;
    }
}

export async function changePassword(current: string, password: string): Promise<void> {
    await call("POST", "/sdcpp/v1/auth/password", { current, password });
}

export async function listUsers(): Promise<ManagedUser[]> {
    return (await call<{ users: ManagedUser[] }>("GET", "/sdcpp/v1/auth/users")).users;
}

export async function createUser(username: string, password: string, role: AuthRole): Promise<void> {
    await call("POST", "/sdcpp/v1/auth/users", { username, password, role });
}

export async function resetUserPassword(name: string, password: string): Promise<void> {
    await call("POST", `/sdcpp/v1/auth/users/${encodeURIComponent(name)}/password`, { password });
}

export async function deleteUser(name: string): Promise<void> {
    await call("DELETE", `/sdcpp/v1/auth/users/${encodeURIComponent(name)}`);
}

setUnauthorizedHandler(() => {
    if (auth.enabled && auth.user) {
        auth.user = null;
        auth.sessionExpired = true;
    }
});
