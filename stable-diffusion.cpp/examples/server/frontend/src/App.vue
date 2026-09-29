<script setup lang="ts">
import { computed, onBeforeUnmount, onMounted, reactive, ref, watch } from "vue";

import AccountPanel from "./components/AccountPanel.vue";
import AuthScreen from "./components/AuthScreen.vue";
import CollapsibleSection from "./components/CollapsibleSection.vue";
import ImageDropzone from "./components/ImageDropzone.vue";
import StatusChip from "./components/StatusChip.vue";
import { formatNumber, LOCALES, locale, type MessageKey, statusLabel, t } from "./i18n";
import { cancelJob, getCapabilities, getJob, submitImageJob, submitVideoJob } from "./lib/api";
import { auth, loadAuthStatus, signOut } from "./lib/auth";
import { desktop, type EngineMode, setEngineMode } from "./lib/desktop";
import { buildRequestBodyForMode, CACHE_MODES, createBlankForm, formFromCapabilities } from "./lib/form";
import { IMAGE_INPUTS, VIDEO_IMAGE_INPUTS } from "./lib/image-inputs";
import { assignImageEntries, clearImageEntries, filesToImageEntries, removeImageEntry } from "./lib/images";
import { createStoredRef, normalizePollIntervalMs } from "./lib/settings";
import { THEMES, theme } from "./lib/themes";
import type {
    Capabilities,
    GenerationForm,
    GenerationMode,
    ImageEntry,
    ImageTarget,
    Job,
    SampleParams,
} from "./lib/types";

const baseUrl = createStoredRef<string>("sdcpp-webui-base-url", "", (value: unknown) => String(value || ""));
const pollIntervalMs = createStoredRef<number>("sdcpp-webui-poll-interval-ms", 100, normalizePollIntervalMs);
// Local aliases so v-model binds to these module-level refs the same way it does for baseUrl.
const selectedLocale = locale;
const selectedTheme = theme;
const activeTab = ref<"image" | "video" | "settings" | "account">("image");
const selectedGenerationTab = ref<"image" | "video">("image");
const generationMode = computed<GenerationMode>(() => selectedGenerationTab.value === "video" ? "video" : "image");
const lightboxOpen = ref(false);
const lightboxImageSrc = ref("");
const lightboxImageAlt = ref("");
const sectionState = reactive<Record<string, boolean>>({
    sample: true,
    sampleAdvanced: false,
    guidance: true,
    guidanceAdvanced: false,
    highNoise: false,
    highNoiseSample: true,
    highNoiseGuidance: true,
    conditioning: false,
    auxiliaryImages: false,
    lora: false,
    vaeTiling: false,
    cache: false,
});
const loadingCapabilities = ref(false);
const capabilitiesError = ref("");
const serviceOnline = ref(false);
const capabilities = ref<Capabilities | null>(null);
const currentJob = ref<Job | null>(null);
const selectedOutputIndex = ref(0);
// UI-originated messages are kept as keys so they re-render when the language changes;
// server errors arrive as plain text and are shown verbatim.
const statusMessageKey = ref<MessageKey | null>(null);
const statusMessageText = ref("");
const statusMessage = computed(() => statusMessageKey.value ? t(statusMessageKey.value) : statusMessageText.value);
const statusTone = ref("");
const form = reactive<GenerationForm>(createBlankForm());

let pollTimer = 0;
let elapsedTimer = 0;
const nowSeconds = ref(Date.now() / 1000);

// Engine picker, only inside the SD-Studio desktop app.
const engineChipLabel = computed(() => {
    const engine = desktop.engine;
    return engine ? `${engine.active === "cuda" ? "GPU" : "CPU"} · ${engine.device}` : "";
});
const engineTitle = computed(() => {
    const engine = desktop.engine;
    if (!engine || engine.cuda) {
        return t("engine.label");
    }
    return `${t("engine.label")}: ${engine.cudaEngine ? t("engine.noDriver") : t("engine.noEngine")}`;
});
const gpuOptionLabel = computed(() => {
    return desktop.engine?.cuda ? t("engine.gpu") : `${t("engine.gpu")} — ${t("engine.unavailable")}`;
});

function onEngineChange(event: Event): void {
    setEngineMode((event.target as HTMLSelectElement).value as EngineMode);
}

const modelName = computed(() => {
    const model = capabilities.value?.model;
    return model?.stem || model?.name || t("app.noModelInfo");
});

const supportedModes = computed(() => capabilities.value?.supported_modes || []);
const supportsImageMode = computed(() => {
    return !supportedModes.value.length || supportedModes.value.includes("img_gen");
});
const supportsVideoMode = computed(() => {
    return !supportedModes.value.length || supportedModes.value.includes("vid_gen");
});
const selectedModeKey = computed<"img_gen" | "vid_gen">(() => generationMode.value === "video" ? "vid_gen" : "img_gen");
const currentJobModeKey = computed<"img_gen" | "vid_gen">(() => currentJob.value?.kind || selectedModeKey.value);
const selectedModeFeatures = computed(() => {
    return capabilities.value?.features_by_mode?.[selectedModeKey.value] || {};
});
const currentJobFeatures = computed(() => {
    return capabilities.value?.features_by_mode?.[currentJobModeKey.value] || selectedModeFeatures.value;
});
const imageOutputFormats = computed(() => {
    return capabilities.value?.output_formats_by_mode?.img_gen || ["png", "jpeg"];
});
const videoOutputFormats = computed(() => {
    return capabilities.value?.output_formats_by_mode?.vid_gen || ["webm", "avi"];
});
const outputFormats = computed(() => generationMode.value === "video" ? videoOutputFormats.value : imageOutputFormats.value);
const samplers = computed(() => capabilities.value?.samplers || ["default"]);
const schedulers = computed(() => capabilities.value?.schedulers || ["default"]);
const availableLoras = computed(() => capabilities.value?.loras || []);
const currentImageInputs = computed(() => generationMode.value === "video" ? VIDEO_IMAGE_INPUTS : IMAGE_INPUTS);
const gridImageInputs = computed(() => currentImageInputs.value.filter((input) => input.layout === "grid"));
const fullImageInputs = computed(() => currentImageInputs.value.filter((input) => input.layout === "full"));
const queueLimit = computed(() => capabilities.value?.limits?.max_queue_size ?? t("chip.unknown"));
const canCancelQueued = computed(() => Boolean(currentJobFeatures.value.cancel_queued));
const canCancelGenerating = computed(() => Boolean(currentJobFeatures.value.cancel_generating));

