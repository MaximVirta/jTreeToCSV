#include "TRandom.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TTree.h"
#include "TStopwatch.h"
#include <iostream>
#include <fstream>
#include "src/JBaseTrack.h"
#include "src/JBaseEventHeader.h"

R__LOAD_LIBRARY(src/JBaseTrack_cxx.so);
void draw_dNdeta(TString filename) {
	TH1D *hEta = new TH1D("heta", "heta", 200, -6.2, 6.2);
        TH1D *hEta_2_5 = new TH1D("heta_2-5", "heta_2-5", 200, -4.2, 4.2);
        TH1D *hEta_5_8 = new TH1D("heta_5-8", "heta_5-8", 200, -4.2, 4.2);
        TH1D *hEta_8_13 = new TH1D("heta_8-13", "heta_8-13", 200, -4.2, 4.2);
	TH1D *hPt = new TH1D("pT", "pT", 1000, 0, 50);
	TH1D *hPhi = new TH1D("hphi", "hphi", 200, .0, TMath::TwoPi());
	hEta->Sumw2();
	hPt->Sumw2();
	hEta_2_5->Sumw2();
	hEta_5_8->Sumw2();
	hEta_8_13->Sumw2();
	TFile *fin = new TFile(filename.Data(), "READ");

	TTree *jTree = (TTree*)fin->Get("jTree");

	TClonesArray *events = new TClonesArray("JBaseEventHeader", 1000);
	TClonesArray *tracks = new TClonesArray("JBaseTrack", 10000);

	jTree->SetBranchAddress("JTrackList", &tracks);
	jTree->SetBranchAddress("JEventHeaderList", &events);

	Int_t nevents = jTree->GetEntries();
	printf("%d\n", nevents);
	int tt = (int)(0.1*nevents);
	for (int i = 1; i < nevents; ++i){
		tracks->Clear();
		events->Clear();
		jTree->GetEntry(i);

		Int_t ntracks = tracks->GetEntries();
		if (i>0 && i % tt == 0) printf("%.0f %% completed\n", 100*(float)i/(float)nevents);
		// if (i<100) printf("%d\n", ntracks);

		for (int j = 0; j < ntracks; ++j) {
			JBaseTrack *trk = (JBaseTrack*)tracks->At(j);
			//if (i==5) trk->Print();

			if(TMath::Abs(trk->Eta()) < 10.0 && trk->Pt() > 0.){ //
				double phi = trk->Phi() < 0.0 ? 2.0*TMath::Pi()+trk->Phi():trk->Phi(); //Add 2pi if negative
				double eta = trk->Eta();
				hEta->Fill(eta);
				hPt->Fill(trk->Pt());
				if(trk->Pt()>0.2 && trk->Pt()<0.5) hEta_2_5->Fill(eta);
				if(trk->Pt()>0.5 && trk->Pt()<0.8) hEta_5_8->Fill(eta);
				if(trk->Pt()>0.8 && trk->Pt()<1.3) hEta_8_13->Fill(eta);
				hPhi->Fill(phi);
			}
		}
	}
	hEta->Scale(1./nevents, "width");
	hPt->Scale(1./nevents,"width");
	hEta_2_5->Scale(1./nevents, "width");
        hEta_5_8->Scale(1./nevents, "width");
        hEta_8_13->Scale(1./nevents, "width");

	TCanvas *c1 = new TCanvas("c1", "c1");
	hEta->Draw();

	TCanvas *c2 = new TCanvas("c2", "c2");
	
	hPhi->Draw();
	c1->SaveAs("eta.pdf");
	c2->SaveAs("phi.pdf");
	TFile *fout = new TFile("output_histos.root", "recreate");
	hEta->Write();
	hPt->Write();
	hEta_2_5->Write();
        hEta_5_8->Write();
        hEta_8_13->Write();

	hPhi->Write();
	fin->Close();
}
