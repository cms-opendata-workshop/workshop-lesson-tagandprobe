 /*
!--------------------------------
!Purpose: Compare efficiency of files
!--------------------------------	
!author: Allan Jales
!--------------------------------
*/

//CONFIGS

int useScheme = 0;
//Upsilon Sideband Run vs Upsilon Sideband MC
//Upsilon Fitting  Run vs Upsilon Fitting  MC
//Upsilon Sideband Run vs Upsilon Fitting  Run

//Root files and paths for Tefficiency objects inside these files
const char* filePathsEff0[][2] = {
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Pt_Tracker_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Eta_Tracker_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Phi_Tracker_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Pt_Standalone_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Eta_Standalone_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Phi_Standalone_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Pt_Global_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Eta_Global_Probe_Efficiency"},
	{"../results/Upsilon_Run_2011/generated_hist.root", "efficiency/plots/Muon_Phi_Global_Probe_Efficiency"}
};

//Root files and paths for Tefficiency objects inside these files
const char* filePathsEff1[][2] = {
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Pt_Tracker_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Eta_Tracker_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Phi_Tracker_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Pt_Standalone_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Eta_Standalone_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Phi_Standalone_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Pt_Global_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Eta_Global_Probe_Efficiency"},
	{"../results/Upsilon_MC_2020/generated_hist.root", "efficiency/plots/Muon_Phi_Global_Probe_Efficiency"}
};

//How comparisons will be saved
const char* resultNames[] = {
	"Muon_Pt_Tracker_Probe_Efficiency.png",
	"Muon_Eta_Tracker_Probe_Efficiency.png",
	"Muon_Phi_Tracker_Probe_Efficiency.png",
	"Muon_Pt_Standalone_Probe_Efficiency.png",
	"Muon_Eta_Standalone_Probe_Efficiency.png",
	"Muon_Phi_Standalone_Probe_Efficiency.png",
	"Muon_Pt_Global_Probe_Efficiency.png",
	"Muon_Eta_Global_Probe_Efficiency.png",
	"Muon_Phi_Global_Probe_Efficiency.png"
};

//Label of each comparison
const char* labelScheme[][2] = {
	{"#Upsilon real data", "Simulated data"},
	{"#Upsilon real data", "Simulated data"},
	{"#Upsilon Sideband",  "#Upsilon Fitting"},
	{"J/#psi real data",   "Simulated data"},
	{"J/#psi real data",   "Simulated data"},
	{"J/#psi Sideband",    "J/#psi Fitting"}
};

//Where to save each comparison
const char* directoriesToSave[] = {
	"../results/Comparison_Upsilon_Sideband_Run_vs_MC/",
	"../results/Comparison_Upsilon_Fitting_Run_vs_MC/",
	"../results/Comparison_Upsilon_Sideband_vs_Fitting/"
	"../results/Comparison_Jpsi_Sideband_Run_vs_MC/",
	"../results/Comparison_Jpsi_Fitting_Run_vs_MC/",
	"../results/Comparison_Jpsi_Sideband_vs_Fitting/"
};

//Colors for each comparison
int colorScheme[][2] = {
	{kGreen - 2, kRed},
	{kGreen - 2, kRed},
	{kRed - 4,   kBlue + 1},
	{kBlue,      kRed},
	{kBlue,      kRed},
	{kRed - 4,   kBlue + 1}
};

//Get TEfficiency from file_path
TEfficiency* get_eff_plot(const char* file_path, const char* hist_path)
{
	TFile *file0 = TFile::Open(file_path);

	if (file0 == NULL)
	{
		std::cerr << "Could not find \"" << file_path << "\" file.\n";
		std::cerr << "ABORTING...\n";
		abort();
	}

	TEfficiency* pEff0 = (TEfficiency*)file0->Get(hist_path);

	if (pEff0 == NULL)
	{
		std::cerr << "Could not find the histogram path \"" << hist_path << "\" in \"" << file_path << "\" file.\n";
		std::cerr << "ABORTING...\n";
		abort();
	}

	return pEff0;
}

//Compare two Tefficiencies and save
void compare_eff(TEfficiency* pEff0, TEfficiency* pEff1, string resultName, const char* quantity = "")
{
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
	pEff1->SetLineWidth(2);
	pEff1->Draw("same");
	
	//Set range in y axis
	gPad->Update();
	auto graph = pEff0->GetPaintedGraph(); 
	graph->SetMinimum(0.0);
	graph->SetMaximum(1.2);
	gPad->Update();

	//Set range if is pT
	if (strcmp(quantity, "Pt") == 0)
	{
		pEff0->GetPaintedGraph()->GetHistogram()->GetXaxis()->SetRangeUser(0.,40.);
		graph->SetMinimum(0.5);
		graph->SetMaximum(1.2);
	}
	
	//Set range if is eta
	if (strcmp(quantity, "Eta") == 0)
	{
		pEff0->GetPaintedGraph()->GetHistogram()->GetXaxis()->SetRangeUser(-3.,3.);
		graph->SetMinimum(0.8);
		graph->SetMaximum(1.08);
	}
	
	//Set range if is phi
	if (strcmp(quantity, "Phi") == 0)
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
	string saveAs = string(directoryToSave) + resultName;

	c1->SaveAs(saveAs.data());
}

//Compare efficiency
void compare_efficiency()
{
	//Get lists of file
	int arraySize = sizeof(filePathsEff0)/sizeof(*filePathsEff0);
	for (int i = 0; i < arraySize; i++)
	{
		//Get Tefficiency object
		TEfficiency* pEff0 = get_eff_plot(filePathsEff0[i][0], filePathsEff0[i][1]);
		TEfficiency* pEff1 = get_eff_plot(filePathsEff1[i][0], filePathsEff1[i][1]);

		//Name of file
		string resultName = resultNames[i];

		//Get which quantity is working on
		const char* quantity = "";

		if (regex_match(resultNames[i], regex(".*Pt.*")))
			quantity = "Pt";

		if (regex_match(resultNames[i], regex(".*Eta.*")))
			quantity = "Eta";

		if (regex_match(resultNames[i], regex(".*Phi.*")))
			quantity = "Phi";


		//Compare efficiency polts
		compare_eff(pEff0, pEff1, resultName, quantity);
	}

	cout << "\nUse Scheme: " << useScheme << "\n";
	cout << "Done. All result files can be found at \"" << directoriesToSave[useScheme] << "\"\n";
}