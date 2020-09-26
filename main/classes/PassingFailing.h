#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"

#include <iostream>

using namespace std;

//Holder signal, background and signal+background histograms
class PassingFailing{
private:
	int& method;
	const char*& resonance;
	const char*& particleName;
	const char*& canvasWatermark;
	const char*& directoryToSave;
	const char*& particleType;
	const char*& tagOrProbe;
	InvariantMass& ObjMass;

	//About histograms
	const char*& quantityName;
	const char*& xAxisName;
	const char*& quantityUnit;
	const char*& extendedQuantityName;

	int& 	nBins;
	int& 	decimals;
	double& xMin;
	double&	xMax;

	void createHistogram(TH1D* &histo, const char* histoName)
	{
		//Set parameters
		string hName 		  = string(particleType) + string(passingOrFailing) + string(tagOrProbe) + string(particleName) + "_" + string(quantityName) + string(histoName);
		string hTitle 		  = string(passingOrFailing) + " in " + string(particleType) + " " + string(tagOrProbe);
		string xAxisTitle 	  = string(xAxisName);
		string yAxisTitleForm = "Events";

		//Add unit if has
		if (strcmp(quantityUnit, "") != 0)
			xAxisTitle += " [" + string(quantityUnit) + "]";

		//Change title is passing
		if (strcmp(passingOrFailing, "Passing") == 0)
			hTitle = string(particleType) + " " + string(particleName) + " " + string(tagOrProbe);

		if (strcmp(passingOrFailing, "All") == 0)
			hTitle = "All " + string(particleName) + " " + string(tagOrProbe);


		//Variable bin for pT
		if (strcmp(quantityName, "Pt") == 0)
		{
			double xbins[10000];
			xbins[0] = .0;
			int nbins = 0;
			double binWidth = 1.;
			for (int i = 1; xbins[i-1] < xMax+binWidth; i++)
			{
				xbins[i] = xbins[i-1] < 1. ? 1. : xbins[i-1] *(1+binWidth);
				nbins++;
			}

			histo = new TH1D(hName.data(), hTitle.data(), nbins, xbins);
		}

		//Variable bin for eta
		else if (strcmp(quantityName, "Eta") == 0)
		{
			double xbins[10000];
			xbins[0] = .5;
			int nbins = 0;
			double binWidth = 0.2;

			//For positive
			for (int i = 1; xbins[i-1] < xMax+binWidth; i++)
			{
				xbins[i] = xbins[i-1] < 1. ? 1. : xbins[i-1] *(1+binWidth);
				nbins++;
			}

			//Duplicate array and create another
			double rxbins[nbins*2+1];
			int entry = 0;
			for (int i = nbins; i >= 0; i--)
			{
				rxbins[entry] = -xbins[i];
				entry++;
			}
			rxbins[entry] = 0.;
			entry++;
			for (int i = 0; i <= nbins; i++)
			{
				rxbins[entry] = xbins[i];
				entry++;
			}
			
			histo = new TH1D(hName.data(), hTitle.data(), entry-1, rxbins);
		}

		//Bins for phi 
		else
		{
			if (strcmp(quantityUnit, "") == 0)
			{
				yAxisTitleForm += " / (%1." + to_string(decimals) + "f)";
			}
			else
			{
				yAxisTitleForm += " / (%1." + to_string(decimals) + "f " + string(quantityUnit) + ")";
			}

			histo = new TH1D(hName.data(), hTitle.data(), nBins, xMin, xMax);
		}

		//Edit histogram axis
		histo->GetYaxis()->SetTitle(Form(yAxisTitleForm.data(), histo->GetBinWidth(0)));
		histo->GetXaxis()->SetTitle(xAxisTitle.data());
	}

	//For consistencyDebugCout()
	string fillAfter(string text, char fillWith, int targetLength)
	{
		//Store size of text
		int textLength = strlen(text.data());

		//Fill de text in the end
		if(textLength < targetLength)
		{
			text.append(targetLength - textLength, fillWith);
		}

		return text;
	}

public:
	const char* passingOrFailing = NULL;

	TH1D* hSigBack  = NULL;
	TH1D* hSig 		= NULL;
	TH1D* hBack 	= NULL;

