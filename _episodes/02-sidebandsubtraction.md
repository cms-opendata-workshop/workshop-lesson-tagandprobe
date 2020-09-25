---
title: "Introduction"
teaching: 5
exercises: 40
questions:
- "HWhat is sideband subtraction?"
objectives:
- "Get first efficiency."
keypoints:
- "There is a file in **main/config/settings.cpp** where you can add some options."
- "You can edit the binnig in **main/classes/PtEtaPhi.h** file."
---

## Getting files

### Getting sideband subtraction code

Firstly, we need to get the code. Type on your terminal;

~~~
git clone -b sideband https://github.com/allanjales/efficiency_tagandprobe
cd efficiency_tagandprobe
~~~
{: .language-bash}

### Getting Run 2011 dataset created with CMS Open Data 

Now we want to copy Upsilon dataset files to machine running this command (It is about 441 MB):

~~~
wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1Fj-rrKts8jSSMdwvOnvux68ydZcKB521' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1Fj-rrKts8jSSMdwvOnvux68ydZcKB521" -O Run2011A_MuOnia_Upsilon.root && rm -rf /tmp/cookies.txt
~~~
{: .language-bash}

This code seems to be large, but it is required to copy large files from google drive.

### Getting Monte Carlo dataset created with CMS Open Data 

Run this code to download the simulated data ntupple for Upsilon (It is about 66 MB):

~~~
wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1ZzAOOLCKmCz0Q6pVi3AAiYFGKEpP2efM' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1ZzAOOLCKmCz0Q6pVi3AAiYFGKEpP2efM" -O Upsilon1SToMuMu_MC_full.root && rm -rf /tmp/cookies.txt
~~~
{: .language-bash}

## Preparing for running
sh
$ cd main


{% include links.md %}