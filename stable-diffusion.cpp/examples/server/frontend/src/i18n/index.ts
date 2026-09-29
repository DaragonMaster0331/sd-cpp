import { computed, watch } from "vue";

import { createStoredRef } from "../lib/settings";
import de from "./locales/de";
import en, { type MessageKey, type Messages } from "./locales/en";
import es from "./locales/es";
import fr from "./locales/fr";
import it from "./locales/it";
import ja from "./locales/ja";
import ko from "./locales/ko";
import ptBR from "./locales/pt-BR";
import ru from "./locales/ru";
import vi from "./locales/vi";
import zhCN from "./locales/zh-CN";
import zhTW from "./locales/zh-TW";

export type { MessageKey } from "./locales/en";

export interface LocaleInfo {
    code: string;
    // Native name, shown in the language picker regardless of the active locale.
    name: string;
    messages: Messages;
}

export const LOCALES: readonly LocaleInfo[] = [
    { code: "en", name: "English", messages: en },
    { code: "ko", name: "한@@", messages: ko },
    { code: "ja", name: "日本語", messages: ja },
    { code: "zh-CN", name: "简体中文", messages: zhCN },
    { code: "zh-TW", name: "繁體中文", messages: zhTW },
    { code: "es", name: "Español", messages: es },
    { code: "fr", name: "Français", messages: fr },
    { code: "de", name: "Deutsch", messages: de },
    { code: "it", name: "Italiano", messages: it },
    { code: "pt-BR", name: "Português (Brasil)", messages: ptBR },
    { code: "ru", name: "Русский", messages: ru },
    { code: "vi", name: "Tiếng Việt", messages: vi },
];

const DEFAULT_LOCALE = "en";

function matchLocale(raw: string): string | null {
    const tag = raw.trim().toLowerCase();
    if (!tag) {
        return null;
    }
    const exact = LOCALES.find((entry) => entry.code.toLowerCase() === tag);
    if (exact) {
        return exact.code;
    }
    if (tag.startsWith("zh")) {
        return /^zh-(tw|hk|mo|hant)/.test(tag) ? "zh-TW" : "zh-CN";
    }
    const primary = tag.split("-")[0];
    const byPrimary = LOCALES.find((entry) => entry.code.toLowerCase().split("-")[0] === primary);
    return byPrimary ? byPrimary.code : null;
}

function detectLocale(): string {
    const candidates = navigator.languages?.length ? navigator.languages : [navigator.language || ""];
    for (const candidate of candidates) {
        const match = matchLocale(candidate);
        if (match) {
            return match;
        }
    }
    return DEFAULT_LOCALE;
}

function normalizeLocale(value: unknown): string {
    return matchLocale(String(value || "")) || DEFAULT_LOCALE;
}

export const locale = createStoredRef<string>("sdcpp-webui-locale", detectLocale(), normalizeLocale);

const activeMessages = computed<Messages>(() => {
    return LOCALES.find((entry) => entry.code === locale.value)?.messages || en;
});

export function t(key: MessageKey, params?: Record<string, string | number>): string {
    const template = activeMessages.value[key] || en[key] || key;
    if (!params) {
        return template;
    }
    return template.replace(/\{(\w+)\}/g, (placeholder, name: string) => {
        return name in params ? String(params[name]) : placeholder;
    });
}

// Job states come from the server as raw identifiers; unknown ones are shown verbatim.
export function statusLabel(status: string): string {
    const key = `status.${status}` as MessageKey;
    return key in en ? t(key) : status;
}

export function formatNumber(value: number, fractionDigits: number): string {
    return value.toLocaleString(locale.value, {
        minimumFractionDigits: fractionDigits,
        maximumFractionDigits: fractionDigits,
    });
}

watch(locale, (code) => {
    document.documentElement.lang = code;
    document.title = t("app.title");
}, { immediate: true });
