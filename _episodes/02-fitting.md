---
title: "The Fitting Method"
teaching: 5
exercises: 25
questions:
- "What is this fitting method?"
- "how do we use it to calculate the efficiency?"
objectives:
- "Understand the fitting method, it's advantages and disadvantages"
- "Learn how to implement this method using ROOT library for c++"

keypoints:
- "The dataset for this tutorial contemplates one Muon Id (Tracker Muon) and further contains the three kinematic variables (pT, Eta, Phi)"
- "You'll only need to change the ```/src/DoFit.cpp``` file if some of the fits aren't usable. Apart from that everything will be done on the ```Efficiency.C```"
- "Documentation available [here](https://github.com/AthomsG/LIP_INTERNSHIP/blob/master/Documentation.md)"
---
## Prerequisite

To prepare for this tutorial, clone the respective github repository

~~~
git clone git://github.com/AthomsG/CMS-tutorial
~~~
{: .language-bash}

## The Fitting Method

First, a brief explanation of the method we’ll be studying. It consists in fitting the invariantmass of the tag+probe pairs, in the two categories: passing probes, and all probes. 
The fit allows to statistically discriminate between signal and background. To compute the efficiency we simply divide the signal yield from the fits to the Passing category by the signal yield from the fit the inclusive (All) category. The following image illustrates the procedure.
The procedure is applied after splitting the dataset in bins of a kinematic variable of theprobe (e.g. the traverse momentum); as such, the efficiency will be measured as a function of that quantity.

<img width="700px" src="../fig/esquema.png">


Let's start exploring our dataset. From the cloned directory, type:

~~~
cd DATA/Upsilon/trackermuon
root T\&P_UPSILON_DATA.root
~~~
{: .language-bash}

If everything's right, you should get the following output:
~~~
Attaching file T&P_UPSILON_DATA.root as _file0...
U(TFile *) 0x7fe2f34ca270
~~~
{: .output}
Remember that the units of the x axis are GeV/c.

Now, before we start fitting the invariant mass it's important to look at it's shape first. To visualize our data's invariant mass, do:

~~~
UPSILON_DATA->Draw("InvariantMass")
~~~
{: .language-bash}

<img width="500px" src="../fig/InvariantMass.png">

If you got the previous result, we're ready to go.

