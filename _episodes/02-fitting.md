---
title: "The Fitting Method"
teaching: 5
exercises: 40
questions:
- "What is this fitting method?"
- "how do we use it to calculate the efficiency?"
- "Why is it better than sideband subtraction?"
objectives:
- "Understand the fitting method, it's advantages and disadvantages"
- "Learn how to implement this method using ROOT library for c++"

keypoints:
- "Skrrt."
- "There are essentially three important files in an EDAnalyzer package, the source code in c++, the python config file and a Buildfile for tracking dependencies."
---
## Prerequisite

To prepare for this tutorial, clone the respective github repository

~~~
git clone https://github.com/AthomsG/CMS-tutorial
~~~
{: .language-bash}

## Structure

First, a brief explanaiton of the method we'll be studying.
The fitting method consists in dividing the quantity we want to use to calculate the efficiency into a certain amout of bins and then fitting the invariant mass of the muons (All and Passing) on the specified region. 
To compute the efficiency we simply divide the yield from the fit of the Passing muons by the yield of the fit of All the muons. The following image tries to illustrate this idea.

<img width="500px" src="../fig/esquema.png">


Let's start exploring our dataset. From the

~~~
root T\&P_UPSILON_DATA.root
~~~
{: .language-bash}

If everything's right, you should get the following output:
~~~
Attaching file T&P_UPSILON_DATA.root as _file0...
U(TFile *) 0x7fe2f34ca270
~~~
{: .output}

Now, before we start fitting the invariant mass it's important to look at it's shape first. To visualize our data's invariant mass, do:

~~~
UPSILON_DATA->Draw("InvariantMass")
~~~
{: .language-bash}

If you got the following result, we're ready to go

<img width="500px" src="../fig/output.png">

The dataset used in this exercise consists of 986100 entries (muons) with an associated invariant mass, pT, eta, phi and a binary **PassingProbeTrackingMuon** that is 1 in case the corresponding muon is a Tracker Muon and 0 in case it isn't.

| UPSILON_DATA |
 ------------- |
| InvarianMass |
| PassingProbeTrackingMuon | 
| ProbeMuon_Pt |
| ProbeMuon_Eta |
| ProbeMuon_Phi|

Open `Efficiency.C`.

We'll start by calculating the efficiency associated with pT.
It is useful to have an idea of the distribution of the quantity we want to study. In order to do this, we'll repeat the steps followed to plot the invariant mass but for the **ProbeMuon_Pt**.

~~~
root T\&P_UPSILON_DATA.root
UPSILON_DATA->Draw("ProbeMuon_Pt")
~~~
{: .language-bash}

Hmm.. seems like our domain is larger than we need it to be. To fix this, we can apply a contraint to our plot. Try:

~~~
root T\&P_UPSILON_DATA.root
UPSILON_DATA->Draw("ProbeMuon_Pt", "ProbeMuon_Pt < 20")
~~~
{: language-bash}

Now that we have a clear view of the transverse momentum, we can now choose the appropriate bins for our fit. Remember that we need a fair amount of data in order to have a good fit, so be careful not to include too few events on a given bin. I've left a suggestion for an appropriate bin setup on `Bin_Suggestion.txt`



We also notice we have a python configuration file called `demoanalyzer_cfg.py` (unlike its cousins, it is not inside the `python` directory).  This is the default configurator for the `DemoAnalyzer.cc` code.

Finally, there is a `BuildFile.xml`, where we can include any dependencies if needed.

All EDAnalyzers are created equal.  Of course, if made with the same `mkedanlzr`, they will look identical.  The `DemoAnalyzer.cc` is a skeleton, written in C++, that contains all the basic ingredients to use CMSSW libraries.  So, in order to perform a physics analysis, and extract information from our CMS open data, we just need to understand what to add to this code and how to configure it.


{% include links.md %}
