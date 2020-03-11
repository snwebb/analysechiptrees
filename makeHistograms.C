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
  //  const unsigned nS = 1;//nom,jesup/down,jerup/down...
  //  std::string syst[5] = {"Nominal","JESUP","JESDOWN","JERUP","JERDOWN"};
  //  std::vector<std::string> syst = {"Nominal","ElectronVetoUp","ElectronVetoDown","MuonVetoUp","MuonVetoDown","TauVetoUp","TauVetoDown","BjetVetoUp","BjetVetoDown",};
  //  std::vector<std::string> syst = {"Nominal"};
  //  const unsigned nP = 13;//QCDW,Z,EWKW,Z
  // std::string proc[6] = {"QCDW","QCDZinv","QCDZll","EWKW","EWKZinv","EWKZll"};
  // std::string name[6] = {"WJETS","ZJETS","DY","EWKW","EWKZNUNU","EWKZll"};

  // std::string proc[6] = {"QCDZinv","QCDZll","EWKW","EWKZinv","EWKZll"};
  // std::string name[6] = {"ZJETS","DY","EWKW","EWKZNUNU","EWKZll"};
  //


  //std::string proc[13] = { "DATA","QCD", "GluGluHtoInv",  "VBFHtoInv",  "EWKZNUNU",  "VV",  "EWKZll",  "EWKW",  "ZJETS"  ,  "DY",  "SingleElectron",  "WJETS","TOP"};
			   //  std::string proc[2] = {"DATA","QCD",};


  //    std::string proc[2] = {"DATA","QCD"};
  std::vector<std::string> proc = {"QCD"};
    std::vector<std::string> years = {"2017","2018"};
    //  std::vector<std::string> years = {"2017"};
  //  std::string year = "2018";

  std::vector<std::string> name;
  for (auto process: proc){
    if ( process == "DATA" ){
      name.push_back("JetHT");
    }
    else{
      name.push_back(process);
    }
  }

  // std::vector<TFile*> fin;
  // std::vector<TTree*> tree;

  // TFile* fin[nS][nP];
  // TTree* tree[nS][nP];

  //  for (unsigned iS(0); iS<nS; ++iS){//loop on syst


  //  for (unsigned iS(0); iS<syst.size(); ++iS){//loop on syst
  TFile * fin = 0;
  TTree * tree = 0;


  for (auto year: years){
    for (unsigned iP(0); iP<proc.size(); ++iP){//loop on proc
    
      fin = new TFile( (baseDir+"/"+year+"/"+name[iP]+".root").c_str() , "READ" );
    
      //fin.push_back(TFile::Open((baseDir+"/"+year+"/"+name[iP]+".root").c_str()))
      
      if ( fin ){
	//      if (fin[iS][iP]){
	//fin[iS][iP]->cd();
	fin->cd();
	tree = (TTree*) fin->Get( "Events" );
	//	tree[iS][iP] = (TTree*)gDirectory->Get("Events");
	//	if (tree[iS][iP]){
	if (tree){
	  
	  Events * selector = (Events*)TSelector::GetSelector("Events.C+");
	  selector->SetLumiPb(lLumi);
	  selector->SetProcess(proc[iP]);
	  std::cout << "Set Year" << std::endl;
	  selector->SetYear(year);
	  selector->SetSystematic("Nominal");
	  selector->SetOutFileName(lPlotDir+"Histos_Nominal_"+proc[iP]+"_" + year + ".root");
	  std::cout << " -- Tree Nominal " << proc[iP] << " entries = " << tree->GetEntries() << ", outfile: " << selector->GetOutFileName() << std::endl;
	  //	tree[iS][iP]->Process(selector);
	  tree->Process(selector);
	}
	else {
	  std::cout << " -- Tree not found for Nominal " << proc[iP] << std::endl;
	}
	
	fin->Close();
    }
    }//loop on proc
    //  }//loop on syst
    
  }
  return 0;
  
}//main

