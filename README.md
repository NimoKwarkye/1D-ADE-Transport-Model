# Transport Model

This repository contains a C++ implementation of the PEG–carbonate transport model described in  
*Kwarkye et al. (2025)* :contentReference[oaicite:0]{index=0}:contentReference[oaicite:1]{index=1}. The model simulates advection, dispersion, and adsorption–desorption (with hysteresis) of polyethylene glycol (PEG) in a porous carbonate medium.

---

## Table of Contents

1. [Model Overview](#model-overview)  
2. [Parameters](#parameters)  
   - [Physical Properties](#physical-properties)  
   - [Flow & Dispersion](#flow--dispersion)  
   - [Adsorption Isotherm](#adsorption-isotherm)  
   - [Hysteresis](#hysteresis)  
   - [Two-Site Adsorption](#two-site-adsorption)  
3. [Usage](#usage)  
4. [References](#references)  

---

## Model Overview

The model solves the advection–dispersion equation with equilibrium/non-equilibrium adsorption and incorporates Langmuir-type adsorption with hysteresis:

$$

\frac{\partial (\theta c)}{\partial t} + \frac{\partial (q\,c)}{\partial x}
= \frac{\partial}{\partial x}\!\Bigl(\theta D\,\frac{\partial c}{\partial x}\Bigr)- \rho\,\frac{\partial s}{\partial t}.

$$
---

## Parameters

### Physical Properties

| Parameter | Symbol | Units        | Description                                |
|-----------|--------|--------------|--------------------------------------------|
| Water content             | θ      | – (vol. frac.)   | Porosity × saturation degree             |
| Bulk density              | ρ      | g L⁻¹         | Dry bulk density of the packed medium      |
| Column length             | L      | cm           | Total length of the flow domain            |
| Spatial discretization    | Δx     | cm           | Grid spacing                               |
| Time step                 | Δt     | h            | Temporal resolution                        |

### Flow & Dispersion

| Parameter        | Symbol   | Units         | Description                                                                      |
|------------------|----------|---------------|----------------------------------------------------------------------------------|
| Darcy velocity   | q        | cm h⁻¹        | Interstitial groundwater velocity                                                |
| Dispersion length| λ        | cm            | Dispersivity; used to compute dispersion coefficient: \(D = \frac{\lambda\,q}{\theta}\) :contentReference[oaicite:2]{index=2}:contentReference[oaicite:3]{index=3} |
| Dispersion coeff.| D        | cm² h⁻¹       | Apparent dispersion coefficient                                                  |

### Adsorption Isotherm

Langmuir equilibrium (no hysteresis):
\[
s = \frac{s_\mathrm{max}\,K\,c}{1 + K\,c}
\tag{1}
\]
- **Maximum sorption capacity**  
  - Symbol: `s_max`  
  - Units: mg g⁻¹  
  - Definition: maximum mass of PEG adsorbed per g of solid :contentReference[oaicite:4]{index=4}:contentReference[oaicite:5]{index=5}  
- **Langmuir equilibrium constant**  
  - Symbol: `K`  
  - Units: L mg⁻¹  
  - Definition: affinity constant of PEG to clay surfaces :contentReference[oaicite:6]{index=6}:contentReference[oaicite:7]{index=7}  

### Hysteresis

To account for adsorption–desorption hysteresis, the model uses:
\[
s = \frac{s_\mathrm{max}\,K\,c}{(c/c_a)^h + K\,c}
\tag{2}
\]
- **Hysteresis coefficient**  
  - Symbol: `h`  
  - Units: – (dimensionless; 0 ≤ h ≤ 1)  
  - Definition:  
    - `h = 0` → fully reversible desorption  
    - `h = 1` → fully irreversible  
  - Controls how past maximum concentration (`c_a`) suppresses desorption :contentReference[oaicite:8]{index=8}:contentReference[oaicite:9]{index=9}  
- **Historical max concentration**  
  - Symbol: `c_a`  
  - Units: mg L⁻¹  
  - Definition: highest PEG concentration encountered so far (advances during breakthrough) :contentReference[oaicite:10]{index=10}:contentReference[oaicite:11]{index=11}  

### Two-Site Adsorption

Fractional partitioning between equilibrium and kinetically-limited sites:

| Parameter                      | Symbol       | Units      | Description                                                        |
|--------------------------------|--------------|------------|--------------------------------------------------------------------|
| Equilibrium partition         | `f`          | –          | Fraction of total sites at equilibrium                            |
| Non-equilibrium partition     | `1 - f`      | –          | Fraction of kinetically controlled sites                           |
| Kinetic rate coefficient      | `alpha`      | h⁻¹        | Rate of mass exchange at non-equilibrium sites                     |
| Non-equilib. Langmuir constant| `K_neq`      | L mg⁻¹     | Affinity at non-eq. sites                                          |
| Equilib. Langmuir constant    | `K_eq` ≡ `K` | L mg⁻¹     | Affinity at equilibrium sites                                      |
| Non-equilib. sorption max     | `s_eq, s_neq`| mg g⁻¹     | Max capacity split: `s_eq = f·s_max`, `s_neq = (1–f)·s_max`           |

---