The dataset used in this exercise has been collected by the CMS experiment, in proton-proton collisions at the LHC. It contains 986100 entries (muon pair candidates) with an associated invariant mass. For each candidate, the transverse momentum (pT), rapidity(\eta) and asymuthal angle (\phi) are stored, along with a binary flag, **PassingProbeTrackingMuon** (that is 1 in case the corresponding probe satisfied theTracker Muon selection criteria and 0 in case it doesn't).

| UPSILON_DATA |
 ------------- |
| InvarianMass |
| PassingProbeTrackingMuon | 
| ProbeMuon_Pt |
| ProbeMuon_Eta |
| ProbeMuon_Phi|

Open `Efficiency.C`.

We'll start by calculating the efficiency as a function of pT.
It is useful to have an idea of the distribution of the quantity we want to study. In order to do this, we’ll repeat the steps perviously used to plot the invariant mass, but now for the **ProbeMuon_Pt** variable.

~~~
UPSILON_DATA->Draw("ProbeMuon_Pt")
~~~
{: .language-bash}

<img width="500px" src="../fig/total.png">

Hmm.. seems like our domain is larger than we need it to be. To fix this, we can apply a constraint to our plot. Try:

~~~
UPSILON_DATA->Draw("ProbeMuon_Pt", "ProbeMuon_Pt < 20")
~~~
{: .language-bash}

<img width="500px" src="../fig/zoom.png">

Now that we have a clear view of the transverse momentum, we can choose the appropriate bins for our fit. Remember that we need a fair amount of data in order to have a good fit, so be careful not to include too few events on a given bin. We've left a suggestion for an appropriate bin setup on `Bin_Suggestion.txt`

> ## Open `Bin_Suggestion.txt`
>
> When opening the file, you'll find
>
> ~~~
> // TRACKER MUON BINS -------------------------------------------------------------------
>//double bins[] = {2, 3.4, 4, 4.2, 4.4, 4.7, 5.0, 5.1, 5.2, 5.4, 5.5, 5.6, 5.7, 5.8, 5.9, 6.2, 6.4, 6.6, 6.8, 7.3, 9.5, 13.0, 17.0, 40};
>//int bin_n = 23;       //-- BINS USED TO CALCULATE PT
>
>//double bins[] = {-3, -2.8, -2.6, -2.4, -2.2, -2.0, -1.8, -1.6, -1.4, -1.2, -1.0, -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.5, 0.6, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0};
>//int bin_n = 30;       //-- BINS USED TO CALCULATE PHI
>
>//double bins[] = {-2.0, -1.9, -1.8, -1.7, -1.6, -1.5, -1.4, -1.2, -1.0, -0.8, -0.6, -0.4, 0, 0.2, 0.4, 0.6, 0.7, 0.95, 1.2, 1.4, 1.5, 1.6, 2.0};
>//int bin_n = 23;      // -- BINS USED TO CALCULATE ETA
> ~~~
> {: .language-cpp}
>
> Since we're starting with the transverse momentum, copy and paste the respective lines onto your  `Efficiency.C` file 
{: .solution}

Now that the bins are set, we'll define a string array that will store the binning conditions so that we can apply them to the fits

~~~
string* conditions = get_conditions(bin_n, bins);
~~~
{: language-bash}

## The Fit

The fits in this tutorial will be executed using the `/src/DoFit.cpp` function based on the [RooFit](https://root.cern.ch/doc/master/group__Roofit.html) library.

You can find generic tutorials [_here_](https://root.cern.ch/doc/master/group__tutorial__roofit.html).
and if you’re starting with **RooFit** you may find [_this one_ ](https://indico.scc.kit.edu/event/31/contributions/1864/attachments/1105/1550/lukas_hehn_kseta-workshop_introduction-to-RooFit.pdf) particularly useful.

> ## Open `DoFit.cpp`
>
> ~~~
> double* doFit(string condition, string MuonID_str, double* init_conditions, bool save = TRUE) // RETURNS ARRAY WITH [yield_all, yield_pass, err_all, err_pass]    ->   OUTPUT ARRAY
>{
>    TFile *file0    = TFile::Open("DATA/Upsilon/trackerMuon/T&P_UPSILON_DATA.root");
>    TTree *DataTree = (TTree*)file0->Get(("UPSILON_DATA"));
>    
>    double _mmin = 9.1;  double _mmax = 10.6;
>    
>    RooRealVar MuonID(MuonID_str.c_str(), MuonID_str.c_str(), 0, 1); //Muon_Id
>    
>    RooRealVar InvariantMass("InvariantMass", "InvariantMass", _mmin, _mmax);
>    RooRealVar ProbeMuon_Pt("ProbeMuon_Pt", "ProbeMuon_Pt", 0, 60);
>    RooRealVar ProbeMuon_Eta("ProbeMuon_Eta", "ProbeMuon_Eta", -3, 3);
>    RooRealVar ProbeMuon_Phi("ProbeMuon_Phi", "ProbeMuon_Phi", -2, 2);
>    
>    RooFormulaVar* redeuce = new RooFormulaVar("PPTM", condition.c_str(), RooArgList(ProbeMuon_Phi));
>    RooDataSet *Data_ALL    = new RooDataSet("DATA", "DATA", DataTree, RooArgSet(InvariantMass, MuonID, ProbeMuon_Phi),*redeuce);
>    RooFormulaVar* cutvar = new RooFormulaVar("PPTM", strcat(condition.c_str(), "&& PassingProbeTrackerMuon == 1"), RooArgList(MuonID, ProbeMuon_Phi));
>    
>    // CUTAVAR NEEDS TO BE CHANGED
>    RooDataSet *Data_PASSING = new RooDataSet("DATA_PASS", "DATA_PASS", DataTree, RooArgSet(InvariantMass, MuonID, ProbeMuon_Phi), *cutvar);//
>    
>    RooDataHist* dh_ALL     = Data_ALL->binnedClone();
>    RooDataHist* dh_PASSING = Data_PASSING->binnedClone();
>    
>    TCanvas* c_all  = new TCanvas;
>    TCanvas* c_pass = new TCanvas;
>    
>    RooPlot *frame = InvariantMass.frame(RooFit::Title("Invariant Mass"));
>    // BACKGROUND VARIABLES
>    RooRealVar a0("a0", "a0", 2.5875e-02, -1., 1.);
>    RooRealVar a1("a1", "a1", -7.8407e-02, -1., 1.);
>
>    // BACKGROUND FUNCTION
>    RooChebychev background("background","background", InvariantMass, RooArgList(a0,a1));
>    
>    // G A U S S I A N    V A R I A B L E S
>    // ---------------------------------------------------------------------------
>    RooRealVal sigma("sigma", "sigma",init_conditions[3]);
>    RooRealVar mean1("mean1","mean1",init_conditions[0]);
>    RooRealVar mean2("mean2","mean2",init_conditions[1]);
>    RooRealVar mean3("mean3","mean3",init_conditions[2]);
>    // C R Y S T A L L    B A L L    V A R I A B L E S
>    RooRealVar alpha("alpha","alpha", 1.4384e+00, 1.43, 1.44);
>    RooRealVar n("n", "n", 1.6474e+01, 16., 17.);
>    // F I T    F U N C T I O N S
>    RooCBShape  peak1("signal1","signal1",InvariantMass,mean1,sigma, alpha, n);
>    RooGaussian peak2("signal2","signal2",InvariantMass,mean2,sigma);
>    RooGaussian peak3("signal3","signal3",InvariantMass,mean3,sigma);
>    // ---------------------------------------------------------------------------
>    
>    double n_signal_initial1 =(Data_ALL->sumEntries(TString::Format("abs(InvariantMass-%g)<0.015",mass_peak1))-Data_ALL->sumEntries(TString::Format("abs(InvariantMass-%g)<0.030&&abs(InvariantMass-%g)>.015",mass_peak1,mass_peak1))) / Data_ALL->sumEntries();
>    double n_signal_initial2 =(Data_ALL->sumEntries(TString::Format("abs(InvariantMass-%g)<0.015",mass_peak2))-Data_ALL->sumEntries(TString::Format("abs(InvariantMass-%g)<0.030&&abs(InvariantMass-%g)>.015",mass_peak2,mass_peak2))) / Data_ALL->sumEntries();
>    double n_signal_initial3 =(Data_ALL->sumEntries(TString::Format("abs(InvariantMass-%g)<0.015",mass_peak3))-Data_ALL->sumEntries(TString::Format("abs(InvariantMass-%g)<0.030&&abs(InvariantMass-%g)>.015",mass_peak3,mass_peak3))) / Data_ALL->sumEntries();
>    
>    double n_signal_initial_total = n_signal_initial1 + n_signal_initial2 + n_signal_initial3;
>    
>    RooRealVar frac1("frac1","frac1",7.1345e-01,0.6,0.72);
>    RooRealVar frac2("frac2","frac2",1.9309e-01,0.191,0.194);
>
>    RooAddPdf* signal;
>    
>    signal      = new RooAddPdf("signal", "signal", RooArgList(peak1, peak2,peak3), RooArgList(frac1, frac2));
>    double n_back_initial = 1. - n_signal_initial1 - n_signal_initial2 -n_signal_initial3;
>    
>    RooRealVar n_signal_total("n_signal_total","n_signal_total",n_signal_initial_total,0.,Data_ALL->sumEntries());
>    RooRealVar n_signal_total_pass("n_signal_total_pass","n_signal_total_pass",n_signal_initial_total,0.,Data_PASSING->sumEntries());
>    
>    RooRealVar n_back("n_back","n_back",n_back_initial,0.,Data_ALL->sumEntries());
>    RooRealVar n_back_pass("n_back_pass","n_back_pass",n_back_initial,0.,Data_PASSING->sumEntries());
>    RooAddPdf* model;
>    RooAddPdf* model_pass;
>    
>    model      = new RooAddPdf("model","model", RooArgList(*signal, background),RooArgList(n_signal_total, n_back));
>    model_pass = new RooAddPdf("model_pass", "model_pass", RooArgList(*signal, background),RooArgList(n_signal_total_pass, n_back_pass));
>    
>    // S I M U L T A N E O U S    F I T
>    // ---------------------------------------------------------------------------
>    RooCategory sample("sample","sample") ;
>    sample.defineType("All") ;
>    sample.defineType("PASSING") ;
>    
>    RooDataHist combData("combData","combined data",InvariantMass,Index(sample),Import("ALL",*dh_ALL),Import("PASSING",*dh_PASSING));
>    
>    RooSimultaneous simPdf("simPdf","simultaneous pdf",sample) ;
>    
>    simPdf.addPdf(*model,"ALL");
>    simPdf.addPdf(*model_pass,"PASSING");
>    
>    RooFitResult* fitres = new RooFitResult;
>    fitres = simPdf.fitTo(combData, RooFit::Save());
>    
>    // OUTPUT ARRAY
>    double* output = new double[4];
>    
>    RooRealVar* yield_ALL = (RooRealVar*) fitres->floatParsFinal().find("n_signal_total");
>    RooRealVar* yield_PASS = (RooRealVar*) fitres->floatParsFinal().find("n_signal_total_pass");
>    
>    output[0] = yield_ALL->getVal();
>    output[1] = yield_PASS->getVal();
>    
>    output[2] = yield_ALL->getError();
>    output[3] = yield_PASS->getError();
>    
>    frame->SetTitle("ALL");
>    frame->SetXTitle("#mu^{+}#mu^{-} invariant mass [GeV/c^{2}]");
>    Data_ALL->plotOn(frame);
>
>    model->plotOn(frame);
>    model->plotOn(frame,RooFit::Components("signal1"),RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen));
>    model->plotOn(frame,RooFit::Components("signal2"),RooFit::LineStyle(kDashed),RooFit::LineColor(kMagenta - 5));
>    model->plotOn(frame,RooFit::Components("signal3"),RooFit::LineStyle(kDashed),RooFit::LineColor(kOrange));
>    model->plotOn(frame,RooFit::Components("background"),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));
>    
>    c_all->cd();
>    frame->Draw("");
>    
>    RooPlot *frame_pass = InvariantMass.frame(RooFit::Title("Invariant Mass"));
>    
>    c_pass->cd();
>    
>    frame_pass->SetTitle("PASSING");
>    frame_pass->SetXTitle("#mu^{+}#mu^{-} invariant mass [GeV/c^{2}]");
>    Data_PASSING->plotOn(frame_pass);
>    
>    model_pass->plotOn(frame_pass);
>    model_pass->plotOn(frame_pass,RooFit::Components("signal1"),RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen));
>    model_pass->plotOn(frame_pass,RooFit::Components("signal2"),RooFit::LineStyle(kDashed),RooFit::LineColor(kMagenta - 5));
>    model_pass->plotOn(frame_pass,RooFit::Components("signal3"),RooFit::LineStyle(kDashed),RooFit::LineColor(kOrange));
>    model_pass->plotOn(frame_pass,RooFit::Components("background"),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));
>    
>    frame_pass->Draw();
>    
>    string file     = "Fit Result/";
>    string all_pdf  = "_ALL.pdf";
>    string pass_pdf = "_PASS.pdf";
>    
>    if(save)
>    {
>        c_pass->SaveAs(strcat(strcat(file,condition), all_pdf));
>        c_all->SaveAs(strcat(strcat(file, condition),pass_pdf));
>    }
>        
>    // DELETING ALLOCATED MEMORY
>    delete file0;
>    
>    delete Data_ALL;
>    delete Data_PASSING;
>    //
>    delete dh_ALL;
>    delete dh_PASSING;
>    //
>    delete cutvar;
>    delete redeuce;
>    //
>    delete signal;
>    //
>    delete c_all;
>    delete c_pass;
>    //
>    delete model;
>    delete model_pass;
>    delete fitres;
>    
>    return output;
>}
> ~~~
> {: .language-cpp}
>
> 
{: .solution}