const currentStatus = computed(() => currentJob.value?.status || "idle");
const currentJobKind = computed(() => currentJob.value?.kind || null);
const currentImages = computed(() => currentJobKind.value === "img_gen" ? currentJob.value?.result?.images || [] : []);
const selectedImage = computed(() => {
    if (!currentImages.value.length) {
        return null;
    }
    const index = Math.min(selectedOutputIndex.value, currentImages.value.length - 1);
    const image = currentImages.value[index];
    const format = currentJob.value?.result?.output_format || form.output_format || "png";
    return `data:image/${format};base64,${image.b64_json}`;
});
const videoMimeType = computed(() => currentJobKind.value === "vid_gen" ? currentJob.value?.result?.mime_type || "" : "");
const videoFrameCount = computed(() => currentJobKind.value === "vid_gen" ? currentJob.value?.result?.frame_count || 0 : 0);
const videoFps = computed(() => currentJobKind.value === "vid_gen" ? currentJob.value?.result?.fps || 0 : 0);
const videoPreviewSrc = computed(() => {
    if (currentJobKind.value !== "vid_gen" || !currentJob.value?.result?.b64_json) {
        return null;
    }
    if (currentJob.value?.result?.output_format === "avi") {
        return null;
    }
    if (!videoMimeType.value.startsWith("video/")) {
        return null;
    }
    return `data:${videoMimeType.value};base64,${currentJob.value.result.b64_json}`;
});
const animatedVideoImageSrc = computed(() => {
    if (currentJobKind.value !== "vid_gen" || !currentJob.value?.result?.b64_json) {
        return null;
    }
    if (videoMimeType.value !== "image/webp") {
        return null;
    }
    return `data:image/webp;base64,${currentJob.value.result.b64_json}`;
});
const previewImageSrc = computed(() => animatedVideoImageSrc.value || selectedImage.value);
const downloadableSrc = computed(() => {
    const result = currentJob.value?.result;
    if (currentJobKind.value === "vid_gen" && result?.b64_json && videoMimeType.value) {
        return `data:${videoMimeType.value};base64,${result.b64_json}`;
    }
    return selectedImage.value;
});

const canCancelCurrentJob = computed(() => {
    if (!currentJob.value) {
        return false;
    }
    if (currentStatus.value === "queued") {
        return canCancelQueued.value;
    }
    if (currentStatus.value === "generating") {
        return canCancelGenerating.value;
    }
    return false;
});

const isJobRunning = computed(() => {
    return currentStatus.value === "queued" || currentStatus.value === "generating";
});

function optionLabel(value: string | undefined): string {
    return !value || value === "default" ? t("option.default") : value;
}

function buildSampleSummary(sample: SampleParams): string {
    const scheduler = optionLabel(sample.scheduler);
    const method = optionLabel(sample.sample_method);
    const steps = sample.sample_steps || 0;
    const flowShift = sample.flow_shift === "" || sample.flow_shift == null
        ? t("summary.flowAuto")
        : t("summary.flow", { value: sample.flow_shift });
    return `${scheduler} · ${flowShift} · ${method} · ${t("summary.steps", { count: steps })}`;
}

function buildGuidanceSummary(sample: SampleParams): string {
    const cfg = sample.guidance.txt_cfg;
    const distilled = sample.guidance.distilled_guidance;
    return `${t("summary.cfg", { value: cfg })} · ${t("summary.distilled", { value: distilled })}`;
}

const sampleSummary = computed(() => buildSampleSummary(form.sample_params));
const guidanceSummary = computed(() => buildGuidanceSummary(form.sample_params));
const highNoiseSummary = computed(() => {
    return `moe ${formatSummaryNumber(form.moe_boundary)} · ${buildSampleSummary(form.high_noise_sample_params)} · ${buildGuidanceSummary(form.high_noise_sample_params)}`;
});
const loraSummary = computed(() => {
    if (!form.lora.length) {
        return t("summary.noLora");
    }
    return t("summary.loraConfigured", { count: form.lora.length });
});
const imageInputsSummary = computed(() => {
    const parts: string[] = [];
    if (form.init_image) parts.push(generationMode.value === "video" ? t("summary.start") : t("summary.init"));
    if (generationMode.value === "image") {
        if (form.mask_image) parts.push(t("summary.mask"));
        if (form.control_image) parts.push(t("summary.control"));
        if (form.ref_images.length) parts.push(t("summary.refs", { count: form.ref_images.length }));
    } else {
        if (form.end_image) parts.push(t("summary.end"));
        if (form.control_frames.length) parts.push(t("summary.frames", { count: form.control_frames.length }));
    }
    return parts.length ? parts.join(" · ") : t("summary.noImages");
});
const vaeTilingSummary = computed(() => {
    if (!form.vae_tiling_params.enabled) {
        return t("summary.disabled");
    }
    return `${form.vae_tiling_params.tile_size_w}×${form.vae_tiling_params.tile_size_h} · ${t("summary.overlap", { value: form.vae_tiling_params.target_overlap })}`;
});
const cacheSummary = computed(() => {
    const mode = form.cache.mode || "disabled";
    if (mode === "disabled") {
        return t("summary.disabled");
    }
    const option = String(form.cache.option || "").trim();
    return option ? `${mode} · ${option}` : mode;
});
const conditioningSummary = computed(() => {
    const clipSkip = formatSummaryNumber(form.clip_skip, 0);
    const strength = formatSummaryNumber(form.strength);
    if (generationMode.value === "video") {
        return `${t("field.clipSkip")} ${clipSkip} · ${t("field.strength")} ${strength} · ${t("field.vaceStrength")} ${formatSummaryNumber(form.vace_strength)}`;
    }
    const controlStrength = formatSummaryNumber(form.control_strength);
    return `${t("field.clipSkip")} ${clipSkip} · ${t("field.strength")} ${strength} · ${t("field.controlStrength")} ${controlStrength}`;
});

function defaultOutputFormatForMode(mode: GenerationMode): string {
    if (mode === "video") {
        return videoOutputFormats.value[0] || "webm";
    }
    return imageOutputFormats.value[0] || "png";
}

