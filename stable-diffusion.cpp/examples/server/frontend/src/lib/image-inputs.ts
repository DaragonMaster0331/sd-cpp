import type { ImageInputConfig } from "./types";

export const IMAGE_INPUTS: readonly ImageInputConfig[] = [
    {
        target: "init_image",
        labelKey: "input.initImage",
        descriptionKey: "input.initImageDesc",
        layout: "grid",
    },
    {
        target: "mask_image",
        labelKey: "input.maskImage",
        descriptionKey: "input.maskImageDesc",
        layout: "grid",
    },
    {
        target: "control_image",
        labelKey: "input.controlImage",
        descriptionKey: "input.controlImageDesc",
        layout: "full",
    },
];

export const VIDEO_IMAGE_INPUTS: readonly ImageInputConfig[] = [
    {
        target: "init_image",
        labelKey: "input.startFrame",
        descriptionKey: "input.startFrameDesc",
        layout: "grid",
    },
    {
        target: "end_image",
        labelKey: "input.endFrame",
        descriptionKey: "input.endFrameDesc",
        layout: "grid",
    },
];
