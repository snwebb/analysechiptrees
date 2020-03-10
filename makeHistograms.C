#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include "Events.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TLatex.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TSelector.h"
//#include "TDRStyle.h"


int makeHistograms(){//main

  //std::string baseDir = "/home/hep/magnan/CMS/CHIP/analysis/output_skims/";
  //  std::string baseDir = "/home/hep/snwebb/invisible/MakeTrees/CHIP/analysis/output_skims/";
  std::string baseDir = "/vols/cms/snwebb/Common/";
  std::string lPlotDir = "Plots/";
  const double lLumi = 41800;

  
  //  const unsigned nS = 9;//nom,jesup/down,jerup/down...
  const unsigned nS = 1;//nom,jesup/down,jerup/down...
  //  std::string syst[5] = {"Nominal","JESUP","JESDOWN","JERUP","JERDOWN"};
  std::string syst[9] = {"Nominal","ElectronVetoUp","ElectronVetoDown","MuonVetoUp","MuonVetoDown","TauVetoUp","TauVetoDown","BjetVetoUp","BjetVetoDown",};
  const unsigned nP = 2;//QCDW,Z,EWKW,Z
  // std::string proc[6] = {"QCDW","QCDZinv","QCDZll","EWKW","EWKZinv","EWKZll"};
  // std::string name[6] = {"WJETS","ZJETS","DY","EWKW","EWKZNUNU","EWKZll"};

  // std::string proc[6] = {"QCDZinv","QCDZll","EWKW","EWKZinv","EWKZll"};
  // std::string name[6] = {"ZJETS","DY","EWKW","EWKZNUNU","EWKZll"};
  
  std::string proc[2] = {"DATA","QCD"};
  //  std::string proc[1] = {"QCD"};
  std::string year = "2018";

  //std::string name[1] = {"QCD_" + year};
  //  std::string name[2] = {"DATA_" + year, "QCD_" + year};
  //  std::string name[2] = {"DATA_JetHT_" + year, "QCD_" + year};

  std::string name[2] = {"JetHT", "QCD"};

  //  std::string name[1] = {"DATA_2017"};
  //std::string name[1] = {"QCD"};


  TFile* fin[nS][nP];
  TTree* tree[nS][nP];

  for (unsigned iS(0); iS<nS; ++iS){//loop on syst
    for (unsigned iP(0); iP<nP; ++iP){//loop on proc
      //      fin[iS][iP] = TFile::Open((baseDir+syst[iS]+"/"+name[iP]+".root").c_str());
      //      fin[iS][iP] = TFile::Open((baseDir+"Nominal/"+name[iP]+".root").c_str());
      fin[iS][iP] = TFile::Open((baseDir+"/"+year+"/"+name[iP]+".root").c_str());
      if (fin[iS][iP]){
	fin[iS][iP]->cd();
	tree[iS][iP] = (TTree*)gDirectory->Get("Events");
	if (tree[iS][iP]){
	  Events *selector = (Events*)TSelector::GetSelector("Events.C+");
	  selector->SetLumiPb(lLumi);
	  selector->SetProcess(proc[iP]);
	  std::cout << "Set Year" << std::endl;
	  selector->SetYear(year);
	  selector->SetSystematic(syst[iS]);
	  selector->SetOutFileName(lPlotDir+"Histos_"+syst[iS]+"_"+proc[iP]+"_" + year + ".root");
	  std::cout << " -- Tree " << syst[iS] << " " << proc[iP] << " entries = " << tree[iS][iP]->GetEntries() << ", outfile: " << selector->GetOutFileName() << std::endl;
	  tree[iS][iP]->Process(selector);
	}
	else {
	  std::cout << " -- Tree not found for " << syst[iS] << " " << proc[iP] << std::endl;
	}
      }
    }//loop on proc
  }//loop on syst
  
  
  return 0;
  
}//main