the DoFit.cpp function executes a simultaneous fit to two two event categories (Passingand All).

After understanding the basics of how fitting with *RooFit* works, fill in the `init_conditions` with initial approximations (starting values for the fit parametersi) that you find reasonable for each parameter.
I recommend plotting the invariant mass of our dataset again and choosing the values as close as possible to the 'real' ones.

> ## Suggestion for Initial Values
>Try the following initial values:
> 
><ul>
><li>init_conditions[0] = 9.46030;</li>
><li>init_conditions[1] = 10.02326;</li>
><li>init_conditions[2] = 10.3552;</li>
><li>init_conditions[3] = 0.08;</li>
></ul>
>
> 
{: .solution}
Now we only need to create a loop to fit each bin and save the yields and associated errors in order to get the efficiency. This is achieved by:

~~~
for (int i = 0; i < bin_n; i++)
{
    if (DataIsMC)
        yields_n_errs[i] = McYield(conditions[i]);
    else
        yields_n_errs[i] = doFit(conditions[i], "PassingProbeTrackerMuon", init_conditions);
}
~~~
{:  .language-cpp}
The ```McYield``` function has the same output as ```DoFit``` and has to do with Monte Carlo dataset, which only contains signal for the 1S peak.

To get the efficiency plot, we'll use the [TEfficiency](https://root.cern.ch/doc/master/classTEfficiency.html)  class.
You'll see that in order to create a ``TEfficiency`` object, one of the [constructors required](https://root.cern.ch/doc/master/classTEfficiency.html#aa0e99b4161745fd3bee0ae5c0f58880e) requires two ``TH1``objects. One with _All_ the probes and one with the _Passing_ probes.

