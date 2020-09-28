---
title: "Introduction"
teaching: 5
exercises: 0
questions:
- "What is tag and probe method?"
- "How to get efficiency?"
objectives:
- "Know what is tag and probe method."
- "Know what kind of efficiency we are looking for."
keypoints:
- "Tag and probe are labels for each muon of dimuon resonance."
- "Tag is a biased particle while probe are unbiased."
- "The efficiency we are looking in this exercise is for tracker muons."
---

## What is tag and probe method?

The tag and probe method consists in calculating the efficiency of a dataset without depending on simulations. As it doesn't need simulations, it's necessary to use known resonances such as J/ψ, ϒ and Z Boson. We'll be focusing on the first two during this workshop.

## What is "tag" and "probe"?

In dimuons resonances we work with at least two muons which are labeled as tag and probe:

* Tag muon = well identified, triggered muon (tight selection criteria).
* Probe muon = unbiased set of the desired particle type with a very loose selection criteria.

## How to get an efficiency?

We used a simple definition of efficiency:

<img width="300px" src="../fig/efficiency.svg" alt="Efficiency equation">

In this workshop, the passing particles that we are evaluating are **Tracker muons** and the equation above returns the resonance reconstruction efficiency. In the section below, we make a short description of the Muon Identification and Reconstruction definition used by the CMS experiment.

## CMS Muon identification reconstruction

![CMS muon id](../fig/tracker_muon.png)

In the standard CMS reconstruction for proton-proton collisions, tracks are first reconstructed independently in the inner tracker and in the muon system. Based on these objects, two reconstruction approaches are used:

* Tracker Muon reconstruction (red line): In this approach, all tracker tracks with pT > 0.5 GeV/c and total momentum p > 2.5 GeV/c are considered as possible muon candidates and are extrapolated to the muon system taking into account the magnetic field;

* Standalone Muon reconstruction (green line): they are all tracks of the segments reconstructed in the muon chambers (performed using segments and hits from Drift Tubes - DTs in the barrel region, Cathode strip chambers - CSCs in the endcaps and Resistive Plates Chambers - RPCs for all muon system) are used to generate "seeds" consisting of position and direction vectors and an estimate of the muon transverse momentum;

* Global Muon reconstruction (blue line): For each standalone-muon track, a matching tracker track is found by comparing parameters of the two tracks propagated onto a common surface.

You can find more details concerning CMS Muon Identification and reconstruction in this paper [JINST 7 (2012) P10002].

{% include links.md %}
