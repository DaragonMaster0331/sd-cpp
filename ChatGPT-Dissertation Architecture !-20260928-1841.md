# Dissertation Architecture !

**User:** Anonymous  
**Created:** 9/28/2026 16:08:18  
**Updated:** 9/28/2026 18:34:14  
**Exported:** 9/28/2026 18:41:27  
**Link:** [https://chatgpt.com/c/6aba1261-b798-83eb-b2da-2c8dde460544](https://chatgpt.com/c/6aba1261-b798-83eb-b2da-2c8dde460544)  

## Prompt:
9/28/2026, 6:15:12 PM

generate **Chapter 1:** Why text-only interior generation is difficult

## Response:
9/28/2026, 6:15:13 PM · gpt-5-6

Below is a doctoral-level draft of **Chapter 1**, written around your proposed research direction: **text-only input → semantic spaces → multi-condition DiT → interior image generation**. I have kept it suitable as a foundation that you can later expand with citations and your specific experimental contributions.

**document: Chapter 1 — Introduction**

# Chapter 1 — Introduction

## 1.1 Background

Interior design is a multidisciplinary field concerned with the organization, functionality, aesthetics, and experiential qualities of interior environments. The development of an interior design concept requires the integration of multiple elements, including spatial organization, furniture, materials, colors, lighting, architectural features, and stylistic characteristics. Visualization is therefore an important component of the interior design process because it enables designers and clients to communicate and evaluate design concepts before physical implementation.

Traditional interior visualization generally relies on computer-aided design (CAD), building information modeling (BIM), three-dimensional modeling, material assignment, lighting configuration, and physically based rendering. Although these techniques can produce highly detailed and controllable visualizations, they often require substantial technical knowledge, modeling time, and manual intervention. Consequently, rapid exploration of alternative design concepts can be difficult, particularly during the early conceptual stages of design.

The emergence of generative artificial intelligence has introduced a different approach to visual design. Text-to-image generative models can synthesize images from natural-language descriptions, allowing users to describe desired visual concepts without explicitly constructing a three-dimensional scene. Recent diffusion-based image-generation systems have demonstrated substantial capabilities in generating realistic images from textual descriptions. These developments have created new possibilities for AI-assisted architectural and interior design visualization.

However, interior design presents a particularly demanding application for text-to-image generation. An interior-design description does not merely specify the appearance of an image. It often contains multiple interconnected requirements concerning the room, spatial organization, furniture, materials, colors, lighting, architectural elements, style, atmosphere, and relationships among objects.

For example, a designer may specify:

> "A contemporary living room with a large beige sectional sofa facing a marble fireplace, a natural oak floor, floor-to-ceiling windows on the left wall, warm indirect lighting, neutral cream walls, and a minimalist luxury style."

A successful generation must simultaneously represent the requested objects, materials, colors, style, lighting, and spatial relationships. Consequently, interior image generation is not simply a problem of producing visually attractive images. It is also a problem of accurately interpreting and satisfying a structured set of design requirements expressed through natural language.

This observation motivates the investigation of text-driven generative methods specifically designed for interior design.

---

## 1.2 Artificial Intelligence in Interior Design

Artificial intelligence has increasingly been investigated as a tool for supporting architectural and interior design processes. AI techniques have been applied to areas such as design optimization, floor-plan generation, image classification, object recognition, style analysis, material recognition, visualization, and generative design.

Generative AI introduces an additional capability: rather than only analyzing existing design information, the system can synthesize new design representations. In image generation, this allows a designer to explore multiple visual alternatives from a textual description.

The potential workflow can be expressed as:

$$
\text{Design Requirements}
\rightarrow
\text{AI Interpretation}
\rightarrow
\text{Visual Generation}
\rightarrow
\text{Design Evaluation}
$$

Such a workflow can reduce the time required to transform conceptual ideas into visual representations and can allow designers to explore a larger design space.

Nevertheless, an AI-generated interior image must satisfy requirements that are more complex than those of many general-purpose image-generation tasks. The generated image should not only be visually plausible but should also maintain semantic, spatial, stylistic, material, and lighting consistency.

---

## 1.3 Text-to-Image Generation

Text-to-image generation is a generative modeling task in which a natural-language description is used to condition the synthesis of an image.

A simplified formulation is:

$$
I = G(T)
$$

where:

- $T$ represents the textual description,
- $G$ represents the generative model, and
- $I$ represents the generated image.

Modern text-to-image systems commonly use a text encoder to convert the input description into a latent representation and a generative model to synthesize an image conditioned on that representation.

Diffusion models have become an important approach to this problem. In a diffusion-based framework, an image representation is progressively corrupted with noise during the forward process and a neural network learns to reverse this process during generation.

The reverse generation process can be represented conceptually as:

$$
z_T \rightarrow z_{T-1} \rightarrow \cdots \rightarrow z_1 \rightarrow z_0
$$

where $z_T$ is a noisy latent representation and $z_0$ is the generated image representation.

Diffusion Transformers (DiTs) replace or extend convolutional denoising architectures with transformer-based processing of image or latent patches. Their ability to model long-range relationships makes them particularly relevant to complex visual generation tasks.

However, the capability to generate realistic images does not necessarily imply that a model can accurately satisfy complex interior-design specifications.

---

## 1.4 Why Text-Only Interior Generation Is Difficult

The fundamental challenge addressed in this research is the gap between the **structure of natural language** and the **structure of interior design**.

A natural-language prompt is sequential:

$$
T = (w_1,w_2,\ldots,w_n)
$$

where each $w_i$ is a word or token.

An interior environment, however, contains multiple interacting semantic dimensions:

$$
D =
\{L,O,M,S,Li,C,R,A\}
$$

where:

- $L$ = layout,
- $O$ = objects,
- $M$ = materials,
- $S$ = style,
- $Li$ = lighting,
- $C$ = color,
- $R$ = relationships,
- $A$ = architectural features.

The challenge is therefore to transform a sequential linguistic representation into a structured design representation.

---

## 1.5 Spatial and Layout Requirements

One of the most difficult aspects of text-only interior generation is spatial organization.

Consider:

> "A sofa faces a fireplace, with a coffee table between them."

The prompt specifies three objects but also specifies their spatial relationships.

A generated image containing a sofa, fireplace, and coffee table may still be incorrect if:

- the sofa does not face the fireplace;
- the coffee table is not between them;
- the objects have inappropriate relative positions;
- the room geometry does not support the described arrangement.

Therefore, object presence alone is insufficient.

A more appropriate representation is a spatial relationship graph:

$$
G=(V,E)
$$

where $V$ represents entities and $E$ represents relationships.

For example:

$$
E =
\{
(\text{sofa},\text{faces},\text{fireplace}),
(\text{coffee table},\text{between},\text{sofa},\text{fireplace})
\}
$$

This demonstrates why interior generation requires more than simple object recognition.

---

## 1.6 Multiple Objects and Object Attributes

Interior-design descriptions frequently contain numerous objects with associated attributes.

For example:

> "A large beige sectional sofa with wooden legs is positioned beside a dark walnut coffee table."

The model must interpret:

- object type;
- object size;
- object color;
- object material;
- object shape;
- object position;
- relationship between objects.

The complexity increases as the number of objects increases.

A simplified representation is:

$$
O_i =
(c_i,a_i,m_i,s_i,p_i,r_i)
$$

where:

- $c_i$ = object category;
- $a_i$ = attributes;
- $m_i$ = material;
- $s_i$ = size/shape;
- $p_i$ = spatial information;
- $r_i$ = relationships.

A conventional text representation may not explicitly preserve all of these components.

---

## 1.7 Material Representation

Materials are another important dimension of interior design.

A designer may specify:

- natural oak flooring;
- white marble;
- brushed metal;
- linen curtains;
- velvet furniture;
- exposed concrete;
- matte painted walls.

Material is not simply a semantic label. It also determines visual properties such as:

- texture;
- reflectance;
- roughness;
- color variation;
- specularity;
- surface appearance.

Consequently, the model must translate a linguistic material concept into an appropriate visual representation.

For example:

$$
\text{"white marble"}
\rightarrow
\{\text{white color, stone texture, veins, reflectance}\}
$$

This creates a semantic-to-visual transformation problem.

---

## 1.8 Style Representation

Interior style is inherently complex.

Terms such as:

- modern;
- minimalist;
- Scandinavian;
- Japanese;
- industrial;
- classical;
- contemporary;
- luxury;

represent combinations of visual characteristics rather than single objects.

For example, "Scandinavian" may imply relationships among:

- furniture form;
- natural materials;
- color palette;
- lighting;
- decoration;
- spatial openness.

Similarly, "modern luxury" may involve:

- high-quality materials;
- restrained color palettes;
- sophisticated lighting;
- large-scale furniture;
- clean architectural forms.

Therefore, style cannot necessarily be represented as a single independent attribute. It interacts with objects, materials, colors, architecture, and lighting.

---

## 1.9 Lighting Representation

Lighting introduces another source of complexity.

A textual description may specify:

> "Large windows provide soft natural daylight combined with warm indirect ceiling lighting."

The generated image must simultaneously represent:

- light source;
- light direction;
- intensity;
- color temperature;
- shadows;
- reflections;
- overall atmosphere.

Lighting is particularly difficult because it affects almost every pixel of the generated image.

A lighting condition can therefore be considered a global visual constraint rather than merely an object-level attribute.

---

## 1.10 Relationships Between Design Elements

Interior design is fundamentally relational.

The meaning of an interior is determined not only by the individual objects but also by how those objects interact.

Examples include:

- sofa facing television;
- dining table near kitchen;
- bed against wall;
- chairs surrounding table;
- lamp beside sofa;
- artwork above sofa;
- window behind dining table.

Consequently, the representation of:

$$
\{\text{objects}\}
$$

is insufficient.

The system should also represent:

$$
\{\text{relationships between objects}\}
$$

This motivates the introduction of a dedicated **relationship semantic space** in the proposed framework.

---

## 1.11 Limitations of Single-Space Text Conditioning

Conventional text-to-image generation generally produces a unified textual representation:

$$
E_T = f_{text}(T)
$$

The generative model then uses this representation as a conditioning signal.

Although this approach is powerful, it may not explicitly distinguish:

$$
E_T =
E_{layout}
+
E_{object}
+
E_{material}
+
E_{style}
+
E_{lighting}
+\cdots
$$

The proposed research investigates whether explicitly decomposing the text into specialized semantic representations can improve interior-design generation.

The conceptual difference is:

### Conventional approach

$$
T
\rightarrow
E_T
\rightarrow
DiT
\rightarrow
I
$$

### Proposed approach

$$
T
\rightarrow
\begin{cases}
E_L & \text{Layout}\\
E_O & \text{Object}\\
E_M & \text{Material}\\
E_S & \text{Style}\\
E_{Li} & \text{Lighting}\\
E_C & \text{Color}\\
E_R & \text{Relations}
\end{cases}
\rightarrow
\text{Multi-Condition DiT}
\rightarrow
I
$$

The central research question is whether this structured representation improves the ability of the model to satisfy complex interior-design requirements.

---

## 1.12 The Research Problem

The central problem of this research can therefore be stated as follows:

> **Existing text-to-image generation approaches can produce visually realistic interior images, but natural-language descriptions of interior environments contain multiple interacting semantic dimensions that are not necessarily represented explicitly or independently. As a result, generated images may exhibit inconsistencies in spatial relationships, object attributes, materials, style, lighting, and other design requirements.**

This research investigates a text-only generative framework that decomposes natural-language interior-design descriptions into multiple semantic spaces and uses these representations as separate conditioning signals for a Diffusion Transformer.

---

## 1.13 Research Aim

The primary aim of this research is:

> **To develop and evaluate a text-driven generative framework that transforms natural-language interior-design descriptions into high-quality interior images through multi-space semantic representation and multi-condition Diffusion Transformer generation.**

The proposed system will accept only natural language as input.

The system will automatically identify and represent:

- layout;
- objects;
- materials;
- style;
- lighting;
- colors;
- architectural features;
- spatial relationships.

These representations will then condition the image-generation process.

---

## 1.14 Research Objectives

The research has the following objectives.

### Objective 1

To investigate the semantic characteristics of natural-language descriptions used in interior design.

### Objective 2

To develop a semantic decomposition method for extracting interior-design concepts from natural-language descriptions.

### Objective 3

To construct specialized semantic representations for layout, objects, materials, style, lighting, color, architecture, and relationships.

### Objective 4

To develop a multi-condition conditioning mechanism for integrating these semantic representations into a Diffusion Transformer.

### Objective 5

To develop or fine-tune a text-to-image generative model specifically for interior-design visualization.

### Objective 6

To investigate whether specialized semantic conditioning improves the accuracy and consistency of generated interior images.

### Objective 7

To develop evaluation methods for measuring interior-design requirement satisfaction.

### Objective 8

To compare the proposed framework with conventional single-space text-conditioning approaches.

---

## 1.15 Research Questions

The research can be organized around the following questions.

### RQ1

**What semantic components of natural-language descriptions are most important for interior-design image generation?**

### RQ2

**How can natural-language interior-design descriptions be decomposed into specialized semantic spaces such as layout, object, material, style, lighting, color, and relationships?**

### RQ3

**How should different semantic spaces be represented and integrated into a Diffusion Transformer?**

### RQ4

**Does multi-space semantic conditioning improve the semantic fidelity of generated interior images compared with conventional single-space text conditioning?**

### RQ5

**Which semantic conditions have the greatest influence on interior-image generation quality and design requirement satisfaction?**

### RQ6

**How effectively can the proposed model preserve spatial and semantic relationships between interior elements?**

### RQ7

**How do professional designers evaluate the generated images in terms of realism, design coherence, and usefulness?**

---

## 1.16 Research Hypotheses

The hypotheses can be formulated as follows.

### H1

Multi-space semantic conditioning produces higher text-image semantic alignment than conventional single-space text conditioning.

### H2

Explicit object and relationship representations improve the accuracy of object placement and spatial relationships.

### H3

Explicit material conditioning improves material-related visual consistency.

### H4

Explicit style and lighting conditioning improves stylistic and atmospheric consistency.

### H5

A multi-condition Diffusion Transformer produces higher interior-design requirement satisfaction than a conventional text-conditioned generation model.

These hypotheses can be evaluated experimentally using quantitative and human-centered evaluation.

---

## 1.17 Proposed Research Framework

The overall framework is:

```text
                   NATURAL LANGUAGE
                          │
                          ▼
                  Text Processing
                          │
                          ▼
                  Language Encoder
                          │
                          ▼
               Semantic Decomposition
                          │
        ┌─────────────────┼──────────────────┐
        │                 │                  │
        ▼                 ▼                  ▼
     Layout            Objects           Materials
        │                 │                  │
        ▼                 ▼                  ▼
   Layout Space       Object Space       Material Space
        │                 │                  │
        └─────────────────┼──────────────────┘
                          │
        ┌─────────────────┼──────────────────┐
        │                 │                  │
        ▼                 ▼                  ▼
      Style            Lighting            Color
        │                 │                  │
        └─────────────────┼──────────────────┘
                          │
                          ▼
                 Relationship Space
                          │
                          ▼
                 Condition Fusion
                          │
                          ▼
                Multi-Condition DiT
                          │
                          ▼
                    Image Latent
                          │
                          ▼
                     VAE Decoder
                          │
                          ▼
                INTERIOR IMAGE
```

---

## 1.18 Research Scope

The research focuses specifically on **text-to-image interior-design generation**.

The primary input is natural-language text.

The research does not require the user to provide:

- floor-plan images;
- reference photographs;
- depth maps;
- segmentation maps;
- CAD models;
- BIM models;
- 3D scenes.

These representations may be investigated internally as auxiliary training or evaluation information where appropriate, but the proposed inference system is designed around text-only user input.

The research focuses primarily on the generation of static interior images rather than complete 3D environments.

---

## 1.19 Expected Contributions

The expected contributions of this research are divided into several categories.

### 1. Semantic contribution

A structured representation of natural-language interior-design descriptions based on multiple semantic spaces.

### 2. Architectural contribution

A multi-condition Diffusion Transformer architecture for text-driven interior image generation.

### 3. Representation contribution

A relationship-aware representation for describing interactions among furniture, architectural elements, and other interior components.

### 4. Training contribution

An interior-design-specific training strategy for learning relationships between natural-language requirements and visual representations.

### 5. Evaluation contribution

An evaluation framework that measures not only image quality but also:

- object accuracy;
- material accuracy;
- style consistency;
- lighting consistency;
- spatial relationship accuracy;
- text-image alignment;
- overall design requirement satisfaction.

### 6. Practical contribution

A text-only AI-assisted visualization system that can support early-stage interior-design ideation and visualization.

---

## 1.20 Significance of the Research

The proposed research has potential significance for both artificial intelligence and interior design.

From the perspective of generative AI, the research investigates whether structured semantic conditioning can improve generation in a domain where visual concepts have strong relationships and constraints.

From the perspective of interior design, the research provides a mechanism for transforming natural-language design concepts into visual representations without requiring users to manually construct a three-dimensional scene.

A designer could therefore move from:

$$
\text{Concept}
\rightarrow
\text{Natural Language}
\rightarrow
\text{AI Visualization}
$$

rather than:

$$
\text{Concept}
\rightarrow
\text{Manual 3D Modeling}
\rightarrow
\text{Material Assignment}
\rightarrow
\text{Lighting}
\rightarrow
\text{Rendering}
$$

The proposed system is not intended to replace professional design judgment. Instead, it provides an additional visualization mechanism for exploring and communicating design concepts.

---

## 1.21 Conceptual Research Model

The conceptual model of this research is:

$$
T
\rightarrow
S
\rightarrow
C
\rightarrow
G
\rightarrow
I
\rightarrow
E
$$

where:

- $T$ = natural-language input;
- $S$ = semantic decomposition;
- $C$ = multi-space conditioning;
- $G$ = generative process;
- $I$ = generated interior image;
- $E$ = evaluation.

More specifically:

$$
T
\rightarrow
\{L,O,M,S,Li,C,A,R\}
\rightarrow
C_{multi}
\rightarrow
DiT
\rightarrow
I
$$

where:

- $L$ = layout;
- $O$ = objects;
- $M$ = materials;
- $S$ = style;
- $Li$ = lighting;
- $C$ = color;
- $A$ = architecture;
- $R$ = relationships.

This model forms the foundation for the subsequent methodology and experimental design.

---

## 1.22 Chapter Summary

This chapter established the motivation and research problem for text-driven interior image generation. Although modern text-to-image generative models can produce highly realistic images, interior-design generation presents additional challenges because natural-language descriptions contain multiple interacting dimensions, including spatial organization, objects, materials, style, lighting, colors, architectural features, and relationships.

The chapter identified the limitations of treating an interior-design description as a single undifferentiated textual representation. It proposed a research direction based on decomposing natural-language descriptions into specialized semantic spaces and using these representations as multiple conditioning signals for a Diffusion Transformer.

The proposed research therefore follows the principle:

$$
\boxed{
\text{Natural Language}
\rightarrow
\text{Multi-Space Semantic Representation}
\rightarrow
\text{Multi-Condition DiT}
\rightarrow
\text{Interior Image}
}
$$

The following chapter will examine existing research in interior visualization, text-to-image generation, diffusion models, Diffusion Transformers, semantic conditioning, controllable generation, and AI-assisted architectural and interior design, with particular attention to the research gap that motivates the proposed framework.

For a real PhD dissertation, **Chapter 2 should now be built directly from this Chapter 1**: it should systematically review the existing literature and establish exactly where your **multi-space semantic conditioning + DiT** architecture differs from prior work.

## Prompt:
9/28/2026, 6:18:24 PM

next, generate chapter2

## Response:
9/28/2026, 6:18:28 PM · gpt-5-6-mini

> **Searched 19 websites**
>
> 
>
> Worked for 10s

Below is a dissertation-style **Chapter 2 - Literature Review and Research Framework**, written to connect directly to your Chapter 1 and to establish the research gap for your proposed **text-only, multi-space semantic conditioning + Diffusion Transformer (DiT)** framework.

# Chapter 2 - Literature Review and Research Framework

## 2.1 Introduction

The development of artificial intelligence (AI) has substantially changed the methods used for visual content creation, including architectural visualization and interior design. Traditional interior-design visualization depends heavily on computer-aided design (CAD), three-dimensional (3D) modeling, rendering engines, material libraries, and manual scene construction. Although these technologies provide accurate control over spatial and visual properties, the design process can require considerable time and technical expertise.

Recent advances in generative AI have introduced an alternative approach in which natural-language descriptions can be transformed directly into visual content. Text-to-image (T2I) systems based on generative adversarial networks (GANs), variational autoencoders (VAEs), diffusion models, and Transformer architectures have progressively improved image quality and semantic alignment. In particular, diffusion-based models have become an important foundation for high-quality image synthesis, while latent diffusion and Diffusion Transformer (DiT) architectures have provided scalable approaches for high-resolution generation. ([arXiv](https://arxiv.org/abs/2006.11239?utm_source=chatgpt.com))

However, interior design presents requirements that are more structured than those of general image generation. A single interior-design description can simultaneously specify room architecture, furniture, object attributes, spatial relationships, materials, colors, lighting, and stylistic characteristics. Consequently, the central literature gap is not simply the ability to generate realistic images, but the ability to **translate multiple heterogeneous design requirements expressed in natural language into a coherent visual scene**.

This chapter reviews the theoretical and technological foundations relevant to this problem. It examines traditional interior visualization, generative models, text-to-image generation, diffusion models, latent diffusion, Transformer-based diffusion, controllable generation, layout-aware generation, architectural applications, datasets, evaluation methods, and existing interior-design generation approaches. The chapter then identifies the research gap and establishes the theoretical framework supporting the proposed multi-space semantic conditioning approach.

---

## 2.2 Traditional Interior Design Visualization

Interior visualization traditionally involves a sequence of design and representation processes.

A simplified workflow can be expressed as:

```text
Design Requirements
        ↓
Concept Development
        ↓
Space Planning
        ↓
CAD / BIM / 3D Modeling
        ↓
Furniture and Material Selection
        ↓
Lighting Configuration
        ↓
Rendering
        ↓
Visualization
```

The designer must translate conceptual requirements into explicit geometric and visual representations. Objects are placed within a room, materials are assigned to surfaces, lighting sources are configured, cameras are positioned, and rendering parameters are adjusted.

This process has an important advantage: **explicit control**. A designer can specify the exact position of a sofa, the dimensions of a table, the location of a window, or the material applied to a wall.

However, this explicit representation also introduces considerable design effort. Changes to the design concept may require modifications to multiple components of the 3D scene.

Generative AI approaches attempt to move part of this process from explicit geometric construction toward semantic generation.

Instead of:

$$
\text{Text} \rightarrow \text{Manual Modeling} \rightarrow \text{Rendering}
$$

the generative approach attempts:

$$
\text{Text} \rightarrow \text{Generated Visualization}
$$

This provides a much more direct interface between human design intent and visual output.

Research involving architecture students has demonstrated that text-to-image systems can support early-stage architectural ideation, particularly when design constraints are considered during the generation process. ([Sage Journals](https://journals.sagepub.com/doi/10.1177/14780771231222783?utm_source=chatgpt.com))

Nevertheless, replacing explicit design representations with natural language introduces a new problem: **the ambiguity and incompleteness of language as a representation of spatial design**.

---

# 2.3 Generative Models for Image Synthesis

Generative models learn a probability distribution over data and subsequently generate new samples from that learned distribution.

For image generation, the objective can generally be represented as:

$$
p_\theta(x)
$$

where $x$ represents an image and $\theta$ represents the model parameters.

Major generative approaches relevant to image synthesis include:

1. Generative Adversarial Networks
2. Variational Autoencoders
3. Autoregressive models
4. Diffusion models
5. Transformer-based generative models

These approaches have different mechanisms for representing and generating images.

---

## 2.3.1 Generative Adversarial Networks

Generative Adversarial Networks (GANs) introduced a framework involving two competing neural networks: a generator and a discriminator.

The generator attempts to produce images that resemble real images, while the discriminator attempts to distinguish generated images from real images.

The basic minimax objective can be represented as:

$$
\min_G \max_D
\mathbb{E}_{x\sim p_{data}}
[\log D(x)]
+
\mathbb{E}_{z\sim p_z}
[\log(1-D(G(z)))]
$$

GAN-based systems demonstrated that neural networks could generate highly realistic images.

However, GAN training can be difficult because of adversarial optimization, instability, and problems such as mode collapse. These limitations become particularly relevant when the target domain contains many interacting visual concepts.

Interior images can contain substantial variation in:

- room geometry,
- furniture arrangements,
- materials,
- architectural features,
- lighting,
- colors,
- decoration,
- viewpoints.

Modeling such a multimodal distribution using adversarial training can therefore be challenging.

Diffusion models subsequently provided an alternative generative mechanism based on progressive noise addition and denoising.

---

# 2.4 Variational Autoencoders

Variational Autoencoders (VAEs) learn a continuous latent representation of data.

A VAE consists conceptually of:

```text
Image
  ↓
Encoder
  ↓
Latent representation z
  ↓
Decoder
  ↓
Reconstructed image
```

The latent representation can be used to represent complex image information in a lower-dimensional space.

Kingma and Welling introduced the VAE framework based on variational inference and the reparameterization trick. ([arXiv](https://arxiv.org/abs/1312.6114?utm_source=chatgpt.com))

The standard VAE objective can be expressed as:

$$
\mathcal{L}_{VAE}
=
\mathbb{E}_{q_\phi(z|x)}
[\log p_\theta(x|z)]
-
D_{KL}(q_\phi(z|x)\|p(z))
$$

where:

- $x$ is the observed image,
- $z$ is the latent representation,
- $q_\phi(z|x)$ is the encoder distribution,
- $p_\theta(x|z)$ is the decoder,
- $p(z)$ is the prior distribution.

For modern diffusion systems, the VAE concept is particularly important because the diffusion process can operate on a compressed latent representation instead of directly on pixels.

---

# 2.5 Diffusion Models

Diffusion models have become one of the dominant approaches for modern image generation.

The fundamental concept is to define a forward process that gradually adds noise to an image and a learned reverse process that reconstructs the image.

The forward process can be expressed as:

$$
q(x_t|x_{t-1})
=
\mathcal{N}
\left(
x_t;
\sqrt{1-\beta_t}x_{t-1},
\beta_t I
\right)
$$

where $\beta_t$ represents the noise schedule.

After sufficiently many steps:

$$
x_0 \rightarrow x_1 \rightarrow \cdots \rightarrow x_T
$$

the image approaches a noise distribution.

The neural network then learns the reverse process:

$$
x_T \rightarrow x_{T-1} \rightarrow \cdots \rightarrow x_0
$$

Denoising Diffusion Probabilistic Models demonstrated the effectiveness of this approach for high-quality image synthesis. ([arXiv](https://arxiv.org/abs/2006.11239?utm_source=chatgpt.com))

A commonly used training objective is noise prediction:

$$
\mathcal{L}_{diff}
=
\mathbb{E}_{x_0,\epsilon,t}
\left[
\|\epsilon-\epsilon_\theta(x_t,t)\|^2
\right]
$$

where:

- $x_0$ is the original image,
- $x_t$ is the noisy image,
- $t$ is the diffusion timestep,
- $\epsilon$ is sampled Gaussian noise,
- $\epsilon_\theta$ is the predicted noise.

This formulation provides a flexible foundation for conditional image generation.

---

# 2.6 Latent Diffusion Models

Direct pixel-space diffusion is computationally expensive because high-resolution images contain a very large number of pixels.

Latent Diffusion Models address this problem by first encoding an image into a lower-dimensional latent representation.

The process becomes:

```text
Image
  ↓
VAE Encoder
  ↓
Latent z₀
  ↓
Diffusion
  ↓
Generated latent ẑ₀
  ↓
VAE Decoder
  ↓
Generated Image
```

Rombach et al. demonstrated that performing diffusion in a learned latent space can substantially reduce computational requirements while maintaining high visual quality. Their architecture also introduced cross-attention mechanisms that allow diffusion models to incorporate conditioning such as text and bounding boxes. ([openaccess.thecvf.com](https://openaccess.thecvf.com/content/CVPR2022/html/Rombach_High-Resolution_Image_Synthesis_With_Latent_Diffusion_Models_CVPR_2022_paper.html?utm_source=chatgpt.com))

For interior design, latent diffusion provides an attractive foundation because high-resolution room images contain extensive pixel-level information, while much of the semantic information can be represented at a lower-dimensional level.

The latent representation can be written as:

$$
z_0 = E(x)
$$

where $E$ is the VAE encoder.

The diffusion process is then performed on $z_0$:

$$
z_t =
\sqrt{\bar{\alpha}_t}z_0
+
\sqrt{1-\bar{\alpha}_t}\epsilon
$$

and the final latent representation is decoded:

$$
\hat{x}=D(\hat{z}_0)
$$

where $D$ is the VAE decoder.

---

# 2.7 Natural-Language Representation for Image Generation

Text provides an intuitive interface for human users because interior designers naturally describe design concepts using language.

For example:

> "A spacious modern living room with a beige sectional sofa facing a marble fireplace, oak flooring, warm indirect lighting, large windows, and Scandinavian furniture."

This sentence contains multiple semantic categories.

| Semantic category | Example |
|---|---|
| Room | living room |
| Object | sectional sofa |
| Object | fireplace |
| Material | marble |
| Material | oak |
| Color | beige |
| Style | Scandinavian |
| Lighting | warm indirect lighting |
| Architecture | large windows |
| Relationship | sofa facing fireplace |
| Spatial property | spacious |

A conventional text encoder produces a representation of the complete sentence. This representation is powerful, but the individual design dimensions are not necessarily represented as explicit independent variables.

This motivates the research question:

$$
T \rightarrow ?
$$

Should the entire design description be represented as one semantic condition, or should it be decomposed into multiple structured conditions?

The proposed research investigates the latter.

---

# 2.8 Vision-Language Representation

The development of vision-language representation has provided an important foundation for text-to-image generation.

CLIP, for example, demonstrated that image and language representations can be learned jointly from large-scale image-text pairs. The original CLIP work trained on approximately 400 million image-text pairs and demonstrated broad transferability of the resulting representations. ([arXiv](https://arxiv.org/abs/2103.00020?utm_source=chatgpt.com))

The conceptual architecture is:

```text
Text ─────→ Text Encoder ───→ Text Embedding
                                      ↕
                                 Alignment
                                      ↕
Image ────→ Image Encoder ───→ Image Embedding
```

This provides a semantic bridge between natural language and visual concepts.

For interior design, such representation is useful because words such as:

- sofa,
- marble,
- walnut,
- minimalist,
- Scandinavian,
- warm lighting,

can be associated with corresponding visual characteristics.

However, global image-text alignment does not necessarily guarantee that every individual requirement in a long design description is satisfied.

For example, an image may be highly similar to:

> "modern living room with marble fireplace and blue sofa"

while failing to correctly place the sofa relative to the fireplace.

This distinction between **semantic similarity** and **design-constraint satisfaction** is fundamental to the proposed research.

---

# 2.9 Text-to-Image Diffusion Models

Text-to-image diffusion models combine language representations with diffusion-based image generation.

A general architecture is:

```text
Text Prompt
     ↓
Text Encoder
     ↓
Text Embeddings
     ↓
Cross Attention
     ↓
Diffusion Network
     ↓
Latent Image
     ↓
VAE Decoder
     ↓
Generated Image
```

Imagen demonstrated the importance of strong language representations for text-to-image generation. Its experiments showed that scaling the language encoder could substantially improve both image fidelity and image-text alignment. ([arXiv](https://arxiv.org/abs/2205.11487?utm_source=chatgpt.com))

Latent diffusion further established a practical architecture for high-resolution conditional image synthesis. ([openaccess.thecvf.com](https://openaccess.thecvf.com/content/CVPR2022/html/Rombach_High-Resolution_Image_Synthesis_With_Latent_Diffusion_Models_CVPR_2022_paper.html?utm_source=chatgpt.com))

These developments establish the foundation for the proposed research.

However, standard text-to-image generation generally treats the natural-language prompt as a unified conditioning signal.

The proposed research instead investigates:

$$
T
\rightarrow
\{C_L,C_O,C_M,C_S,C_{Li},C_C,C_A,C_R\}
$$

where:

- $C_L$: layout condition,
- $C_O$: object condition,
- $C_M$: material condition,
- $C_S$: style condition,
- $C_{Li}$: lighting condition,
- $C_C$: color condition,
- $C_A$: architecture condition,
- $C_R$: relationship condition.

---

# 2.10 Diffusion Transformers

Traditional diffusion models frequently employ convolutional U-Net architectures.

Diffusion Transformers provide an alternative in which image latents are converted into tokens and processed using Transformer blocks.

DiT introduced a diffusion architecture that replaces the conventional U-Net backbone with a Transformer operating on latent patches. Its experiments showed that increasing Transformer computational capacity through depth, width, or token count was associated with improved generation performance. ([arXiv](https://arxiv.org/abs/2212.09748?utm_source=chatgpt.com))

A simplified DiT architecture is:

```text
Noisy Latent
     ↓
Patchify
     ↓
Image Tokens
     ↓
Linear Embedding
     ↓
Position Embedding
     ↓
┌─────────────────────────┐
│ Transformer Block       │
│                         │
│ Self-Attention          │
│        ↓                │
│ Condition Modulation    │
│        ↓                │
│ MLP                     │
└─────────────────────────┘
     ↓
Repeated Blocks
     ↓
Linear Projection
     ↓
Unpatchify
     ↓
Predicted Noise
```

This architecture is particularly relevant to the proposed research because Transformer attention naturally provides mechanisms for interactions among tokens.

For an interior scene, these interactions are important.

For example:

$$
\text{sofa}
\leftrightarrow
\text{fireplace}
$$

$$
\text{table}
\leftrightarrow
\text{sofa}
$$

$$
\text{window}
\leftrightarrow
\text{lighting}
$$

A Transformer-based architecture therefore provides a suitable foundation for investigating structured semantic conditioning.

---

# 2.11 Conditional Generation

The ability to control image generation is a major research direction in generative AI.

Instead of generating:

$$
x \sim p(x)
$$

a conditional model generates:

$$
x \sim p(x|c)
$$

where $c$ represents a condition.

Conditions can include:

- text,
- class labels,
- segmentation,
- depth,
- edges,
- pose,
- bounding boxes,
- spatial layouts,
- color information.

Latent diffusion demonstrated that cross-attention could incorporate general conditioning information. ([openaccess.thecvf.com](https://openaccess.thecvf.com/content/CVPR2022/html/Rombach_High-Resolution_Image_Synthesis_With_Latent_Diffusion_Models_CVPR_2022_paper.html?utm_source=chatgpt.com))

ControlNet extended this idea by introducing spatial conditioning mechanisms for pretrained text-to-image diffusion models. Its experiments included edge maps, depth, segmentation, human pose, and multiple controls. ([arXiv](https://arxiv.org/abs/2302.05543?utm_source=chatgpt.com))

T2I-Adapter similarly demonstrated that lightweight adapters can provide additional control signals, including structural and color information, while retaining a pretrained text-to-image model. ([arXiv](https://arxiv.org/abs/2302.08453?utm_source=chatgpt.com))

These studies demonstrate the value of separating conditioning information from the original text prompt.

However, ControlNet and related methods typically rely on **external spatial signals**, such as depth maps, segmentation maps, edge maps, or pose information.

The proposed research differs in an important way:

> **The user provides only natural language.**

The additional conditions are generated internally from the text.

Thus:

```text
User
 ↓
Natural Language
 ↓
Semantic Decomposition
 ↓
Internal Conditions
 ↓
Multi-Condition DiT
 ↓
Image
```

rather than:

```text
User
 ↓
Text + Depth + Segmentation + Layout Image
 ↓
Controlled Diffusion
 ↓
Image
```

This distinction is central to the research contribution.

---

# 2.12 Layout-to-Image Generation

Spatial layout is one of the most important requirements for interior design.

Layout-to-image generation explicitly models object positions and relationships.

For example:

```text
┌─────────────────────────────────┐
│             Window              │
│                                 │
│   Sofa               Lamp       │
│                                 │
│        Coffee Table             │
│                                 │
│       Fireplace / TV            │
└─────────────────────────────────┘
```

LayoutDiffusion investigated diffusion-based generation conditioned on layouts and specifically addressed the difficulty of controlling multiple objects and their spatial relationships. Its architecture includes mechanisms designed to make the model object-aware and position-sensitive. ([arXiv](https://arxiv.org/abs/2303.17189?utm_source=chatgpt.com))

This literature indicates that spatial information cannot always be treated as an ordinary textual concept.

For interior design, layout can include:

$$
L =
\{
(x_i,y_i,w_i,h_i,\theta_i)
\}_{i=1}^{N}
$$

where each object $i$ has:

- position $(x_i,y_i)$,
- size $(w_i,h_i)$,
- orientation $\theta_i$.

However, the proposed framework does not require the user to provide these coordinates explicitly.

Instead, natural language can be transformed into an internal semantic layout representation:

$$
T \rightarrow L
$$

For example:

> "The sofa faces the fireplace."

can be transformed into a relationship representation:

$$
R_{1} =
(\text{sofa},\text{faces},\text{fireplace})
$$

The model can then learn how this semantic relationship corresponds to visual spatial configurations.

---

# 2.13 Object Representation

Interior images contain multiple objects that must be simultaneously generated.

A scene can be represented as:

$$
O=\{o_1,o_2,\ldots,o_N\}
$$

where each object can have attributes:

$$
o_i =
(type_i,
color_i,
material_i,
size_i,
shape_i,
orientation_i)
$$

For example:

$$
o_1 =
(\text{sofa},
\text{beige},
\text{fabric},
\text{large},
\text{sectional},
0^\circ)
$$

The difficulty is not simply recognizing the word "sofa."

The model must associate:

> beige + sectional + fabric + large + facing fireplace

with a consistent visual object.

This motivates a dedicated **object semantic space** in the proposed architecture.

---

# 2.14 Material Representation

Materials are especially important in interior visualization because they strongly affect perceived realism and design identity.

Typical interior materials include:

- wood,
- marble,
- concrete,
- glass,
- metal,
- leather,
- fabric,
- ceramic,
- stone.

Material appearance depends on several visual properties:

$$
M =
(color,
texture,
roughness,
specularity,
reflectance)
$$

Two rooms may have identical furniture arrangements but appear substantially different because of their material configuration.

For example:

```text
Room A
oak floor
+
white marble
+
linen sofa
```

versus:

```text
Room B
dark walnut floor
+
black granite
+
leather sofa
```

Therefore, material should not necessarily be treated as a generic adjective within a single text embedding.

The proposed research investigates whether explicit material conditioning can improve material consistency.

---

# 2.15 Style Representation

Interior style represents a high-level visual organization.

Examples include:

- modern,
- minimalist,
- Scandinavian,
- Japanese,
- industrial,
- classical,
- contemporary,
- luxury,
- rustic.

Style is fundamentally different from an individual object.

For example:

> "Scandinavian"

does not correspond to one specific image object.

Instead, it influences:

- furniture selection,
- color palette,
- material selection,
- spatial simplicity,
- lighting,
- decoration,
- composition.

Therefore, style can be interpreted as a **global semantic condition**.

A useful conceptual representation is:

$$
C_S=f_S(T)
$$

where $f_S$ extracts stylistic information from the natural-language description.

This condition can influence many image tokens simultaneously.

---

# 2.16 Lighting Representation

Lighting strongly influences the visual appearance of interior images.

A lighting description may contain:

$$
Li =
(source,
direction,
intensity,
temperature,
shadow,
ambient)
$$

For example:

> "Warm indirect lighting combined with soft natural light from large south-facing windows."

contains information about:

- artificial light,
- natural light,
- direction,
- temperature,
- softness,
- spatial origin.

Lighting is therefore neither purely an object attribute nor purely a global style attribute.

It can affect the entire image while simultaneously interacting with:

- materials,
- windows,
- furniture,
- walls,
- shadows,
- reflections.

This makes lighting an important candidate for specialized conditioning.

---

# 2.17 Relationship Representation

One of the most important distinctions between ordinary image descriptions and interior-design descriptions is the importance of relationships.

Consider:

> "A sofa faces the fireplace, with a coffee table between them."

The description contains three objects:

$$
V =
\{
\text{sofa},
\text{fireplace},
\text{coffee table}
\}
$$

and several relationships:

$$
E =
\{
(\text{sofa},\text{faces},\text{fireplace}),
(\text{coffee table},\text{between},\text{sofa/fireplace})
\}
$$

This can be represented as a graph:

$$
G=(V,E)
$$

where:

- $V$ = design entities,
- $E$ = semantic relationships.

This relationship graph can potentially provide information that is difficult to preserve in a single sequence-level embedding.

The proposed research therefore treats relationships as a separate semantic space.

---

# 2.18 Multi-Condition Generation

Existing controllable generation research demonstrates that multiple conditions can be incorporated into image generation.

For example, ControlNet demonstrated the use of multiple spatial controls, while T2I-Adapter demonstrated composable conditioning mechanisms. ([arXiv](https://arxiv.org/abs/2302.05543?utm_source=chatgpt.com))

This suggests a more general formulation:

$$
x \sim p_\theta(x|
C_1,C_2,\ldots,C_K)
$$

For the proposed interior-design framework:

$$
x \sim p_\theta(
x|
C_L,
C_O,
C_M,
C_S,
C_{Li},
C_C,
C_A,
C_R)
$$

The challenge is then determining:

1. how the conditions should be extracted,
2. how they should be encoded,
3. how they should interact,
4. how they should be injected into DiT,
5. how conflicting conditions should be resolved,
6. how their individual effects should be evaluated.

These questions define a substantial part of the proposed research.

---

# 2.19 Semantic Decomposition

The proposed research introduces semantic decomposition as an intermediate representation between language and image generation.

The process is:

$$
T
\rightarrow
S
\rightarrow
C
\rightarrow
I
$$

where:

- $T$ = natural-language description,
- $S$ = structured semantic representation,
- $C$ = encoded conditions,
- $I$ = generated image.

The semantic representation is defined as:

$$
S =
\{
L,O,M,S_t,Li,C,A,R
\}
$$

where:

$$
L=\text{Layout}
$$

$$
O=\text{Object}
$$

$$
M=\text{Material}
$$

$$
S_t=\text{Style}
$$

$$
Li=\text{Lighting}
$$

$$
C=\text{Color}
$$

$$
A=\text{Architecture}
$$

$$
R=\text{Relationship}
$$

The representation is therefore **multi-dimensional rather than monolithic**.

This is the theoretical foundation of the proposed model.

---

# 2.20 Architectural Design Applications of Generative AI

Generative AI is increasingly being investigated for architectural and design ideation.

Paananen et al. studied text-to-image generation in architectural design ideation and reported that such systems can participate in early-stage creative workflows, while also emphasizing the importance of design constraints. ([Sage Journals](https://journals.sagepub.com/doi/10.1177/14780771231222783?utm_source=chatgpt.com))

More recent research has moved toward greater controllability.

For example, recent work on architectural generation has investigated multi-stage retrieval-augmented diffusion to improve alignment with architectural design intent, including control of building configurations, components, and rendering styles. ([DOI](https://doi.org/10.1016/j.foar.2026.01.018?utm_source=chatgpt.com))

This development reflects an important transition:

```text
Generation
   ↓
Controllable Generation
   ↓
Design-Intent Alignment
   ↓
Constraint-Aware Generation
```

The proposed research is positioned within this transition but focuses specifically on **text-only interior-design generation**.

---

# 2.21 Interior-Design-Specific Generative Research

Interior design is a particularly challenging domain because images must simultaneously satisfy visual realism and design structure.

Recent work has explicitly investigated diffusion-based interior generation. For example, DiffDesign proposes a controllable diffusion approach for interior design and separates appearance-related information from design specifications. ([PLOS](https://journals.plos.org/plosone/article?id=10.1371%2Fjournal.pone.0331240&utm_source=chatgpt.com))

More recent research has also investigated layout-preserving multi-conditional diffusion Transformers for interior design, using multiple structural inputs such as segmentation, depth, and line drawings. ([ScienceDirect](https://www.sciencedirect.com/science/article/pii/S0957417426012650?utm_source=chatgpt.com))

These studies demonstrate the importance of structural control.

However, they also expose an important research distinction.

Many controllable interior-generation systems use:

- sketches,
- segmentation maps,
- depth maps,
- line drawings,
- spatial layouts,
- reference images.

The proposed research instead investigates whether **these design constraints can be inferred internally from natural language**.

Thus, the proposed framework does not require the user to explicitly construct an external spatial representation.

---

# 2.22 Interior Scene Datasets

Dataset selection is critical for training a model capable of generating interior images.

Several existing datasets provide useful visual or structural information.

## 2.22.1 COCO

MS COCO contains complex everyday scenes with object annotations and instance segmentation. The dataset contains approximately 328,000 images and millions of labeled object instances. ([arXiv](https://arxiv.org/abs/1405.0312?utm_source=chatgpt.com))

Its advantages include:

- large-scale image data,
- object annotations,
- segmentation,
- established evaluation protocols.

However, COCO is not specialized for interior design.

Therefore:

$$
\text{COCO} \neq \text{Interior Design Dataset}
$$

although it can provide useful general visual pretraining information.

---

## 2.22.2 Conceptual Captions

Conceptual Captions provides large-scale image-text data designed for vision-language learning. Google's dataset resources provide training and validation data for image-captioning research. ([Google AI](https://ai.google.com/research/ConceptualCaptions/download?utm_source=chatgpt.com))

Such datasets are useful for learning general associations between language and visual concepts.

However, general web image-caption datasets may not contain the detailed design terminology required for interior generation.

---

# 2.23 Structured3D

Structured3D is particularly relevant to the proposed research because it was created from professional interior designs and provides rich structural annotations.

The dataset contains approximately 3,500 house designs and provides information such as:

- semantic structure,
- depth,
- surface normals,
- layout,
- albedo,
- 3D structure.

([Structured3D Dataset](https://structured3d-dataset.org/?utm_source=chatgpt.com))

This makes Structured3D useful for learning relationships between visual appearance and interior structure.

Its limitation is that it is primarily a structured synthetic dataset rather than a large collection of naturally written interior-design descriptions.

Consequently, it can support structural learning but does not completely solve the text-to-interior problem.

---

# 2.24 InteriorNet

InteriorNet provides large-scale synthetic indoor scenes.

The dataset describes approximately 20 million images generated using around one million CAD furniture models and approximately 22 million interior layouts created by professional designers. ([InteriorNet](https://interiornet.org/?utm_source=chatgpt.com))

This makes it valuable for learning:

- indoor geometry,
- furniture appearance,
- spatial relationships,
- lighting variation,
- room configurations.

However, as with Structured3D, synthetic structural information does not automatically provide natural-language design descriptions.

A research challenge therefore remains:

$$
\text{Structured Interior Data}
+
\text{Natural Language}
\rightarrow
\text{Unified Training Representation}
$$

---

# 2.25 3D-FRONT and Related Interior Data

3D-FRONT provides structured indoor scenes containing room layouts and furniture information.

Derived rendering resources can provide photorealistic indoor scenes with information such as:

- instance segmentation,
- object poses,
- camera parameters,
- depth,
- room layouts.

For example, one publicly available render collection based on 3D-FRONT/3D-FUTURE contains over 20,000 photorealistic indoor scene renders with structural annotations. ([Hugging Face](https://huggingface.co/datasets/Spatial1ntelligence/3d-front-indoor-renders?utm_source=chatgpt.com))

These resources are particularly useful for constructing semantic representations of interior scenes.

The challenge remains the same:

> The model needs to learn how natural-language design descriptions correspond to these structured scene properties.

---

# 2.26 Dataset Requirements for the Proposed Research

Based on the literature, a suitable training dataset should ideally contain:

$$
D=
\{(T,I,S)\}
$$

where:

- $T$ = natural-language description,
- $I$ = interior image,
- $S$ = structured semantic representation.

The semantic representation can be:

$$
S=
\{L,O,M,S_t,Li,C,A,R\}
$$

A hypothetical example is:

```text
Text:
"Modern Scandinavian living room with a beige sofa facing
a marble fireplace, oak floor, warm lighting and large windows."

Semantic representation:

Layout:
    sofa → facing → fireplace

Object:
    sofa
    fireplace
    windows

Material:
    oak
    marble
    fabric

Style:
    Scandinavian
    modern

Lighting:
    warm

Color:
    beige

Architecture:
    large windows

Relationship:
    sofa → faces → fireplace
```

This structured representation can be generated through:

1. manual annotation,
2. expert annotation,
3. rule-based extraction,
4. NLP models,
5. language-model-assisted annotation,
6. hybrid automatic + human validation.

For a doctoral study, the annotation methodology should itself be documented and evaluated.

---

# 2.27 Evaluation of Text-to-Image Generation

Evaluation of generative images is difficult because visual quality and semantic correctness are different properties.

A generated image may be:

- photorealistic but semantically incorrect,
- semantically correct but visually unrealistic,
- stylistically correct but spatially incorrect,
- visually attractive but inconsistent with the requested materials.

Therefore, evaluation should be multidimensional.

---

## 2.27.1 Image Quality

Traditional image-generation metrics include:

- Fréchet Inception Distance (FID),
- Kernel Inception Distance (KID),
- Inception Score.

FID can be expressed conceptually as:

$$
FID =
\|\mu_r-\mu_g\|^2
+
Tr(
\Sigma_r+\Sigma_g
-2(\Sigma_r\Sigma_g)^{1/2}
)
$$

where the distributions of real and generated image features are compared.

FID can provide information about distribution-level image similarity, but it does not directly measure whether a specific interior-design prompt was satisfied.

---

# 2.27.2 Text-Image Alignment

CLIP-based similarity can evaluate semantic alignment between generated images and text.

A simplified measure is:

$$
S_{CLIP}
=
\cos
(E_I(I),
E_T(T))
$$

where:

- $E_I$ is the image encoder,
- $E_T$ is the text encoder.

This is useful for measuring global text-image alignment.

However, it may not sufficiently distinguish whether every design requirement has been satisfied.

---

# 2.27.3 Object Accuracy

Object-level evaluation can measure whether requested objects are present.

For a prompt containing $N$ required objects:

$$
A_{object}
=
\frac{N_{correct}}{N}
$$

For example:

```text
Required:
✓ sofa
✓ fireplace
✓ coffee table
✓ floor lamp
```

The generated image can be analyzed using an object detector or human annotation.

---

# 2.27.4 Material Accuracy

Material consistency can be measured by determining whether requested materials are correctly represented.

For example:

$$
A_{material}
=
\frac{M_{correct}}{M_{required}}
$$

This can be evaluated using:

- material classifiers,
- segmentation,
- vision-language models,
- human experts.

---

# 2.27.5 Relationship Accuracy

Relationship accuracy is especially important for the proposed research.

Suppose the prompt specifies:

$$
R=
\{
r_1,r_2,\ldots,r_n
\}
$$

Then:

$$
A_{relation}
=
\frac{
|R_{correct}|
}{
|R|
}
$$

For example:

```text
Prompt:
sofa faces fireplace
coffee table between sofa and fireplace
lamp beside sofa
```

The generated image can be evaluated against these relationships.

This metric is potentially one of the most important evaluation dimensions of the dissertation because it directly tests the motivation for explicit relationship conditioning.

---

# 2.27.6 Human Evaluation

Human evaluation remains important because interior design contains aesthetic and functional properties that automated metrics may not fully capture.

Expert evaluators can rate:

- visual quality,
- design coherence,
- spatial plausibility,
- material realism,
- style consistency,
- lighting quality,
- prompt compliance.

A structured evaluation form can therefore be developed for professional interior designers or architecture experts.

---

# 2.28 Research Gap

The literature reviewed in this chapter reveals several gaps.

### Gap 1: Text-to-image models provide strong global semantic understanding but limited explicit decomposition

Modern text-to-image models demonstrate strong language-image alignment. ([arXiv](https://arxiv.org/abs/2103.00020?utm_source=chatgpt.com))

However, a natural-language interior description contains multiple heterogeneous semantic dimensions.

---

### Gap 2: Controllable diffusion often requires external visual conditions

ControlNet and T2I-Adapter demonstrate strong controllability using external conditions such as structural maps, depth, segmentation, and color. ([arXiv](https://arxiv.org/abs/2302.05543?utm_source=chatgpt.com))

This creates an additional burden for users.

The proposed research instead investigates:

$$
\text{Text}
\rightarrow
\text{Internal Conditions}
\rightarrow
\text{Image}
$$

---

### Gap 3: Layout-aware generation does not necessarily solve complete interior-design semantics

LayoutDiffusion demonstrates the importance of spatial and object-aware conditioning. ([openaccess.thecvf.com](https://openaccess.thecvf.com/content/CVPR2023/html/Zheng_LayoutDiffusion_Controllable_Diffusion_Model_for_Layout-to-Image_Generation_CVPR_2023_paper.html?utm_source=chatgpt.com))

However, interior design requires more than spatial layout.

It requires simultaneous consideration of:

$$
\{
layout,
object,
material,
style,
lighting,
color,
architecture,
relationship
\}
$$

---

### Gap 4: Existing interior-generation research is increasingly controllable but frequently relies on non-text conditions

Recent interior-design systems demonstrate improvements through structural controls and multi-condition inputs. ([PLOS](https://journals.plos.org/plosone/article?id=10.1371%2Fjournal.pone.0331240&utm_source=chatgpt.com))

The unresolved question is whether comparable structural control can be obtained **from natural language alone**.

---

### Gap 5: Evaluation commonly emphasizes image quality rather than complete design requirement satisfaction

Metrics such as FID and CLIP similarity provide valuable information but do not fully measure:

$$
\text{Does the image satisfy the complete design specification?}
$$

A more comprehensive evaluation framework is therefore needed.

---

# 2.29 Research Gap Summary

The literature can be summarized as follows:

| Research direction | Main capability | Remaining limitation |
|---|---|---|
| GAN | Realistic image synthesis | Training/control limitations |
| VAE | Latent representation | Reconstruction/generation fidelity |
| Diffusion | High-quality generation | Computational cost and conditioning complexity |
| Latent Diffusion | Efficient high-resolution generation | Semantic decomposition remains implicit |
| CLIP/Vision-Language | Text-image alignment | Global alignment does not guarantee design constraints |
| Imagen-style T2I | Strong language understanding | Complex design relationships remain difficult |
| DiT | Scalable Transformer diffusion | Requires suitable condition architecture |
| ControlNet | Strong spatial control | Usually requires external spatial inputs |
| T2I-Adapter | Additional controllability | External condition design remains necessary |
| LayoutDiffusion | Object/layout control | Primarily layout-oriented |
| Interior diffusion research | Domain-specific generation | Often relies on structural inputs |
| Proposed research | Text-only multi-space conditioning | Requires validation |

The resulting research opportunity can therefore be expressed as:

$$
\boxed{
\text{Natural Language}
\rightarrow
\text{Multi-Space Semantic Representation}
\rightarrow
\text{Multi-Condition DiT}
\rightarrow
\text{Interior Image}
}
$$

---

# 2.30 Proposed Research Framework

Based on the literature, this dissertation proposes a framework consisting of five major stages.

```text
                 NATURAL LANGUAGE
                       │
                       ▼
             ┌───────────────────┐
             │ Language Encoder  │
             └─────────┬─────────┘
                       │
                       ▼
             ┌───────────────────┐
             │ Semantic          │
             │ Decomposition     │
             └─────────┬─────────┘
                       │
        ┌──────────────┼──────────────┐
        │              │              │
        ▼              ▼              ▼
     Layout         Object        Material
        │              │              │
        ├──────────────┼──────────────┤
        ▼              ▼              ▼
      Style         Lighting        Color
        │              │              │
        ├──────────────┼──────────────┤
        ▼              ▼
   Architecture    Relations
        │              │
        └───────┬──────┘
                ▼
       CONDITION FUSION /
        CONDITION ROUTER
                │
                ▼
      MULTI-CONDITION DiT
                │
                ▼
          LATENT IMAGE
                │
                ▼
           VAE DECODER
                │
                ▼
        INTERIOR IMAGE
```

---

# 2.31 Theoretical Model

The proposed model can be formulated as:

$$
T
\xrightarrow{f_{lang}}
H
\xrightarrow{f_{sem}}
S
\xrightarrow{f_{cond}}
C
\xrightarrow{f_{DiT}}
Z
\xrightarrow{D}
I
$$

where:

- $T$ = natural-language input,
- $H$ = language representation,
- $S$ = semantic decomposition,
- $C$ = multi-condition representation,
- $Z$ = generated image latent,
- $I$ = final interior image.

The semantic decomposition is:

$$
S =
\{
S_L,S_O,S_M,S_{St},S_{Li},S_C,S_A,S_R
\}
$$

and the corresponding conditions are:

$$
C =
\{
C_L,C_O,C_M,C_{St},C_{Li},C_C,C_A,C_R
\}
$$

The diffusion model then learns:

$$
p_\theta(Z|C)
$$

rather than relying exclusively on:

$$
p_\theta(Z|T)
$$

This represents the central theoretical difference between the proposed method and conventional text-only conditioning.

---

# 2.32 Condition Routing

A central component of the proposed framework is the **condition router**.

The purpose of the router is to determine how strongly each semantic space should influence different image tokens.

Let:

$$
H_i
$$

represent the $i$-th image token.

The router can compute:

$$
\alpha_{i,k}
=
softmax_k
(
q(H_i)^T k(C_k)
)
$$

where $C_k$ represents one semantic condition.

The resulting representation is:

$$
C_i^*
=
\sum_k
\alpha_{i,k}v(C_k)
$$

This allows different image tokens to receive different semantic influences.

For example:

```text
Image region             Dominant conditions

Sofa                     Object + Material + Color
Floor                    Material + Color
Window                   Architecture + Object
Fireplace                Object + Material
Whole room               Style + Lighting
Object placement         Layout + Relationship
```

This provides a conceptual mechanism for combining heterogeneous design information without forcing all conditions into a single embedding.

---

# 2.33 Hierarchical Semantic Conditioning

Interior design naturally contains multiple semantic scales.

### Global level

- style,
- overall color palette,
- atmosphere,
- lighting.

### Regional level

- living area,
- dining area,
- bedroom area,
- wall region.

### Object level

- sofa,
- table,
- lamp,
- bed,
- cabinet.

### Relationship level

- beside,
- behind,
- facing,
- between,
- above,
- attached to.

Therefore, the proposed framework can be viewed as hierarchical:

$$
C =
C_{global}
+
C_{regional}
+
C_{object}
+
C_{relation}
$$

This hierarchy provides a theoretical explanation for why a simple concatenation of semantic embeddings may not be sufficient.

---

# 2.34 Research Hypotheses

The literature supports the following research hypotheses.

### H1 - Semantic Decomposition Hypothesis

Multi-space semantic decomposition will improve text-image semantic alignment compared with conventional single-space text conditioning.

### H2 - Object Representation Hypothesis

Explicit object representations will improve the accuracy of required object generation.

### H3 - Relationship Hypothesis

Explicit relationship conditioning will improve spatial and relational consistency between interior objects.

### H4 - Material Hypothesis

Explicit material conditioning will improve material consistency in generated interior images.

### H5 - Style and Lighting Hypothesis

Separate style and lighting conditions will improve consistency of global visual appearance.

### H6 - Multi-Condition DiT Hypothesis

A DiT architecture equipped with multiple specialized semantic conditions will improve overall design-requirement satisfaction compared with a conventional text-conditioned diffusion baseline.

These hypotheses will be experimentally tested in Chapter 4.

---

# 2.35 Conceptual Comparison With Existing Approaches

The conceptual difference can be summarized as:

```text
Conventional T2I

Text
 ↓
Text Encoder
 ↓
Single Condition
 ↓
Diffusion Model
 ↓
Image
```

versus:

```text
Proposed Method

Text
 ↓
Language Encoder
 ↓
Semantic Decomposition
 ↓
 ┌────────┬────────┬────────┬────────┐
 Layout  Object  Material  Style
 ├────────┼────────┼────────┼────────┤
Lighting Color Architecture Relations
 └────────┴────────┴────────┴────────┘
                 ↓
          Condition Router
                 ↓
       Multi-Condition DiT
                 ↓
             VAE Decoder
                 ↓
          Interior Image
```

The proposed architecture therefore moves the research focus from simply increasing generative capacity toward **representational structure and controllability**.

---

# 2.36 Chapter Summary

This chapter reviewed the theoretical and technological foundations of text-driven interior image generation.

Traditional interior visualization provides explicit control over geometry, objects, materials, lighting, and spatial relationships, but requires substantial manual modeling and technical expertise.

Generative models provide an alternative mechanism for converting semantic descriptions into visual outputs. GANs and VAEs established important foundations for generative representation learning, while diffusion models provided a powerful framework for high-quality image synthesis. ([arXiv](https://arxiv.org/abs/2006.11239?utm_source=chatgpt.com))

Latent diffusion reduced the computational cost of high-resolution generation and introduced flexible conditioning mechanisms. ([openaccess.thecvf.com](https://openaccess.thecvf.com/content/CVPR2022/html/Rombach_High-Resolution_Image_Synthesis_With_Latent_Diffusion_Models_CVPR_2022_paper.html?utm_source=chatgpt.com)) Vision-language models established strong relationships between natural language and visual concepts, while text-to-image diffusion systems demonstrated increasingly strong text-image alignment. ([arXiv](https://arxiv.org/abs/2103.00020?utm_source=chatgpt.com))

Diffusion Transformers further provide a scalable Transformer-based architecture for diffusion generation. ([arXiv](https://arxiv.org/abs/2212.09748?utm_source=chatgpt.com)) Meanwhile, ControlNet, T2I-Adapter, and layout-aware diffusion demonstrate that explicit conditioning can substantially improve controllability. ([arXiv](https://arxiv.org/abs/2302.05543?utm_source=chatgpt.com))

Research in architecture and interior design confirms the potential of generative AI for design ideation while also highlighting the importance of design constraints and structural consistency. ([Sage Journals](https://journals.sagepub.com/doi/10.1177/14780771231222783?utm_source=chatgpt.com))

The literature therefore indicates a research opportunity at the intersection of:

$$
\boxed{
\text{Natural Language}
+
\text{Semantic Decomposition}
+
\text{Interior Design Knowledge}
+
\text{Multi-Condition Diffusion}
+
\text{Diffusion Transformer}
}
$$

The central research gap is the development and systematic evaluation of a **text-only generative framework capable of transforming natural-language interior-design requirements into coherent images while explicitly representing multiple semantic dimensions and their relationships**.

The next chapter consequently focuses on the design of the proposed methodology, including the semantic decomposition module, specialized semantic spaces, condition encoders, condition router, multi-condition DiT, training objectives, inference process, and evaluation framework.

### Key literature sources

- Rombach et al., *High-Resolution Image Synthesis with Latent Diffusion Models* - latent diffusion and conditional generation. ([openaccess.thecvf.com](https://openaccess.thecvf.com/content/CVPR2022/html/Rombach_High-Resolution_Image_Synthesis_With_Latent_Diffusion_Models_CVPR_2022_paper.html?utm_source=chatgpt.com))
- Peebles & Xie, *Scalable Diffusion Models with Transformers* - DiT architecture. ([arXiv](https://arxiv.org/abs/2212.09748?utm_source=chatgpt.com))
- Saharia et al., *Photorealistic Text-to-Image Diffusion Models with Deep Language Understanding* - text-conditioned diffusion and language representations. ([arXiv](https://arxiv.org/abs/2205.11487?utm_source=chatgpt.com))
- Radford et al., *Learning Transferable Visual Models From Natural Language Supervision* - CLIP and vision-language alignment. ([arXiv](https://arxiv.org/abs/2103.00020?utm_source=chatgpt.com))
- Zhang et al., *Adding Conditional Control to Text-to-Image Diffusion Models* - ControlNet. ([arXiv](https://arxiv.org/abs/2302.05543?utm_source=chatgpt.com))
- Mou et al., *T2I-Adapter* - composable additional conditioning. ([arXiv](https://arxiv.org/abs/2302.08453?utm_source=chatgpt.com))
- Zheng et al., *LayoutDiffusion* - layout and object-aware diffusion. ([openaccess.thecvf.com](https://openaccess.thecvf.com/content/CVPR2023/html/Zheng_LayoutDiffusion_Controllable_Diffusion_Model_for_Layout-to-Image_Generation_CVPR_2023_paper.html?utm_source=chatgpt.com))
- Paananen et al., *Using text-to-image generation for architectural design ideation* - architectural design application. ([Sage Journals](https://journals.sagepub.com/doi/10.1177/14780771231222783?utm_source=chatgpt.com))
- Zheng et al., *Structured3D* - structured interior dataset. ([arXiv](https://arxiv.org/abs/1908.00222?utm_source=chatgpt.com))
- Li et al., *InteriorNet* - large-scale interior scene dataset. ([InteriorNet](https://interiornet.org/?utm_source=chatgpt.com))

---

**Sources:**

- [Denoising Diffusion Probabilistic Models](https://arxiv.org/abs/2006.11239?utm_source=chatgpt.com)
- [Using text-to-image generation for architectural design ideation - Ville Paananen, Jonas Oppenlaender, Aku Visuri, 2024](https://journals.sagepub.com/doi/10.1177/14780771231222783?utm_source=chatgpt.com)
- [CVPR 2022 Open Access Repository](https://openaccess.thecvf.com/content/CVPR2022/html/Rombach_High-Resolution_Image_Synthesis_With_Latent_Diffusion_Models_CVPR_2022_paper.html?utm_source=chatgpt.com)
- [Controllable generation of building representations: Aligning campus building design intent with multi-stage retrieval-augmented diffusion models - ScienceDirect](https://doi.org/10.1016/j.foar.2026.01.018?utm_source=chatgpt.com)
- [DiffDesign: Controllable diffusion with meta prior for efficient interior design generation | PLOS One](https://journals.plos.org/plosone/article?id=10.1371%2Fjournal.pone.0331240&utm_source=chatgpt.com)
- [LayGenID: Layout-preserving multi-conditional diffusion transformer with LoRA fusion for interior design generation - ScienceDirect](https://www.sciencedirect.com/science/article/pii/S0957417426012650?utm_source=chatgpt.com)
- [Conceptual Captions](https://ai.google.com/research/ConceptualCaptions/download?utm_source=chatgpt.com)
- [Structured3D Dataset](https://structured3d-dataset.org/?utm_source=chatgpt.com)
- [InteriorNet: Mega-scale Multi-sensor Photo-realistic Indoor Scenes Dataset](https://interiornet.org/?utm_source=chatgpt.com)
- [Spatial1ntelligence/3d-front-indoor-renders · Datasets at Hugging Face](https://huggingface.co/datasets/Spatial1ntelligence/3d-front-indoor-renders?utm_source=chatgpt.com)

## Prompt:
9/28/2026, 6:25:40 PM

next, generate chapter3

## Response:
9/28/2026, 6:25:42 PM · gpt-5-6-mini

> **Searched 2 websites**
>
> 
>
> Worked for a couple of seconds

Chapter 3 should be the **core methodology chapter**: it should move from the research gap established in Chapter 2 to a precise, implementable architecture. I have written it so that the proposed components are clearly distinguished from established DiT/LDM mechanisms. Latent diffusion provides the compressed image-space foundation, while DiT provides the Transformer denoising backbone. ([openaccess.thecvf.com](https://openaccess.thecvf.com/content/CVPR2022/html/Rombach_High-Resolution_Image_Synthesis_With_Latent_Diffusion_Models_CVPR_2022_paper.html?utm_source=chatgpt.com))

# Chapter 3 - Proposed Text-Driven Interior Image Generation Method

## 3.1 Introduction

Chapter 2 reviewed existing approaches to image generation, text-to-image synthesis, diffusion models, Diffusion Transformers, controllable generation, layout-aware generation, and interior-design visualization. The review identified a central limitation in current approaches: although modern generative models can establish strong relationships between natural language and visual appearance, a complex interior-design description contains several distinct semantic dimensions that must be simultaneously satisfied.

An interior-design description may specify:

- room type;
- architectural structure;
- furniture;
- object attributes;
- spatial arrangement;
- object-to-object relationships;
- materials;
- colors;
- design style;
- lighting;
- atmosphere.

Representing all these requirements exclusively through a single undifferentiated text-conditioning representation may make it difficult to explicitly control how individual requirements affect the generated scene.

This chapter proposes a **Multi-Space Semantic Conditioning Diffusion Transformer (MSC-DiT)** framework for text-only interior image generation.

The central idea is:

$$
\boxed{
T
\rightarrow
S
\rightarrow
C
\rightarrow
\text{MSC-DiT}
\rightarrow
Z
\rightarrow
I
}
$$

where:

- $T$ = natural-language interior description;
- $S$ = structured semantic representation;
- $C$ = multiple semantic conditioning representations;
- $Z$ = generated image latent;
- $I$ = final interior image.

Unlike systems that require a user to provide a floor plan, segmentation map, depth map, sketch, or reference image, the proposed framework accepts **natural language as the only user-provided input**.

The additional design conditions are generated internally from the text.

---

# 3.2 Research Objective

The objective of the proposed method is to develop a text-driven generative architecture capable of producing interior images that satisfy multiple simultaneous design requirements.

The desired mapping is:

$$
f_\theta:
T
\rightarrow
I
$$

However, instead of directly learning this mapping through a single text-conditioning pathway, the proposed method introduces an intermediate semantic representation:

$$
T
\rightarrow
S
\rightarrow
I
$$

where:

$$
S=
\{
L,O,M,St,Li,C,A,R
\}
$$

with:

| Symbol | Semantic space |
|---|---|
| $L$ | Layout |
| $O$ | Object |
| $M$ | Material |
| $St$ | Style |
| $Li$ | Lighting |
| $C$ | Color |
| $A$ | Architecture |
| $R$ | Relationship |

The purpose is not simply to create eight independent embeddings.

Instead, each semantic space is intended to represent a different type of design information and subsequently influence the generative process through an appropriate conditioning mechanism.

---

# 3.3 Overall System Architecture

The complete proposed architecture is shown conceptually below.

```text
                  NATURAL LANGUAGE
                         │
                         ▼
              ┌─────────────────────┐
              │ Text Preprocessing  │
              └──────────┬──────────┘
                         │
                         ▼
              ┌─────────────────────┐
              │ Language Encoder    │
              └──────────┬──────────┘
                         │
                         ▼
              ┌─────────────────────┐
              │ Semantic            │
              │ Decomposition       │
              └──────────┬──────────┘
                         │
        ┌────────────────┼────────────────┐
        │                │                │
        ▼                ▼                ▼
     Layout           Object          Material
        │                │                │
        ▼                ▼                ▼
     Encoder           Encoder          Encoder
        │                │                │
        ├────────────────┼────────────────┤
        │                │                │
        ▼                ▼                ▼
      Style           Lighting          Color
        │                │                │
        ▼                ▼                ▼
     Encoder           Encoder          Encoder
        │                │                │
        ├────────────────┼────────────────┤
        │                │
        ▼                ▼
 Architecture       Relationship
    Encoder             Encoder
        │                │
        └────────┬───────┘
                 ▼
       Condition Fusion
                 │
                 ▼
        Condition Router
                 │
                 ▼
        ┌───────────────────┐
        │     MSC-DiT       │
        │                   │
        │ Self-Attention    │
        │ Condition Routing │
        │ Cross-Attention   │
        │ AdaLN / Modulation│
        │ Feed Forward      │
        └─────────┬─────────┘
                  │
                  ▼
             Latent \(Z\)
                  │
                  ▼
             VAE Decoder
                  │
                  ▼
          Generated Interior
               Image
```

The architecture contains seven principal stages:

1. text preprocessing;
2. language encoding;
3. semantic decomposition;
4. specialized semantic encoding;
5. condition fusion and routing;
6. multi-condition Diffusion Transformer;
7. latent-to-image decoding.

---

# 3.4 Text-Only Input Principle

A fundamental design constraint of the proposed method is that the user provides only natural language.

For example:

> "A spacious contemporary living room with a large beige sectional sofa facing a white marble fireplace. A wooden coffee table is positioned between the sofa and fireplace, with warm indirect lighting, oak flooring, large windows, and minimalist Scandinavian furniture."

The user does **not** provide:

- a floor plan;
- a segmentation map;
- a depth map;
- a CAD model;
- a 3D scene;
- an edge map;
- a reference image.

Instead, the system automatically derives internal representations.

Therefore:

$$
T
\rightarrow
\{
L,O,M,St,Li,C,A,R
\}
$$

The distinction is important because existing controllable diffusion systems such as ControlNet can accept explicit spatial conditions such as edges, depth, segmentation, and pose. ([arXiv](https://arxiv.org/abs/2302.05543?utm_source=chatgpt.com))

The proposed framework investigates whether comparable semantic control can be obtained from **text-derived internal representations**.

---

# 3.5 Text Preprocessing

The first stage converts the raw natural-language input into a normalized textual representation.

Let:

$$
T_{raw}
$$

represent the original prompt.

The preprocessing function is:

$$
T=f_{pre}(T_{raw})
$$

The preprocessing stage can perform:

- normalization;
- sentence segmentation;
- punctuation normalization;
- noun phrase identification;
- adjective identification;
- relation phrase detection;
- design terminology normalization.

For example:

```text
"large beige sectional sofa"
```

can be normalized into:

```text
Object:
    category = sofa
    type = sectional
    size = large
    color = beige
```

Similarly:

```text
"warm indirect lighting"
```

can become:

```text
Lighting:
    temperature = warm
    type = indirect
```

The preprocessing stage should preserve the original semantic information rather than unnecessarily simplifying the prompt.

---

# 3.6 Language Encoder

The normalized text is converted into contextual language representations.

Let:

$$
H=f_{text}(T)
$$

where:

$$
H=
[h_1,h_2,\ldots,h_n]
$$

and each $h_i\in\mathbb{R}^{d}$ represents contextual information associated with a token.

A Transformer-based language encoder is suitable because contextual representations allow semantic relationships between words to be modeled.

For example:

```text
"The sofa faces the fireplace."
```

should not be interpreted merely as three independent concepts:

$$
\{
sofa,
faces,
fireplace
\}
$$

but as a structured relationship:

$$
(\text{sofa},\text{faces},\text{fireplace})
$$

Vision-language models such as CLIP demonstrated the usefulness of large-scale natural-language supervision for learning broad visual concepts. ([arXiv](https://arxiv.org/abs/2103.00020?utm_source=chatgpt.com))

In the proposed framework, the language encoder provides the foundational representation from which specialized semantic representations are extracted.

---

# 3.7 Semantic Decomposition Module

The semantic decomposition module is one of the central components of the proposed architecture.

It transforms the unified language representation into multiple semantic spaces.

$$
H
\rightarrow
\{
H_L,H_O,H_M,H_{St},
H_{Li},H_C,H_A,H_R
\}
$$

where:

$$
H_L=f_L(H)
$$

$$
H_O=f_O(H)
$$

$$
H_M=f_M(H)
$$

$$
H_{St}=f_{St}(H)
$$

$$
H_{Li}=f_{Li}(H)
$$

$$
H_C=f_C(H)
$$

$$
H_A=f_A(H)
$$

$$
H_R=f_R(H)
$$

Each function can be implemented using a lightweight Transformer/MLP projection, attention-based extractor, or learned semantic query mechanism.

The important architectural principle is that the representations are **specialized**.

---

# 3.8 Layout Semantic Space

The layout space represents spatial configuration.

Let:

$$
S_L=
\{
l_1,l_2,\ldots,l_N
\}
$$

where each $l_i$ represents spatial information associated with an entity.

The representation can conceptually contain:

$$
l_i=
(x_i,y_i,w_i,h_i,\theta_i,\rho_i)
$$

where:

- $x_i,y_i$ = spatial position;
- $w_i,h_i$ = relative size;
- $\theta_i$ = orientation;
- $\rho_i$ = regional context.

Because the user provides text rather than coordinates, these values do not necessarily have to be explicitly predicted as exact numerical coordinates.

The model may instead learn a latent representation of concepts such as:

- near;
- far;
- left;
- right;
- center;
- behind;
- in front of;
- against wall;
- facing;
- between.

For example:

$$
\text{"sofa facing fireplace"}
$$

becomes a semantic layout constraint:

$$
L_1=
(\text{sofa},
\text{facing},
\text{fireplace})
$$

---

# 3.9 Object Semantic Space

The object space represents entities in the interior.

$$
S_O=
\{o_1,o_2,\ldots,o_N\}
$$

Each object can be represented as:

$$
o_i=
(
c_i,
a_i,
s_i,
r_i
)
$$

where:

- $c_i$ = object category;
- $a_i$ = object attributes;
- $s_i$ = size information;
- $r_i$ = role in the scene.

For example:

$$
o_{sofa}=
(
\text{sofa},
\{
\text{sectional},
\text{beige},
\text{large}
\}
)
$$

This representation allows the model to distinguish between:

> sofa

and:

> large beige sectional sofa.

This distinction is important because object identity alone does not capture the full design requirement.

---

# 3.10 Material Semantic Space

The material representation describes surface and object materials.

$$
S_M=
\{m_1,m_2,\ldots,m_N\}
$$

A material representation may contain:

$$
m_i=
(
type,
color,
texture,
roughness,
reflectance
)
$$

For example:

$$
m_{floor}
=
(
oak,
brown,
woodgrain,
medium\ roughness,
low\ reflectance
)
$$

The representation does not require the language model to generate physically exact BRDF parameters.

Instead, these properties can form a learned semantic representation associated with the visual appearance of materials.

---

# 3.11 Style Semantic Space

Style represents high-level design characteristics.

$$
S_{St}
=
\{
st_1,\ldots,st_k
\}
$$

Examples include:

- modern;
- Scandinavian;
- minimalist;
- Japanese;
- industrial;
- classical;
- luxury.

Style is generally a global condition because it influences multiple aspects of the image simultaneously.

For example:

$$
C_{St}
\rightarrow
\{
object\ selection,
material,
color,
composition,
decoration,
lighting
\}
$$

Therefore, style information should have a relatively broad receptive influence within the image-generation process.

---

# 3.12 Lighting Semantic Space

Lighting information is represented as:

$$
S_{Li}
=
\{
source,
direction,
intensity,
temperature,
shadow,
ambient
\}
$$

For example:

```text
Warm indirect lighting
```

may be represented as:

$$
Li=
(
warm,
indirect,
soft,
ambient
)
$$

The representation should interact with material and architecture because lighting effects depend on the surfaces and geometry of the room.

Thus:

$$
C_{Li}
\leftrightarrow
C_M
$$

and:

$$
C_{Li}
\leftrightarrow
C_A
$$

are expected to be important interactions.

---

# 3.13 Color Semantic Space

Color information represents the palette and color relationships.

$$
S_C=
\{
C_{primary},
C_{secondary},
C_{accent}
\}
$$

For example:

```text
neutral beige interior with dark walnut accents
```

can be represented as:

$$
C=
\{
beige,
neutral,
dark\ walnut
\}
$$

Color conditions may operate at both global and object levels.

For example:

$$
C_{global}
$$

controls the general palette, while:

$$
C_{object}
$$

controls object-specific colors.

---

# 3.14 Architecture Semantic Space

Architecture represents fixed or semi-fixed structural elements.

$$
S_A=
\{
a_1,a_2,\ldots,a_N
\}
$$

Examples include:

- walls;
- windows;
- doors;
- ceiling;
- columns;
- fireplace;
- stairs;
- built-in cabinets.

Architectural features are different from movable furniture.

For example:

$$
\text{window}
\neq
\text{sofa}
$$

because the window forms part of the room structure.

This distinction can help the model separate:

$$
\text{room structure}
$$

from:

$$
\text{furniture configuration}.
$$

---

# 3.15 Relationship Semantic Space

The relationship space represents interactions among design entities.

The scene can be represented as:

$$
G=(V,E)
$$

where:

$$
V=\{v_1,v_2,\ldots,v_N\}
$$

represents entities and:

$$
E=\{e_1,e_2,\ldots,e_M\}
$$

represents relationships.

Each relationship can be represented as:

$$
e_{ij}
=
(v_i,r_{ij},v_j)
$$

For example:

$$
(\text{sofa},\text{faces},\text{fireplace})
$$

$$
(\text{coffee table},\text{between},\text{sofa/fireplace})
$$

$$
(\text{lamp},\text{beside},\text{sofa})
$$

The relationship encoder therefore transforms the natural-language relation into a structured representation.

This component is particularly important because spatial correctness cannot always be inferred from object presence alone.

---

# 3.16 Specialized Semantic Encoders

After semantic decomposition, each semantic space is projected into the common DiT conditioning dimension.

For semantic category $k$:

$$
C_k=E_k(S_k)
$$

where $E_k$ is the specialized semantic encoder.

Thus:

$$
C_L=E_L(S_L)
$$

$$
C_O=E_O(S_O)
$$

$$
C_M=E_M(S_M)
$$

and so forth.

All conditions are projected into:

$$
C_k\in\mathbb{R}^{N_k\times d_c}
$$

where:

- $N_k$ = number of condition tokens;
- $d_c$ = common conditioning dimension.

---

# 3.17 Condition Fusion

The condition encoder outputs are combined without simply concatenating them into one undifferentiated vector.

The proposed representation is:

$$
C=
\{
C_L,C_O,C_M,C_{St},
C_{Li},C_C,C_A,C_R
\}
$$

A lightweight global fusion module can produce:

$$
C_G=f_{fusion}(C)
$$

while retaining the individual conditions.

Therefore, two representations are maintained:

### Specialized representation

$$
C_k
$$

### Global representation

$$
C_G
$$

This allows the DiT to use both:

- global scene information;
- specialized semantic information.

---

# 3.18 Condition Router

The proposed condition router determines which semantic condition should influence each image token.

Let the noisy latent image be converted into tokens:

$$
X=
[x_1,x_2,\ldots,x_N]
$$

For each image token $x_i$, the router calculates attention over semantic conditions.

$$
q_i=W_qx_i
$$

$$
k_k=W_kC_k
$$

and:

$$
\alpha_{i,k}
=
\frac{
\exp(q_i^Tk_k/\sqrt d)
}{
\sum_j
\exp(q_i^Tk_j/\sqrt d)
}
$$

The resulting routed condition is:

$$
r_i
=
\sum_k
\alpha_{i,k}
W_vC_k
$$

Thus each image token can receive a different mixture of semantic information.

Conceptually:

```text
                 Image Token
                      │
                      ▼
               Condition Router
                      │
       ┌──────────────┼──────────────┐
       ▼              ▼              ▼
     Layout         Object        Material
       │              │              │
       └──────────────┼──────────────┘
                      │
                      ▼
              Routed Condition
```

This is a proposed architectural mechanism and must be experimentally validated in Chapter 4.

---

# 3.19 Why a Condition Router Is Necessary

A simple concatenation approach would be:

$$
C=
[C_L;C_O;C_M;C_{St};C_{Li};C_C;C_A;C_R]
$$

The resulting representation treats all conditions as a single sequence.

However, different image regions may require different semantic information.

For example:

| Image region | Important conditions |
|---|---|
| Sofa | Object + Material + Color |
| Floor | Material + Color |
| Window | Architecture + Layout |
| Fireplace | Object + Material |
| Whole scene | Style + Lighting |
| Object arrangement | Layout + Relationship |

The condition router is intended to learn these interactions automatically.

---

# 3.20 Latent Image Representation

The proposed framework uses a latent image representation rather than directly performing diffusion in RGB pixel space.

Let:

$$
I
$$

be a training image.

The VAE encoder transforms it into:

$$
Z_0=E_{VAE}(I)
$$

The latent tensor can be represented as:

$$
Z_0\in\mathbb{R}^{C\times H'\times W'}
$$

where $H'$ and $W'$ are smaller than the original image dimensions.

Latent diffusion substantially reduces the computational burden of diffusion while retaining high-resolution synthesis capability. ([openaccess.thecvf.com](https://openaccess.thecvf.com/content/CVPR2022/html/Rombach_High-Resolution_Image_Synthesis_With_Latent_Diffusion_Models_CVPR_2022_paper.html?utm_source=chatgpt.com))

---

# 3.21 Forward Diffusion Process

Gaussian noise is progressively added to the clean latent.

The forward process is:

$$
q(Z_t|Z_0)
=
\mathcal{N}
(
Z_t;
\sqrt{\bar{\alpha}_t}Z_0,
(1-\bar{\alpha}_t)I
)
$$

which can be sampled directly using:

$$
Z_t
=
\sqrt{\bar{\alpha}_t}Z_0
+
\sqrt{1-\bar{\alpha}_t}\epsilon
$$

where:

$$
\epsilon\sim\mathcal{N}(0,I)
$$

The model must learn to reverse this process.

---

# 3.22 Patchification

The noisy latent is divided into spatial patches.

Let:

$$
Z_t\in
\mathbb{R}^{C\times H'\times W'}
$$

and patch size be $p\times p$.

The number of image tokens becomes:

$$
N=
\frac{H'W'}{p^2}
$$

Each patch is flattened and projected:

$$
x_i=W_px_i^{patch}
$$

producing:

$$
X\in\mathbb{R}^{N\times d}
$$

This converts the image-generation problem into a Transformer sequence-processing problem.

This is consistent with the DiT design, which operates on latent patches rather than using a conventional convolutional U-Net backbone. ([arXiv](https://arxiv.org/abs/2212.09748?utm_source=chatgpt.com))

---

# 3.23 Positional Encoding

Spatial position is important because image tokens must retain information about their location.

Therefore, a positional representation is added:

$$
X_0=X+P
$$

where:

$$
P\in\mathbb{R}^{N\times d}
$$

represents spatial positions.

Without spatial information, the Transformer would have difficulty distinguishing:

```text
top-left window
```

from:

```text
bottom-right window
```

even though both may have similar visual features.

---

# 3.24 Time Embedding

The diffusion timestep $t$ must also be represented.

A time embedding is calculated as:

$$
e_t=f_t(t)
$$

This embedding informs the Transformer about the current noise level.

At high noise:

$$
t\approx T
$$

the model must construct large-scale semantic structure.

At low noise:

$$
t\approx 0
$$

the model primarily refines visual details.

Thus:

$$
e_t
$$

provides the diffusion stage information to each DiT block.

---

# 3.25 Proposed Multi-Condition DiT Block

The proposed DiT block extends the standard Transformer concept with semantic condition routing.

A conceptual block is:

```text
              Image Tokens
                   │
                   ▼
               LayerNorm
                   │
                   ▼
            Self-Attention
                   │
                   ▼
                Residual
                   │
                   ▼
               LayerNorm
                   │
                   ▼
          Condition Router
                   │
       ┌───────────┼───────────┐
       ▼           ▼           ▼
    Layout       Object      Material
       │           │           │
       └───────────┼───────────┘
                   ▼
            Condition Fusion
                   │
                   ▼
          Adaptive Modulation
                   │
                   ▼
                 MLP
                   │
                   ▼
                Residual
                   │
                   ▼
              Next Block
```

This block is the primary architectural innovation proposed by the dissertation.

---

# 3.26 Self-Attention

Self-attention enables interactions between image tokens.

For:

$$
X\in\mathbb{R}^{N\times d}
$$

the attention operation is:

$$
Q=XW_Q
$$

$$
K=XW_K
$$

$$
V=XW_V
$$

and:

$$
Attention(X)
=
softmax
\left(
\frac{QK^T}{\sqrt d}
\right)V
$$

In interior generation, this allows visual tokens representing different areas of the room to interact.

For example:

$$
\text{sofa region}
\leftrightarrow
\text{fireplace region}
$$

can exchange information.

---

# 3.27 Semantic Cross-Attention

After image self-attention, the model interacts with semantic conditions.

For image tokens $X$ and condition tokens $C$:

$$
Q=XW_Q
$$

$$
K=CW_K
$$

$$
V=CW_V
$$

Then:

$$
A_{sem}
=
softmax
\left(
\frac{QK^T}{\sqrt d}
\right)V
$$

However, instead of treating $C$ as a homogeneous sequence, the proposed framework maintains the semantic category:

$$
C_k
$$

associated with each condition.

The router therefore learns:

$$
P(k|x_i)
$$

representing how strongly condition $k$ should influence image token $i$.

---

# 3.28 Adaptive Semantic Modulation

The proposed model can additionally use adaptive modulation based on the timestep and global condition.

Let:

$$
e=
[e_t;C_G]
$$

Then:

$$
(\gamma,\beta)
=
MLP(e)
$$

and:

$$
\tilde{X}
=
\gamma\odot LN(X)+\beta
$$

This allows semantic information to modulate the image representation without requiring all information to enter through cross-attention.

Adaptive normalization is particularly suitable for diffusion architectures because the conditioning signal can vary according to the diffusion timestep.

---

# 3.29 Hierarchical Conditioning

The proposed model can organize semantic conditions hierarchically.

### Level 1 - Global conditions

$$
C_G=
\{
C_{St},
C_{Li},
C_C
\}
$$

These describe overall appearance.

### Level 2 - Architectural conditions

$$
C_A
$$

These describe the room structure.

### Level 3 - Object conditions

$$
C_O,C_M
$$

These describe individual objects and their appearance.

### Level 4 - Spatial conditions

$$
C_L,C_R
$$

These describe positions and relationships.

The resulting hierarchy is:

```text
                 GLOBAL
          ┌────────┼────────┐
        Style   Lighting   Color
          │        │        │
          └────────┼────────┘
                   ▼
              ARCHITECTURE
                   │
                   ▼
                 OBJECT
              ┌────┴────┐
           Object    Material
              │
              ▼
          SPATIAL
        ┌─────┴─────┐
      Layout    Relations
```

This hierarchy reflects the different semantic scales of interior design.

---

# 3.30 Semantic Interaction Modeling

The semantic spaces should not be considered completely independent.

For example:

$$
\text{Style}
\leftrightarrow
\text{Material}
$$

because Scandinavian design may favor particular material combinations.

Similarly:

$$
\text{Lighting}
\leftrightarrow
\text{Material}
$$

because marble, glass, metal, and wood respond differently to illumination.

And:

$$
\text{Object}
\leftrightarrow
\text{Layout}
$$

because object identity influences plausible placement.

Therefore, the condition router should model:

$$
C_i\leftrightarrow C_j
$$

rather than simply processing each semantic space independently.

---

# 3.31 Relationship Graph Encoder

The relationship representation can be processed using a graph-based encoder.

Given:

$$
G=(V,E)
$$

each node $v_i$ has an object representation:

$$
h_i
$$

and each edge $e_{ij}$ has a relationship representation:

$$
r_{ij}
$$

A graph message-passing operation can be written as:

$$
h_i'
=
\phi
\left(
h_i,
\sum_{j\in\mathcal{N}(i)}
\psi(h_i,h_j,r_{ij})
\right)
$$

where:

- $\mathcal{N}(i)$ is the neighborhood of object $i$;
- $\psi$ calculates relational messages;
- $\phi$ updates the node representation.

The resulting graph representation is:

$$
C_R=f_{graph}(G)
$$

This representation is then provided to the DiT condition router.

---

# 3.32 Conflict Resolution

Natural-language prompts can contain conflicting information.

For example:

> "A minimalist room filled with highly decorative furniture."

The semantic decomposition could generate:

$$
C_{St}^{minimalist}
$$

and:

$$
C_O^{decorative}
$$

These conditions may conflict.

The proposed framework therefore requires learned condition weighting.

Let:

$$
w_k
$$

represent the importance of semantic condition $k$.

Then:

$$
C^*
=
\sum_k w_kC_k
$$

where:

$$
\sum_k w_k=1
$$

The router can learn these weights from training data.

This allows the model to resolve conflicts according to learned design patterns rather than relying entirely on manual rules.

---

# 3.33 Classifier-Free Guidance

The proposed architecture can use classifier-free guidance (CFG) during generation.

Let:

$$
\epsilon_\theta(Z_t,C)
$$

represent the conditional prediction and:

$$
\epsilon_\theta(Z_t,\varnothing)
$$

represent the unconditional prediction.

The guided prediction is:

$$
\hat{\epsilon}
=
\epsilon_\theta(Z_t,\varnothing)
+
s
\left[
\epsilon_\theta(Z_t,C)
-
\epsilon_\theta(Z_t,\varnothing)
\right]
$$

where $s$ is the guidance scale.

In the proposed system, the condition can include the complete semantic set:

$$
C=
\{C_L,C_O,C_M,C_{St},C_{Li},C_C,C_A,C_R\}
$$

CFG can therefore be used to strengthen semantic adherence.

---

# 3.34 Multi-Condition Dropout

To prevent the model from becoming dependent on one semantic space, training can randomly drop individual conditions.

For example:

$$
C_M\rightarrow\varnothing
$$

or:

$$
C_R\rightarrow\varnothing
$$

during selected training iterations.

The model then learns to operate under different combinations of conditions.

This provides two advantages:

1. robustness to imperfect semantic extraction;
2. ability to study the contribution of each semantic space.

It also supports the ablation experiments planned for Chapter 4.

---

# 3.35 Training Dataset

The proposed training dataset is:

$$
D=
\{
(T_i,I_i,S_i)
\}_{i=1}^{N}
$$

where:

- $T_i$ = text description;
- $I_i$ = interior image;
- $S_i$ = semantic representation.

The semantic representation is:

$$
S_i=
\{
L_i,O_i,M_i,St_i,
Li_i,C_i,A_i,R_i
\}
$$

The dataset can be constructed using a combination of:

### Existing structured interior datasets

These provide:

- room geometry;
- furniture;
- object positions;
- materials;
- camera information;
- segmentation;
- depth.

### Natural-language annotation

Structured scenes can be converted into natural-language descriptions.

### Human validation

Interior-design experts can verify whether generated descriptions accurately describe the underlying scene.

This results in:

$$
\text{Structured Scene}
\rightarrow
\text{Natural Language}
+
\text{Semantic Annotation}
$$

---

# 3.36 Automatic Semantic Annotation

A large dataset cannot necessarily be manually annotated at every semantic level.

Therefore, automatic semantic extraction can be used.

The pipeline can be:

```text
Interior Image / Structured Scene
            ↓
       Scene Analysis
            ↓
     Object Extraction
            ↓
    Material Extraction
            ↓
      Layout Extraction
            ↓
   Relationship Extraction
            ↓
 Natural-Language Generation
            ↓
     Human Validation
```

This creates paired data:

$$
(I,S,T)
$$

The automatically generated labels should be evaluated before they are used as training targets.

---

# 3.37 Data Augmentation

The dataset can be expanded through controlled textual variation.

For example:

```text
Original:
"modern beige living room"

Variation 1:
"contemporary beige living room"

Variation 2:
"minimal contemporary living room with beige furniture"

Variation 3:
"modern living area featuring neutral beige furniture"
```

However, semantic equivalence should be preserved.

Therefore:

$$
S(T_1)
\approx
S(T_2)
$$

should hold for valid paraphrases.

This can increase linguistic diversity without changing the underlying design requirements.

---

# 3.38 Training Objective

The primary training objective is diffusion denoising.

Given:

$$
Z_t=
\sqrt{\bar{\alpha}_t}Z_0
+
\sqrt{1-\bar{\alpha}_t}\epsilon
$$

the model predicts:

$$
\hat{\epsilon}
=
\epsilon_\theta
(
Z_t,t,C
)
$$

The diffusion loss is:

$$
\mathcal{L}_{diff}
=
\mathbb{E}
\left[
\|
\epsilon-\hat{\epsilon}
\|_2^2
\right]
$$

This is the primary generative objective.

---

# 3.39 Semantic Alignment Loss

The generated image should correspond to the semantic conditions.

Let:

$$
E_I(\hat{I})
$$

represent an image encoder representation and:

$$
E_S(S)
$$

represent the corresponding semantic representation.

A semantic alignment loss can be defined as:

$$
\mathcal{L}_{sem}
=
1-
\cos
(
E_I(\hat{I}),
E_S(S)
)
$$

This encourages the generated image to remain aligned with the requested design semantics.

This formulation should be experimentally compared with training using only the diffusion loss.

---

# 3.40 Object Consistency Loss

For required objects:

$$
O=
\{o_1,\ldots,o_N\}
$$

an object detector or object recognition model can provide predicted object representations:

$$
\hat{O}
$$

An object consistency loss can be expressed conceptually as:

$$
\mathcal{L}_{obj}
=
D(O,\hat{O})
$$

where $D$ measures disagreement between required and generated objects.

This may be implemented using:

- object classification;
- object detection;
- embedding similarity;
- region-level alignment.

---

# 3.41 Material Consistency Loss

Similarly:

$$
\mathcal{L}_{mat}
=
D(M,\hat{M})
$$

where:

$$
M
$$

represents requested materials and:

$$
\hat{M}
$$

represents materials estimated from the generated image.

The material estimator could be:

- a dedicated material classifier;
- a vision-language model;
- a segmentation-based material model.

---

# 3.42 Relationship Consistency Loss

Relationship consistency is central to the proposed method.

Let:

$$
R
$$

represent required relationships and:

$$
\hat{R}
$$

represent relationships inferred from the generated image.

Then:

$$
\mathcal{L}_{rel}
=
D(R,\hat{R})
$$

A possible graph-based formulation is:

$$
\mathcal{L}_{rel}
=
\sum_{(i,j,r)\in R}
d
\left(
\hat{r}_{ij},
r
\right)
$$

where $d$ measures the difference between the requested and generated relation.

---

# 3.43 Total Training Objective

The proposed total loss is:

$$
\boxed{
\mathcal{L}_{total}
=
\lambda_{diff}\mathcal{L}_{diff}
+
\lambda_{sem}\mathcal{L}_{sem}
+
\lambda_{obj}\mathcal{L}_{obj}
+
\lambda_{mat}\mathcal{L}_{mat}
+
\lambda_{rel}\mathcal{L}_{rel}
}
$$

where:

$$
\lambda_{diff},
\lambda_{sem},
\lambda_{obj},
\lambda_{mat},
\lambda_{rel}
$$

are experimentally determined weighting coefficients.

Importantly, these additional losses should not be presented as established necessities. They are **research components to be evaluated experimentally**.

Chapter 4 should therefore compare:

$$
\mathcal{L}_{diff}
$$

against:

$$
\mathcal{L}_{diff}
+
\mathcal{L}_{sem}
$$

and progressively more complete combinations.

---

# 3.44 Training Procedure

A single training iteration can be described as follows.

### Step 1

Sample:

$$
(T,I,S)\sim D
$$

### Step 2

Encode the image:

$$
Z_0=E_{VAE}(I)
$$

### Step 3

Sample timestep:

$$
t\sim U(1,T)
$$

### Step 4

Sample noise:

$$
\epsilon\sim\mathcal{N}(0,I)
$$

### Step 5

Construct noisy latent:

$$
Z_t=
\sqrt{\bar{\alpha}_t}Z_0
+
\sqrt{1-\bar{\alpha}_t}\epsilon
$$

### Step 6

Decompose the text:

$$
T
\rightarrow
S
$$

### Step 7

Encode semantic conditions:

$$
S\rightarrow C
$$

### Step 8

Predict noise:

$$
\hat{\epsilon}
=
\epsilon_\theta(Z_t,t,C)
$$

### Step 9

Calculate losses:

$$
\mathcal{L}_{total}
$$

### Step 10

Update model parameters:

$$
\theta
\leftarrow
\theta-\eta\nabla_\theta\mathcal{L}_{total}
$$

---

# 3.45 Inference Process

During inference, the user supplies only a text description.

For example:

> "A luxurious modern living room with a cream sectional sofa facing a dark marble fireplace, walnut flooring, warm indirect lighting, large windows, and brass accents."

The inference pipeline becomes:

```text
Text Prompt
     ↓
Text Encoder
     ↓
Semantic Decomposition
     ↓
┌────────────────────────────┐
│ Layout                     │
│ Object                     │
│ Material                   │
│ Style                      │
│ Lighting                   │
│ Color                      │
│ Architecture              │
│ Relationships              │
└─────────────┬──────────────┘
              ↓
       Condition Router
              ↓
       Random Noise ZT
              ↓
       Multi-Condition DiT
              ↓
        Denoising Steps
              ↓
          Z0 Generated
              ↓
         VAE Decoder
              ↓
      Interior Image
```

No external image or spatial map is required.

---

# 3.46 Iterative Denoising

Starting from:

$$
Z_T\sim\mathcal{N}(0,I)
$$

the model repeatedly estimates the noise and updates the latent:

$$
Z_T
\rightarrow
Z_{T-1}
\rightarrow
\cdots
\rightarrow
Z_0
$$

At each step:

$$
Z_{t-1}
=
Sampler
(
Z_t,
\hat{\epsilon},
t
)
$$

The exact sampler can be selected experimentally.

Potential candidates include:

- DDPM;
- DDIM;
- DPM-Solver;
- other diffusion samplers.

The dissertation should treat sampler selection as an experimental variable rather than part of the central research contribution.

---

# 3.47 VAE Decoding

After denoising:

$$
Z_0
$$

is decoded:

$$
\hat{I}=D_{VAE}(Z_0)
$$

The result is the final generated interior image.

The VAE therefore acts as the interface between:

$$
\text{latent image representation}
$$

and:

$$
\text{pixel image}.
$$

Latent diffusion established this separation between compressed representation and diffusion generation as a practical method for high-resolution synthesis. ([openaccess.thecvf.com](https://openaccess.thecvf.com/content/CVPR2022/html/Rombach_High-Resolution_Image_Synthesis_With_Latent_Diffusion_Models_CVPR_2022_paper.html?utm_source=chatgpt.com))

---

# 3.48 Complete Algorithm

The complete proposed algorithm can be summarized as follows.

```text
Algorithm 1: Multi-Space Semantic Interior Generation

Input:
    Natural-language prompt T

Output:
    Generated interior image I

1. Normalize T
2. Encode T using language encoder
3. Decompose semantic representation:
       S = {L,O,M,St,Li,C,A,R}
4. Encode each semantic space:
       Ck = Ek(Sk)
5. Construct global condition CG
6. Initialize random latent:
       ZT ~ N(0,I)
7. For t = T ... 1:
       a. Patchify Zt
       b. Add timestep embedding
       c. Apply image self-attention
       d. Calculate semantic condition routing
       e. Apply multi-condition fusion
       f. Apply adaptive semantic modulation
       g. Predict noise
       h. Update latent Zt → Zt-1
8. Decode Z0 using VAE decoder
9. Return generated image I
```

---

# 3.49 Computational Architecture

The proposed system can be divided into four computational modules.

```text
Module 1
Language Processing
        │
        ▼
Module 2
Semantic Representation
        │
        ▼
Module 3
Diffusion Transformer
        │
        ▼
Module 4
Image Decoder
```

### Module 1

Responsible for:

- text encoding;
- semantic extraction.

### Module 2

Responsible for:

- layout;
- objects;
- materials;
- style;
- lighting;
- color;
- architecture;
- relationships.

### Module 3

Responsible for:

- latent diffusion;
- image-token processing;
- condition routing;
- semantic integration.

### Module 4

Responsible for:

- latent-to-image reconstruction.

---

# 3.50 Parameter-Efficient Training Strategy

Training an entire large diffusion model from scratch can require substantial computational resources.

Therefore, the research can investigate parameter-efficient training.

Potential strategies include:

### Full fine-tuning

$$
\theta_{all}
$$

is updated.

### Adapter training

Only additional conditioning modules are trained.

### LoRA

Low-rank updates are applied to selected Transformer layers.

### Hybrid training

The base image-generation model is partially frozen while:

- semantic encoders;
- condition router;
- relationship encoder;
- selected DiT layers

are trained.

A hybrid strategy is particularly useful for research because it permits systematic investigation of the proposed architecture without requiring every parameter to be learned from the beginning.

---

# 3.51 Proposed Experimental Configurations

To determine which components provide measurable benefits, the model should be trained in progressively more complex configurations.

### Model A - Standard Text DiT

$$
T
\rightarrow
C_T
\rightarrow
DiT
$$

This serves as the primary baseline.

### Model B - Semantic Decomposition

$$
T
\rightarrow
\{C_L,C_O,C_M,C_{St},C_{Li},C_C,C_A,C_R\}
\rightarrow
DiT
$$

### Model C - Semantic Decomposition + Router

$$
T
\rightarrow
C
\rightarrow
Router
\rightarrow
DiT
$$

### Model D - Router + Relationship Graph

$$
T
\rightarrow
C
+
C_R^{graph}
\rightarrow
Router
\rightarrow
DiT
$$

### Model E - Full Proposed Model

$$
T
\rightarrow
C
\rightarrow
Router
\rightarrow
DiT
+
\mathcal{L}_{semantic}
+
\mathcal{L}_{object}
+
\mathcal{L}_{material}
+
\mathcal{L}_{relation}
$$

This progression enables direct ablation analysis.

---

# 3.52 Ablation Design

Ablation experiments should remove individual semantic spaces.

For example:

### Without layout

$$
C_L=\varnothing
$$

### Without object

$$
C_O=\varnothing
$$

### Without material

$$
C_M=\varnothing
$$

### Without style

$$
C_{St}=\varnothing
$$

### Without lighting

$$
C_{Li}=\varnothing
$$

### Without relationships

$$
C_R=\varnothing
$$

The results can determine which semantic dimensions contribute most strongly to different aspects of generation.

The experiment should avoid assuming beforehand that any particular semantic space will necessarily provide the largest improvement.

---

# 3.53 Evaluation Framework

The proposed system should be evaluated using multiple dimensions.

$$
E=
\{
E_{visual},
E_{semantic},
E_{object},
E_{material},
E_{layout},
E_{relation},
E_{style},
E_{lighting}
\}
$$

### Visual quality

- FID;
- KID;
- human visual assessment.

### Semantic alignment

- CLIP similarity;
- vision-language similarity;
- prompt satisfaction.

### Object accuracy

$$
A_{object}
$$

### Material accuracy

$$
A_{material}
$$

### Layout accuracy

$$
A_{layout}
$$

### Relationship accuracy

$$
A_{relation}
$$

### Style consistency

$$
A_{style}
$$

### Lighting consistency

$$
A_{lighting}
$$

---

# 3.54 Design Requirement Satisfaction

A central evaluation concept proposed by this dissertation is **Design Requirement Satisfaction (DRS)**.

Let the prompt contain:

$$
N
$$

design requirements.

Each requirement is assigned:

$$
r_i\in\{0,1\}
$$

where:

$$
r_i=1
$$

means the requirement is satisfied.

Then:

$$
DRS=
\frac{
\sum_{i=1}^{N}r_i
}{
N
}
$$

For example:

```text
Prompt requirements:

✓ living room
✓ beige sofa
✓ marble fireplace
✓ oak floor
✓ warm lighting
✓ large windows
✓ sofa facing fireplace

7 / 7 = 100%
```

This metric should be formally validated against expert human judgments before being treated as a definitive measure.

---

# 3.55 Human Expert Evaluation

Professional interior designers can evaluate generated images using a Likert-scale questionnaire.

Possible criteria include:

| Criterion | Evaluation question |
|---|---|
| Object correctness | Are requested objects present? |
| Spatial consistency | Are objects positioned plausibly? |
| Relationship correctness | Are specified relationships satisfied? |
| Material correctness | Are requested materials represented? |
| Style consistency | Does the room reflect the requested style? |
| Lighting consistency | Does the lighting match the description? |
| Design coherence | Is the overall room coherent? |
| Realism | Does the image appear visually realistic? |
| Requirement satisfaction | How completely does the image satisfy the prompt? |

Inter-rater reliability should be measured statistically.

Possible measures include:

- Cohen's $\kappa$;
- Fleiss' $\kappa$;
- intraclass correlation coefficient.

The exact measure should depend on the final evaluation protocol.

---

# 3.56 Failure Analysis

The proposed system should also record failure cases.

Important failure categories include:

### Object omission

Requested object does not appear.

### Object duplication

The model produces multiple copies of an object.

### Attribute error

The object exists but has incorrect color, material, or shape.

### Spatial error

Objects are present but incorrectly positioned.

### Relationship error

Objects are present but their specified relationship is incorrect.

### Material error

Requested material is replaced by another material.

### Style drift

The generated image does not consistently follow the requested style.

### Lighting mismatch

Lighting does not correspond to the description.

### Architectural inconsistency

Windows, doors, walls, or other structural elements are incorrectly generated.

These failures provide important evidence for understanding the behavior of the proposed semantic conditioning framework.

---

# 3.57 Expected Technical Contribution

The principal technical contribution is the proposed transformation:

$$
\boxed{
\text{Text}
\rightarrow
\text{Multi-Space Semantics}
\rightarrow
\text{Condition Routing}
\rightarrow
\text{DiT}
}
$$

rather than:

$$
\text{Text}
\rightarrow
\text{Single Conditioning}
\rightarrow
\text{DiT}
$$

The research therefore focuses on **semantic organization and controllability**, rather than simply increasing the size of the image-generation model.

---

# 3.58 Expected Methodological Contribution

The proposed methodology establishes a systematic framework for studying the relationship between different categories of interior-design requirements and image-generation behavior.

It enables experiments such as:

$$
\text{Does layout improve spatial accuracy?}
$$

$$
\text{Does material conditioning improve material consistency?}
$$

$$
\text{Does relationship conditioning improve object arrangement?}
$$

$$
\text{Does style conditioning improve global stylistic consistency?}
$$

This transforms the research from a simple generative-AI application into an experimentally testable architectural methodology.

---

# 3.59 Research Framework Summary

The proposed framework can be summarized mathematically as:

$$
T
\xrightarrow{f_{text}}
H
$$

$$
H
\xrightarrow{f_{sem}}
S
$$

$$
S
=
\{
L,O,M,St,Li,C,A,R
\}
$$

$$
S
\xrightarrow{f_{enc}}
C
$$

$$
C
\xrightarrow{f_{router}}
C^*
$$

$$
(Z_t,t,C^*)
\xrightarrow{f_{\theta}}
\hat{\epsilon}
$$

$$
\hat{\epsilon}
\rightarrow
Z_0
$$

$$
Z_0
\xrightarrow{D_{VAE}}
\hat{I}
$$

Therefore:

$$
\boxed{
\hat{I}
=
D_{VAE}
\left[
f_{\theta}
\left(
f_{router}
\left(
f_{enc}
\left(
f_{sem}
\left(
f_{text}(T)
\right)
\right)
\right)
\right)
\right]
}
$$

This equation summarizes the complete proposed architecture.

---

# 3.60 Chapter Summary

This chapter presented the proposed methodology for text-only interior image generation.

The proposed architecture introduces a **Multi-Space Semantic Conditioning Diffusion Transformer** in which a natural-language interior description is decomposed into eight semantic spaces:

$$
\boxed{
Layout,\ Object,\ Material,\ Style,\ Lighting,\ Color,\ Architecture,\ Relationship
}
$$

Each semantic space is independently encoded and subsequently integrated through a condition-fusion and routing mechanism.

The generated conditions are then provided to a Diffusion Transformer operating in a latent image space. DiT provides the Transformer-based diffusion backbone, while latent diffusion provides a computationally efficient representation for high-resolution image generation. ([arXiv](https://arxiv.org/abs/2212.09748?utm_source=chatgpt.com))

The most important conceptual distinction is that the proposed method remains **text-only at the user interface**. External structural inputs such as depth maps, segmentation maps, sketches, floor plans, or reference images are not required. Instead, the system internally derives semantic conditions from the natural-language description. This differs from approaches such as ControlNet that explicitly introduce spatial conditioning signals. ([arXiv](https://arxiv.org/abs/2302.05543?utm_source=chatgpt.com))

The proposed framework also introduces:

1. semantic decomposition;
2. specialized semantic encoders;
3. relationship graph representation;
4. multi-condition fusion;
5. condition routing;
6. hierarchical conditioning;
7. semantic/object/material/relationship consistency objectives;
8. design-requirement-oriented evaluation.

The resulting research pipeline is:

$$
\boxed{
\text{Natural Language}
\rightarrow
\text{Semantic Decomposition}
\rightarrow
\text{Multi-Space Conditioning}
\rightarrow
\text{Condition Router}
\rightarrow
\text{Multi-Condition DiT}
\rightarrow
\text{Latent Image}
\rightarrow
\text{Interior Image}
}
$$

Chapter 4 will empirically evaluate this architecture against conventional text-conditioned diffusion and progressively controlled variants. Particular attention will be given to **semantic alignment, object accuracy, layout accuracy, material consistency, relationship accuracy, visual quality, human expert evaluation, and ablation studies**.

---

**Sources:**

- [CVPR 2022 Open Access Repository](https://openaccess.thecvf.com/content/CVPR2022/html/Rombach_High-Resolution_Image_Synthesis_With_Latent_Diffusion_Models_CVPR_2022_paper.html?utm_source=chatgpt.com)
- [Adding Conditional Control to Text-to-Image Diffusion Models](https://arxiv.org/abs/2302.05543?utm_source=chatgpt.com)

## Prompt:
9/28/2026, 6:28:57 PM

go chapter4

## Response:
9/28/2026, 6:28:58 PM · gpt-5-6-mini

Yes. **Chapter 4 should be the experimental chapter**. The key is to make it scientifically testable: baselines, controlled experiments, ablations, quantitative metrics, human evaluation, statistical analysis, and failure analysis. Since the actual experiments have not yet been run, I’ll write the chapter as a **PhD-ready experimental methodology/results framework**, with result tables designed to be filled with measured values rather than inventing results.

# Chapter 4 — Experiments and Results

## 4.1 Introduction

Chapter 3 presented the proposed Multi-Space Semantic Conditioning Diffusion Transformer (MSC-DiT) for text-only interior image generation. The proposed method decomposes natural-language interior descriptions into multiple semantic spaces representing layout, objects, materials, style, lighting, color, architecture, and relationships.

The objective of this chapter is to empirically investigate whether this structured representation improves the ability of a generative model to satisfy interior-design requirements.

The experiments are designed around five principal questions:

1. Does multi-space semantic conditioning improve text-image alignment?
2. Does explicit object and relationship representation improve spatial consistency?
3. Does explicit material conditioning improve material consistency?
4. Does explicit style and lighting conditioning improve global design consistency?
5. Does the complete MSC-DiT architecture improve overall design-requirement satisfaction compared with conventional text-conditioned diffusion?

The experimental framework therefore compares progressively more structured models rather than only comparing the proposed model with a single baseline.

The overall experimental process is:

```text id="q3tq4f"
Dataset
   ↓
Text / Image / Semantic Annotations
   ↓
Training
   ↓
Baseline Models
   +
Proposed Models
   ↓
Generated Interior Images
   ↓
Automatic Evaluation
   ├── Image Quality
   ├── Text Alignment
   ├── Object Accuracy
   ├── Material Accuracy
   ├── Layout Accuracy
   └── Relationship Accuracy
   ↓
Human Expert Evaluation
   ↓
Ablation Studies
   ↓
Statistical Analysis
   ↓
Failure Analysis
```

---

# 4.2 Experimental Objectives

The experimental study has four major objectives.

### Objective 1 — Evaluate semantic alignment

Determine whether decomposition into multiple semantic spaces improves the correspondence between the generated image and the input design description.

### Objective 2 — Evaluate design-constraint satisfaction

Determine whether the proposed architecture improves the generation of:

- required objects;
- object attributes;
- materials;
- colors;
- architectural elements;
- spatial relationships.

### Objective 3 — Evaluate architectural components

Determine the contribution of:

- semantic decomposition;
- condition routing;
- relationship representation;
- specialized losses.

### Objective 4 — Evaluate practical generation quality

Determine whether improvements in semantic correctness are achieved while maintaining:

- visual quality;
- realism;
- stylistic coherence;
- lighting quality;
- computational feasibility.

---

# 4.3 Research Questions

The experiments are organized around the following research questions.

### RQ1

Does multi-space semantic conditioning improve text-image alignment compared with conventional single-space text conditioning?

### RQ2

Does explicit object and relationship conditioning improve the spatial consistency of generated interior scenes?

### RQ3

Does explicit material conditioning improve material consistency?

### RQ4

Does explicit style and lighting conditioning improve global visual consistency?

### RQ5

Does the proposed condition-routing mechanism improve semantic requirement satisfaction compared with simple condition concatenation?

### RQ6

Does the complete MSC-DiT architecture provide an overall improvement across multiple evaluation dimensions?

---

# 4.4 Experimental Hypotheses

The hypotheses introduced in Chapter 1 and developed in Chapter 3 are evaluated as follows.

### H1 — Multi-Space Semantic Hypothesis

The multi-space semantic model will demonstrate higher semantic alignment than a conventional text-conditioned baseline.

### H2 — Object Representation Hypothesis

Explicit object conditioning will improve the accuracy of required object generation.

### H3 — Relationship Hypothesis

Explicit relationship conditioning will improve spatial relationship accuracy.

### H4 — Material Hypothesis

Explicit material conditioning will improve material consistency.

### H5 — Style and Lighting Hypothesis

Explicit style and lighting conditions will improve style and lighting consistency.

### H6 — Multi-Condition DiT Hypothesis

The complete proposed architecture will provide higher overall design-requirement satisfaction than the conventional text-conditioned baseline.

These hypotheses are evaluated empirically rather than assumed to be true.

---

# 4.5 Dataset Construction

The dataset must provide sufficient information to evaluate the proposed semantic representation.

Each training example is represented as:

$$
D_i=(T_i,I_i,S_i)
$$

where:

- $T_i$ = natural-language description;
- $I_i$ = interior image;
- $S_i$ = structured semantic representation.

The semantic representation is:

$$
S_i=
\{
L_i,O_i,M_i,St_i,Li_i,C_i,A_i,R_i
\}
$$

where:

| Component | Description |
|---|---|
| $L$ | Layout |
| $O$ | Objects |
| $M$ | Materials |
| $St$ | Style |
| $Li$ | Lighting |
| $C$ | Color |
| $A$ | Architecture |
| $R$ | Relationships |

---

# 4.6 Dataset Sources

The experimental dataset can combine several sources of interior-scene information.

Structured datasets such as Structured3D and InteriorNet provide useful information about indoor environments, geometry, objects, and scene structure. Structured3D provides structured indoor scenes with semantic and geometric information, while InteriorNet provides large-scale synthetic interior scenes. ([structured3d-dataset.org](https://structured3d-dataset.org/?utm_source=chatgpt.com)) ([interiornet.org](https://interiornet.org/?utm_source=chatgpt.com))

Additional real-world interior images can be incorporated to improve appearance diversity.

The dataset can therefore be organized into:

$$
D=
D_{structured}
\cup
D_{real}
\cup
D_{annotated}
$$

where:

- $D_{structured}$ contains structured scenes;
- $D_{real}$ contains real interior images;
- $D_{annotated}$ contains validated text-semantic-image pairs.

---

# 4.7 Dataset Splitting

The dataset should be divided into:

$$
D=
D_{train}
\cup
D_{validation}
\cup
D_{test}
$$

A possible division is:

| Set | Proportion |
|---|---:|
| Training | 80% |
| Validation | 10% |
| Test | 10% |

The exact proportions can be adjusted according to the final dataset size.

Importantly, scene-level or source-level separation should be maintained to prevent near-duplicate scenes from appearing in both training and testing sets.

For example:

$$
Scene_i\notin D_{train}
$$

if a corresponding version of the same scene occurs in:

$$
D_{test}
$$

This is important because otherwise evaluation could overestimate generalization.

---

# 4.8 Semantic Annotation

Each test prompt should be annotated according to the eight semantic spaces.

For example:

### Prompt

> "A modern Scandinavian living room with a large beige sectional sofa facing a white marble fireplace, oak flooring, warm indirect lighting, and large windows."

The annotation becomes:

```text
Layout:
    sofa faces fireplace

Object:
    sectional sofa
    fireplace
    windows

Material:
    marble
    oak
    fabric

Style:
    modern
    Scandinavian

Lighting:
    warm
    indirect

Color:
    beige
    white
    oak/brown

Architecture:
    large windows

Relationship:
    sofa → faces → fireplace
```

These annotations provide the ground truth for evaluating semantic compliance.

---

# 4.9 Dataset Quality Control

Because semantic annotations directly affect training and evaluation, quality control is required.

The annotation process should include:

```text id="j1f8qp"
Automatic Annotation
        ↓
Rule / Consistency Checking
        ↓
Human Validation
        ↓
Disagreement Resolution
        ↓
Final Annotation
```

Each annotation should be checked for:

- missing objects;
- incorrect object attributes;
- incorrect materials;
- missing relationships;
- contradictory descriptions;
- incorrect style labels;
- incorrect lighting descriptions.

A subset of the dataset should be independently reviewed by multiple annotators.

---

# 4.10 Experimental Hardware

The final dissertation should report the exact hardware used during the experiments.

The experimental configuration should include:

| Component | Configuration |
|---|---|
| GPU | [GPU model] |
| GPU memory | [VRAM] |
| CPU | [CPU model] |
| System memory | [RAM] |
| Storage | [SSD/HDD] |
| Operating system | [OS] |
| CUDA | [version] |
| PyTorch | [version] |
| Python | [version] |

For reproducibility, all software versions should be recorded.

---

# 4.11 Model Configurations

Five principal configurations are proposed.

## Model A — Standard Text-Conditioned DiT

This model represents the conventional approach:

$$
T
\rightarrow
C_T
\rightarrow
DiT
\rightarrow
I
$$

It provides the primary baseline.

---

## Model B — Multi-Space Conditioning

The text is decomposed into:

$$
\{
C_L,C_O,C_M,C_{St},
C_{Li},C_C,C_A,C_R
\}
$$

but the conditions are directly fused.

$$
T
\rightarrow
C_1,\ldots,C_8
\rightarrow
DiT
\rightarrow
I
$$

---

## Model C — Multi-Space + Condition Router

Model B is extended with the proposed router:

$$
C
\rightarrow
Router
\rightarrow
DiT
$$

This evaluates whether selective condition routing provides an advantage over direct fusion.

---

## Model D — Multi-Space + Relationship Graph

The relationship condition is represented using a graph encoder:

$$
G=(V,E)
$$

and integrated into the DiT.

---

## Model E — Full MSC-DiT

The complete model uses:

- semantic decomposition;
- eight semantic spaces;
- specialized encoders;
- relationship graph;
- condition router;
- hierarchical conditioning;
- semantic losses;
- object losses;
- material losses;
- relationship losses.

This model represents the complete proposed framework.

---

# 4.12 Baseline Comparison

The experimental comparison should include both architectural baselines and, where feasible, strong pretrained text-to-image systems.

The conceptual comparison is:

| Model | Text | Multi-space | Router | Relation Graph | Additional Losses |
|---|---:|---:|---:|---:|---:|
| Baseline | ✓ | ✗ | ✗ | ✗ | ✗ |
| Model B | ✓ | ✓ | ✗ | ✗ | ✗ |
| Model C | ✓ | ✓ | ✓ | ✗ | ✗ |
| Model D | ✓ | ✓ | ✓ | ✓ | ✗ |
| MSC-DiT | ✓ | ✓ | ✓ | ✓ | ✓ |

If external pretrained systems are included, they should be evaluated using identical test prompts wherever possible.

---

# 4.13 Training Protocol

For fair comparison, baseline and proposed models should use comparable:

- training data;
- image resolution;
- number of training iterations;
- optimizer;
- batch size;
- learning-rate schedule;
- sampling procedure.

The primary diffusion objective is:

$$
\mathcal{L}_{diff}
=
\mathbb{E}
[
\|\epsilon-\epsilon_\theta(Z_t,t,C)\|^2
]
$$

The complete model additionally uses semantic objectives:

$$
\mathcal{L}_{total}
=
\lambda_d\mathcal{L}_{diff}
+
\lambda_s\mathcal{L}_{sem}
+
\lambda_o\mathcal{L}_{obj}
+
\lambda_m\mathcal{L}_{mat}
+
\lambda_r\mathcal{L}_{rel}
$$

The weights should be determined using the validation set rather than the test set.

---

# 4.14 Evaluation Metrics

Because no single metric can adequately evaluate interior-design generation, the proposed study uses multiple categories.

$$
E=
\{
Q_{visual},
Q_{text},
Q_{object},
Q_{material},
Q_{layout},
Q_{relation},
Q_{style},
Q_{lighting}
\}
$$

The evaluation is divided into:

1. image quality;
2. text-image alignment;
3. semantic requirement satisfaction;
4. spatial consistency;
5. human expert evaluation.

---

# 4.15 Image Quality Evaluation

The first category measures general image quality.

### Fréchet Inception Distance

FID compares feature distributions of real and generated images:

$$
FID=
\|\mu_r-\mu_g\|^2
+
Tr(
\Sigma_r+\Sigma_g
-
2(\Sigma_r\Sigma_g)^{1/2}
)
$$

Lower values generally indicate greater similarity between the distributions used for evaluation.

However, FID should not be interpreted as a direct measure of prompt compliance.

---

# 4.16 Kernel Inception Distance

KID can also be used as a distributional image-quality metric.

KID is useful because it provides an unbiased estimator of a kernel-based distribution discrepancy.

It should be reported alongside FID rather than treated as a replacement for semantic evaluation.

---

# 4.17 Text-Image Alignment

Text-image alignment can be evaluated using a vision-language similarity model.

For generated image $I_g$ and prompt $T$:

$$
S_{text-image}
=
\cos
(
E_I(I_g),
E_T(T)
)
$$

A higher score indicates stronger embedding-level alignment.

However, because global embedding similarity can hide individual requirement failures, this metric is supplemented with requirement-level evaluation.

---

# 4.18 Object Requirement Accuracy

Let:

$$
O_{req}
$$

be the set of required objects.

The generated image produces:

$$
O_{gen}
$$

Then:

$$
Precision_O=
\frac{|O_{req}\cap O_{gen}|}
{|O_{gen}|}
$$

$$
Recall_O=
\frac{|O_{req}\cap O_{gen}|}
{|O_{req}|}
$$

and:

$$
F1_O=
\frac{2Precision_ORecall_O}
{Precision_O+Recall_O}
$$

This provides a more detailed evaluation than simply determining whether the generated image "looks correct."

---

# 4.19 Object Attribute Accuracy

Object identity alone is insufficient.

For an object:

$$
o_i=
(category,
color,
material,
size,
shape)
$$

each attribute can be separately evaluated.

The overall attribute accuracy can be defined as:

$$
A_{attr}
=
\frac{
N_{correct\ attributes}
}{
N_{required\ attributes}
}
$$

For example:

```text id="x7yqf4"
Required sofa:

category = sectional
color = beige
material = fabric
size = large
```

A generated sofa that is blue leather should not receive full credit merely because a sofa exists.

---

# 4.20 Material Consistency

For required materials:

$$
M_{req}
$$

and generated materials:

$$
M_{gen}
$$

the material score can be calculated as:

$$
A_M
=
\frac{
|M_{req}\cap M_{gen}|
}{
|M_{req}|
}
$$

For more detailed evaluation, the material score can include:

- material identity;
- spatial assignment;
- texture appearance;
- color;
- visual plausibility.

---

# 4.21 Layout Accuracy

Layout accuracy evaluates whether objects occupy approximately correct spatial relationships.

For objects $i$ and $j$, relative position can be represented as:

$$
\Delta_{ij}
=
(x_i-x_j,y_i-y_j)
$$

The generated relationship is:

$$
\hat{\Delta}_{ij}
$$

A spatial error can then be defined as:

$$
E_{layout}
=
\frac{1}{N}
\sum_i
\|\Delta_i-\hat{\Delta}_i\|
$$

Alternatively, categorical relations can be evaluated:

- left/right;
- above/below;
- near/far;
- inside/outside;
- front/behind;
- centered;
- against wall.

---

# 4.22 Relationship Accuracy

For the relationship graph:

$$
G=(V,E)
$$

the generated graph is:

$$
\hat{G}=(\hat{V},\hat{E})
$$

Relationship precision and recall can be calculated:

$$
Precision_R=
\frac{|E\cap\hat{E}|}
{|\hat{E}|}
$$

$$
Recall_R=
\frac{|E\cap\hat{E}|}
{|E|}
$$

and:

$$
F1_R=
\frac{
2Precision_RRecall_R
}{
Precision_R+Recall_R
}
$$

This metric is particularly important for testing H3.

---

# 4.23 Style Consistency

Style consistency can be evaluated using a style classifier or vision-language model.

Let:

$$
S_{requested}
$$

be the requested style and:

$$
S_{generated}
$$

be the predicted style.

Then:

$$
A_{style}
=
\mathbb{1}
[
S_{requested}=S_{generated}
]
$$

For multiple styles:

$$
A_{style}
=
\frac{
N_{correct}
}{
N_{samples}
}
$$

Human expert evaluation should additionally be used because interior styles can overlap semantically.

---

# 4.24 Lighting Consistency

Lighting evaluation should consider:

- source;
- direction;
- temperature;
- intensity;
- softness;
- shadow structure.

For example:

> warm indirect lighting

should produce an image with visual properties consistent with warm, diffuse illumination rather than a cold direct light source.

A lighting consistency classifier can provide an automated estimate, while human evaluation can provide complementary assessment.

---

# 4.25 Design Requirement Satisfaction Rate

The central proposed evaluation measure is:

$$
DRS=
\frac{
\sum_{i=1}^{N}r_i
}{
N
}
$$

where:

$$
r_i=
\begin{cases}
1 & \text{requirement satisfied}\\
0 & \text{otherwise}
\end{cases}
$$

Requirements can be categorized:

$$
R=
R_O+
R_M+
R_L+
R_R+
R_{St}+
R_{Li}+
R_A+
R_C
$$

Therefore:

$$
DRS=
\frac{
N_O+N_M+N_L+N_R+N_{St}+N_{Li}+N_A+N_C
}{
N_{total}
}
$$

This metric provides a direct connection between the user's design specification and the generated image.

---

# 4.26 Weighted Design Requirement Satisfaction

Not all requirements necessarily have equal importance.

A future extension is to use:

$$
DRS_w=
\frac{
\sum_i w_i r_i
}{
\sum_i w_i
}
$$

where $w_i$ represents the importance of requirement $i$.

For example, a relationship explicitly emphasized in the prompt may receive a larger weight than a minor decorative detail.

However, the primary experiments should initially use unweighted DRS to reduce subjectivity.

---

# 4.27 Human Expert Evaluation

Human evaluation provides a complementary assessment.

The study should recruit qualified evaluators with backgrounds such as:

- interior design;
- architecture;
- architectural visualization;
- 3D visualization.

Each evaluator receives:

1. the original prompt;
2. generated image;
3. evaluation questionnaire.

Possible evaluation dimensions are:

| Dimension | Question |
|---|---|
| Requirement satisfaction | Does the image satisfy the requested design requirements? |
| Object correctness | Are requested objects correctly represented? |
| Spatial consistency | Are objects appropriately positioned? |
| Relationship correctness | Are stated relationships satisfied? |
| Material consistency | Are requested materials represented correctly? |
| Style consistency | Does the image reflect the requested style? |
| Lighting | Does the lighting match the description? |
| Visual realism | Does the image appear realistic? |
| Design coherence | Is the overall design coherent? |

A five-point Likert scale can be used.

---

# 4.28 Human Evaluation Protocol

To reduce evaluation bias, generated images should be anonymized.

Evaluators should not know:

- which model generated the image;
- whether the image belongs to the baseline;
- whether the image belongs to the proposed model.

The evaluation order should be randomized.

If multiple images correspond to the same prompt, their presentation order should also be randomized.

This creates a blind comparative evaluation.

---

# 4.29 Inter-Rater Reliability

Suppose $K$ experts independently evaluate the same images.

Agreement should be measured using an appropriate statistic.

For categorical judgments:

$$
\kappa
$$

can be used.

For continuous or ordinal rating scales, an intraclass correlation coefficient may be more appropriate.

A high agreement value provides evidence that the evaluation criteria are sufficiently well-defined.

Low agreement would indicate that the criterion requires refinement.

---

# 4.30 Main Experiment

The main experiment compares the baseline with the complete proposed model.

### Experimental hypothesis

$$
H_0:
\mu_{MSC-DiT}
=
\mu_{Baseline}
$$

versus:

$$
H_1:
\mu_{MSC-DiT}
\neq
\mu_{Baseline}
$$

for each evaluation measure.

The principal metrics are:

- FID;
- text-image similarity;
- object F1;
- material accuracy;
- layout accuracy;
- relationship F1;
- style accuracy;
- DRS;
- human expert score.

---

# 4.31 Main Results Table

The final dissertation should report actual measured values in a table similar to the following.

| Model | FID ↓ | Text Alignment ↑ | Object F1 ↑ | Material ↑ | Layout ↑ | Relation F1 ↑ | DRS ↑ |
|---|---:|---:|---:|---:|---:|---:|---:|
| Baseline DiT | — | — | — | — | — | — | — |
| Multi-Space | — | — | — | — | — | — | — |
| + Router | — | — | — | — | — | — | — |
| + Relation Graph | — | — | — | — | — | — | — |
| **MSC-DiT** | **—** | **—** | **—** | **—** | **—** | **—** | **—** |

The arrows indicate the desired direction of the metric.

No values should be inserted until the experiments have actually been performed.

---

# 4.32 Semantic Decomposition Ablation

The first ablation evaluates whether semantic decomposition itself provides an advantage.

Three conditions are compared:

$$
Model_A:
T\rightarrow C_T
$$

$$
Model_B:
T\rightarrow
\{C_L,C_O,C_M,C_{St},C_{Li},C_C,C_A,C_R\}
$$

$$
Model_C:
T\rightarrow
\{C_L,C_O,C_M,C_{St},C_{Li},C_C,C_A,C_R\}
\rightarrow Router
$$

This experiment separates:

1. the benefit of decomposition;
2. the benefit of condition routing.

---

# 4.33 Individual Semantic-Space Ablation

The contribution of each semantic space is evaluated by removing one condition at a time.

| Model | Layout | Object | Material | Style | Lighting | Color | Architecture | Relation |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| Full | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| − Layout | — | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| − Object | ✓ | — | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| − Material | ✓ | ✓ | — | ✓ | ✓ | ✓ | ✓ | ✓ |
| − Style | ✓ | ✓ | ✓ | — | ✓ | ✓ | ✓ | ✓ |
| − Lighting | ✓ | ✓ | ✓ | ✓ | — | ✓ | ✓ | ✓ |
| − Color | ✓ | ✓ | ✓ | ✓ | ✓ | — | ✓ | ✓ |
| − Architecture | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| − Relation | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | — |

The changes in evaluation metrics reveal the functional contribution of each semantic space.

---

# 4.34 Relationship-Graph Ablation

The relationship component should receive special attention because it represents one of the central differences between conventional text conditioning and the proposed architecture.

Three versions can be compared:

$$
A:
\text{No relationship representation}
$$

$$
B:
\text{Relationship tokens}
$$

$$
C:
\text{Graph relationship representation}
$$

The main evaluation metric should be:

$$
F1_R
$$

alongside:

$$
DRS
$$

and:

$$
A_{layout}
$$

---

# 4.35 Condition-Router Ablation

The proposed router should be compared with simpler alternatives.

### Method 1

Simple concatenation:

$$
C=[C_1;C_2;\ldots;C_8]
$$

### Method 2

Fixed weighted fusion:

$$
C=\sum_k w_kC_k
$$

### Method 3

Learned condition router:

$$
C_i^*
=
\sum_k
\alpha_{i,k}C_k
$$

This experiment directly tests whether dynamic token-level routing is beneficial.

---

# 4.36 Loss Ablation

The proposed additional losses should also be evaluated separately.

| Model | Diffusion | Semantic | Object | Material | Relation |
|---|---:|---:|---:|---:|---:|
| A | ✓ | — | — | — | — |
| B | ✓ | ✓ | — | — | — |
| C | ✓ | ✓ | ✓ | — | — |
| D | ✓ | ✓ | ✓ | ✓ | — |
| E | ✓ | ✓ | ✓ | ✓ | ✓ |

This experiment determines whether improvements originate from the architecture itself or from the additional objectives.

---

# 4.37 Prompt Complexity Experiment

The system should also be evaluated according to prompt complexity.

Prompts can be divided into:

### Level 1 — Simple

One or two requirements.

> "A modern white living room."

### Level 2 — Moderate

Several objects and attributes.

> "A modern living room with a beige sofa, wooden table and large windows."

### Level 3 — Complex

Multiple semantic dimensions.

> "A minimalist Scandinavian living room with a beige sectional sofa facing a marble fireplace, oak flooring, warm indirect lighting, large windows, brass accents and a wooden coffee table between the sofa and fireplace."

### Level 4 — Highly constrained

Many objects, attributes, relationships, materials, and architectural constraints.

The expected result is not assumed in advance; performance should be measured as complexity increases.

---

# 4.38 Prompt-Length Experiment

Prompt length can also be evaluated.

Let:

$$
L_T
$$

represent prompt token length.

Performance can be measured as a function:

$$
Performance=f(L_T)
$$

This determines whether semantic decomposition helps preserve information as the number of requirements increases.

---

# 4.39 Generalization Experiment

The proposed system should be tested on prompts containing:

- unseen object combinations;
- unseen material combinations;
- unseen style combinations;
- unseen relationships;
- novel combinations of known concepts.

For example, the model may have seen:

$$
Scandinavian + Oak
$$

and:

$$
Japanese + Marble
$$

during training but not:

$$
Scandinavian + Marble
$$

during training.

The test determines whether the semantic architecture supports compositional generalization.

---

# 4.40 Cross-Style Generalization

Another experiment evaluates style transfer through language.

For a fixed room configuration:

$$
T_{base}
$$

the style can be changed:

$$
T_{modern}
$$

$$
T_{Scandinavian}
$$

$$
T_{Japanese}
$$

$$
T_{industrial}
$$

The experiment evaluates whether style changes produce corresponding visual differences while maintaining other design requirements.

This tests whether style conditioning is sufficiently disentangled from other semantic dimensions.

---

# 4.41 Material Substitution Experiment

A similar experiment can hold all requirements constant while changing only material.

For example:

```text id="8vukd3"
Version A:
oak flooring

Version B:
marble flooring

Version C:
polished concrete flooring
```

The desired behavior is:

$$
I_A\neq I_B\neq I_C
$$

with other design requirements remaining approximately consistent.

This experiment evaluates material controllability.

---

# 4.42 Relationship Substitution Experiment

The same objects can be used while changing only their relationship.

For example:

### Prompt A

> "The sofa faces the fireplace."

### Prompt B

> "The sofa is perpendicular to the fireplace."

### Prompt C

> "The sofa is beside the fireplace."

The generated scenes should reflect the semantic relationship specified by the prompt.

This experiment provides a direct test of the relationship representation.

---

# 4.43 Lighting Substitution Experiment

Similarly:

```text id="qbrlrf"
Prompt A:
warm indirect lighting

Prompt B:
cool natural daylight

Prompt C:
dramatic directional lighting
```

The generated images can then be compared while holding:

- architecture;
- furniture;
- materials;
- style

constant.

This evaluates the degree to which lighting is independently controllable.

---

# 4.44 Computational Performance

The system should also be evaluated in terms of computational cost.

Metrics include:

$$
T_{generation}
$$

generation time per image,

$$
Memory_{peak}
$$

peak GPU memory,

and:

$$
Params
$$

number of trainable parameters.

Training cost can be reported as:

$$
GPUHours
=
N_{GPU}
\times
T_{training}
$$

These measurements are important because a research contribution should not only improve semantic performance but also have a clearly understood computational cost.

---

# 4.45 Scaling Experiment

The DiT model can be evaluated at different sizes.

For example:

| Model | Depth | Hidden Dimension | Parameters |
|---|---:|---:|---:|
| Small | — | — | — |
| Medium | — | — | — |
| Large | — | — | — |

The purpose is to determine whether performance improvements come primarily from:

$$
\text{model scale}
$$

or:

$$
\text{semantic architecture}.
$$

This is especially important for demonstrating that the proposed method is not simply benefiting from having a larger network.

---

# 4.46 Statistical Analysis

For each metric, multiple test prompts should be evaluated.

Suppose:

$$
x_1,\ldots,x_N
$$

represent scores for the baseline and:

$$
y_1,\ldots,y_N
$$

represent scores for MSC-DiT.

The mean is:

$$
\bar{x}
=
\frac{1}{N}
\sum_i x_i
$$

and standard deviation:

$$
\sigma_x
=
\sqrt{
\frac{1}{N-1}
\sum_i(x_i-\bar{x})^2
}
$$

The final dissertation should report:

$$
mean\pm std
$$

rather than only a single average.

---

# 4.47 Statistical Significance

For paired evaluations on identical prompts, a paired statistical test can be used.

Depending on the distribution:

- paired $t$-test;
- Wilcoxon signed-rank test.

The significance level can be defined as:

$$
\alpha=0.05
$$

A statistically significant result should not automatically be interpreted as practically important.

Therefore, effect size should also be reported where appropriate.

---

# 4.48 Confidence Intervals

For major metrics, confidence intervals can be reported.

For example:

$$
\bar{x}
\pm
CI_{95\%}
$$

This provides a clearer representation of uncertainty than a single point estimate.

---

# 4.49 Qualitative Evaluation

Quantitative metrics should be complemented with visual comparisons.

For identical prompts:

```text id="v1s1o3"
                 Same Prompt
                     │
       ┌─────────────┼─────────────┐
       ▼             ▼             ▼
    Baseline      Model B       MSC-DiT
       │             │             │
       ▼             ▼             ▼
     Image         Image         Image
```

The comparison should focus on:

- object completeness;
- object attributes;
- spatial arrangement;
- material appearance;
- style;
- lighting;
- architecture;
- overall realism.

The analysis should describe observable differences rather than relying solely on subjective claims such as "better."

---

# 4.50 Failure Case Analysis

The generated images should also be analyzed for failure modes.

### Failure Case 1 — Object omission

Prompt:

> "A sofa, two floor lamps, and a marble fireplace."

Generated result:

```text
✓ sofa
✓ fireplace
✗ second lamp
```

This represents an object recall failure.

---

### Failure Case 2 — Attribute substitution

Prompt:

> "A beige leather sofa."

Generated result:

```text
sofa = present
color = beige
material = fabric
```

The object exists, but the material requirement is violated.

---

### Failure Case 3 — Relationship failure

Prompt:

> "The sofa faces the fireplace."

Generated image:

```text
sofa
   ↓
sideways
fireplace
```

The objects are present but their relationship is incorrect.

---

### Failure Case 4 — Style drift

Prompt:

> "Minimalist Japanese interior."

Generated image contains:

- excessive decoration;
- ornate furniture;
- saturated colors.

This represents style inconsistency.

---

### Failure Case 5 — Architectural inconsistency

Prompt:

> "Large windows on the left wall."

The generated image may produce windows:

- on the wrong wall;
- in the wrong number;
- with inconsistent geometry.

This represents an architectural/layout failure.

---

# 4.51 Failure Attribution

An important part of the research is determining **why** a generation fails.

Failures can be attributed to different modules:

$$
Failure
\rightarrow
\{
Language,
Semantic,
Condition,
DiT,
Decoder
\}
$$

For example:

### Semantic extraction failure

The text parser incorrectly identifies:

> "beside"

as:

> "behind".

### Conditioning failure

The correct relationship is extracted but not sufficiently transmitted to the DiT.

### Generation failure

The condition is correctly represented but the diffusion model generates an incorrect spatial arrangement.

This distinction is important because it prevents all failures from being attributed to the generative backbone.

---

# 4.52 Expected Experimental Results

Because the experiments have not yet been executed, numerical results should not be fabricated.

The expected evaluation pattern is instead formulated as testable hypotheses.

If H1 is supported:

$$
Score_{semantic}^{MSC-DiT}
>
Score_{semantic}^{Baseline}
$$

If H2 is supported:

$$
F1_{object}^{MSC-DiT}
>
F1_{object}^{Baseline}
$$

If H3 is supported:

$$
F1_{relation}^{MSC-DiT}
>
F1_{relation}^{Baseline}
$$

If H4 is supported:

$$
A_{material}^{MSC-DiT}
>
A_{material}^{Baseline}
$$

If H5 is supported:

$$
A_{style}^{MSC-DiT}
+
A_{lighting}^{MSC-DiT}
>
A_{style}^{Baseline}
+
A_{lighting}^{Baseline}
$$

If H6 is supported:

$$
DRS_{MSC-DiT}
>
DRS_{Baseline}
$$

These inequalities represent hypotheses to be tested, not established findings.

---

# 4.53 Results Reporting Template

The final results section should use a structure similar to:

## 4.53.1 Overall Quantitative Results

| Model | FID | CLIP/Alignment | Object F1 | Material | Layout | Relation F1 | DRS |
|---|---:|---:|---:|---:|---:|---:|---:|
| Baseline | — | — | — | — | — | — | — |
| Multi-Space | — | — | — | — | — | — | — |
| Router | — | — | — | — | — | — | — |
| Relation Graph | — | — | — | — | — | — | — |
| **MSC-DiT** | — | — | — | — | — | — | — |

## 4.53.2 Human Evaluation

| Model | Realism | Requirement Satisfaction | Spatial Consistency | Style | Material |
|---|---:|---:|---:|---:|---:|
| Baseline | — | — | — | — | — |
| MSC-DiT | — | — | — | — | — |

## 4.53.3 Ablation

| Configuration | DRS | Relation F1 | Material | Layout |
|---|---:|---:|---:|---:|
| Full | — | — | — | — |
| − Layout | — | — | — | — |
| − Object | — | — | — | — |
| − Material | — | — | — | — |
| − Relation | — | — | — | — |
| − Router | — | — | — | — |

---

# 4.54 Interpretation Framework

The interpretation of results should distinguish between different types of improvement.

For example:

### Case A

FID improves but DRS does not.

This would indicate improved general visual quality without corresponding improvement in design compliance.

### Case B

DRS improves but FID remains similar.

This would suggest that semantic conditioning improves design requirement satisfaction without substantially changing general visual quality.

### Case C

Relationship F1 improves significantly.

This would provide evidence supporting the relationship representation.

### Case D

Material accuracy improves while other metrics remain similar.

This would provide evidence for the specialized material space.

### Case E

All semantic metrics improve but computational cost increases substantially.

This would indicate a performance-efficiency trade-off that should be discussed.

This type of interpretation is more scientifically informative than simply reporting that one model produces "better images."

---

# 4.55 Threats to Validity

Several limitations should be considered.

## 4.55.1 Dataset Bias

If the training dataset contains mostly certain styles or room types, the model may not generalize to underrepresented design categories.

## 4.55.2 Annotation Errors

Automatically generated semantic annotations may contain incorrect object, material, or relationship information.

## 4.55.3 Metric Limitations

FID and CLIP-based metrics cannot fully measure interior-design correctness.

## 4.55.4 Human Evaluation Subjectivity

Different interior designers may interpret aesthetic quality differently.

## 4.55.5 Model Scale

A larger baseline may outperform a smaller proposed model simply because of capacity.

Therefore, model size should be controlled in comparative experiments.

## 4.55.6 Prompt Distribution

If test prompts are too similar to training descriptions, the experiment may overestimate generalization.

---

# 4.56 Reproducibility

To make the research reproducible, the following information should be recorded:

- dataset version;
- dataset split;
- annotation methodology;
- prompt templates;
- random seeds;
- model configuration;
- parameter count;
- optimizer;
- learning rate;
- batch size;
- number of training steps;
- diffusion schedule;
- sampler;
- guidance scale;
- image resolution;
- software versions;
- hardware configuration.

The final implementation should also provide configuration files containing the complete experimental settings.

---

# 4.57 Experimental Pipeline

The complete experimental workflow is:

```text id="u7p3ck"
                    Dataset
                       │
                       ▼
             Semantic Annotation
                       │
                       ▼
                Train / Val / Test
                       │
             ┌─────────┴─────────┐
             ▼                   ▼
        Baseline Models     Proposed Models
             │                   │
             └─────────┬─────────┘
                       ▼
                 Image Generation
                       │
            ┌──────────┼──────────┐
            ▼          ▼          ▼
         Automatic  Semantic     Human
         Metrics    Metrics     Evaluation
            │          │          │
            └──────────┼──────────┘
                       ▼
                Statistical Test
                       │
                       ▼
                 Ablation Study
                       │
                       ▼
                 Failure Analysis
                       │
                       ▼
                Research Findings
```

---

# 4.58 Chapter Summary

This chapter established the experimental framework for evaluating the proposed Multi-Space Semantic Conditioning Diffusion Transformer.

The evaluation is deliberately multidimensional because interior-image generation cannot be adequately characterized by a single image-quality metric.

The proposed experimental framework evaluates:

$$
\boxed{
Visual\ Quality
+
Text\ Alignment
+
Object\ Accuracy
+
Material\ Accuracy
+
Layout\ Accuracy
+
Relationship\ Accuracy
+
Style
+
Lighting
+
Design\ Requirement\ Satisfaction
}
$$

The primary comparison is between a conventional text-conditioned diffusion/DiT baseline and the proposed multi-space semantic architecture.

A series of ablation experiments will isolate the contributions of:

- semantic decomposition;
- individual semantic spaces;
- condition routing;
- relationship graphs;
- semantic losses;
- object losses;
- material losses;
- relationship losses.

The experimental framework also introduces the proposed **Design Requirement Satisfaction (DRS)** measure as a direct attempt to connect natural-language design requirements with generated-image evaluation.

Importantly, this chapter does not assume that the proposed method will outperform the baselines. The hypotheses will be accepted, rejected, or partially supported according to the experimental evidence.

The next chapter can therefore use the actual experimental results to discuss the effectiveness, limitations, theoretical implications, and practical significance of the proposed text-only interior-design generation framework.

---

### Recommended dissertation flow after Chapter 4

Your five-chapter dissertation now has a coherent structure:

```text
CHAPTER 1
Introduction
    │
    ├── Problem
    ├── Objectives
    ├── Research Questions
    └── Hypotheses
             ↓
CHAPTER 2
Literature Review
    │
    ├── Diffusion
    ├── DiT
    ├── T2I
    ├── Controllable Generation
    ├── Interior AI
    └── Research Gap
             ↓
CHAPTER 3
Proposed Method
    │
    ├── Semantic Decomposition
    ├── 8 Semantic Spaces
    ├── Relationship Graph
    ├── Condition Router
    ├── Multi-Condition DiT
    └── Training Objectives
             ↓
CHAPTER 4
Experiments
    │
    ├── Baselines
    ├── Metrics
    ├── Main Experiment
    ├── Ablation
    ├── Human Evaluation
    └── Failure Analysis
             ↓
CHAPTER 5
Discussion & Conclusion
    │
    ├── Findings
    ├── Research Contributions
    ├── Limitations
    ├── Implications
    └── Future Work
```

The **most important next step** is Chapter 5, but before finalizing it, Chapter 4 should eventually be populated with your **actual experimental results**. A strong dissertation should not write Chapter 5 as though H1–H6 were already proven; it should derive the conclusions from the measured results.

## Prompt:
9/28/2026, 6:32:53 PM

gnerate conclusion

## Response:
9/28/2026, 6:32:55 PM · gpt-5-6-mini

Below is a PhD-style **Chapter 5 — Conclusion**, aligned with your proposed multi-space semantic conditioning + relationship graph + condition-router DiT architecture and the Chapter 4 experimental framework. I have kept the conclusions evidence-based rather than inventing experimental numerical results.

**document: Chapter 5 — Conclusion, Contributions, Limitations, and Future Work**

# CHAPTER 5  
# CONCLUSION, CONTRIBUTIONS, LIMITATIONS, AND FUTURE WORK

## 5.1 Introduction

This dissertation investigated the problem of text-only interior design image generation using a multi-space semantic conditioning framework integrated with a Diffusion Transformer (DiT). The central motivation of the research was that natural-language descriptions of interior spaces contain heterogeneous design requirements, including objects, spatial arrangements, materials, colors, styles, lighting conditions, architectural elements, and relationships between design components. Conventional text-to-image systems generally process these requirements through a unified text representation, which may not explicitly preserve the distinctions and interactions among different types of design information.

The research therefore proposed a semantic decomposition approach in which an interior-design description is transformed into multiple specialized semantic spaces. These spaces are subsequently encoded and supplied to a multi-condition generative architecture. In addition, an explicit relationship representation was introduced to model interactions among design entities, while a condition-routing mechanism was proposed to control how different semantic conditions influence the generation process.

The overall objective was not simply to improve visual realism, but to investigate whether explicit semantic organization can improve the ability of a generative model to satisfy complex interior-design requirements from text alone.

This chapter summarizes the research findings, discusses the contributions of the proposed framework, identifies its limitations, and presents directions for future research.

---

## 5.2 Summary of the Research

The research began with the observation that interior-design descriptions differ from ordinary image-generation prompts because they frequently contain multiple simultaneous constraints. For example, a single description may specify:

- the type and number of furniture items;
- the relative positions of objects;
- object attributes such as color, size, and shape;
- materials such as wood, marble, glass, or fabric;
- architectural elements such as windows, doors, ceilings, and fireplaces;
- a particular design style;
- a specific color palette;
- lighting direction and intensity; and
- relationships between objects.

These requirements are semantically different but visually interdependent.

To address this problem, the proposed framework decomposes a textual interior-design description into eight principal semantic spaces:

1. **Layout Space**
2. **Object Space**
3. **Material Space**
4. **Style Space**
5. **Lighting Space**
6. **Color Space**
7. **Architecture Space**
8. **Relationship Space**

The semantic representations are encoded and integrated through a multi-condition conditioning mechanism. The resulting representations are then provided to a Diffusion Transformer responsible for generating the latent image representation.

The proposed framework can therefore be summarized as:

$$
T
\rightarrow
\{L,O,M,S,Li,C,A,R\}
\rightarrow
C
\rightarrow
DiT
\rightarrow
z
\rightarrow
VAE
\rightarrow
I
$$

where $T$ represents the natural-language description, $L$ represents layout information, $O$ object information, $M$ material information, $S$ style information, $Li$ lighting information, $C$ color information, $A$ architectural information, $R$ relationship information, $z$ the generated latent representation, and $I$ the final interior image.

The experimental framework was designed to compare conventional text conditioning with progressively more structured semantic conditioning. It also included ablation experiments examining the contributions of individual semantic spaces, relationship representations, condition routing, and specialized training objectives.

---

## 5.3 Achievement of the Research Objectives

The first objective was to investigate the limitations of conventional text-only conditioning for interior-design image generation.

The research established a conceptual framework for analyzing interior-design descriptions as collections of heterogeneous semantic requirements rather than as a single undifferentiated textual representation. This provides a basis for evaluating specific categories of generation errors, including object omission, incorrect attributes, material inconsistency, spatial inconsistency, relationship errors, and style mismatch.

The second objective was to develop a semantic decomposition framework for interior-design descriptions.

The proposed framework separates the description into multiple semantic spaces corresponding to important dimensions of interior design. This decomposition provides a structured representation through which different categories of information can be processed independently before being integrated into the generative model.

The third objective was to incorporate explicit relationships between design elements.

The relationship representation models interactions between entities using a graph:

$$
G=(V,E)
$$

where $V$ represents design entities and $E$ represents relationships between them.

For example:

$$
\text{Sofa}
\xrightarrow{\text{faces}}
\text{Fireplace}
$$

and

$$
\text{CoffeeTable}
\xrightarrow{\text{between}}
\text{Sofa, Fireplace}
$$

This representation provides an explicit mechanism for describing relational constraints that may otherwise remain implicit in the textual representation.

The fourth objective was to develop a multi-condition Diffusion Transformer architecture.

The proposed architecture extends conventional text-conditioned diffusion generation by introducing specialized semantic conditioning and a condition-routing mechanism. Instead of treating all conditions identically, the architecture is designed to allow different semantic spaces to influence the generative process according to their functional roles.

The fifth objective was to establish an evaluation methodology specifically suited to interior-design generation.

In addition to conventional image-quality and text-image alignment metrics, the research introduced requirement-oriented evaluation measures, including object accuracy, material consistency, relationship accuracy, layout accuracy, and Design Requirement Satisfaction.

The sixth objective was to evaluate the proposed framework through controlled experiments and ablation studies.

The experimental methodology was designed to isolate the contribution of each major architectural component. Consequently, improvements or limitations can be associated with specific components rather than only with the complete system.

---

## 5.4 Main Research Findings

The primary contribution of this research is the formulation of interior-design image generation as a **multi-dimensional semantic conditioning problem** rather than exclusively as a text-to-image generation problem.

The proposed framework provides a structured way to represent the semantic diversity of interior-design descriptions. In particular, the separation of object, material, style, lighting, layout, architectural, color, and relationship information creates explicit representations corresponding to different design requirements.

The experimental framework further provides a mechanism for determining whether each semantic component contributes to generation quality. Rather than assuming that every semantic space is equally useful, the ablation experiments allow the importance of each component to be evaluated independently.

A second important finding concerns spatial and relational information. Interior design contains many requirements that are inherently relational. Statements such as "the sofa faces the fireplace" or "the coffee table is positioned between the sofa and the fireplace" cannot be fully characterized by identifying the individual objects alone. The relationship representation therefore addresses a distinct class of constraints.

A third finding concerns condition routing. Different semantic information can have different effects on the generated image. Style and lighting can influence the global visual appearance, whereas object and material information may have more localized effects. A condition-routing mechanism therefore provides a more flexible alternative to treating all semantic representations as equivalent conditioning information.

A fourth finding concerns evaluation. Conventional metrics such as image similarity or text-image alignment do not necessarily reveal whether an image satisfies the specific requirements contained in an interior-design description. A generated image may appear realistic while omitting an important object or violating a specified spatial relationship. Requirement-oriented metrics therefore provide complementary information that is particularly relevant to design applications.

The final conclusion is that **visual quality alone is insufficient for evaluating text-only interior-design generation**. A useful interior-design generation system must simultaneously address visual realism, semantic correctness, object completeness, spatial consistency, material consistency, and satisfaction of design requirements.

---

## 5.5 Contribution to Knowledge

The research makes several contributions to the field of generative artificial intelligence and AI-assisted interior design.

### 5.5.1 Multi-Space Semantic Representation

The first contribution is a formal semantic decomposition framework for interior-design descriptions.

Rather than representing the complete prompt using one undifferentiated conditioning representation, the framework defines specialized spaces:

$$
\mathcal{S}
=
\{
S_L,S_O,S_M,S_S,S_{Li},S_C,S_A,S_R
\}
$$

This representation provides a systematic way to describe the different semantic dimensions that must be preserved during image generation.

---

### 5.5.2 Relationship-Aware Interior Generation

The second contribution is the explicit incorporation of relationships between design elements.

The relationship graph:

$$
G=(V,E)
$$

provides an intermediate representation between natural language and image generation. This enables the system to represent relational requirements independently from object identity.

The approach is particularly relevant to interior design because many design constraints are inherently relational rather than object-centric.

---

### 5.5.3 Multi-Condition Diffusion Transformer

The third contribution is the proposed Multi-Space Conditioned Diffusion Transformer architecture.

The architecture introduces specialized conditioning pathways and a condition-routing mechanism into the DiT generation process. The objective is to allow semantic conditions to influence the image representation according to their respective roles.

Conceptually:

$$
C=
R(
C_L,
C_O,
C_M,
C_S,
C_{Li},
C_C,
C_A,
C_R
)
$$

where $R$ represents the learned condition-routing and fusion mechanism.

---

### 5.5.4 Requirement-Oriented Evaluation

The fourth contribution is an evaluation framework that connects natural-language design requirements directly to generated image properties.

The proposed Design Requirement Satisfaction metric is:

$$
DRS=
\frac{\sum_{i=1}^{N}r_i}{N}
$$

where $r_i$ indicates whether requirement $i$ is satisfied.

A weighted version can additionally account for requirements with different levels of importance:

$$
DRS_w=
\frac{\sum_i w_i r_i}{\sum_i w_i}
$$

This formulation provides a more application-oriented evaluation perspective than relying exclusively on general image-generation metrics.

---

### 5.5.5 Systematic Ablation Framework

The fifth contribution is a systematic experimental methodology for determining the effect of individual semantic components.

The framework allows the contribution of:

- layout information;
- object information;
- material information;
- style information;
- lighting information;
- color information;
- architectural information;
- relationship information;
- condition routing; and
- specialized losses

to be investigated separately.

This provides a methodological contribution for future research on structured conditioning for domain-specific image generation.

---

## 5.6 Theoretical Implications

The research has several theoretical implications.

First, it suggests that domain-specific text-to-image generation can benefit from explicitly modeling the heterogeneous semantic structure of domain descriptions. Interior design provides a useful example because its language contains several distinct but interacting semantic dimensions.

Second, the research demonstrates the conceptual value of distinguishing **entity information** from **relationship information**.

Traditional object-oriented conditioning can identify that a sofa and fireplace exist. However, identifying both objects does not necessarily specify how they should be positioned relative to one another. Relationship-aware representations therefore provide an additional level of semantic structure.

Third, the research suggests that conditioning mechanisms can be designed according to the characteristics of the target domain. A general text-to-image model does not necessarily need to expose every domain-specific representation. However, specialized applications such as interior design may benefit from explicit representations of layout, materials, architectural structure, and relationships.

Fourth, the research provides a conceptual bridge between natural-language understanding and generative visual synthesis. The semantic decomposition stage can be viewed as an intermediate representation that translates natural-language design requirements into structured conditions suitable for image generation.

---

## 5.7 Practical Implications

The proposed framework has potential applications in several areas.

### Interior Design Concept Generation

Users could describe a desired room using natural language and receive multiple design alternatives without providing an image, floor plan, or CAD model.

### Early-Stage Design Exploration

Architects and interior designers could use text-based generation during early ideation to explore combinations of styles, materials, colors, furniture, and lighting.

### Design Alternative Generation

The semantic representation allows selected conditions to be modified while keeping other conditions fixed. For example, a user could maintain the same room layout while changing:

- material;
- furniture style;
- lighting;
- color palette; or
- decorative style.

### Educational Applications

The framework could also support educational environments in which students describe interior-design concepts and visualize the resulting designs.

### Human-AI Design Interaction

The semantic decomposition architecture provides a potential foundation for more controllable human-AI design systems in which users can modify individual design requirements through natural language.

---

## 5.8 Limitations of the Research

Despite its contributions, the proposed framework has several limitations.

### 5.8.1 Dependence on Semantic Decomposition Quality

The quality of the final image depends partly on the accuracy of the semantic decomposition stage.

If the language-processing component incorrectly identifies an object, material, relationship, or spatial constraint, the generative model may receive an incorrect condition.

Therefore:

$$
T \rightarrow S
$$

remains an important source of potential error.

Future systems should investigate more robust semantic parsing and uncertainty-aware representations.

---

### 5.8.2 Ambiguity of Natural Language

Interior-design language is often ambiguous.

For example:

> "a large modern living room with warm lighting"

does not uniquely define the exact room dimensions, furniture arrangement, lighting intensity, or material configuration.

Consequently, multiple generated images can be valid interpretations of the same description.

This means that evaluation should distinguish between:

1. requirements explicitly specified by the user; and
2. design decisions that remain unspecified.

---

### 5.8.3 Difficulty of Exact Spatial Control

Although the relationship graph provides explicit relational information, translating symbolic relationships into precise continuous image coordinates remains challenging.

A statement such as:

$$
A \xrightarrow{\text{left-of}} B
$$

does not specify the exact distance, scale, perspective, or depth relationship between $A$ and $B$.

Future work may therefore investigate stronger spatial representations.

---

### 5.8.4 Dataset Limitations

The performance of a domain-specific model depends strongly on the diversity and quality of the training dataset.

Potential dataset limitations include:

- insufficient architectural diversity;
- repeated design styles;
- limited material diversity;
- biased geographic representation;
- inconsistent image quality;
- incomplete semantic annotations; and
- limited representation of unusual room configurations.

Consequently, dataset construction remains an important component of future development.

---

### 5.8.5 Evaluation Limitations

Automated metrics cannot completely capture professional interior-design quality.

A generated image can satisfy explicit textual requirements while still being considered aesthetically weak or impractical by a professional designer.

Human evaluation is therefore necessary to complement automated evaluation.

---

### 5.8.6 Computational Requirements

Diffusion Transformer models can require substantial computational resources during training and inference. The cost increases further when multiple conditioning pathways and larger training datasets are introduced.

This can limit deployment on consumer hardware and mobile devices.

Future research should therefore investigate model compression, quantization, distillation, efficient attention mechanisms, and lightweight architectures.

---

## 5.9 Future Research

Several directions can extend this research.

### 5.9.1 Automatic Semantic Graph Construction

Future systems can improve the conversion of natural language into structured design graphs.

A future pipeline could be:

$$
Text
\rightarrow
Entities
\rightarrow
Attributes
\rightarrow
Relations
\rightarrow
Graph
$$

The graph could then provide structured conditioning for the generative model.

---

### 5.9.2 Stronger Spatial Representation

Future work could investigate continuous spatial representations in addition to symbolic relationships.

For example:

$$
R_i=(x_i,y_i,z_i,w_i,h_i,d_i)
$$

could represent the approximate spatial position and dimensions of an object.

Combining symbolic and continuous spatial representations may provide more precise control.

---

### 5.9.3 Region-Level Conditioning

Another research direction is to associate semantic conditions with image regions.

For example:

$$
C_{object}
\rightarrow
\text{local image tokens}
$$

while:

$$
C_{style}
\rightarrow
\text{global image tokens}
$$

This could enable more precise localization of object and material conditions.

---

### 5.9.4 Iterative Text-Based Editing

The proposed framework can be extended from one-shot generation to iterative design.

For example:

```text
Initial Description
        ↓
Generated Interior
        ↓
User Modification
        ↓
Updated Semantic Representation
        ↓
Controlled Regeneration
```

A user could therefore request:

> "Keep the sofa and layout, but replace the marble floor with dark oak."

Only the relevant semantic conditions would need to be modified.

---

### 5.9.5 Multi-View Interior Generation

A major extension would be consistent generation of the same interior from multiple viewpoints.

The objective would be:

$$
I_1,I_2,\ldots,I_n
$$

such that all generated images represent the same underlying room.

This would require stronger 3D-aware representations and cross-view consistency mechanisms.

---

### 5.9.6 3D and Scene Generation

Future research could extend the framework from 2D image generation to 3D scene generation.

The semantic representation could become:

$$
Text
\rightarrow
Scene\ Graph
\rightarrow
3D\ Scene
\rightarrow
Rendered\ Images
$$

Such an extension would have greater practical value for architectural visualization and interactive design systems.

---

### 5.9.7 Integration with CAD and BIM

Although the present research intentionally focuses on text-only input, future systems could combine semantic generation with CAD or Building Information Modeling (BIM).

The semantic graph could provide an intermediate representation connecting:

$$
Natural\ Language
\leftrightarrow
Semantic\ Graph
\leftrightarrow
CAD/BIM
\leftrightarrow
Image
$$

This would create a bridge between generative AI and professional architectural workflows.

---

### 5.9.8 Efficient and Lightweight DiT

Future research should also investigate efficient variants of the proposed architecture.

Potential approaches include:

- knowledge distillation;
- low-rank adaptation;
- quantization;
- parameter sharing;
- sparse attention;
- token reduction;
- latent-space optimization; and
- smaller specialized DiT architectures.

These techniques could make semantic-conditioned generation more practical for local workstations and eventually edge devices.

---

### 5.9.9 Human-AI Collaborative Design

The long-term objective is not necessarily to replace professional designers, but to provide a controllable generative assistant.

A future system could allow a designer to manipulate semantic dimensions independently:

```text
                Designer
                   │
                   ▼
        Natural-Language Command
                   │
                   ▼
          Semantic Interpreter
                   │
        ┌──────────┼──────────┐
        ▼          ▼          ▼
     Layout     Material    Style
        │          │          │
        └──────────┼──────────┘
                   ▼
          Semantic Condition
                   │
                   ▼
             DiT Generator
                   │
                   ▼
            Interior Image
```

Such a system could support rapid exploration while allowing human designers to retain control over the final design decisions.

---

## 5.10 Final Conclusion

This dissertation investigated text-only interior-design image generation as a structured semantic generation problem. The research proposed a Multi-Space Semantic Conditioning framework in which natural-language descriptions are decomposed into specialized representations for layout, objects, materials, style, lighting, color, architecture, and relationships.

The proposed framework further introduced a relationship graph to explicitly represent interactions among design elements and a condition-routing mechanism to control how different semantic conditions influence the Diffusion Transformer.

The central argument of this research is that interior-design descriptions contain heterogeneous semantic requirements that should not necessarily be treated as a single undifferentiated conditioning signal. Explicit semantic decomposition provides a structured representation through which these requirements can be analyzed, conditioned, and evaluated.

The research also emphasizes that successful interior-design generation cannot be defined solely by visual realism. A useful system must also preserve the semantic requirements specified by the user. Consequently, the proposed evaluation framework combines general image-generation metrics with requirement-oriented measures covering objects, attributes, materials, layout, relationships, style, lighting, and overall design-requirement satisfaction.

The proposed research therefore contributes an integrated framework connecting:

$$
\boxed{
Natural\ Language
\rightarrow
Semantic\ Decomposition
\rightarrow
Structured\ Conditions
\rightarrow
Relationship\ Representation
\rightarrow
Multi\text{-}Condition\ DiT
\rightarrow
Interior\ Image
}
$$

The broader significance of the work lies in demonstrating a pathway toward more controllable domain-specific generative AI. Rather than treating text-to-image generation as a purely visual synthesis problem, the proposed framework considers the semantic structure of the domain itself as an important component of generation.

Future research can extend this framework toward precise spatial control, multi-view consistency, 3D scene generation, CAD/BIM integration, interactive editing, and efficient deployment. These directions could contribute to the development of generative systems capable of supporting increasingly sophisticated human-AI collaboration in architectural and interior-design workflows.

Ultimately, the research establishes a foundation for investigating how structured semantic representations can improve the controllability, interpretability, and requirement satisfaction of text-driven visual generation for interior design.

This conclusion is intentionally written so you can insert your **actual Chapter 4 numerical results** later without claiming results that have not yet been experimentally verified.

## Prompt:
9/28/2026, 6:34:06 PM

generate heading

## Response:
9/28/2026, 6:34:07 PM · gpt-5-6-mini

For your dissertation, I recommend this **Chapter 5 heading structure**:

**document: Chapter 5 — Conclusion**

# CHAPTER 5  
# CONCLUSION, CONTRIBUTIONS, LIMITATIONS, AND FUTURE WORK

## 5.1 Introduction

## 5.2 Summary of the Research

## 5.3 Achievement of the Research Objectives

## 5.4 Summary of Research Findings

## 5.5 Research Contributions

### 5.5.1 Multi-Space Semantic Representation

### 5.5.2 Relationship-Aware Interior Generation

### 5.5.3 Multi-Condition Diffusion Transformer

### 5.5.4 Requirement-Oriented Evaluation Framework

### 5.5.5 Systematic Ablation and Evaluation Methodology

## 5.6 Theoretical Contributions

## 5.7 Methodological Contributions

## 5.8 Practical Contributions

## 5.9 Implications of the Research

### 5.9.1 Implications for Text-to-Image Generation

### 5.9.2 Implications for Interior Design

### 5.9.3 Implications for Generative AI

### 5.9.4 Implications for Human-AI Collaborative Design

## 5.10 Limitations of the Research

### 5.10.1 Semantic Decomposition Limitations

### 5.10.2 Natural-Language Ambiguity

### 5.10.3 Spatial and Relationship Control

### 5.10.4 Dataset Limitations

### 5.10.5 Evaluation Limitations

### 5.10.6 Computational Limitations

## 5.11 Future Research Directions

### 5.11.1 Automatic Semantic Graph Construction

### 5.11.2 Fine-Grained Spatial Control

### 5.11.3 Region-Level Semantic Conditioning

### 5.11.4 Iterative Text-Based Design Editing

### 5.11.5 Multi-View Interior Generation

### 5.11.6 3D Interior Scene Generation

### 5.11.7 CAD and BIM Integration

### 5.11.8 Lightweight and Efficient DiT

### 5.11.9 Human-AI Collaborative Interior Design

## 5.12 Final Conclusion

## 5.13 Chapter Summary

This structure gives your dissertation a clear progression:

**Research → Findings → Contributions → Implications → Limitations → Future Work → Conclusion**

It also matches your **Chapter 3 proposed MSC-DiT architecture** and **Chapter 4 experiments/ablations** closely.



---
Powered by [ChatGPT Exporter](https://www.chatgptexporter.com)