> ## Important note
>You musn't forget to add the fitting errors to the yield histograms. Using ``/src/make_hist.cpp``  will guarantee that the errors are included.
> 
{: .callout}

The creation of these ``TH1`` objects is taken care of by the ``/src/make_hist.cpp`` function.

> ## Open `make_hist.cpp`
>
> ~~~
> double* doFit(string condition, string MuonID_str, double* init_conditions, bool save = TRUE) // RETURNS ARRAY WITH [yield_all, yield_pass, err_all, err_pass]    ->   OUTPUT ARRAY
>{
>    TH1F* make_hist(string name, double** values, int qnt, int bin_n, Double_t* binning, bool IsDataMc, bool DRAW = FALSE)
>    {
>        TH1F* hist = new TH1F(name.c_str(), name.c_str(), bin_n, binning);
>
>        for (int i = 0; i < bin_n; i++)
>        {
>            hist->SetBinContent(i, values[i][qnt]);
>            if (IsDataMc == FALSE)
>                hist->SetBinError(i, values[i][qnt+2]);
>        }
>        if (DRAW)
>        {
>            TCanvas* xperiment = new TCanvas;
>            xperiment->cd();
>            hist->Draw();
>        }
>        return hist;
>    }
>}
> ~~~
> {: .language-cpp}
>
> 
{: .solution}

