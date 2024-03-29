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


int makeHistograms(){ //Main function

  TH1::SetDefaultSumw2();

  //Set whether we are using Anne-Marie's trees, or the standard set of trees
  bool isAM = false;

  //Location of the QCD trees
  std::string baseDir = "";

  baseDir = "/vols/cms/snwebb/Common/";
    //    baseDir = "/vols/cms/snwebb/Common/August";

  //Plotting directory
  std::string lPlotDir = "Plots/";

  //Luminosity in 2017 and 2018
  double lLumi_2017 = 41529;
  double lLumi_2017_VTR = 36700;
  double lLumi_2018 = 59741;
  //Choose which years to process
  std::vector<std::string> years = {"2017","2018"};  
  //std::vector<std::string> years = {"2018"};  

  //Choose which samples to process
  std::vector<std::string> proc = { "DATA","QCD","QCDRELAX","GluGluHtoInv",  "VBFHtoInv",  "WH", "qqZH", "ggZH", "ttH", "EWKZNUNU",  "VV",  "EWKZll",  "EWKW",  "ZJETS"  ,  "DY",  "WJETS","TOP","MET"};
  //  std::vector<std::string> proc = { "QCD", "QCDRELAX"};
  //  std::vector<std::string> proc = { "VBFHtoInv"};

  //Name of the ROOT tree corresponding to each process. 
  //Generally it is the same as name of the process
  std::vector<std::string> name;

  for (auto process: proc){
    if ( process == "DATA" ){
      name.push_back("JetHT");
    }
    else if ( process == "QCDRELAX" ){
	name.push_back("QCD");
    }
    else{
      name.push_back(process);
    }
  }

  TFile * fin = 0;
  TTree * tree = 0;

  for (auto year: years){//loop over years
    for (unsigned iP(0); iP<proc.size(); ++iP){//loop over processes
      
      fin = new TFile( (baseDir+"/"+year+"/"+name[iP]+".root").c_str() , "READ" );

      if ( fin ){
	fin->cd();
	tree = (TTree*) fin->Get( "Events" );

	if (tree){
	  
	  Events * selector = (Events*)TSelector::GetSelector("Events.C+");//Compile the selector

	  //Set the appropriate luminosity
	  if ( year == "2017" )
	    selector->SetLumiPb(lLumi_2017, lLumi_2017_VTR);
	  else if ( year == "2018" )
	    selector->SetLumiPb(lLumi_2018, lLumi_2018);
	  	  
	  selector->SetProcess(proc[iP]);//Set the process
	  std::cout << "Set Year" << std::endl; //Set the year
	  selector->SetYear(year);
	  selector->SetSystematic("Nominal");//Set the systematic to "Nominal" (only option available currently)
	  selector->SetOutFileName(lPlotDir+"Histos_Nominal_"+proc[iP]+"_" + year + ".root"); //Set output file name
	  std::cout << " -- Tree Nominal " << proc[iP] << " entries = " << tree->GetEntries() << ", outfile: " << selector->GetOutFileName() << std::endl;

	  tree->Process(selector);//Main tree loop
	}
	else {
	  std::cout << " -- Tree not found for Nominal " << proc[iP] << std::endl;
	}
	
	fin->Close();
      }

    }//loop over process
    
  }//loop over years
  return 0;
  
}//end of main function

