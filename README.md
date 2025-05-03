# Transport Model

This repository contains a C++ implementation of the PEG–carbonate transport model described in  
*Kwarkye et al. (2025)* :contentReference[oaicite:0]{index=0}:contentReference[oaicite:1]{index=1}. The model simulates advection, dispersion, and adsorption–desorption (with hysteresis) of polyethylene glycol (PEG) in a porous carbonate medium.

---

## Table of Contents

1. [Model Overview](#model-overview)
   - [Basic Model](#basic-model)
   - [Reactions](reactions)
   - [Sorption Isotherms](sorption-isotherms)  
3. [Parameters](#parameters)  
   - [Descretization Parameters](#descretization-parameters)  
   - [Transport Parameters](#transport-parameters)  
   - [Sorption Parameters](#sorption-parameters)  
   - [Degradation Parameters](#degradation-parameters)  
5. [References](#references)  

---

## Model Overview

The model solves the advection–dispersion equation with equilibrium/non-equilibrium adsorption and incorporates Langmuir-type adsorption with hysteresis:

### Basic Model 

$`
\frac{\partial (\theta\,c)}{\partial t} =\frac{\partial}{\partial x}\Bigl(\theta\,D\frac{\partial c}{\partial x}\Bigl) - 
\,q\frac{\partial c}{\partial x} - \,P
`$


### Reactions

$`
P = f\rho\frac{\partial s(c)_{eq}}{\partial t} + (1 - f)\rho\alpha(s(c)_{neq} - \gamma) + \theta\mu_lc + \rho\mu_{s_{eq}}c
`$

$`
\frac{\partial s(c)_{eq}}{\partial t}=\Gamma(c)
`$

$`
\frac{\partial \gamma}{\partial t}=\alpha(s(c)_{neq} - \gamma), \,s(c)_{neq}=\Gamma(c)
`$

### Sorption Isotherms

$`\Gamma(c)`$ represents the adsorption isotherm, which is defined according to the following isotherms:

**Linear Isotherm**

$`
\Gamma(c) = Kc
`$

**Langmuir Isotherm**

$`
\Gamma(c) = \frac{s_{max}Kc}{1+Kc}
`$

**Freundlich Isotherm**

$`
\Gamma(c) = Kc^n
`$

---

## Parameters

### Descretization Parameters

| Parameter | Symbol | Units        | Description                                |
|-----------|--------|--------------|--------------------------------------------|
| Column length             | L      | cm           | Total length of the flow domain            |
| Spatial discretization    | Δx     | cm           | Grid spacing                               |
| Time step                 | Δt     | h            | Temporal resolution                        |
| Simulation step                 | T     | h            | Total Simulation time                        |


### Transport Parameters

| Parameter | Symbol | Units        | Description                                |
|-----------|--------|--------------|--------------------------------------------|
| Water content             | θ      | – (vol. frac.)   | Porosity × saturation degree             |
| Dispersion length              |$`\lambda`$      | cm         | solute dispersion in porous medium      |
| Molecular diffusion              |$`D_m`$      | $`cm^2\, h^{-1}`$         | solute dispersion in porous medium      |
| Pulse concentration             | c      | mg L⁻¹           | Solute concentration in stock solution            |
| Fraction of mobile region    | $`\beta`$     | -           | partition between mobile and immobile regions                               |
| mob-imobile exh rate                 | $`\omega`$     | $`h^{-1}`$            | Mass exchange rate between mobile and immobile regions                        |

### Sorption Parameters

| Parameter | Symbol | Units        | Description                                |
|-----------|--------|--------------|--------------------------------------------|
| Bulk density              | ρ      | g L⁻¹         | Dry bulk density of the packed medium      |
| fraction of eq sites             | f      | -           | Partition between equilibrium and non-equilibrium sorption sites            |
| Neq mass transfer rate    | $`\alpha`$     | $`h^{-1}`$           | Mass transfer rate to non-equilibrium adsorption site                               |
| Isotherm constant                 | K     | $`L\, mg^{-1}`$            | Sorption isotherm constant                        |
| Adsorption capacity                 | Smax/n     | $`mg\, g^{-1}`$ if smax            | adsorption capacity for Langmuir isotherm or n for Freundlich                        |

### Degradation Parameters

| Parameter | Symbol | Units        | Description                                |
|-----------|--------|--------------|--------------------------------------------|
| Solution degradation rate             | $`\mu_l`$      | $`h^{-1}`$   | Rate of solute degradation in solution             |
| Degradation at Eq sites              | $`\mu_{s_{eq}}`$      | $`h^{-1}`$         | Degradation of solute adsorbed at equilibrium sites      |
| Degradation at Neq sites              | $`\mu_{s_{neq}}`$      | $`h^{-1}`$         | Degradation of solute adsorbed at nonequilibrium sites      |

---

## References