To plot the efficiency we'll the use the ``/src/get_efficiency.cpp`` function.

> ## Open `get_efficiency.cpp`
>
> ~~~
>TEfficiency* get_efficiency(TH1F* ALL, TH1F* PASS)ID_str, double* init_conditions, bool save = TRUE) // RETURNS ARRAY WITH [yield_all, yield_pass, err_all, err_pass]    ->   OUTPUT ARRAY
>{
>    TFile* pFile = new TFile("Efficiency_Run2011.root","recreate");lues, int qnt, int bin_n, Double_t* binning, bool IsDataMc, bool DRAW = FALSE)
>    TEfficiency* pEff = new TEfficiency();
>    pEff->SetName("Efficiency");name.c_str(), bin_n, binning);
>
>    pEff->SetPassedHistogram(*PASS, "f");
>    pEff->SetTotalHistogram (*ALL,"f");
>    [qnt]);
>    pEff->SetDirectory(gDirectory);
>    pFile->Write();i][qnt+2]);
>    
>    TCanvas* oi = new TCanvas();
>    oi->cd();
>    pEff->Draw();;
>    
>    gPad->Update();
>
>    //Set range in y axis
>
>    auto graph = pEff->GetPaintedGraph();
>    graph->SetMinimum(0.8);
>    graph->SetMaximum(1.2);
>    gPad->Update();
>  
>    return pEff;
>}
> ~~~
> {: .language-cpp}
>
> 
{: .solution}

to run your code, type:
~~~
root Efficiency.C -q
~~~
{: .language-bash}

when the macro stops doing its thing, you should have 2 new files. One on your working directory: ```Histograms.root```, and another one ```Efficiency_Run2011.root``` located at ```/Efficiency Result/Pt```.
The second contains the efficiency we calculated! the first file is used to redo any unusable fits.
To open ``Efficiency_Run2011.root``, on your working directory type:

~~~
root
new TBrowser
~~~
{: .language-bash}

<img width="700px" src="../fig/TBrowser_efficiency.png">

A window like this should have popped up.
If you click on ``Efficiency_Run2011.root``, a plot will show up with the efficiency value for each bin!

Now we must re-run the code, but before that, change ``DataIsMc`` value to **TRUE**. This will generate an efficiency for the simulated data, so that we can compare it with the 2011 run.

Check that you have both ```Efficiency_Run2011.root``` and ```Efficiency_MC.root``` files in each of these directories:

<ul>
<li>Efficiency Result/pT</li>
<li>Efficiency Result/Eta</li>
<li>Efficiency Result/Phi</li>
</ul>

If so, uncomment:

~~~
//compare_efficiency(quantity, "Efficiency_Result/Pt/Efficiency_Run2011.root", "Efficiency_Result/Pt/Efficiency_MC.root");
~~~
{: .language-cpp}

and run the macro again.
You should get something like the following result. If so, repeat this process for the two quantites left to go, Eta and Phi!
In case you want to change on of the fits, use the ```change_bin.cpp```function.

<img width="500px" src="../fig/Efficiency_thomas.png">

> ## Extra challenge
>
> Fancy some more work? download [this](https://drive.google.com/drive/folders/1YytJ7iTVYToUZemYIDLqr13wTjIYGERP?usp=sharing) J/ψ dataset and try out the new methods you just learned!
>You'll have to change the DoFit.cpp function since J/ψ's only peak is made up of a crystall ball and a gaussian curve.
>Good luck!
{: .challenge}

{% include links.md %}