	MassValues* PassFailObj()
	{
		if (strcmp(passingOrFailing, "Passing") == 0)
			return &ObjMass.Pass;
			//return &ObjMass.All;

		if (strcmp(passingOrFailing, "All") == 0)
			return &ObjMass.Pass;
			//return &ObjMass.All;

		cerr << "Could not find PassFailObj in PassingFailing class: " << particleType << " " << tagOrProbe << " " << quantityName <<  " " << passingOrFailing << " ERROR" << endl;
		return NULL;
	}

	void normalizeHistograms()
	{
		//Now normalize yields (to adapt variable binning)
		for (int i = 1; i <= hSig->GetXaxis()->GetNbins(); i++)
		{
			hSigBack->SetBinContent(i, hSigBack->GetBinContent(i)/hSigBack->GetBinWidth(i));
			hBack->SetBinContent(i, hBack->GetBinContent(i)/hBack->GetBinWidth(i));
		}
	}

	void subtractSigHistogram()
	{
		hSig->Add(hSigBack, 1.);
		hSig->Add(hBack, -PassFailObj()->subtractionFactor());

		//Set bin errors
		for (int i = 0; i <= hSig->GetXaxis()->GetNbins(); i++)
		{
			double n_sb = hSigBack->GetBinContent(hSigBack->GetBin(i));
			double n_b  = hBack->GetBinContent(hBack->GetBin(i));

			double error = TMath::Sqrt(n_sb + n_b);

			hSig->SetBinError(hSig->GetBin(i), error);
		}
	}

	//Fill histogram
	void fillQuantitiesHistograms(double& quantity, double& InvariantMass, bool storeInSignalHistogram = false)
	{
		if (!storeInSignalHistogram)
		{
			if (PassFailObj()->isInSignalRegion(InvariantMass))
				hSigBack->Fill(quantity);

			if (PassFailObj()->isInSidebandRegion(InvariantMass))
				hBack->Fill(quantity);
		}
		else
		{
			hSig->Fill(quantity);
		}

	}

	TCanvas* createQuantitiesCanvas(bool shouldWrite = false, bool shouldSavePNG = true)
	{
		string canvasName 	= string(particleName) + " " + string(passingOrFailing) + " in " + string(particleType) + " " + string(tagOrProbe) + " " + string(quantityName);
		string canvastitle 	= string(extendedQuantityName) + " (" + string(passingOrFailing) + " in " + string(particleType) + " " + string(tagOrProbe) + ")";
		string saveAs 		= string(directoryToSave) + string(particleType) + "_" + string(tagOrProbe) + "_" + string(quantityName) + "_" + string(passingOrFailing) + ".png";

		//Create canvas and divide it
		TCanvas* c1 = new TCanvas(canvasName.data(), canvastitle.data());
		c1->SetMargin(0.12, 0.03, 0.11, 0.07);

		if (strcmp(quantityName, "Pt") == 0)
		{
			c1->SetLogy();
			hSigBack->SetMaximum(10.*(hSigBack->GetMaximum() > hSig->GetMaximum() ? hSigBack->GetMaximum() : hSig->GetMaximum()));
		}
		else if (strcmp(quantityName, "Eta") == 0)
		{
			c1->SetLogy();
			hSigBack->SetMinimum(0.1);
			hSigBack->SetMaximum(10.*(hSigBack->GetMaximum() > hSig->GetMaximum() ? hSigBack->GetMaximum() : hSig->GetMaximum()));
		}
		else
		{
			hSigBack->SetMinimum(0.);
			hSigBack->SetMaximum(1.3*(hSigBack->GetMaximum() > hSig->GetMaximum() ? hSigBack->GetMaximum() : hSig->GetMaximum()));
		}

		//Draws Main histogram
		hSigBack->SetLineColor(kBlue);
		hSigBack->SetLineStyle(kSolid);
		hSigBack->SetLineWidth(2);
		hSigBack->Draw();

		//Draws Background histogram
		hBack->SetLineColor(kBlue);
		hBack->SetLineStyle(kDashed);
		hBack->SetLineWidth(2);
		hBack->Draw("same");

		//Same range as comparision and Draws
		hSig->SetLineColor(kMagenta);
		hSig->SetLineStyle(kSolid);
		hSig->SetLineWidth(2);
		hSig->Draw("same HIST");

		//Not show frame with mean, std dev
		//gStyle->SetOptTitle(0);
		gStyle->SetOptStat(0);

		//Add legend
		TLegend* l = new TLegend(0.74,0.75,0.94,0.89);
		l->SetTextSize(0.04);
		l->AddEntry(hSigBack,	"Total",		"l");
		l->AddEntry(hBack,		"Background",	"l");
		l->AddEntry(hSig,       "Signal",       "l"); 
		l->Draw();
		
		//Draws text information
		TLatex* tx = new TLatex();
		tx->SetTextSize(0.04);
		tx->SetTextFont(42);
		tx->SetNDC(kTRUE);
		tx->DrawLatex(0.15,0.85,Form(canvasWatermark, ""));

		/*
		if (strcmp(quantityName, "Pt") == 0)
		{
			tx->SetTextAlign(12);	//Align left, center
			tx->DrawLatex(0.48,0.50,Form("%g entries (total)",		hSigBack->GetEntries()));
			tx->DrawLatex(0.48,0.45,Form("%g entries (background)",	hBack->GetEntries()));
			tx->DrawLatex(0.48,0.40,Form("%g entries (signal)",     hSig->GetEntries()));
		}
		else
		{
			tx->SetTextAlign(22);	//Align center, center
			tx->DrawLatex(0.55,0.50,Form("%g entries (total)",		hSigBack->GetEntries()));
			tx->DrawLatex(0.55,0.45,Form("%g entries (background)",	hBack->GetEntries()));
			tx->DrawLatex(0.55,0.40,Form("%g entries (signal)",     hSig->GetEntries()));
		}
		*/

		//Writes in file
		if (shouldWrite == true)
		{
			c1->Write();
		}

		//If should save
		if (shouldSavePNG == true)
		{
			//Saves as image
			c1->SaveAs(saveAs.data());
		}

		return c1;
	}