function ensureOutputFormatForMode(mode = generationMode.value): void {
    const validFormats = mode === "video" ? videoOutputFormats.value : imageOutputFormats.value;
    if (!validFormats.length) {
        return;
    }
    if (!validFormats.includes(form.output_format)) {
        form.output_format = defaultOutputFormatForMode(mode);
    }
}

function setMessage(message: string, tone = ""): void {
    statusMessageKey.value = null;
    statusMessageText.value = message;
    statusTone.value = tone;
}

function setMessageKey(key: MessageKey, tone = ""): void {
    statusMessageKey.value = key;
    statusMessageText.value = "";
    statusTone.value = tone;
}

function clearMessage(): void {
    setMessage("", "");
}

function deepAssign(target: Record<string, any>, ...sources: Record<string, any>[]): Record<string, any> {
    for (const source of sources) {
        if (!source) continue;
        for (const key of Object.keys(source)) {
            const sv = source[key];
            if (sv !== null && typeof sv === "object" && !Array.isArray(sv) &&
                target[key] !== null && typeof target[key] === "object" && !Array.isArray(target[key])) {
                deepAssign(target[key], sv);
            } else {
                target[key] = sv;
            }
        }
    }
    return target;
}

function applyForm(nextForm: GenerationForm): void {
    deepAssign(form, createBlankForm(), nextForm);
}

async function refreshCapabilities(): Promise<void> {
    loadingCapabilities.value = true;
    capabilitiesError.value = "";
    try {
        const response = await getCapabilities(baseUrl.value);
        capabilities.value = response;
        serviceOnline.value = true;
        applyForm(formFromCapabilities(response));
        if (selectedGenerationTab.value === "image" && !supportsImageMode.value && supportsVideoMode.value) {
            selectedGenerationTab.value = "video";
        } else if (selectedGenerationTab.value === "video" && !supportsVideoMode.value && supportsImageMode.value) {
            selectedGenerationTab.value = "image";
        }
        if (activeTab.value === "image" && !supportsImageMode.value && supportsVideoMode.value) {
            activeTab.value = "video";
        } else if (activeTab.value === "video" && !supportsVideoMode.value && supportsImageMode.value) {
            activeTab.value = "image";
        }
        ensureOutputFormatForMode();
        clearMessage();
    } catch (error) {
        capabilitiesError.value = error instanceof Error ? error.message : String(error);
        serviceOnline.value = false;
        setMessage(capabilitiesError.value, "error");
    } finally {
        loadingCapabilities.value = false;
    }
}

function toggleSection(section: string): void {
    sectionState[section] = !sectionState[section];
}

function selectGenerationMode(mode: GenerationMode): void {
    if (mode === "image" && !supportsImageMode.value) {
        setMessageKey("msg.onlyVideo", "error");
        return;
    }
    if (mode === "video" && !supportsVideoMode.value) {
        setMessageKey("msg.onlyImage", "error");
        return;
    }
    selectedGenerationTab.value = mode;
    activeTab.value = mode;
    ensureOutputFormatForMode(mode);
}

function stopPolling(): void {
    if (pollTimer) {
        window.clearTimeout(pollTimer);
        pollTimer = 0;
    }
}

function stopElapsedTimer(): void {
    if (elapsedTimer) {
        window.clearInterval(elapsedTimer);
        elapsedTimer = 0;
    }
}

function startElapsedTimer(): void {
    stopElapsedTimer();
    nowSeconds.value = Date.now() / 1000;
    elapsedTimer = window.setInterval(() => {
        nowSeconds.value = Date.now() / 1000;
    }, 100);
}

async function pollJob(id: string): Promise<void> {
    stopPolling();
    try {
        currentJob.value = await getJob(baseUrl.value, id);
        serviceOnline.value = true;
        if (currentStatus.value === "queued" || currentStatus.value === "generating") {
            pollTimer = window.setTimeout(() => pollJob(id), normalizePollIntervalMs(pollIntervalMs.value));
            clearMessage();
            return;
        }
        stopElapsedTimer();
        if (currentStatus.value === "completed") {
            setMessageKey(currentJob.value?.kind === "vid_gen" ? "msg.videoCompleted" : "msg.imageCompleted", "success");
            return;
        }
        if (currentStatus.value === "cancelled") {
            setMessageKey("msg.cancelled", "error");
            return;
        }
        if (currentStatus.value === "failed") {
            const serverMessage = currentJob.value?.error?.message;
            if (serverMessage) {
                setMessage(serverMessage, "error");
            } else {
                setMessageKey("msg.failed", "error");
            }
        }
    } catch (error) {
        stopElapsedTimer();
        serviceOnline.value = false;
        setMessage(error instanceof Error ? error.message : String(error), "error");
    }
}

async function generate(): Promise<void> {
    try {
        const request = buildRequestBodyForMode(generationMode.value, form);
        clearMessage();
        selectedOutputIndex.value = 0;
        startElapsedTimer();
        currentJob.value = generationMode.value === "video"
            ? await submitVideoJob(baseUrl.value, request)
            : await submitImageJob(baseUrl.value, request);
        await pollJob(currentJob.value.id);
    } catch (error) {
        stopElapsedTimer();
        setMessage(error instanceof Error ? error.message : String(error), "error");
    }
}

async function cancelCurrentJob(): Promise<void> {
    if (!currentJob.value?.id) {
        return;
    }
    try {
        currentJob.value = await cancelJob(baseUrl.value, currentJob.value.id);
        stopPolling();
        stopElapsedTimer();
        setMessageKey("msg.cancelled", "error");
    } catch (error) {
        setMessage(error instanceof Error ? error.message : String(error), "error");
    }
}

function addLora(): void {
    form.lora.push({
        path: availableLoras.value[0]?.path || "",
        multiplier: 1,
        is_high_noise: false,
    });
}

function removeLora(index: number): void {
    form.lora.splice(index, 1);
}

async function assignImages(target: ImageTarget, files: FileList): Promise<void> {
    const images = await filesToImageEntries(files);
    if (!images.length) {
        return;
    }
    assignImageEntries(form, target, images);
}

function clearImage(target: ImageTarget): void {
    clearImageEntries(form, target);
}

