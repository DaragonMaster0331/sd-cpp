<script setup lang="ts">
import { computed } from "vue";
import { statusLabel } from "../i18n";

const STATUS_TONES: Record<string, string> = {
    online: "online",
    completed: "online",
    queued: "queued",
    generating: "generating",
    failed: "failed",
    cancelled: "cancelled",
    offline: "offline",
    // SD-Studio engine chip
    cuda: "gpu",
    cpu: "cpu",
};

const props = withDefaults(defineProps<{
    status?: string;
    label?: string;
}>(), {
    status: "",
    label: "",
});

const tone = computed(() => STATUS_TONES[props.status] || "");

const classes = computed(() => ["chip", tone.value && `chip--${tone.value}`].filter(Boolean));
</script>

<template>
    <span :class="classes">{{ label || statusLabel(status || "idle") }}</span>
</template>
