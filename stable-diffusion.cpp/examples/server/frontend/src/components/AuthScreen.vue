<script setup lang="ts">
import { computed, ref } from "vue";

import { LOCALES, locale, t } from "../i18n";
import { auth, authErrorText, createAdministrator, signIn } from "../lib/auth";
import { THEMES, theme } from "../lib/themes";

// Sign-in screen; on a server without users it becomes the first-start administrator setup.
const selectedLocale = locale;
const selectedTheme = theme;
const setup = computed(() => auth.setupRequired);

const username = ref("");
const password = ref("");
const confirmPassword = ref("");
const remember = ref(false);
const busy = ref(false);
const error = ref("");

async function submit(): Promise<void> {
    error.value = "";
    if (setup.value && password.value !== confirmPassword.value) {
        error.value = t("auth.error.mismatch");
        return;
    }
    busy.value = true;
    try {
        if (setup.value) {
            await createAdministrator(username.value.trim(), password.value);
        } else {
            await signIn(username.value.trim(), password.value, remember.value);
        }
    } catch (err) {
        error.value = authErrorText(err);
    } finally {
        password.value = "";
        confirmPassword.value = "";
        busy.value = false;
    }
}
</script>

<template>
    <div class="auth-screen">
        <div class="auth-screen__tools header-tools">
            <label class="picker" :title="t('settings.language')">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true">
                    <circle cx="12" cy="12" r="10" />
                    <path d="M2 12h20" />
                    <path d="M12 2a15.3 15.3 0 0 1 4 10 15.3 15.3 0 0 1-4 10 15.3 15.3 0 0 1-4-10 15.3 15.3 0 0 1 4-10z" />
                </svg>
                <select v-model="selectedLocale" :aria-label="t('settings.language')">
                    <option v-for="entry in LOCALES" :key="entry.code" :value="entry.code">{{ entry.name }}</option>
                </select>
            </label>
            <label class="picker" :title="t('settings.theme')">
                <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true">
                    <path d="M12 3a6 6 0 0 0 9 9 9 9 0 1 1-9-9Z" />
                </svg>
                <select v-model="selectedTheme" :aria-label="t('settings.theme')">
                    <option v-for="entry in THEMES" :key="entry.id" :value="entry.id">{{ entry.name }}</option>
                </select>
            </label>
        </div>

        <form class="auth-card panel" @submit.prevent="submit">
            <div class="auth-card__brand">
                <span class="brand-mark" aria-hidden="true">SD</span>
                <span>stable-diffusion.cpp</span>
            </div>
            <h1 class="auth-card__title">{{ setup ? t("auth.setupTitle") : t("auth.signInTitle") }}</h1>
            <p class="auth-card__subtitle">{{ setup ? t("auth.setupSubtitle") : t("auth.signInSubtitle") }}</p>

            <div v-if="auth.sessionExpired && !setup" class="status-message status-message--error">{{ t("auth.error.session") }}</div>

            <div class="field">
                <label for="auth-username">{{ t("auth.username") }}</label>
                <input id="auth-username" v-model="username" autocomplete="username" maxlength="32" required autofocus />
                <span v-if="setup" class="hint">{{ t("auth.usernameHint") }}</span>
            </div>
            <div class="field">
                <label for="auth-password">{{ t("auth.password") }}</label>
                <input id="auth-password" v-model="password" type="password" :autocomplete="setup ? 'new-password' : 'current-password'" maxlength="256" required />
                <span v-if="setup" class="hint">{{ t("auth.passwordHint") }}</span>
            </div>
            <div v-if="setup" class="field">
                <label for="auth-confirm">{{ t("auth.confirmPassword") }}</label>
                <input id="auth-confirm" v-model="confirmPassword" type="password" autocomplete="new-password" maxlength="256" required />
            </div>
            <label v-else class="checkbox">
                <input v-model="remember" type="checkbox" />
                <span>{{ t("auth.remember") }}</span>
            </label>

            <div v-if="error" class="status-message status-message--error" role="alert">{{ error }}</div>

            <button class="btn auth-card__submit" type="submit" :disabled="busy">
                {{ setup ? t("auth.createAdmin") : t("auth.signIn") }}
            </button>
        </form>
    </div>
</template>
