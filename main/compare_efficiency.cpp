 /*
!--------------------------------
!Purpose: Compare efficiency of files
!--------------------------------	
!author: Allan Jales
!--------------------------------
*/

//CONFIGS

int useScheme = 0;
//Upsilon vs Jpsi
//Jpsi    Run vs MC
//Upsilon Run vs MC

//Root file comparison
const char* filePaths[][2] = {
		{"../results/Upsilon Run 2011/generated_hist.root", "../results/Jpsi Run 2011/generated_hist.root"},
		{"../results/Jpsi Run 2011/generated_hist.root",    "../results/Jpsi MC 2020/generated_hist.root"},
		{"../results/Upsilon Run 2011/generated_hist.root", "../results/Upsilon MC 2020/generated_hist.root"}
	};

//Label of each comparison
const char* labelScheme[][2] = {
	{"#Upsilon data",      "J/#psi data"},
	{"J/#psi real data",   "Simulated data"},
	{"#Upsilon real data", "Simulated data"}
};

//Where to save each comparison
const char* directoriesToSave[] = {
	"../results/Comparison Jpsi vs Upsilon/",
	"../results/Comparison Jpsi Run vs MC/",
	"../results/Comparison Upsilon Run vs MC/",
};

//Colors for each comparison
int colorScheme[][2] = {
	{kGreen - 2, kBlue},
	{kBlue,      kRed},
	{kGreen - 2, kRed}
};



//Global paths of Tefficiency inside .root file
const char* TEfficiencyPaths[] = {
	"efficiency/plots/Muon_Pt_Tracker_Probe_Efficiency",
	"efficiency/plots/Muon_Eta_Tracker_Probe_Efficiency",
	"efficiency/plots/Muon_Phi_Tracker_Probe_Efficiency",
	"efficiency/plots/Muon_Pt_Standalone_Probe_Efficiency",
	"efficiency/plots/Muon_Eta_Standalone_Probe_Efficiency",
	"efficiency/plots/Muon_Phi_Standalone_Probe_Efficiency",
	"efficiency/plots/Muon_Pt_Global_Probe_Efficiency",
	"efficiency/plots/Muon_Eta_Global_Probe_Efficiency",
	"efficiency/plots/Muon_Phi_Global_Probe_Efficiency"
};



void compare_plot(TFile *file0, TFile *file1, const char* path)
{
	TEfficiency* pEff0 = (TEfficiency*)file0->Get(path);
	TEfficiency* pEff1 = (TEfficiency*)file1->Get(path);

	if (pEff0 == NULL)
	{
		cerr << "Could not read the path in file0\n";
		abort();
	}
	
	if (pEff1 == NULL)
	{
		cerr << "Could not read the path in file1\n";
		abort();
	}

	//Create canvas
	TCanvas* c1 = new TCanvas();
	//gStyle->SetOptTitle(0);
	c1->SetMargin(0.10, 0.03, 0.11, 0.07);

	//Plot
	pEff0->SetMarkerColor(colorScheme[useScheme][0]);
	pEff0->SetLineColor(colorScheme[useScheme][0]);
	pEff0->SetLineWidth(2);
	pEff0->Draw();

	pEff1->SetMarkerColor(colorScheme[useScheme][1]);
	pEff1->SetLineColor(colorScheme[useScheme][1]);
	pEff0->SetLineWidth(2);
	pEff1->Draw("same");
	
	//Set range in y axis
	gPad->Update();
	auto graph = pEff0->GetPaintedGraph(); 
	graph->SetMinimum(0.0);
	graph->SetMaximum(1.2);
	gPad->Update();

	//Set range if is pT
	if (regex_match(path, regex(".*Pt.*")))
	{
		pEff0->GetPaintedGraph()->GetHistogram()->GetXaxis()->SetRangeUser(0.,40.);
		graph->SetMinimum(0.5);
		graph->SetMaximum(1.2);
	}
	
	//Set range if is pT
	if (regex_match(path, regex(".*Eta.*")))
	{
		pEff0->GetPaintedGraph()->GetHistogram()->GetXaxis()->SetRangeUser(-3.,3.);
		graph->SetMinimum(0.8);
		graph->SetMaximum(1.08);
	}
	
	//Set range if is pT
	if (regex_match(path, regex(".*Phi.*")))
	{
		graph->SetMinimum(0.8);
		graph->SetMaximum(1.08);
	}

	//Legenda
	TLegend* tl = new TLegend(0.68,0.78,0.94,0.88);
	tl->SetTextSize(0.04);
	tl->AddEntry(pEff0, labelScheme[useScheme][0], "lp");
	tl->AddEntry(pEff1, labelScheme[useScheme][1], "lp");
	tl->Draw();

	//CMS Open Data
	TLatex* txCOD = new TLatex();
	txCOD->SetTextSize(0.04);
	txCOD->SetTextAlign(12);
	txCOD->SetTextFont(42);
	txCOD->SetNDC(kTRUE);
	txCOD->DrawLatex(0.14,0.85,Form("#bf{CMS Open Data}"));







	//Saving as png


	//Path where is going to save results 
	const char* directoryToSave = directoriesToSave[useScheme];

	//Check if dir exists and create
	if (gSystem->AccessPathName(directoryToSave))
	{
		if (gSystem->mkdir(directoryToSave, true))
		{
			cerr << "\"" << directoryToSave << "\" directory not found and could not be created ERROR" << endl;
			abort();
		}
		else
		{
			cout << "\"" << directoryToSave << "\" directory created OK" << endl;
		}
	}
	else
	{
		cout << "\"" << directoryToSave << "\" directory OK" << endl;
	}

	//Path of file
	string saveAs = string(directoryToSave) + string(pEff0->GetName()) + ".png";

	c1->SaveAs(saveAs.data());
}

//Compare efficiency
void compare_efficiency()
{
	TFile *file0 = TFile::Open(filePaths[useScheme][0]);
	TFile *file1 = TFile::Open(filePaths[useScheme][1]);

	if (file0 == NULL || file1 == NULL)
	{
		std::cerr << "ABORTING...\n";
		abort();
	}

	int arraySize = sizeof(TEfficiencyPaths)/sizeof(*TEfficiencyPaths);
	for (int i = 0; i < arraySize; i++)
		compare_plot(file0, file1, TEfficiencyPaths[i]);

	cout << "\nUse Scheme: " << useScheme << "\n";
	cout << "Done. All result files can be found at \"" << directoriesToSave[useScheme] << "\"\n";
}