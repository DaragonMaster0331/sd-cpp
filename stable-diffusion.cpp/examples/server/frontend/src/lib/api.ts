import type { Capabilities, Job } from "./types";

export function withBase(baseUrl: string, path: string): string {
    const base = String(baseUrl || window.location.pathname).trim().replace(/\/+$/, "");
    return `${base}${path}`;
}

// Called when the server answers 401 (sign-in enabled and the session is missing or has ended).
let unauthorizedHandler: (() => void) | null = null;

export function setUnauthorizedHandler(handler: () => void): void {
    unauthorizedHandler = handler;
}

async function fetchJson<T>(url: string, init?: RequestInit): Promise<T> {
    const response = await fetch(url, { credentials: "same-origin", ...init });
    if (response.status === 401) {
        unauthorizedHandler?.();
    }
    let payload: any = null;
    try {
        payload = await response.json();
    } catch {
        payload = null;
    }
    if (!response.ok) {
        throw new Error(
            (payload && (payload.error || payload.message)) || `HTTP ${response.status}`
        );
    }
    return payload as T;
}

export function getCapabilities(baseUrl: string): Promise<Capabilities> {
    return fetchJson<Capabilities>(withBase(baseUrl, "/sdcpp/v1/capabilities"));
}

export function submitImageJob(baseUrl: string, body: unknown): Promise<Job> {
    return fetchJson<Job>(withBase(baseUrl, "/sdcpp/v1/img_gen"), {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(body),
    });
}

export function submitVideoJob(baseUrl: string, body: unknown): Promise<Job> {
    return fetchJson<Job>(withBase(baseUrl, "/sdcpp/v1/vid_gen"), {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(body),
    });
}

export function getJob(baseUrl: string, id: string): Promise<Job> {
    return fetchJson<Job>(withBase(baseUrl, `/sdcpp/v1/jobs/${id}`));
}

export function cancelJob(baseUrl: string, id: string): Promise<Job> {
    return fetchJson<Job>(withBase(baseUrl, `/sdcpp/v1/jobs/${id}/cancel`), {
        method: "POST",
    });
}
