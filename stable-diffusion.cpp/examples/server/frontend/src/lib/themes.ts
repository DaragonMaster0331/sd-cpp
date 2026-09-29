import { watch } from "vue";

import { createStoredRef } from "./settings";

export interface ThemeInfo {
    id: string;
    // Proper names of the original themes; not translated.
    name: string;
}

// Palettes live in styles.css under [data-theme="<id>"]. Dracula is the default.
export const THEMES: readonly ThemeInfo[] = [
    { id: "dracula", name: "Dracula" },
    { id: "one-dark", name: "One Dark Pro" },
    { id: "github-dark", name: "GitHub Dark" },
    { id: "tokyo-night", name: "Tokyo Night" },
    { id: "catppuccin-mocha", name: "Catppuccin Mocha" },
    { id: "nord", name: "Nord" },
    { id: "monokai", name: "Monokai" },
    { id: "vscode-dark", name: "VS Code Dark Modern" },
];

export const DEFAULT_THEME = "dracula";

function normalizeTheme(value: unknown): string {
    const id = String(value || "");
    return THEMES.some((theme) => theme.id === id) ? id : DEFAULT_THEME;
}

export const theme = createStoredRef<string>("sdcpp-webui-theme", DEFAULT_THEME, normalizeTheme);

watch(theme, (id) => {
    document.documentElement.dataset.theme = id;
}, { immediate: true });
