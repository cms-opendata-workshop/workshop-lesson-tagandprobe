---
title: "Introduction"
teaching: 10
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

## What is the tag and probe method?

The **tag and probe** method is a **data-driven technique** for measuring particle detection efficiencies. It is based on the decays of known resonances (e.g. J/ψ, ϒ and Z) to pairs of the particles being studied. In this exercise, these particles are muons, and the ϒ(1S) resonance is nominally used.
The determination of the detector efficiency is a critical ingredient in any physics measurement. It accounts for the particles that were produced in the collision but escaped detection (did not reach the detector elements, were missed by the reconstructions algorithms, etc). It can be in general estimated using simulations, but simulations need to be calibrated with data. The T&P method here described provides a useful and elegant mechanism for extracting efficiencies directly from data.

## What is "tag" and "probe"?

The resonance used to calculate the efficiency decays to a pair of particles: the tag and the probe. 

* Tag muon = well identified, triggered muon (tight selection criteria).
* Probe muon = unbiased set of muon candidates (very loose selection criteria), either passing or failing the criteria for which the efficiency is to be measured.

## How do we calculate the efficiency?

The efficiency is given by the fraction of probe muons that pass a given criteria (in this case, the **Muon ID** which we explain [here](#cms-muon-identification-reconstruction)):

<img width="300px" src="../fig/efficiency.svg" alt="Efficiency equation">

The denominator corresponds to the number of resonance candidates (tag+probe pairs) reconstructed in the dataset. The numerator corresponds to the subset for which the probe passes the criteria. 

The tag+probe invariant mass distribution is used to select only signal, that is, only true Y(1S) candidates decaying to dimuons. This is achieved in this exercise with two methods: fitting and side-band-subtraction.

## CMS Muon identification reconstruction

In this workshop, the passing particles for which we are evaluating the efficiency are **Tracker muons**.  We present here a short description of the Muon Identification and Reconstruction employed in the CMS experiment at the LHC.

![CMS muon id](../fig/tracker_muon.png)

In the standard CMS reconstruction for proton-proton collisions, tracks are first reconstructed independently in the inner tracker and in the muon system. Based on these objects, two reconstruction approaches are used:

* Tracker Muon reconstruction (red line): In this approach, all tracker tracks with pT > 0.5 GeV/c and total momentum p > 2.5 GeV/c are considered as possible muon candidates and are extrapolated to the muon system taking into account the magnetic field;

* Standalone Muon reconstruction (green line): they are all tracks of the segments reconstructed in the muon chambers (performed using segments and hits from Drift Tubes - DTs in the barrel region, Cathode strip chambers - CSCs in the endcaps and Resistive Plates Chambers - RPCs for all muon system) are used to generate "seeds" consisting of position and direction vectors and an estimate of the muon transverse momentum;

* Global Muon reconstruction (blue line): For each standalone-muon track, a matching tracker track is found by comparing parameters of the two tracks propagated onto a common surface.

You can find more details concerning CMS Muon Identification and reconstruction in this paper [JINST 7 (2012) P10002].

{% include links.md %}