function getFormImage(target: ImageTarget): ImageEntry | null {
    if (target === "ref_images" || target === "control_frames") return null;
    return form[target];
}

function openImageEntry(image: ImageEntry | null): void {
    openLightbox(image?.dataUrl, image?.name);
}

function removeCollectionImage(target: "ref_images" | "control_frames", index: number): void {
    removeImageEntry(form, target, index);
}

function selectOutput(index: number): void {
    selectedOutputIndex.value = index;
}

function formatUnixTime(seconds: number | undefined): string {
    if (!seconds) {
        return t("metric.noJob");
    }
    return new Date(seconds * 1000).toLocaleString(locale.value);
}

function formatElapsed(started: number | undefined, completed: number | undefined): string {
    if (!started) {
        return t("metric.idle");
    }
    const end = completed || nowSeconds.value;
    const total = Math.max(0, end - started);
    if (total < 60) {
        return t("time.seconds", { s: formatNumber(total, 1) });
    }
    const minutes = Math.floor(total / 60);
    const seconds = total - minutes * 60;
    return t("time.minutesSeconds", { m: minutes, s: formatNumber(seconds, 1) });
}

function formatSummaryNumber(value: number, digits = 3): string {
    const numeric = Number(value ?? 0);
    if (!Number.isFinite(numeric)) {
        return "0";
    }
    return Number(numeric.toFixed(digits)).toString();
}

function downloadSelected(): void {
    if (!downloadableSrc.value) {
        return;
    }
    const link = document.createElement("a");
    link.href = downloadableSrc.value;
    link.download = `${currentJob.value?.id || "output"}.${currentJob.value?.result?.output_format || form.output_format}`;
    document.body.appendChild(link);
    link.click();
    link.remove();
}

function openLightbox(src: string | null | undefined = previewImageSrc.value, alt = t("output.expandedAlt")): void {
    if (!src) {
        return;
    }
    lightboxImageSrc.value = src;
    lightboxImageAlt.value = alt;
    lightboxOpen.value = true;
}

function closeLightbox(): void {
    lightboxOpen.value = false;
    lightboxImageSrc.value = "";
    lightboxImageAlt.value = "";
}

async function onPaste(event: ClipboardEvent): Promise<void> {
    if (!event.clipboardData?.files?.length) {
        return;
    }
    await assignImages("init_image", event.clipboardData.files);
    setMessageKey("msg.pasted", "success");
}

watch(generationMode, (mode) => {
    ensureOutputFormatForMode(mode);
});

watch(imageOutputFormats, () => {
    ensureOutputFormatForMode();
});

watch(videoOutputFormats, () => {
    ensureOutputFormatForMode();
});

// Sign-in: load the server state after sign-in; drop everything of the previous user on sign-out.
watch(() => auth.user?.name, (name, previous) => {
    if (name && name !== previous) {
        refreshCapabilities();
    }
    if (!name) {
        stopPolling();
        stopElapsedTimer();
        currentJob.value = null;
        clearMessage();
        if (activeTab.value === "account") {
            activeTab.value = selectedGenerationTab.value;
        }
    }
});

onMounted(async () => {
    window.addEventListener("paste", onPaste);
    await loadAuthStatus();
    if (!auth.enabled || auth.user) {
        refreshCapabilities();
    }
});

onBeforeUnmount(() => {
    stopPolling();
    stopElapsedTimer();
    window.removeEventListener("paste", onPaste);
});
</script>

