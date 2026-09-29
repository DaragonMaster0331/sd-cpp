<script setup lang="ts">
import { computed, onMounted, reactive, ref } from "vue";

import { locale, t } from "../i18n";
import {
    auth,
    authErrorText,
    type AuthRole,
    changePassword,
    createUser,
    deleteUser,
    listUsers,
    type ManagedUser,
    resetUserPassword,
} from "../lib/auth";

// "Account" tab: own password for everyone, user management for administrators.
const isAdmin = computed(() => auth.user?.role === "admin");
const roleLabel = (role: string) => (role === "admin" ? t("auth.roleAdmin") : t("auth.roleUser"));

const own = reactive({ current: "", password: "", confirm: "", busy: false, message: "", error: false });

async function submitOwnPassword(): Promise<void> {
    own.message = "";
    if (own.password !== own.confirm) {
        own.message = t("auth.error.mismatch");
        own.error = true;
        return;
    }
    own.busy = true;
    try {
        await changePassword(own.current, own.password);
        own.message = t("auth.passwordChanged");
        own.error = false;
        own.current = own.password = own.confirm = "";
    } catch (err) {
        own.message = authErrorText(err);
        own.error = true;
    } finally {
        own.busy = false;
    }
}

const users = ref<ManagedUser[]>([]);
const notice = reactive({ message: "", error: false });
const added = reactive({ username: "", password: "", role: "user" as AuthRole, busy: false });
const reset = reactive({ name: "", password: "", busy: false });

function report(message: string, error = false): void {
    notice.message = message;
    notice.error = error;
}

async function refreshUsers(): Promise<void> {
    if (!isAdmin.value) {
        return;
    }
    try {
        users.value = await listUsers();
    } catch (err) {
        report(authErrorText(err), true);
    }
}

async function submitNewUser(): Promise<void> {
    added.busy = true;
    try {
        const name = added.username.trim();
        await createUser(name, added.password, added.role);
        report(t("auth.userCreated", { name }));
        added.username = added.password = "";
        added.role = "user";
        await refreshUsers();
    } catch (err) {
        report(authErrorText(err), true);
    } finally {
        added.busy = false;
    }
}

function startReset(name: string): void {
    reset.name = reset.name === name ? "" : name;
    reset.password = "";
}

async function submitReset(): Promise<void> {
    reset.busy = true;
    try {
        await resetUserPassword(reset.name, reset.password);
        report(t("auth.passwordReset", { name: reset.name }));
        reset.name = reset.password = "";
    } catch (err) {
        report(authErrorText(err), true);
    } finally {
        reset.busy = false;
    }
}

async function removeUser(name: string): Promise<void> {
    if (!window.confirm(t("auth.confirmDelete", { name }))) {
        return;
    }
    try {
        await deleteUser(name);
        report(t("auth.userDeleted", { name }));
        await refreshUsers();
    } catch (err) {
        report(authErrorText(err), true);
    }
}

function formatDate(seconds: number): string {
    return seconds ? new Date(seconds * 1000).toLocaleDateString(locale.value) : "-";
}

onMounted(refreshUsers);
</script>

