import { reactive, watch } from "vue";

import { locale } from "../i18n";
import { theme } from "./themes";

// Bridge to the SD-Studio desktop host (WebView2). In a normal browser `present` stays false and
// nothing here has any effect.
export type EngineMode = "auto" | "cuda" | "cpu";

export interface EngineInfo {
    mode: EngineMode;
    active: "cuda" | "cpu";
    device: string;
    cuda: boolean;        // GPU engine usable (engine files + NVIDIA driver)
    driver: boolean;      // NVIDIA driver present
    cudaEngine: boolean;  // GPU engine files present
}

interface WebViewBridge {
    postMessage(message: unknown): void;
    addEventListener(type: "message", listener: (event: MessageEvent) => void): void;
}

const bridge: WebViewBridge | undefined = (window as any).chrome?.webview;

export const desktop = reactive({
    present: Boolean(bridge),
    engine: null as EngineInfo | null,
    switching: false,
});

if (bridge) {
    bridge.addEventListener("message", (event: MessageEvent) => {
        const data = event.data;
        if (data && data.type === "engine") {
            desktop.engine = {
                mode: data.mode,
                active: data.active,
                device: String(data.device || ""),
                cuda: Boolean(data.cuda),
                driver: Boolean(data.driver),
                cudaEngine: Boolean(data.cudaEngine),
            };
            desktop.switching = false;
        }
    });
    bridge.postMessage("getEngine");
    // The host uses these for its own loading/error pages and window colours.
    watch([locale, theme], ([code, id]) => bridge.postMessage(`prefs:${code}|${id}`), { immediate: true });
}

// Switching to an engine that is not the one running restarts it: the host shows its loading page
// and reloads the UI when the new engine is ready.
export function setEngineMode(mode: EngineMode): void {
    if (!bridge || !desktop.engine || desktop.engine.mode === mode) {
        return;
    }
    desktop.switching = true;
    bridge.postMessage(`setEngine:${mode}`);
}