	void consistencyDebugCout()
	{
		const int minLegendSpace = 21;

		//Set information what are shown
		string legend = "- #";
		legend += fillAfter(string(particleType),     ' ', 10);
		legend += " ";
		legend += fillAfter(string(tagOrProbe),       ' ', 5);
		legend += " ";
		legend += fillAfter(string(quantityName), 	  ' ', 3);
		legend += " ";
		legend += fillAfter(string(passingOrFailing), ' ', 7);
		legend += " ";
		legend += "= ";

		//Diference calculus
		double diff = hSigBack->Integral() - (hSig->Integral() + PassFailObj()->subtractionFactor()*hBack->Integral());

		const char* addSpace = "";
		if (diff >= 0.)
			addSpace = " ";

		//Show consistency
		cout << legend << fixed << addSpace << diff << "\n";
	}

	void writeQuantitiesHistogramsOnFile(bool hSigBack, bool hSig, bool hBack)
	{
		if (hSigBack == true)
			this->hSigBack->Write();

		if (hSig == true)
			this->hSig->Write();

		if (hBack == true)
			this->hBack->Write();
	}



	PassingFailing(int& method,
		const char*& resonance,
		const char*& particleName,
		const char*& canvasWatermark,
		const char*& directoryToSave,
	 	const char*& particleType,
	 	InvariantMass& ObjMass,
	 	const char*& tagOrProbe,
		const char*  passingOrFailing,
		const char*& quantityName,
		const char*& xAxisName,
		const char*& quantityUnit,
		const char*& extendedQuantityName,
		double& 	 xMin,
		double& 	 xMax,
		int&    	 nBins,
		int&    	 decimals)
		  : method(method),
		    resonance(resonance),
			particleName(particleName),
		    canvasWatermark(canvasWatermark),
		    directoryToSave(directoryToSave),
			particleType(particleType),
		    ObjMass(ObjMass),
			tagOrProbe(tagOrProbe),
			passingOrFailing(passingOrFailing),
			quantityName(quantityName),
			xAxisName(xAxisName),
			quantityUnit(quantityUnit),
			extendedQuantityName(extendedQuantityName),
			nBins(nBins),
			xMin(xMin),
			xMax(xMax),
			decimals(decimals)
	{
		createHistogram(hSigBack, "SigBack");
		createHistogram(hSig, 	  "Sig");
		createHistogram(hBack, 	  "Back");
	}
};