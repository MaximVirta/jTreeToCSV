#include "TRandom.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TTree.h"
#include "TStopwatch.h"
#include <iostream>
#include <fstream>
#include "src/JBaseTrack.h"
#include "src/JBaseEventHeader.h"

float etaMax = 0.8;
float pTmin = 0.2;
float pTmax = 5.0;


R__LOAD_LIBRARY(src/JBaseTrack_cxx.so);
void jTreeToCSV(TString inputFile, TString outputFolder) {
	TFile *fin = new TFile(inputFile.Data(), "read");

	TTree *jTree = (TTree*)fin->Get("jTree");

	TClonesArray *events = new TClonesArray("JBaseEventHeader", 1000);
	TClonesArray *tracks = new TClonesArray("JBaseTrack", 10000);

	jTree->SetBranchAddress("JTrackList", &tracks);
	jTree->SetBranchAddress("JEventHeaderList", &events);

	Int_t nevents = jTree->GetEntries();
	ofstream csvFile;
	printf("Processing %d events...\n", nevents);
	int tt = (int)(0.1*nevents);
	for (int iEvt = 0; iEvt < nevents; ++iEvt){
		csvFile.open(Form("%s/evt_%d.csv", outputFolder.Data(), iEvt));
		tracks->Clear();
		events->Clear();
		jTree->GetEntry(iEvt);

		Int_t ntracks = tracks->GetEntries();
		if (iEvt>0 && iEvt % tt == 0) printf("%.0f %% completed\n", 100*(float)iEvt/(float)nevents);

		for (int iTrk = 0; iTrk < ntracks; ++iTrk) {
			JBaseTrack *trk = (JBaseTrack*)tracks->At(iTrk);

			double phi = trk->Phi();
			double eta = trk->Eta();
			double pT = trk->Pt();
			double M = trk->M();

			if (TMath::Abs(eta)>etaMax || pT>pTmax || pT<pTmin) continue;
			csvFile << Form("%.5f, %.5f, %.5f, %.5f\n", phi, eta, pT, M);
		}
		csvFile.close();
	}

}