<template>
    <div v-if="!auth.checked" class="boot" aria-hidden="true"></div>
    <AuthScreen v-else-if="auth.enabled && !auth.user" />
    <div v-else class="shell">
        <header class="page-header panel">
            <div class="page-header__top">
                <div class="page-header__copy">
                    <div class="breadcrumb">
                        <span class="breadcrumb__org"><span class="brand-mark" aria-hidden="true">SD</span>stable-diffusion.cpp</span>
                        <span class="breadcrumb__slash">/</span>
                        <span>{{ modelName }}</span>
                    </div>
                    <h1 class="page-title">{{ modelName }}</h1>
                    <p class="page-description">{{ t("app.description") }}</p>
                </div>
                <div class="page-header__aside">
                    <div class="header-tools">
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
                        <label v-if="desktop.engine" class="picker" :title="engineTitle">
                            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true">
                                <rect x="4" y="4" width="16" height="16" rx="2" />
                                <rect x="9" y="9" width="6" height="6" />
                                <path d="M15 2v2M15 20v2M2 15h2M2 9h2M20 15h2M20 9h2M9 2v2M9 20v2" />
                            </svg>
                            <select :value="desktop.engine.mode" :disabled="desktop.switching" :aria-label="t('engine.label')" @change="onEngineChange">
                                <option value="auto">{{ t("engine.auto") }}</option>
                                <option value="cuda" :disabled="!desktop.engine.cuda">{{ gpuOptionLabel }}</option>
                                <option value="cpu">{{ t("engine.cpu") }}</option>
                            </select>
                        </label>
                    </div>
                    <div class="page-header__meta">
                        <button v-if="auth.user" class="chip chip--user" type="button" :title="t('auth.account')" @click="activeTab = 'account'">
                            {{ auth.user.name }}
                        </button>
                        <StatusChip v-if="desktop.engine" :status="desktop.engine.active" :label="engineChipLabel" />
                        <StatusChip :status="serviceOnline ? 'online' : 'offline'" :label="serviceOnline ? t('chip.online') : t('chip.offline')" />
                        <StatusChip :label="t('chip.queue', { limit: queueLimit })" />
                        <StatusChip :status="currentStatus" :label="statusLabel(currentStatus)" />
                    </div>
                </div>
            </div>
            <div class="page-tabs">
                <div class="page-tabs__list">
                    <button class="page-tab" :class="{ 'page-tab--active': activeTab === 'image' }" type="button" @click="selectGenerationMode('image')" :disabled="!supportsImageMode">{{ t("tab.image") }}</button>
                    <button class="page-tab" :class="{ 'page-tab--active': activeTab === 'video' }" type="button" @click="selectGenerationMode('video')" :disabled="!supportsVideoMode">{{ t("tab.video") }}</button>
                    <button class="page-tab" :class="{ 'page-tab--active': activeTab === 'settings' }" type="button" @click="activeTab = 'settings'">{{ t("tab.settings") }}</button>
                    <button v-if="auth.user" class="page-tab" :class="{ 'page-tab--active': activeTab === 'account' }" type="button" @click="activeTab = 'account'">{{ t("auth.account") }}</button>
                </div>
                <div class="page-tabs__actions">
                    <button v-if="auth.user" class="btn-ghost" type="button" @click="signOut">{{ t("auth.signOut") }}</button>
                    <button class="btn-secondary" type="button" @click="refreshCapabilities" :disabled="loadingCapabilities">{{ loadingCapabilities ? t("action.refreshing") : t("action.refresh") }}</button>
                </div>
            </div>
            <div v-if="activeTab === 'settings'" class="settings">
                <div class="settings__grid">
                    <div class="field">
                        <label>{{ t("settings.language") }}</label>
                        <select v-model="selectedLocale">
                            <option v-for="entry in LOCALES" :key="entry.code" :value="entry.code">{{ entry.name }}</option>
                        </select>
                    </div>
                    <div class="field">
                        <label>{{ t("settings.theme") }}</label>
                        <select v-model="selectedTheme">
                            <option v-for="entry in THEMES" :key="entry.id" :value="entry.id">{{ entry.name }}</option>
                        </select>
                    </div>
                    <div v-if="desktop.engine" class="field">
                        <label>{{ t("engine.label") }}</label>
                        <select :value="desktop.engine.mode" :disabled="desktop.switching" :title="engineTitle" @change="onEngineChange">
                            <option value="auto">{{ t("engine.auto") }}</option>
                            <option value="cuda" :disabled="!desktop.engine.cuda">{{ gpuOptionLabel }}</option>
                            <option value="cpu">{{ t("engine.cpu") }}</option>
                        </select>
                    </div>
                    <div class="field">
                        <label>{{ t("settings.baseUrl") }}</label>
                        <input v-model="baseUrl" :placeholder="t('settings.baseUrlPlaceholder')" />
                    </div>
                    <div class="field">
                        <label>{{ t("settings.queueLimit") }}</label>
                        <input :value="queueLimit" readonly />
                    </div>
                    <div class="field">
                        <label>{{ t("settings.outputFormats", { mode: generationMode === "video" ? t("mode.video") : t("mode.image") }) }}</label>
                        <input :value="outputFormats.join(', ')" readonly />
                    </div>
                    <div class="field">
                        <label>{{ t("settings.outputFormat") }}</label>
                        <select v-model="form.output_format">
                            <option v-for="format in outputFormats" :key="format" :value="format">{{ format }}</option>
                        </select>
                    </div>
                    <div class="field">
                        <label>{{ t("settings.outputCompression") }}</label>
                        <input v-model.number="form.output_compression" type="number" min="0" max="100" />
                    </div>
                    <div class="field">
                        <label>{{ t("settings.pollInterval") }}</label>
                        <input v-model.number="pollIntervalMs" type="number" min="1" step="1" />
                    </div>
                </div>
                <div v-if="capabilitiesError" class="status-message status-message--error">{{ capabilitiesError }}</div>
            </div>
        </header>

        <AccountPanel v-if="activeTab === 'account' && auth.user" />
        <div v-else-if="activeTab === 'image' || activeTab === 'video'" class="layout">
            <section class="panel control-panel">
                <div class="panel-header">
                    <div>
                        <h2 class="panel-title">{{ t("panel.input") }}</h2>
                    </div>
                </div>

                <div class="prompt-card">
                    <div class="field--full">
                        <label>{{ t("field.prompt") }}</label>
                        <textarea v-model="form.prompt" :placeholder="generationMode === 'video' ? t('field.promptPlaceholderVideo') : t('field.promptPlaceholderImage')" />
                    </div>
                    <div class="field--full stack-top">
                        <label>{{ t("field.negativePrompt") }}</label>
                        <textarea v-model="form.negative_prompt" :placeholder="t('field.negativePromptPlaceholder')" />
                    </div>
                </div>

                <div class="fields stack-top">
                    <div class="field"><label>{{ t("field.width") }}</label><input v-model.number="form.width" type="number" min="64" /></div>
                    <div class="field"><label>{{ t("field.height") }}</label><input v-model.number="form.height" type="number" min="64" /></div>
                </div>

                <div v-if="generationMode === 'image'" class="fields stack-top">
                    <div class="field"><label>{{ t("field.batchCount") }}</label><input v-model.number="form.batch_count" type="number" min="1" /></div>
                    <div class="field"><label>{{ t("field.seed") }}</label><input v-model.number="form.seed" type="number" /></div>
                </div>
                <div v-else class="fields stack-top">
                    <div class="field"><label>{{ t("field.videoFrames") }}</label><input v-model.number="form.video_frames" type="number" min="1" /></div>
                    <div class="field"><label>{{ t("field.fps") }}</label><input v-model.number="form.fps" type="number" min="1" /></div>
                </div>
                <div v-if="generationMode === 'video'" class="field stack-top">
                    <label>{{ t("field.seed") }}</label>
                    <input v-model.number="form.seed" type="number" />
                </div>

                <CollapsibleSection class="stack-top" :eyebrow="t('section.sample')" :summary="sampleSummary" :open="sectionState.sample" variant="module" @toggle="toggleSection('sample')">
                    <div class="fields">
                        <div class="field">
                            <label>{{ t("field.scheduler") }}</label>
                            <select v-model="form.sample_params.scheduler">
                                <option value="default">{{ t("option.default") }}</option>
                                <option v-for="scheduler in schedulers" :key="scheduler" :value="scheduler">{{ scheduler }}</option>
                            </select>
                        </div>
                        <div class="field"><label>{{ t("field.flowShift") }}</label><input v-model="form.sample_params.flow_shift" type="number" step="0.01" :placeholder="t('placeholder.blankDefault')" /></div>
                        <div class="field">
                            <label>{{ t("field.method") }}</label>
                            <select v-model="form.sample_params.sample_method">
                                <option value="default">{{ t("option.default") }}</option>
                                <option v-for="sampler in samplers" :key="sampler" :value="sampler">{{ sampler }}</option>
                            </select>
                        </div>
                        <div class="field"><label>{{ t("field.steps") }}</label><input v-model.number="form.sample_params.sample_steps" type="number" /></div>
                    </div>
                    <div class="sample-panel__extras">
                        <button class="module-card__link" type="button" @click="toggleSection('sampleAdvanced')">
                            {{ sectionState.sampleAdvanced ? t("section.hideExtras") : t("section.showExtras") }}
                        </button>
                    </div>
                    <div v-if="sectionState.sampleAdvanced" class="fields">
                        <div class="field"><label>{{ t("field.eta") }}</label><input v-model="form.sample_params.eta" type="number" step="0.01" :placeholder="t('placeholder.blankDefault')" /></div>
                        <div class="field"><label>{{ t("field.shiftedTimestep") }}</label><input v-model.number="form.sample_params.shifted_timestep" type="number" /></div>
                    </div>
                </CollapsibleSection>

                <CollapsibleSection class="stack-top" :eyebrow="t('section.guidance')" :summary="guidanceSummary" :open="sectionState.guidance" variant="module" @toggle="toggleSection('guidance')">
                    <div class="fields">
                        <div class="field"><label>{{ t("field.cfgScale") }}</label><input v-model.number="form.sample_params.guidance.txt_cfg" type="number" step="0.1" /></div>
                        <div class="field"><label>{{ t("field.distilledGuidance") }}</label><input v-model.number="form.sample_params.guidance.distilled_guidance" type="number" step="0.1" /></div>
                    </div>
                    <div class="sample-panel__extras">
                        <button class="module-card__link" type="button" @click="toggleSection('guidanceAdvanced')">
                            {{ sectionState.guidanceAdvanced ? t("section.hideExtras") : t("section.showExtras") }}
                        </button>
                    </div>
                    <div v-if="sectionState.guidanceAdvanced" class="fields">
                        <div class="field"><label>{{ t("field.imageCfg") }}</label><input v-model="form.sample_params.guidance.img_cfg" type="number" step="0.1" :placeholder="t('placeholder.blankFollowCfg')" /></div>
                        <div class="field"><label>{{ t("field.slgLayers") }}</label><input v-model="form.sample_params.guidance.slg_layers" placeholder="7,8,9" /></div>
                        <div class="field"><label>{{ t("field.slgLayerStart") }}</label><input v-model.number="form.sample_params.guidance.layer_start" type="number" step="0.01" /></div>
                        <div class="field"><label>{{ t("field.slgLayerEnd") }}</label><input v-model.number="form.sample_params.guidance.layer_end" type="number" step="0.01" /></div>
                        <div class="field"><label>{{ t("field.slgScale") }}</label><input v-model.number="form.sample_params.guidance.scale" type="number" step="0.01" /></div>
                    </div>
                </CollapsibleSection>

                <CollapsibleSection v-if="generationMode === 'video'" class="stack-top" :eyebrow="t('section.highNoise')" :summary="highNoiseSummary" :open="sectionState.highNoise" variant="module" @toggle="toggleSection('highNoise')">
                    <div class="field">
                        <label>{{ t("field.moeBoundary") }}</label>
                        <input v-model.number="form.moe_boundary" type="number" step="0.001" />
                    </div>
                    <CollapsibleSection :eyebrow="t('section.sample')" :summary="buildSampleSummary(form.high_noise_sample_params)" :open="sectionState.highNoiseSample" variant="plain" @toggle="toggleSection('highNoiseSample')">
                        <div class="fields">
                            <div class="field">
                                <label>{{ t("field.scheduler") }}</label>
                                <select v-model="form.high_noise_sample_params.scheduler">
                                    <option value="default">{{ t("option.default") }}</option>
                                    <option v-for="scheduler in schedulers" :key="`high-noise-${scheduler}`" :value="scheduler">{{ scheduler }}</option>
                                </select>
                            </div>
                            <div class="field"><label>{{ t("field.flowShift") }}</label><input v-model="form.high_noise_sample_params.flow_shift" type="number" step="0.01" :placeholder="t('placeholder.blankDefault')" /></div>
                            <div class="field">
                                <label>{{ t("field.method") }}</label>
                                <select v-model="form.high_noise_sample_params.sample_method">
                                    <option value="default">{{ t("option.default") }}</option>
                                    <option v-for="sampler in samplers" :key="`high-noise-${sampler}`" :value="sampler">{{ sampler }}</option>
                                </select>
                            </div>
                            <div class="field"><label>{{ t("field.steps") }}</label><input v-model.number="form.high_noise_sample_params.sample_steps" type="number" /></div>
                            <div class="field"><label>{{ t("field.eta") }}</label><input v-model="form.high_noise_sample_params.eta" type="number" step="0.01" :placeholder="t('placeholder.blankAuto')" /></div>
                            <div class="field"><label>{{ t("field.shiftedTimestep") }}</label><input v-model.number="form.high_noise_sample_params.shifted_timestep" type="number" /></div>
                        </div>
                    </CollapsibleSection>
                    <CollapsibleSection class="stack-top" :eyebrow="t('section.guidance')" :summary="buildGuidanceSummary(form.high_noise_sample_params)" :open="sectionState.highNoiseGuidance" variant="plain" @toggle="toggleSection('highNoiseGuidance')">
                        <div class="fields">
                            <div class="field"><label>{{ t("field.cfgScale") }}</label><input v-model.number="form.high_noise_sample_params.guidance.txt_cfg" type="number" step="0.1" /></div>
                            <div class="field"><label>{{ t("field.distilledGuidance") }}</label><input v-model.number="form.high_noise_sample_params.guidance.distilled_guidance" type="number" step="0.1" /></div>
                            <div class="field"><label>{{ t("field.imageCfg") }}</label><input v-model="form.high_noise_sample_params.guidance.img_cfg" type="number" step="0.1" :placeholder="t('placeholder.blankFollowCfg')" /></div>
                            <div class="field"><label>{{ t("field.slgLayers") }}</label><input v-model="form.high_noise_sample_params.guidance.slg_layers" placeholder="7,8,9" /></div>
                            <div class="field"><label>{{ t("field.slgLayerStart") }}</label><input v-model.number="form.high_noise_sample_params.guidance.layer_start" type="number" step="0.01" /></div>
                            <div class="field"><label>{{ t("field.slgLayerEnd") }}</label><input v-model.number="form.high_noise_sample_params.guidance.layer_end" type="number" step="0.01" /></div>
                            <div class="field"><label>{{ t("field.slgScale") }}</label><input v-model.number="form.high_noise_sample_params.guidance.scale" type="number" step="0.01" /></div>
                        </div>
                    </CollapsibleSection>
                </CollapsibleSection>

                <CollapsibleSection class="stack-top" :eyebrow="t('section.conditioning')" :summary="conditioningSummary" :open="sectionState.conditioning" @toggle="toggleSection('conditioning')">
                    <div class="fields">
                        <div class="field"><label>{{ t("field.clipSkip") }}</label><input v-model.number="form.clip_skip" type="number" /></div>
                        <div class="field"><label>{{ t("field.strength") }}</label><input v-model.number="form.strength" type="number" step="0.01" /></div>
                        <div v-if="generationMode === 'image'" class="field"><label>{{ t("field.controlStrength") }}</label><input v-model.number="form.control_strength" type="number" step="0.01" /></div>
                        <div v-if="generationMode === 'video'" class="field"><label>{{ t("field.vaceStrength") }}</label><input v-model.number="form.vace_strength" type="number" step="0.01" /></div>
                    </div>
                </CollapsibleSection>

                <CollapsibleSection class="stack-top" :eyebrow="t('section.lora')" :summary="loraSummary" :open="sectionState.lora" @toggle="toggleSection('lora')">
                    <div v-if="!availableLoras.length" class="hint">{{ t("lora.noneAvailable") }}</div>
                    <div v-else-if="!form.lora.length" class="hint">{{ t("lora.noneConfigured") }}</div>
                    <div v-else class="list-editor">
                        <div class="list-row list-row--header">
                            <div>{{ t("lora.header") }}</div>
                            <div>{{ t("lora.multiplier") }}</div>
                            <div>{{ t("lora.highNoise") }}</div>
                            <div></div>
                        </div>
                        <div v-for="(item, index) in form.lora" :key="index" class="list-row">
                            <select v-model="item.path">
                                <option v-for="lora in availableLoras" :key="lora.path" :value="lora.path">
                                    {{ lora.name }} ({{ lora.path }})
                                </option>
                            </select>
                            <input v-model.number="item.multiplier" type="number" step="0.1" />
                            <label class="checkbox list-row__checkbox">
                                <input v-model="item.is_high_noise" type="checkbox" />
                                <span>{{ item.is_high_noise ? t("lora.on") : t("lora.off") }}</span>
                            </label>
                            <button class="btn-ghost" type="button" @click="removeLora(index)">{{ t("action.remove") }}</button>
                        </div>
                    </div>
                    <div><button class="btn-ghost" type="button" @click="addLora" :disabled="!availableLoras.length">{{ t("lora.add") }}</button></div>
                </CollapsibleSection>

                <CollapsibleSection class="stack-top" :eyebrow="t('section.imageInputs')" :summary="imageInputsSummary" :open="sectionState.auxiliaryImages" @toggle="toggleSection('auxiliaryImages')">
                    <div class="upload-grid">
                        <ImageDropzone
                            v-for="input in gridImageInputs"
                            :key="input.target"
                            :label="t(input.labelKey)"
                            :description="t(input.descriptionKey)"
                            :preview="getFormImage(input.target)"
                            @select="assignImages(input.target, $event)"
                            @clear="clearImage(input.target)"
                            @preview="openImageEntry($event)"
                        />
                    </div>
                    <div v-for="input in fullImageInputs" :key="input.target">
                        <ImageDropzone
                            :label="t(input.labelKey)"
                            :description="t(input.descriptionKey)"
                            :preview="getFormImage(input.target)"
                            @select="assignImages(input.target, $event)"
                            @clear="clearImage(input.target)"
                            @preview="openImageEntry($event)"
                        />
                    </div>

                    <div v-if="generationMode === 'image'" class="group">
                        <label>{{ t("input.refImages") }}</label>
                        <ImageDropzone
                            :label="t('input.refImages')"
                            :description="t('input.refImagesDesc')"
                            :items="form.ref_images"
                            multiple
                            @select="assignImages('ref_images', $event)"
                            @clear="clearImage('ref_images')"
                        />
                        <div v-if="!form.ref_images.length" class="hint">{{ t("input.noFiles") }}</div>
                        <div v-else class="file-list">
                            <div v-for="(item, index) in form.ref_images" :key="item.name + index" class="file-chip file-chip--preview">
                                <button class="file-chip__thumb-button" type="button" @click="openLightbox(item.dataUrl, item.name)">
                                    <img class="file-chip__thumb" :src="item.dataUrl" :alt="item.name" />
                                </button>
                                <span class="file-chip__name">{{ item.name }}</span>
                                <button class="icon-button" type="button" @click="removeCollectionImage('ref_images', index)">{{ t("action.remove") }}</button>
                            </div>
                        </div>
                    </div>

                    <div v-else class="group">
                        <label>{{ t("input.controlFrames") }}</label>
                        <ImageDropzone
                            :label="t('input.controlFrames')"
                            :description="t('input.controlFramesDesc')"
                            :items="form.control_frames"
                            multiple
                            @select="assignImages('control_frames', $event)"
                            @clear="clearImage('control_frames')"
                        />
                        <div v-if="!form.control_frames.length" class="hint">{{ t("input.noFiles") }}</div>
                        <div v-else class="file-list">
                            <div v-for="(item, index) in form.control_frames" :key="item.name + index" class="file-chip file-chip--preview">
                                <button class="file-chip__thumb-button" type="button" @click="openLightbox(item.dataUrl, item.name)">
                                    <img class="file-chip__thumb" :src="item.dataUrl" :alt="item.name" />
                                </button>
                                <span class="file-chip__name">{{ item.name }}</span>
                                <button class="icon-button" type="button" @click="removeCollectionImage('control_frames', index)">{{ t("action.remove") }}</button>
                            </div>
                        </div>
                    </div>
                </CollapsibleSection>

                <CollapsibleSection class="stack-top" :eyebrow="t('section.vaeTiling')" :summary="vaeTilingSummary" :open="sectionState.vaeTiling" @toggle="toggleSection('vaeTiling')">
                    <label class="checkbox"><input v-model="form.vae_tiling_params.enabled" type="checkbox" /><span>{{ t("field.enabled") }}</span></label>
                    <div class="fields">
                        <div class="field"><label>{{ t("field.tileWidth") }}</label><input v-model.number="form.vae_tiling_params.tile_size_w" type="number" /></div>
                        <div class="field"><label>{{ t("field.tileHeight") }}</label><input v-model.number="form.vae_tiling_params.tile_size_h" type="number" /></div>
                    </div>
                    <div class="field"><label>{{ t("field.targetOverlap") }}</label><input v-model.number="form.vae_tiling_params.target_overlap" type="number" step="0.01" /></div>
                    <div class="fields">
                        <div class="field"><label>{{ t("field.relativeWidth") }}</label><input v-model.number="form.vae_tiling_params.rel_size_w" type="number" step="0.01" /></div>
                        <div class="field"><label>{{ t("field.relativeHeight") }}</label><input v-model.number="form.vae_tiling_params.rel_size_h" type="number" step="0.01" /></div>
                    </div>
                </CollapsibleSection>

                <CollapsibleSection class="stack-top" :eyebrow="t('section.cache')" :summary="cacheSummary" :open="sectionState.cache" @toggle="toggleSection('cache')">
                    <div class="field">
                        <label>{{ t("field.cacheMode") }}</label>
                        <select v-model="form.cache.mode">
                            <option v-for="mode in CACHE_MODES" :key="mode" :value="mode">{{ mode === "disabled" ? t("summary.disabled") : mode }}</option>
                        </select>
                    </div>
                    <div class="field field--full"><label>{{ t("field.cacheOption") }}</label><input v-model="form.cache.option" placeholder="threshold=0.25,start=0.15,end=0.95" /></div>
                    <div class="field"><label>{{ t("field.scmMask") }}</label><input v-model="form.cache.scm_mask" /></div>
                    <label class="checkbox"><input v-model="form.cache.scm_policy_dynamic" type="checkbox" /><span>{{ t("field.dynamicScmPolicy") }}</span></label>
                </CollapsibleSection>
            </section>

            <section class="panel output-panel">
                <div class="panel-header">
                    <div>
                        <h2 class="panel-title">{{ t("panel.output") }}</h2>
                    </div>
                </div>

                <div v-if="videoPreviewSrc" class="hero-frame hero-frame--media">
                    <video class="hero-frame__video" :src="videoPreviewSrc" controls autoplay loop muted playsinline />
                </div>
                <button v-else class="hero-frame hero-frame--button" type="button" :disabled="!previewImageSrc" @click="openLightbox(previewImageSrc, t('output.alt'))">
                    <img v-if="previewImageSrc" :src="previewImageSrc" :alt="t('output.alt')" />
                    <div v-else class="hero-placeholder">
                        <h2>{{ generationMode === "video" ? t("output.generateVideo") : t("output.generateImages") }}</h2>
                        <p>
                            {{ generationMode === "video" ? t("output.placeholderVideo") : t("output.placeholderImage") }}
                        </p>
                    </div>
                </button>

                <div v-if="currentJobKind === 'vid_gen' && currentJob?.result?.output_format === 'avi'" class="hint stack-top">
                    {{ t("output.aviHint") }}
                </div>

                <div class="metrics output-metrics">
                    <div class="metric">
                        <div class="metric__label">{{ t("metric.status") }}</div>
                        <div class="metric__value">{{ statusLabel(currentStatus) }}</div>
                    </div>
                    <div class="metric">
                        <div class="metric__label">{{ t("metric.queue") }}</div>
                        <div class="metric__value">{{ currentJob?.queue_position ?? 0 }}</div>
                    </div>
                    <div class="metric">
                        <div class="metric__label">{{ t("metric.created") }}</div>
                        <div class="metric__value mono">{{ formatUnixTime(currentJob?.created) }}</div>
                    </div>
                    <div class="metric">
                        <div class="metric__label">{{ t("metric.elapsed") }}</div>
                        <div class="metric__value">{{ formatElapsed(currentJob?.started, currentJob?.completed) }}</div>
                    </div>
                    <div v-if="currentJobKind === 'vid_gen'" class="metric">
                        <div class="metric__label">{{ t("metric.fps") }}</div>
                        <div class="metric__value">{{ videoFps || "-" }}</div>
                    </div>
                    <div v-if="currentJobKind === 'vid_gen'" class="metric">
                        <div class="metric__label">{{ t("metric.frames") }}</div>
                        <div class="metric__value">{{ videoFrameCount || "-" }}</div>
                    </div>
                </div>

                <div v-if="statusMessage" class="status-message" :class="statusTone === 'error' ? 'status-message--error' : 'status-message--success'">{{ statusMessage }}</div>
                <div v-else-if="currentJob?.error?.message" class="status-message status-message--error">{{ currentJob.error.message }}</div>

                <div class="output-controls">
                    <button class="btn output-controls__primary" type="button" :disabled="isJobRunning" @click="generate">
                        {{ generationMode === "video" ? t("output.generateVideo") : t("output.generateImage") }}
                    </button>
                    <div class="actions output-controls__secondary">
                        <button class="btn-secondary" type="button" :disabled="!downloadableSrc" @click="downloadSelected">{{ t("output.download") }}</button>
                        <button class="btn-danger" type="button" :disabled="!canCancelCurrentJob" @click="cancelCurrentJob">{{ t("output.cancel") }}</button>
                    </div>
                </div>

                <div v-if="currentImages.length > 1" class="thumb-row">
                    <button v-for="(image, index) in currentImages" :key="image.index" class="thumb" :class="{ 'thumb--active': index === selectedOutputIndex }" type="button" @click="selectOutput(index)">
                        <img :src="`data:image/${currentJob?.result?.output_format || 'png'};base64,${image.b64_json}`" :alt="t('output.thumbAlt', { index: index + 1 })" />
                    </button>
                </div>
            </section>
        </div>
        <div v-if="lightboxOpen && lightboxImageSrc" class="lightbox" @click.self="closeLightbox">
            <button class="lightbox__close" type="button" @click="closeLightbox">{{ t("output.close") }}</button>
            <img class="lightbox__image" :src="lightboxImageSrc" :alt="lightboxImageAlt" />
        </div>
    </div>
</template>
