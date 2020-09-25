---
title: "Sideband subtraction method"
teaching: 5
exercises: 40
questions:
- "What is sideband subtraction?"
objectives:
- "Learn how to set bins in this tool."
- "Get efficiency by sideband subtraction real and simulated data."
keypoints:
- "There is a file in **main/config/settings.cpp** where you can add some options."
- "You can edit the binnig in **main/classes/PtEtaPhi.h** file."
---

## Preparing files

Firstly, we need to get the code. Type on your terminal;

~~~
git clone -b sideband https://github.com/allanjales/efficiency_tagandprobe
cd efficiency_tagandprobe
~~~
{: .language-bash}

Now we want to copy Upsilon dataset (from run 2011) file to machine running this command (It is about 441 MB):

~~~
wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1Fj-rrKts8jSSMdwvOnvux68ydZcKB521' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1Fj-rrKts8jSSMdwvOnvux68ydZcKB521" -O Run2011A_MuOnia_Upsilon.root && rm -rf /tmp/cookies.txt
~~~
{: .language-bash}

This code seems to be large, but it is required to copy large files from google drive.

Run this code to download the simulated data ntupple for Upsilon (It is about 66 MB):

~~~
wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1ZzAOOLCKmCz0Q6pVi3AAiYFGKEpP2efM' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1ZzAOOLCKmCz0Q6pVi3AAiYFGKEpP2efM" -O Upsilon1SToMuMu_MC_full.root && rm -rf /tmp/cookies.txt
~~~
{: .language-bash}

Now, verify if everything is ok:

~~~
ls
~~~
{: .language-bash}

~~~
main  README.md  Run2011A_MuOnia_Upsilon.root  Upsilon1SToMuMu_MC_full.root
~~~
{: .output}

## Preparing code for Run 2011

> It will teach you localize files by terminal, but you can use some file explorer program.

Now we need to edit some settings. Go localize `settings.cpp`:

~~~
cd main/config
ls
~~~
{: .language-bash}

~~~
cuts.h  settings.cpp
~~~
{: .output}

There is some ways to open this file. You can try run:

~~~
gedit settings.cpp
~~~
{: .language-bash}

Else if it does no work, try use nano:

~~~
nano settings.cpp
~~~
{: .language-bash}

We are looking for calculating **efficiencies of tracker muons**, so do not need to measure standalone and global.

With **setting.cpp** file opened, make sure let variables like this:

~~~
//Canvas drawing
bool shouldDrawInvariantMassCanvas       = true;
bool shouldDrawInvariantMassCanvasRegion = true;
bool shouldDrawQuantitiesCanvas          = true;
bool shouldDrawEfficiencyCanvas          = true;

//Muon id analyse	
bool doTracker    = true;
bool doStandalone = false;
bool doGlobal     = false;
~~~
{: .language-cpp}

Also we are looking for getting efficiency of specifics file we downloaded. They name are `Run2011A_MuOnia_Upsilon.root` and `Upsilon1SToMuMu_MC_full.root`. They are listed in `const char *files[]`. While **settings.cpp** is open, try to use the variable `int useFile` to run `Run2011A_MuOnia_Upsilon.root`.

> ## How to do this?
>
> Make sure `useFile` is correct:
>
> ~~~
> //List of files
> const char *files[] = {"../data_histoall.root",
>                        "../Run2011AMuOnia_mergeNtuple.root",""
>                        "../JPsiToMuMu_mergeMCNtuple.root",
>                        "../Run2011A_MuOnia_Upsilon.root",
>                        "../Upsilon1SToMuMu_MC_full.root"};
> 
> const char* directoriesToSave[] = {"../results/result/",
>                                    "../results/Jpsi Run 2011/",
>                                    "../results/Jpsi MC 2020/",
>                                    "../results/Upsilon Run 2011/",
>                                    "../results/Upsilon MC 2020/"};
> 
> 
> //MAIN OPTIONS
> 
> //Which file of files (variable above) should use
> int useFile = 3;
> ~~~
> {: .language-cpp}
>
> It will tell wich configuration will use. So, the macro will run with the Ntupple in `files[useFile]` and results will be stored in `directoriesToSave[useFile]`.
>
> Three firsts files are not used in this execise.
{: .solution}


> ## About code
>
> Normally we need to set variabl
e `bool isMC` and `const char* resonance`, but at this time it is done already and set automatically for these ntupples names.
>
> Also this code was made for Upsilon and J/psi only. We pretend to work on for Z Boson in future.
{: .callout}

## Running the code

After set configurations, it is time to run the code. Go back to **main** directory and make sure `macro.cpp` is there.

~~~
cd ..
ls
~~~
{: .language-bash}

~~~
classes  compare_efficiency.cpp  config  macro.cpp
~~~
{: .output}

Initialize root:

~~~
root -l
~~~
{: .language-bash}

~~~
root[0]
~~~
{: .output}

Now run the macro.cpp:

~~~
.x macro.cpp
~~~
{: .language-bash}

~~~
"../results/Upsilon Run 2011/" directory created OK
Using "../Run2011A_MuOnia_Upsilon.root" ntupple
resonance: Upsilon
Using method 2
Data analysed = 986100 of 986100
~~~
{: .output}

In this process, more informations will be printed in terminal while plots will pop up on your screen (these plots are been saved in a folder). **It will take a couple of minutes**. The message below represents that code finish running:

~~~
Done. All result files can be found at "../results/Upsilon Run 2011/"
~~~
{: .output}

Now you can type the code below to **quit root** and close all created windows:

~~~
.q
~~~
{: .language-bash}

## Preparing and running the code for simulated data

> ## Challenge
>
> Try to run the same code for `Upsilon1SToMuMu_MC_full.root` file we downloaded.
>
> > ## Tip
> > 
> > You will need the redo the steps above, but setting `int useFile = 3;`.
> >
> {: .solution}
>
{: .challenge}


{% include links.md %}