<template>
    <div class="account">
        <section class="panel account__card">
            <div class="account__head">
                <div>
                    <h2 class="panel-title">{{ t("auth.account") }}</h2>
                    <p class="account__who">
                        {{ t("auth.signedInAs", { name: auth.user?.name || "" }) }}
                        <span class="account__role">{{ roleLabel(auth.user?.role || "user") }}</span>
                    </p>
                </div>
            </div>

            <form class="account__form" @submit.prevent="submitOwnPassword">
                <h3 class="account__subtitle">{{ t("auth.changePassword") }}</h3>
                <input class="account__hidden-user" :value="auth.user?.name" autocomplete="username" readonly tabindex="-1" aria-hidden="true" />
                <div class="fields fields--three">
                    <div class="field">
                        <label>{{ t("auth.currentPassword") }}</label>
                        <input v-model="own.current" type="password" autocomplete="current-password" required />
                    </div>
                    <div class="field">
                        <label>{{ t("auth.newPassword") }}</label>
                        <input v-model="own.password" type="password" autocomplete="new-password" maxlength="256" required />
                    </div>
                    <div class="field">
                        <label>{{ t("auth.confirmPassword") }}</label>
                        <input v-model="own.confirm" type="password" autocomplete="new-password" maxlength="256" required />
                    </div>
                </div>
                <p class="hint">{{ t("auth.passwordHint") }}</p>
                <div v-if="own.message" class="status-message" :class="own.error ? 'status-message--error' : 'status-message--success'">{{ own.message }}</div>
                <div><button class="btn" type="submit" :disabled="own.busy">{{ t("auth.changePassword") }}</button></div>
            </form>
        </section>

        <section v-if="isAdmin" class="panel account__card">
            <h2 class="panel-title">{{ t("auth.users") }}</h2>
            <div class="user-table" role="table">
                <div class="user-table__row user-table__row--head" role="row">
                    <div role="columnheader">{{ t("auth.username") }}</div>
                    <div role="columnheader">{{ t("auth.role") }}</div>
                    <div role="columnheader">{{ t("auth.created") }}</div>
                    <div role="columnheader"></div>
                </div>
                <template v-for="user in users" :key="user.name">
                    <div class="user-table__row" role="row">
                        <div class="user-table__name" role="cell">
                            {{ user.name }}
                            <span v-if="user.name === auth.user?.name" class="account__role">{{ t("auth.you") }}</span>
                        </div>
                        <div role="cell">{{ roleLabel(user.role) }}</div>
                        <div role="cell" class="mono">{{ formatDate(user.created) }}</div>
                        <div role="cell" class="user-table__actions">
                            <button class="btn-ghost" type="button" @click="startReset(user.name)">{{ t("auth.resetPassword") }}</button>
                            <button v-if="user.name !== auth.user?.name" class="btn-danger" type="button" @click="removeUser(user.name)">{{ t("auth.deleteUser") }}</button>
                        </div>
                    </div>
                    <form v-if="reset.name === user.name" class="user-table__reset" @submit.prevent="submitReset">
                        <input v-model="reset.password" type="password" autocomplete="new-password" maxlength="256" :placeholder="t('auth.newPassword')" required />
                        <button class="btn" type="submit" :disabled="reset.busy">{{ t("auth.resetPassword") }}</button>
                        <button class="btn-ghost" type="button" @click="startReset(user.name)">{{ t("output.cancel") }}</button>
                    </form>
                </template>
            </div>

            <form class="account__form" @submit.prevent="submitNewUser">
                <h3 class="account__subtitle">{{ t("auth.addUser") }}</h3>
                <div class="fields fields--three">
                    <div class="field">
                        <label>{{ t("auth.username") }}</label>
                        <input v-model="added.username" autocomplete="off" maxlength="32" required />
                    </div>
                    <div class="field">
                        <label>{{ t("auth.password") }}</label>
                        <input v-model="added.password" type="password" autocomplete="new-password" maxlength="256" required />
                    </div>
                    <div class="field">
                        <label>{{ t("auth.role") }}</label>
                        <select v-model="added.role">
                            <option value="user">{{ t("auth.roleUser") }}</option>
                            <option value="admin">{{ t("auth.roleAdmin") }}</option>
                        </select>
                    </div>
                </div>
                <p class="hint">{{ t("auth.usernameHint") }} {{ t("auth.passwordHint") }}</p>
                <div><button class="btn" type="submit" :disabled="added.busy">{{ t("auth.addUser") }}</button></div>
            </form>

            <div v-if="notice.message" class="status-message" :class="notice.error ? 'status-message--error' : 'status-message--success'" role="status">{{ notice.message }}</div>
        </section>
    </div>
</template>
