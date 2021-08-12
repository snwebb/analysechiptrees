#ifndef Events_cxx
#define Events_cxx

#include "Events.h"
#include <TH2.h>
#include <TH3.h>
#include <TLorentzVector.h>
#include <TStyle.h>
#include <stdlib.h>   


Events::Events(TTree *){
  mFout = 0;
  mOutFile = "Histos.root";
  mCat=CatType::MTR;
  mReg=RegionType::SR;
  mLumiPb=41800.;
  mProc = "QCD";
  lTreeContent.clear();
}

Events::~Events(){
  mVarVec.clear();
  mVarVec2D.clear();
  mVarVec3D.clear();
  lTreeContent.clear();
  for (unsigned iR(RegionType::SR); iR!=RegionType::Last; ++iR){//loop on region
    for (unsigned iC(CatType::MTR); iC!=CatType::LastCat; ++iC){//loop on category
      mHistVec[iR][iC].clear();
      mHistVec2D[iR][iC].clear();
      mHistVec3D[iR][iC].clear();
    }
  }
}

void Events::Begin(TTree * /*tree*/)
{
  //////////////////////////////////////////////////////
  //                                                  //
  //   The USER does not need to edit the following   //
  //   unless adding new variables to plot            //
  //                                                  //
  //////////////////////////////////////////////////////


  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).

  //Reweight based on MET_Phi
  mreweight_met_phi = false;
  Met_phi_SF = 0;
  if ( mreweight_met_phi ){
    TFile * file_Met_phi_SF = new TFile("Met_phi_Wmunu_2018_ratio.root", "READ");
    TH1D * gethist = (TH1D*)file_Met_phi_SF->Get("Met_phi_Wmunu_2018_ratio");
    Met_phi_SF = (TH1D*) gethist->Clone("Met_phi_Wmunu_2018_ratio_clone");
  }

  TString option = GetOption();
  mFout = TFile::Open(mOutFile.c_str(),"RECREATE");

  //The following structs are use to organise the histograms that are created 
  //(separate structs for 1D, 2D and 3D histograms).
  struct variable {
    std::string name;
    std::string title;
    int nBins;
    double binMin = -999;
    double binMax = -999;
  } ;
 
  struct variable2D {
    std::string name1;
    std::string name2;
    int nBins1;
    int nBins2 = -999;
    double binMin1 = -999;
    double binMin2 = -999;
    double binMax1 = -999;
    double binMax2 = -999;
  } ;

  struct variable3D {
    std::string name1;
    std::string name2;
    std::string name3;
    int nBins1;
    int nBins2 = -999;
    int nBins3 = -999;
    double binMin1 = -999;
    double binMin2 = -999;
    double binMin3 = -999;
    double binMax1 = -999;
    double binMax2 = -999;
    double binMax3 = -999;
  } ;
 
  //Define all the possible variables to plot in 1D, 2D and 3D
  variable MetNoLep_pt{ .name = "MetNoLep_pt", .title = "E_{T}^{miss,no #mu} (GeV)", .nBins = 350, .binMin = 100, .binMax = 600};
  variable diCleanJet_M{ .name = "diCleanJet_M", .title = "M_{jj} (GeV)", .nBins = 380, .binMin = 200, .binMax = 4000};
  variable lMjj{ .name = "lMjj", .title = "M_{jj} (GeV)", .nBins = 350, .binMin = 200, .binMax = 4000}; 
  variable diCleanJet_M_LeadingPosEta{ .name = "diCleanJet_M_LeadingPosEta", .title = "M_{jj} (GeV)", .nBins = 380, .binMin = 200, .binMax = 4000};
  variable diCleanJet_M_LeadingNegEta{ .name = "diCleanJet_M_LeadingNegEta", .title = "M_{jj} (GeV)", .nBins = 380, .binMin = 200, .binMax = 4000};
  variable diCleanJet_dEta{ .name ="diCleanJet_dEta", .title = "#Delta#eta_{jj}", .nBins = 120, .binMin = 1, .binMax = 7};
  variable lMjj_dijet_deta{ .name ="lMjj_dijet_deta", .title = "#Delta#eta_{jj}", .nBins = 120, .binMin = 1, .binMax = 7};
  variable diCleanJet_dEtaCmF{ .name = "diCleanJet_dEtaCmF", .title = "#Delta#eta_{jj} Central-Forward", .nBins = 140, .binMin = -7, .binMax = 7};
  variable lMjj_dijet_detaCmF{ .name = "lMjj_dijet_detaCmF", .title = "#Delta#eta_{jj} Central-Forward", .nBins = 140, .binMin = -7, .binMax = 7};
  variable diCleanJet_dPhi{ .name ="diCleanJet_dPhi", .title = "#Delta#Phi_{jj}", .nBins = 50, .binMin = 0, .binMax = 1.5};
  variable lMjj_dijet_dphi{ .name ="lMjj_dijet_dphi", .title = "#Delta#Phi_{jj}", .nBins = 50, .binMin = 0, .binMax = 1.5};
  variable Leading_jet_pt{ .name = "Leading_jet_pt", .title = "p_{T}^{jet1} (GeV)", .nBins = 84, .binMin = 80, .binMax = 500};
  variable Subleading_jet_pt{ .name ="Subleading_jet_pt", .title = "p_{T}^{jet2} (GeV)", .nBins = 52, .binMin = 40, .binMax = 300};
  variable Leading_jet_eta{ .name ="Leading_jet_eta", .title = "#eta^{jet1}", .nBins = 50, .binMin = -5, .binMax = 5};
  variable Subleading_jet_eta{ .name = "Subleading_jet_eta", .title = "#eta^{jet2}", .nBins = 50, .binMin = -5, .binMax = 5};
  variable CentralEtaMTR{ .name ="CentralEtaMTR", .title = "#eta^{central jet}", .nBins = 50, .binMin = -5, .binMax = 5};
  variable ForwardEtaMTR{ .name ="ForwardEtaMTR", .title = "#eta^{forward jet}", .nBins = 50, .binMin = -5, .binMax = 5};
  variable CentralEtaVTR{ .name ="CentralEtaVTR", .title = "#eta^{central jet}", .nBins = 50, .binMin = -5, .binMax = 5};
  variable ForwardEtaVTR{ .name ="ForwardEtaVTR", .title = "#eta^{forward jet}", .nBins = 50, .binMin = -5, .binMax = 5};
  variable nCleanJet30{ .name ="nCleanJet30", .title = "nCleanJet30 (p_{T}>30 GeV)", .nBins = 11, .binMin = 1.5, .binMax = 12.5};
  variable MetNoLep_CleanJet_mindPhi{ .name ="MetNoLep_CleanJet_mindPhi", .title = "min#Delta#Phi(jets,E_{T}^{miss,no #mu})", .nBins = 160, .binMin = 0, .binMax = 3.2};
  variable LHE_Vpt{ .name ="LHE_Vpt", .title = "LHE V p_{T} (GeV)", .nBins = 60, .binMin = 0, .binMax = 600};
  variable LHE_HT{ .name = "LHE_HT", .title = "LHE HT (GeV)", .nBins = 60, .binMin = 0, .binMax = 2000};
  variable decayLeptonId{ .name ="decayLeptonId", .title = "decayLeptonId", .nBins = 7, .binMin = 0, .binMax = 7};
  variable LHE_Nuds{ .name = "LHE_Nuds", .title = "LHE_Nuds", .nBins = 10, .binMin = 0, .binMax = 10};
  variable LHE_Nb{ .name = "LHE_Nb", .title = "LHE_Nb", .nBins = 10, .binMin = 0, .binMax = 10};
  variable LHE_Nc{ .name = "LHE_Nc", .title = "LHE_Nc", .nBins = 10, .binMin = 0, .binMax = 10};
  variable Pileup_nPU{ .name = "Pileup_nPU", .title = "N_PU", .nBins = 10, .binMin = 0, .binMax = 80};
  variable dijet_met_balance{ .name ="dijet_met_balance", .title = "p_{T}^{jj}-E_{T}^{miss}/E_{T}^{miss}", .nBins = 42, .binMin = -1, .binMax = 3.2};
  variable lMjj_binned{ .name ="lMjj", .title = "M_{jj} (GeV)", .nBins = 10, .binMin = -999};
  variable diCleanJet_M_binned{ .name ="diCleanJet_M", .title = "M_{jj} (GeV)", .nBins = 10, .binMin = -999};
  variable diCleanJet_M_binned_reduced{ .name ="diCleanJet_M", .title = "M_{jj} (GeV)", .nBins = 6, .binMin = -998};
     
  variable2D MetNoLep_CleanJet_mindPhi_MetNoLep_pt{ .name1 = "MetNoLep_CleanJet_mindPhi", .name2 = "MetNoLep_pt", .nBins1 = 32, .nBins2 = 500, .binMin1 = 0, .binMin2 = 100, .binMax1 = 3.2, .binMax2 = 600};
  variable2D MetNoLep_CleanJet_mindPhi_diCleanJet_dPhi{ .name1 = "MetNoLep_CleanJet_mindPhi", .name2 = "diCleanJet_dPhi", .nBins1 = 32, .nBins2 = 50, .binMin1 = 0, .binMin2 = 0, .binMax1 = 3.2, .binMax2 = 2.5};
  variable2D MetNoLep_CleanJet_mindPhi_lMjj_dijet_dphi{ .name1 = "MetNoLep_CleanJet_mindPhi", .name2 = "lMjj_dijet_dphi", .nBins1 = 32, .nBins2 = 50, .binMin1 = 0, .binMin2 = 0, .binMax1 = 3.2, .binMax2 = 2.5};
  variable2D MetNoLep_CleanJet_mindPhi_diCleanJet_dEta{ .name1 = "MetNoLep_CleanJet_mindPhi", .name2 = "diCleanJet_dEta", .nBins1 = 32, .nBins2 = 60, .binMin1 = 0, .binMin2 = 1, .binMax1 = 3.2, .binMax2 = 7};
  variable2D MetNoLep_CleanJet_mindPhi_Leading_jet_pt{ .name1 = "MetNoLep_CleanJet_mindPhi", .name2 = "Leading_jet_pt", .nBins1 = 32, .nBins2 = 50, .binMin1 = 0, .binMin2 = 0, .binMax1 = 3.2, .binMax2 = 500};
  variable2D MetNoLep_CleanJet_mindPhi_Subleading_jet_pt{ .name1 = "MetNoLep_CleanJet_mindPhi", .name2 = "Subleading_jet_pt", .nBins1 = 32, .nBins2 = 50, .binMin1 = 0, .binMin2 = 0, .binMax1 = 3.2, .binMax2 = 500};
  variable2D MetNoLep_CleanJet_mindPhi_dijet_met_balance{ .name1 = "MetNoLep_CleanJet_mindPhi", .name2 = "dijet_met_balance", .nBins1 = 32, .nBins2 = 42, .binMin1 = 0, .binMin2 = -1, .binMax1 = 3.2, .binMax2 = 3.2};
  variable2D MetNoLep_CleanJet_mindPhi_Pileup_nPU{ .name1 = "MetNoLep_CleanJet_mindPhi", .name2 = "Pileup_nPU", .nBins1 = 32, .nBins2 = 20, .binMin1 = 0, .binMin2 = 0, .binMax1 = 3.2, .binMax2 = 80};
  variable2D diCleanJet_dPhi_dijet_met_balance{ .name1 = "diCleanJet_dPhi", .name2 = "dijet_met_balance", .nBins1 = 32, .nBins2 = 42, .binMin1 = 0, .binMin2 = -1, .binMax1 = 2.5, .binMax2 = 3.2};
  variable2D diCleanJet_dEta_diCleanJet_M{ .name1 = "diCleanJet_dEta", .name2 = "diCleanJet_M", .nBins1 = 50, .nBins2 = 20, .binMin1 = 0, .binMin2 = 0, .binMax1 = 8, .binMax2 = 2000};
  variable2D lMjj_dijet_deta_lMjj{ .name1 = "lMjj_dijet_deta", .name2 = "lMjj", .nBins1 = 50, .nBins2 = 20, .binMin1 = 0, .binMin2 = 0, .binMax1 = 8, .binMax2 = 2000};
  variable2D diCleanJet_dEta_LHE_HT{ .name1 = "diCleanJet_dEta", .name2 = "LHE_HT", .nBins1 = 50, .nBins2 = 50, .binMin1 = 0, .binMin2 = 0, .binMax1 = 8, .binMax2 = 2000};
  variable2D lMjj_dijet_deta_LHE_HT{ .name1 = "lMjj_dijet_deta", .name2 = "LHE_HT", .nBins1 = 50, .nBins2 = 50, .binMin1 = 0, .binMin2 = 0, .binMax1 = 8, .binMax2 = 2000};
  variable2D MetNoLep_CleanJet_mindPhi_diCleanJet_M{ .name1 = "MetNoLep_CleanJet_mindPhi", .name2 = "diCleanJet_M", .nBins1 = 32,.nBins2 = 32,.binMin1 = -998};
  variable2D MetNoLep_CleanJet_mindPhi_lMjj{ .name1 = "MetNoLep_CleanJet_mindPhi", .name2 = "lMjj", .nBins1 = 32,.nBins2 = 32,.binMin1 = -998};
  variable2D CentralEtaMTR_ForwardEtaMTR{ .name1 = "CentralEtaMTR", .name2 = "ForwardEtaMTR", .nBins1 = 32,.nBins2 = 32,.binMin1 = -997};
  variable2D CentralEtaVTR_ForwardEtaVTR{ .name1 = "CentralEtaVTR", .name2 = "ForwardEtaVTR", .nBins1 = 32,.nBins2 = 32,.binMin1 = -997};

  variable3D CentralEtaVTR_ForwardEtaVTR_MetNoLep_CleanJet_mindPhi{ .name1 = "CentralEtaVTR", .name2 = "ForwardEtaVTR", "MetNoLep_CleanJet_mindPhi", .nBins1 = 50, .nBins2 = 50, .nBins3 = 160, .binMin1 = -5., .binMin2 = -5., .binMin3 = 0, .binMax1 = 5., .binMax2 = 5., .binMax3 = 3.2};
  variable3D CentralEtaMTR_ForwardEtaMTR_MetNoLep_CleanJet_mindPhi{ .name1 = "CentralEtaMTR", .name2 = "ForwardEtaMTR", "MetNoLep_CleanJet_mindPhi", .nBins1 = 50, .nBins2 = 50, .nBins3 = 160, .binMin1 = -5., .binMin2 = -5., .binMin3 = 0, .binMax1 = 5., .binMax2 = 5., .binMax3 = 3.2};

  std::vector<variable> list_of_variables;
  std::vector<variable2D> list_of_variables2D;
  std::vector<variable3D> list_of_variables3D;


  ////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////
  //                                                  //
  //   The USER chooses which variables to include    //
  //   in the output files                            //
  //                                                  //
  //////////////////////////////////////////////////////

  list_of_variables.push_back(MetNoLep_pt);
  list_of_variables.push_back(diCleanJet_M);
  list_of_variables.push_back(lMjj);
  // list_of_variables.push_back(diCleanJet_M_LeadingPosEta);
  // list_of_variables.push_back(diCleanJet_M_LeadingNegEta);
  list_of_variables.push_back(diCleanJet_dEta);
  list_of_variables.push_back(lMjj_dijet_deta);
  // list_of_variables.push_back(diCleanJet_dEtaCmF);
  // list_of_variables.push_back(lMjj_dijet_detaCmF);
  list_of_variables.push_back(diCleanJet_dPhi);
  list_of_variables.push_back(lMjj_dijet_dphi);
  list_of_variables.push_back(Leading_jet_pt);
  list_of_variables.push_back(Subleading_jet_pt);
  list_of_variables.push_back(Leading_jet_eta);
  list_of_variables.push_back(Subleading_jet_eta);
  list_of_variables.push_back(CentralEtaMTR);
  list_of_variables.push_back(ForwardEtaMTR);
  list_of_variables.push_back(CentralEtaVTR);
  list_of_variables.push_back(ForwardEtaVTR);
  //  list_of_variables.push_back(nCleanJet30);
  list_of_variables.push_back(MetNoLep_CleanJet_mindPhi);
  // list_of_variables.push_back(LHE_Vpt);
  // list_of_variables.push_back(LHE_HT);
  //  list_of_variables.push_back(decayLeptonId);  
  // list_of_variables.push_back(LHE_Nuds);  
  // list_of_variables.push_back(LHE_Nb);  
  // list_of_variables.push_back(LHE_Nc);  
  // list_of_variables.push_back(Pileup_nPU);  
  //  list_of_variables.push_back(dijet_met_balance);  
  list_of_variables.push_back(lMjj_binned);  
  list_of_variables.push_back(diCleanJet_M_binned);  
  //   list_of_variables.push_back(diCleanJet_M_binned_reduced);

  list_of_variables2D.push_back(MetNoLep_CleanJet_mindPhi_MetNoLep_pt);
  list_of_variables2D.push_back(MetNoLep_CleanJet_mindPhi_diCleanJet_dPhi);
  list_of_variables2D.push_back(MetNoLep_CleanJet_mindPhi_lMjj_dijet_dphi);
  list_of_variables2D.push_back(MetNoLep_CleanJet_mindPhi_diCleanJet_dEta);
  list_of_variables2D.push_back(MetNoLep_CleanJet_mindPhi_Leading_jet_pt);
  list_of_variables2D.push_back(MetNoLep_CleanJet_mindPhi_Subleading_jet_pt);
  list_of_variables2D.push_back(MetNoLep_CleanJet_mindPhi_dijet_met_balance);
  list_of_variables2D.push_back(MetNoLep_CleanJet_mindPhi_Pileup_nPU);
  list_of_variables2D.push_back(diCleanJet_dPhi_dijet_met_balance);
  list_of_variables2D.push_back(diCleanJet_dEta_diCleanJet_M);
  list_of_variables2D.push_back(lMjj_dijet_deta_lMjj);
  list_of_variables2D.push_back(diCleanJet_dEta_LHE_HT);
  list_of_variables2D.push_back(lMjj_dijet_deta_LHE_HT);
  list_of_variables2D.push_back(MetNoLep_CleanJet_mindPhi_diCleanJet_M);
  list_of_variables2D.push_back(MetNoLep_CleanJet_mindPhi_lMjj);
  list_of_variables2D.push_back(CentralEtaMTR_ForwardEtaMTR);
  list_of_variables2D.push_back(CentralEtaVTR_ForwardEtaVTR);

  list_of_variables3D.push_back(CentralEtaVTR_ForwardEtaVTR_MetNoLep_CleanJet_mindPhi);
  list_of_variables3D.push_back(CentralEtaMTR_ForwardEtaMTR_MetNoLep_CleanJet_mindPhi);

  for (auto var: list_of_variables){ //loop on variables
    mVarVec.push_back(var.name);
  }
  for (auto var: list_of_variables2D){ //loop on variables
    mVarVec2D.push_back(std::make_pair(var.name1,var.name2));
  }
  for (auto var: list_of_variables3D){ //loop on variables
    mVarVec3D.push_back(std::make_tuple(var.name1,var.name2,var.name3));
  }

  //////////////////////////////////////////

  // The creation of the histograms takes place here
  // Generally shouldn't need to edit
  // The case of variable bin sized histograms are handled
  // as special cases in the code below

  double mjjbins[11] = {0,  200, 400, 600, 900, 1200, 1500, 2000, 2750, 3500, 5000};
  double mjjbins_reduced[7] = {200,500,800,1250,2000,2800,5000};
  double etabins[17] = {-4.7,-3.2,-3.0,-2.8,-2.4,-2.0,-1.5,-1.0,0,1.0,1.5,2.0,2.4,2.8,3.0,3.2,4.7};
  for (unsigned iR(RegionType::SR); iR!=RegionType::Last; ++iR){//loop on region
    std::string lreg = GetRegionStr(static_cast<RegionType>(iR));
    for (unsigned iC(CatType::MTR); iC!=CatType::LastCat; ++iC){//loop on category
      std::string lcat = GetCatStr(static_cast<CatType>(iC));
      mFout->mkdir((lreg+"/"+lcat).c_str());
      mFout->cd((lreg+"/"+lcat).c_str());

      if (iC==CatType::VTR) mjjbins[3] = 700;
      else mjjbins[3] = 600;

      for (auto var: list_of_variables){ //loop on variables for 1D
	std::ostringstream label;
	label << "h_" << lreg << "_" << lcat << "_" << var.name;
	TH1F *hTmp = 0;
	if ( var.binMin > -990){
	  hTmp = new TH1F(label.str().c_str(),(";"+var.title).c_str(),var.nBins,var.binMin,var.binMax);
	}
	else{
	  if ( var.binMin == -999 ){
	    hTmp = new TH1F((label.str()+ "_binned").c_str(),(";"+var.title).c_str(),var.nBins,mjjbins);
	  }
	  else if ( var.binMin == -998 ){
	    hTmp = new TH1F((label.str() + "_binned_reduced").c_str(),(";"+var.title).c_str(),var.nBins,mjjbins_reduced);   
	  }
	}

	hTmp->Sumw2();
	mHistVec[iR][iC].push_back(hTmp);
      }
      
      for (auto var: list_of_variables2D){ //loop on variables for 2D
      	std::ostringstream label;
      	label << "h_" << lreg << "_" << lcat << "_" << var.name1 << "_" << var.name2;
      	TH2D *hTmp2D = 0;
	if ( var.binMin1 > -990 ){
      	  hTmp2D = new TH2D(label.str().c_str(),(";"+var.name1+";"+var.name2).c_str(),var.nBins1,var.binMin1,var.binMax1,var.nBins2,var.binMin2,var.binMax2);
      	}
      	else if ( var.binMin1 == -998 ){
      	  hTmp2D = new TH2D(label.str().c_str(),(";"+var.name1+";"+var.name2).c_str(),var.nBins1,0,3.2,10,mjjbins);
      	}
      	else if ( var.binMin1 == -997 ){
      	  hTmp2D = new TH2D(label.str().c_str(),(";"+var.name1+";"+var.name2).c_str(),16,etabins,16,etabins);
	}
      	hTmp2D->Sumw2();
      	mHistVec2D[iR][iC].push_back(hTmp2D);
      }
      for (auto var: list_of_variables3D){ //loop on variables for 3D
      	std::ostringstream label;
      	label << "h_" << lreg << "_" << lcat << "_" << var.name1 << "_" << var.name2 << "_" << var.name3;
      	TH3D *hTmp3D = 0;
	hTmp3D = new TH3D(label.str().c_str(),(";"+var.name1+";"+var.name2+";"+var.name3).c_str(),var.nBins1,var.binMin1,var.binMax1,var.nBins2,var.binMin2,var.binMax2,var.nBins3,var.binMin3,var.binMax3);
      	hTmp3D->Sumw2();
      	mHistVec3D[iR][iC].push_back(hTmp3D);
      }
    }
  }
   
}//end of Begin() function

void Events::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();

}

bool Events::CheckValue(ROOT::Internal::TTreeReaderValueBase& value) {
  if (value.GetSetupStatus() < 0) {
    std::cerr << "Error " << value.GetSetupStatus()
	      << "setting up reader for " << value.GetBranchName() << '\n';
    return false;
  }
  return true;
}


///////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////
  //                                                  //
  //   The USER does not need to edit the following   //
  //                                                  //
  //////////////////////////////////////////////////////

//Various small utility functions

//Setting output file name
void Events::SetOutFileName(const std::string aName){
  mOutFile = aName;
}

//Getting output file name
std::string Events::GetOutFileName(){
  return mOutFile;
}

//Setting process
void Events::SetProcess(const std::string aProcess){
  mProc = aProcess;
}

//Setting whether MC or data
void Events::SetMC(bool aisMC){
  misMC = aisMC;
}

//Setting the year
void Events::SetYear(const std::string aYear){
  mYear = aYear;
}

//Set the systematic (only interested in
//"Nominal" in this analysis)
void Events::SetSystematic(const std::string aSyst){
  mSyst = aSyst;
}

//Set the region (SR, QCDCR, QCDA or QCDB)
void Events::SetRegion(const RegionType & aRegion){
  mReg = aRegion;
}

//Set the category (MTR or VTR)
void Events::SetCategory(const CatType & aCategory){
  mCat = aCategory;
}

//Get the region in string format
std::string Events::GetRegionStr(const RegionType & aRegion){
  std::string reg[6] = {"SR","QCDCR","QCDA","QCDB","HFNoise","HFNoiseCR"};
  if (aRegion==RegionType::Last) return "";
  return reg[static_cast<unsigned>(aRegion)];
}

//Get the category in string format
std::string Events::GetCatStr(const CatType & aCat){
  std::string cat[2] = {"MTR","VTR"};
  if (aCat==CatType::LastCat) return "";
  return cat[static_cast<unsigned>(aCat)];
}

//Set the luminosity to scale the MC by 
//(which is different for 2017 and 2018)
void Events::SetLumiPb(const double & aLumi, const double & aLumi_VTR){
  mLumiPb = aLumi;
  mLumiPb_VTR = aLumi_VTR;
  std::cout << " -- INFO: setting MTR lumi to " << mLumiPb << " pb " << std::endl;
  std::cout << " -- INFO: setting VTR lumi to " << mLumiPb_VTR << " pb " << std::endl;
}

///////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////
  //                                                  //
  //   The USER does not need to edit the following   //
  //   unless adding new input variables from the     //
  //   tree                                           //
  //                                                  //
  //////////////////////////////////////////////////////

//Load the variables from the input ROOT trees
void Events::SetTreeContent(std::string year){

  bool isMC = !static_cast<int>(*isData + 0.5);
  bool includeAll = false;//Set to false for speed. If set to
                          // true you will also need to uncomment 
                          //the corresponding variables in Events.h
  SetMC(isMC);

  //Need for both Data and MC
  lTreeContent["event"] = *event;
  lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"] = *HLT_PFMETNoMu120_PFMHTNoMu120_IDTight;
  lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"] = *HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60;
  lTreeContent["Subleading_jet_eta"] = *Subleading_jet_eta;
  lTreeContent["lMjj_jet2_eta"] = *lMjj_jet2_eta;
  lTreeContent["HLT_TripleJet110_35_35_Mjj650_PFMET110"] = *HLT_TripleJet110_35_35_Mjj650_PFMET110;
  lTreeContent["nVLooseTauFix"] = *nVLooseTauFix;
  lTreeContent["nMediumBJet"] = *nMediumBJet;
  lTreeContent["nVetoElectron"] = *nVetoElectron;
  lTreeContent["Leading_jet_pt"] = *Leading_jet_pt;
  lTreeContent["HLT_DiJet110_35_Mjj650_PFMET110"] = *HLT_DiJet110_35_Mjj650_PFMET110;
  lTreeContent["Leading_jet_phi"] = *Leading_jet_phi;
  lTreeContent["Subleading_jet_phi"] = *Subleading_jet_phi;
  lTreeContent["Leading_jet_eta"] = *Leading_jet_eta;
  lTreeContent["lMjj_jet1_eta"] = *lMjj_jet1_eta;
  lTreeContent["lMjj_jet1_pt"] = *lMjj_jet1_pt;
  lTreeContent["lMjj_jet2_pt"] = *lMjj_jet2_pt;
  lTreeContent["nCleanJet30"] = *nCleanJet30;
  lTreeContent["nLoosePhoton"] = *nLoosePhoton;
  lTreeContent["nLooseMuon"] = *nLooseMuon;
  lTreeContent["Subleading_jet_pt"] = *Subleading_jet_pt;
  lTreeContent["jetHFW_NoiseRateMTR"] = *jetHFW_NoiseRateMTR;
  lTreeContent["jetHFW_NoiseRateVTR"] = *jetHFW_NoiseRateVTR;

  lTreeContent["diCleanJet_M"] = *diCleanJet_M;
  lTreeContent["lMjj"] = *lMjj;
  lTreeContent["MetNoLep_CleanJet_mindPhi"] = *min_dphi_MET; //Using MET rather than MetNoLep in the SR
  lTreeContent["MetNoLep_pt"] = *CorrectedMetNoLep_pt;
  lTreeContent["MetNoLep_phi"] = *CorrectedMetNoLep_phi;
  lTreeContent["MET_phi"] = *CorrectedMET_phi;
  lTreeContent["MET_pt"] = *CorrectedMET_pt;
  lTreeContent["jet_chf_nhf_cut"] = *jet_chf_nhf_cut;
  lTreeContent["jet_chf_nhf_vtr_cut"] = *jet_chf_nhf_vtr_cut;
  lTreeContent["lMjj_dijet_dphi"] = *lMjj_dijet_dphi;
  lTreeContent["lMjj_dijet_deta"] = *lMjj_dijet_deta;
  lTreeContent["horn_sel"] = *horn_sel;
  lTreeContent["horn_sel_vtr"] = *horn_sel_vtr;
  lTreeContent["VecBDPhiCutMTR"] = *VecBDPhiCutMTR;
  lTreeContent["VecBDPhiCutVTR"] = *VecBDPhiCutVTR;
  
  lTreeContent["diCleanJet_dEta"] = *diCleanJet_dEta;
  lTreeContent["diCleanJet_dPhi"] = *diCleanJet_dPhi;
  
  lTreeContent["HLT_PFJet40"] = *HLT_PFJet40;
  lTreeContent["HLT_PFJet60"] = *HLT_PFJet60;
  lTreeContent["HLT_PFJet80"] = *HLT_PFJet80;
  lTreeContent["HLT_PFJet140"] = *HLT_PFJet140;
  lTreeContent["HLT_PFJet200"] = *HLT_PFJet200;
  lTreeContent["HLT_PFJet260"] = *HLT_PFJet260;
  lTreeContent["HLT_PFJet320"] = *HLT_PFJet320;
  lTreeContent["HLT_PFJet400"] = *HLT_PFJet400;
  lTreeContent["HLT_PFJet450"] = *HLT_PFJet450;
  lTreeContent["HLT_PFJet500"] = *HLT_PFJet500;
  lTreeContent["HLT_PFJet550"] = *HLT_PFJet550;
  
  lTreeContent["VBF_MTR_QCD_CR_eff_Sel"] = * VBF_MTR_QCD_CR_eff_Sel;  
  lTreeContent["VBF_VTR_QCD_CR_eff_Sel"] = * VBF_VTR_QCD_CR_eff_Sel;
  lTreeContent["VBF_MTR_QCD_SR_eff_Sel"] = * VBF_MTR_QCD_SR_eff_Sel;
  lTreeContent["VBF_VTR_QCD_SR_eff_Sel"] = * VBF_VTR_QCD_SR_eff_Sel;
  lTreeContent["VBF_MTR_QCD_A_eff_Sel"] = * VBF_MTR_QCD_A_eff_Sel; 
  lTreeContent["VBF_VTR_QCD_A_eff_Sel"] = * VBF_VTR_QCD_A_eff_Sel; 
  lTreeContent["VBF_MTR_QCD_B_eff_Sel"] = * VBF_MTR_QCD_B_eff_Sel; 
  lTreeContent["VBF_VTR_QCD_B_eff_Sel"] = * VBF_VTR_QCD_B_eff_Sel; 
  lTreeContent["VBF_VTR_QCD_NoDijetDphiOrMetPt_eff_Sel"] = * VBF_VTR_QCD_NoDijetDphiOrMetPt_eff_Sel; 
  lTreeContent["VBF_MTR_QCD_NoDijetDphiOrMetPt_eff_Sel"] = * VBF_MTR_QCD_NoDijetDphiOrMetPt_eff_Sel; 
  lTreeContent["VBF_MTR_QCD_HFNoise_eff_Sel"] = * VBF_MTR_QCD_HFNoise_eff_Sel;
  lTreeContent["VBF_VTR_QCD_HFNoise_eff_Sel"] = * VBF_VTR_QCD_HFNoise_eff_Sel;
  lTreeContent["VBF_MTR_QCD_HFNoiseCR_eff_Sel"] = * VBF_MTR_QCD_HFNoiseCR_eff_Sel;
  lTreeContent["VBF_VTR_QCD_HFNoiseCR_eff_Sel"] = * VBF_VTR_QCD_HFNoiseCR_eff_Sel;
  lTreeContent["ContainedHFjet"] = * ContainedHFjet;

  if ( includeAll ){
    lTreeContent["Wenu_flag"] = *Wenu_flag;
    lTreeContent["Zee_flag"] = *Zee_flag;
    lTreeContent["Wmunu_flag"] = *Wmunu_flag;
    lTreeContent["Zmumu_flag"] = *Zmumu_flag;
    lTreeContent["Leading_el_pt"] = *Leading_el_pt;
    lTreeContent["Leading_el_phi"] = *Leading_el_phi;
    lTreeContent["Subleading_el_pt"] = *Subleading_el_pt;
    lTreeContent["Subleading_el_eta"] = *Subleading_el_eta;
    lTreeContent["Subleading_el_phi"] = *Subleading_el_phi;
    lTreeContent["HLT_Ele32_WPTight_Gsf"] = *HLT_Ele32_WPTight_Gsf;
    lTreeContent["HLT_Ele35_WPTight_Gsf"] = *HLT_Ele35_WPTight_Gsf;
    lTreeContent["HLT_Ele32_WPTight_Gsf_L1DoubleEG"] = *HLT_Ele32_WPTight_Gsf_L1DoubleEG;
    lTreeContent["HLT_IsoMu27"] = *HLT_IsoMu27;
    lTreeContent["HLT_Photon200"] = *HLT_Photon200;
    lTreeContent["DiVetoElectron_mass"] = *DiVetoElectron_mass;
    lTreeContent["Leading_muon_pt"] = *Leading_muon_pt;
    lTreeContent["Leading_muon_eta"] = *Leading_muon_eta;
    lTreeContent["Leading_muon_phi"] = *Leading_muon_phi;
    lTreeContent["Subleading_muon_pt"] = *Subleading_muon_pt;
    lTreeContent["Subleading_muon_eta"] = *Subleading_muon_eta;
    lTreeContent["Subleading_muon_phi"] = *Subleading_muon_phi;
    lTreeContent["PV_npvsGood"] = *PV_npvsGood;
    lTreeContent["DiLooseMuon_mass"] = *DiLooseMuon_mass;
  }
  //Specific Treatment for 2017 and 2018
  if ( year == "2017"){
    lTreeContent["PrefireWeight"] = *PrefireWeight;
    lTreeContent["trigger_weight_METMHT"] = *trigger_weight_METMHT2017;
    lTreeContent["trigger_weight_VBF"] = *trigger_weight_VBF2017;
    if (includeAll){
      lTreeContent["trigger_weight_SingleEle35"] = *trigger_weight_SingleEle352017;
    }

  }
  else if  ( year == "2018"){
    lTreeContent["PrefireWeight"] = 1;
    lTreeContent["trigger_weight_METMHT"] = *trigger_weight_METMHT2018;
    lTreeContent["trigger_weight_VBF"] = *trigger_weight_VBF2018;
    if (includeAll){
      lTreeContent["trigger_weight_SingleEle32"] = *trigger_weight_SingleEle322018;
    }
  }


  //Variables that are only needed for Monte Carlo

  if ( isMC ){
    lTreeContent["LooseMuon_eventVetoW"] = *LooseMuonFix_eventVetoW;
    lTreeContent["xs_weight"] = *xs_weight;
    lTreeContent["VLooseTauFix_eventVetoW"] = *VLooseTauFix_eventVetoW;
    lTreeContent["MediumBJet_eventVetoW"] = *MediumBJet_eventVetoW;
    lTreeContent["puWeight"] = *puWeight;
    lTreeContent["LHE_Vpt"] = *LHE_Vpt;
    lTreeContent["LHE_Njets"] = *LHE_Njets;
    lTreeContent["LHE_HT"] = *LHE_HT;
    lTreeContent["VetoElectron_eventVetoW"] = *VetoElectronFix_eventVetoW;
    lTreeContent["Pileup_nPU"] = *Pileup_nPU;
    lTreeContent["fnlo_SF_EWK_corr"] = *fnlo_SF_EWK_corr;
    lTreeContent["fnlo_SF_QCD_corr_QCD_proc_MTR"] = *fnlo_SF_QCD_corr_QCD_proc_MTR;
    lTreeContent["fnlo_SF_QCD_corr_QCD_proc_VTR"] = *fnlo_SF_QCD_corr_QCD_proc_VTR;
    lTreeContent["fnlo_SF_QCD_corr_EWK_proc"] = *fnlo_SF_QCD_corr_EWK_proc;
    // lTreeContent["jetemW_MTR"] = *jetemW_MTR;
    // lTreeContent["jetemW_VTR"] = *jetemW_VTR;
    lTreeContent["jetHFW_MTR"] = *jetHFW_MTR;
    lTreeContent["jetHFW_VTR"] = *jetHFW_VTR;

    if (includeAll){
      lTreeContent["decayLeptonId"] = *decayLeptonId;
      lTreeContent["GenMET_pt"] = *GenMET_pt;
      lTreeContent["Pileup_nTrueInt"] = *Pileup_nTrueInt;
      lTreeContent["LHE_Nb"] = *LHE_Nb;
      lTreeContent["LHE_Nglu"] = *LHE_Nglu;
      lTreeContent["LHE_Nuds"] = *LHE_Nuds;
      lTreeContent["LHE_Nc"] = *LHE_Nc;
    }

    //Specific Treatment for 2017 and 2018  
    if ( year == "2017" ){
      if ( includeAll ) 
	lTreeContent["met_filters"] = *met_filters_2017_mc;
      lTreeContent["hem_weight"] = 1;
    }
    else if  ( year == "2018" ){
      if ( includeAll ) 
	lTreeContent["met_filters"] = *met_filters_2018_mc;
      lTreeContent["hem_weight"] = *hem_weight;
    }
  }  
  else if ( !isMC ){//Specific treatment for Data
    lTreeContent["hem_weight"] = 1;

    if ( includeAll ){
      if ( year == "2017" )
	lTreeContent["met_filters"] = *met_filters_2017_data;
      else if  ( year == "2018" )
	lTreeContent["met_filters"] = *met_filters_2018_data;
    }

  }

  
}

//Apply the base selection to the events
Bool_t Events::BaseSelection(){
  
  bool pass=kTRUE;

  //Remove events with HT < 100 in the QCD sample 
  //due to very low statistics
  if ( mProc == "QCD" || mProc == "QCDRELAX" ){
    pass = pass && lTreeContent["LHE_HT"]>100; 
  }

  return pass;

}

//Calculate the base weight of the event
Double_t Events::BaseWeight(){

  double w = 1.0;

  double lumi = 1.0;
  if (mCat==CatType::MTR){
    lumi = mLumiPb;
  }
  else if (mCat==CatType::VTR){
    lumi = mLumiPb_VTR;
  }

  //Only calculate the weight for Monte Carlo
  if ( misMC ){
    w = (lTreeContent["puWeight"])*(lTreeContent["xs_weight"])*lumi*(lTreeContent["PrefireWeight"]);
    
    double tauveto = lTreeContent["VLooseTauFix_eventVetoW"];
    double electronveto = lTreeContent["VetoElectron_eventVetoW"];
    double muonveto = lTreeContent["LooseMuon_eventVetoW"];
    double bjetveto = lTreeContent["MediumBJet_eventVetoW"];
    w *= tauveto*electronveto*muonveto*bjetveto;
    
    w*=lTreeContent["hem_weight"];//1 for data and MC in 2017
  }

  return w;

}

Bool_t Events::PassSelection(){

  bool pass = kTRUE;
  std::string lcat = "";
  double cut_lMjj_dijet_dphi = 2.5;

  //////////////////////////////////

  //Special weights to calculate if we are using Anne Marie's trees    
    
  bool lCleanCut0 = true;
  bool lCleanCut1 = true;
  bool lCleanCut2 = true;
  bool lCleanCut3 = true;

  pass = pass && lCleanCut0;
  pass = pass && lCleanCut1;
  
  if (mYear == "2018"){
    pass = pass && lCleanCut2;
    pass = pass && lCleanCut3;
  }

  //////////////////////////////////

  CalculateAdditionalVariables_Stage1();

  double HFcut = 3.0;
  //Apply the circular Cut and HF HF removal 
  //(circular cut usually already applied in the trees)
  if (mCat==CatType::MTR){
    //    pass = pass && (static_cast<int>(lTreeContent["VecBDPhiCutMTR"]));
    //    pass = pass && ( abs(lTreeContent["Leading_jet_eta"]) < HFcut || abs(lTreeContent["Subleading_jet_eta"]) < HFcut );
  }
  else if (mCat==CatType::VTR){
    pass = pass && lTreeContent["lMjj"] > 700;
    pass = pass && lTreeContent["lMjj_jet1_pt"] > 130;
    pass = pass && lTreeContent["lMjj_jet2_pt"] > 60;
    //pass = pass && lTreeContent["lMjj"] > 900;
    //pass = pass && lTreeContent["lMjj_jet1_pt"] > 140;
    //pass = pass && lTreeContent["lMjj_jet2_pt"] > 70;
  }

  //Apply the trigger selection
  if (mCat==CatType::MTR){
    lcat = "MTR";

    //The MET and triggers are used in the SR and QCDCR, 
    //whilst the JetHT triggers are used in the QCDA and QCDB region
    if (mReg==RegionType::SR || mReg==RegionType::QCDCR || mReg==RegionType::HFNoise || mReg==RegionType::HFNoiseCR ){
      pass = pass && (static_cast<int>(lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"])==1 || static_cast<int>(lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"])==1);
    }
    else{
      pass = pass && (
		      // static_cast<int>(lTreeContent["HLT_PFJet40"])==1 //Too low statistics
		      // || static_cast<int>(lTreeContent["HLT_PFJet60"])==1 //Too low statistics
		      // || static_cast<int>(lTreeContent["HLT_PFJet80"])==1 //Too low statistics
		      // || static_cast<int>(lTreeContent["HLT_PFJet140"])==1 //Too low statistics
		      static_cast<int>(lTreeContent["HLT_PFJet200"])==1
		      || static_cast<int>(lTreeContent["HLT_PFJet260"])==1
		      || static_cast<int>(lTreeContent["HLT_PFJet320"])==1
		      || static_cast<int>(lTreeContent["HLT_PFJet400"])==1
		      || static_cast<int>(lTreeContent["HLT_PFJet450"])==1
		      || static_cast<int>(lTreeContent["HLT_PFJet500"])==1
		      || static_cast<int>(lTreeContent["HLT_PFJet550"])==1
		      );
      
      pass = pass && (lTreeContent["Leading_jet_pt"] > 200);//some say it should be 250?
    }
  }
  else if  (mCat==CatType::VTR){
    lcat = "VTR";
    if (mReg==RegionType::SR || mReg==RegionType::QCDCR || mReg==RegionType::HFNoise || mReg==RegionType::HFNoiseCR ){
      pass = pass && (static_cast<int>(lTreeContent["HLT_DiJet110_35_Mjj650_PFMET110"])==1 || static_cast<int>(lTreeContent["HLT_TripleJet110_35_35_Mjj650_PFMET110"])==1);
    }
    else{
      pass = pass && (
		      // static_cast<int>(lTreeContent["HLT_PFJet40"])==1 //Too low statistics
		      // || static_cast<int>(lTreeContent["HLT_PFJet60"])==1 //Too low statistics
		      // || static_cast<int>(lTreeContent["HLT_PFJet80"])==1 //Too low statistics
		      // || static_cast<int>(lTreeContent["HLT_PFJet140"])==1 //Too low statistics
		      static_cast<int>(lTreeContent["HLT_PFJet200"])==1
		      || static_cast<int>(lTreeContent["HLT_PFJet260"])==1
		      || static_cast<int>(lTreeContent["HLT_PFJet320"])==1
		      || static_cast<int>(lTreeContent["HLT_PFJet400"])==1
		      || static_cast<int>(lTreeContent["HLT_PFJet450"])==1
		      || static_cast<int>(lTreeContent["HLT_PFJet500"])==1
		      || static_cast<int>(lTreeContent["HLT_PFJet550"])==1
		      );
      
      pass = pass && (lTreeContent["Leading_jet_pt"] > 200); //some say it should be 250?
    }    
  }//End of trigger section



  //Now apply the specific cuts for each of the regions

  //SIGNAL AND HFNOISE REGIONS
  
  if (mReg==RegionType::SR || mReg==RegionType::HFNoise){  
    
    if ( mCat == CatType::MTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"] > 0.5;
      pass = pass && lTreeContent["MET_pt"] > 250;
    }
    else if ( mCat == CatType::VTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"] > 1.8;
      pass = pass && lTreeContent["MET_pt"] <= 250;
      pass = pass && lTreeContent["MET_pt"] > 160;
    }
    
    //////////////////////////////////
    //Get the estimate in the "Relaxed" QCD region. I.e. relaxing the requirements on dijet dphi
    
    if ( mProc == "QCDRELAX" ){
      //This is the main selection flag encompassing all other cuts
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_NoDijetDphiOrMetPt_eff_Sel"]));
      
      if ( mCat == CatType::MTR ){
	pass = pass && lTreeContent["diCleanJet_dPhi"]<cut_lMjj_dijet_dphi; 
      }
      else if ( mCat == CatType::VTR ){
	pass = pass && lTreeContent["lMjj_dijet_dphi"]<cut_lMjj_dijet_dphi;
      }      
      if ( mReg==RegionType::SR ){ 
	pass = pass && !(static_cast<int>(lTreeContent["ContainedHFjet"]));
      }
      else{
	pass = pass && (static_cast<int>(lTreeContent["ContainedHFjet"]));
      }

    }
    else{
      //This is the main selection flag encompassing all other cuts
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_"+GetRegionStr(mReg)+"_eff_Sel"]));
    }
      
    //////////////////////////////////
    // Apply the lepton vetoes on data
    
    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseTauFix"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }
    
  }
  else if (mReg==RegionType::QCDCR || mReg==RegionType::HFNoiseCR){   //QCD CONTROL REGION
    
    if ( mCat == CatType::MTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"] < 0.5;
      pass = pass && lTreeContent["MET_pt"] > 250; 
    }
    else if ( mCat == CatType::VTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"] < 1.8;
      pass = pass && lTreeContent["MET_pt"] <= 250;   
      pass = pass && lTreeContent["MET_pt"] > 160;
    }

    //////////////////////////////////
    //Get the estimate in the "Relaxed" QCD region. I.e. relaxing the requirements on dijet dphi

    if ( mProc == "QCDRELAX" ){
      //This is the main selection flag encompassing all other cuts
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_NoDijetDphiOrMetPt_eff_Sel"]));

      if ( mCat == CatType::MTR ){
	pass = pass && lTreeContent["diCleanJet_dPhi"]<cut_lMjj_dijet_dphi; 
      }
      else if ( mCat == CatType::VTR ){
	pass = pass && lTreeContent["lMjj_dijet_dphi"]<cut_lMjj_dijet_dphi; 
      }
      if ( mReg==RegionType::SR ){ 
	pass = pass && !(static_cast<int>(lTreeContent["ContainedHFjet"]));
      }
      else{
	pass = pass && (static_cast<int>(lTreeContent["ContainedHFjet"]));
      }

    }
    else{
      //This is the main selection flag encompassing all other cuts
      if ( mReg==RegionType::QCDCR )
	pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_CR_eff_Sel"]));
      else
	pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_HFNoiseCR_eff_Sel"]));
    }
    
    //////////////////////////////////
    // Apply the lepton vetoes on data
    
    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseTauFix"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }

  }
  else if (mReg==RegionType::QCDA){ //QCD REGION A "Control-like" region
    
    if ( mCat == CatType::MTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]<0.5;
    }
    else if ( mCat == CatType::VTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]<1.8;
    }

    pass = pass && lTreeContent["MET_pt"]>100;
    pass = pass && lTreeContent["MET_pt"]<=160;         

    //////////////////////////////////
    //Get the estimate in the "Relaxed" QCD region. I.e. relaxing the requirements on dijet dphi

    if ( mProc == "QCDRELAX" ){
      //This is the main selection flag encompassing all other cuts
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_NoDijetDphiOrMetPt_eff_Sel"]));

      if ( mCat == CatType::MTR ){
	pass = pass && lTreeContent["diCleanJet_dPhi"]<cut_lMjj_dijet_dphi;
      }
      else if ( mCat == CatType::VTR ){
	pass = pass && lTreeContent["lMjj_dijet_dphi"]<cut_lMjj_dijet_dphi;
      }
    }
    else{
      //This is the main selection flag encompassing all other cuts
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_A_eff_Sel"]));
    }
    
    //////////////////////////////////
    // Apply the lepton vetoes on data

    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseTauFix"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }
    
  }
  else if (mReg==RegionType::QCDB){  //QCD REGION B "Signal-like" region
        
    if ( mCat == CatType::MTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;
    }
    else if ( mCat == CatType::VTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>1.8;
    }
    
    pass = pass && lTreeContent["MET_pt"]>100;
    pass = pass && lTreeContent["MET_pt"]<=160;     

    //////////////////////////////////
    //Get the estimate in the "Relaxed" QCD region. I.e. relaxing the requirements on dijet dphi

    if ( mProc == "QCDRELAX" ){
      //This is the main selection flag encompassing all other cuts
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_NoDijetDphiOrMetPt_eff_Sel"]));

      if ( mCat == CatType::MTR ){
	pass = pass && lTreeContent["diCleanJet_dPhi"]<cut_lMjj_dijet_dphi;
      }
      else if ( mCat == CatType::VTR ){
	pass = pass && lTreeContent["lMjj_dijet_dphi"]<cut_lMjj_dijet_dphi; 
      }
    }
    else{
      //This is the main selection flag encompassing all other cuts
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_B_eff_Sel"]));
    }  
  
    //////////////////////////////////
    // Apply the lepton vetoes on data

    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseTauFix"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }
   
  }
  return pass;
}

//Reweight the MC based on the MET phi distribution
//in the single muon control region
Double_t Events::MetPhiWeight(){
  double sf = Met_phi_SF->GetBinContent( Met_phi_SF->FindBin( lTreeContent["MetNoLep_phi"] ) );
  return sf;
}


//Calculate the HFNoiseRate of the selected event
Double_t Events::HFNoiseWeight(){
  double w = 1.0; 

  if (mCat==CatType::MTR){   
    if ( lTreeContent["jetHFW_NoiseRateMTR"] <= 0.99){
      w = lTreeContent["jetHFW_NoiseRateMTR"]/(1-lTreeContent["jetHFW_NoiseRateMTR"]);
    }
    else{
      std::cout << "weight is zero - should not happen" << std::endl;
    }
  }else if (mCat==CatType::VTR){
    if ( lTreeContent["jetHFW_NoiseRateVTR"] <= 0.99){
      w = lTreeContent["jetHFW_NoiseRateVTR"]/(1-lTreeContent["jetHFW_NoiseRateVTR"]);
    }
    else{
      std::cout << "weight is zero - should not happen" << std::endl;
    }
  }

  return w;
}
//Calculate the full weight of the selected event
Double_t Events::SelWeight(){
  double w = 1.0;

  //Get MET phi SF if neccessary
  if ( misMC ){
    if ( mreweight_met_phi ){
      w *= MetPhiWeight();
    }
  }

  //Apply QCD k-factor SFs to MC
  if ( misMC ) {

    if ( mCat == CatType::MTR ){
      w *= lTreeContent["fnlo_SF_QCD_corr_QCD_proc_MTR"];
      //      w *= lTreeContent["jetemW_MTR"];
      w *= lTreeContent["jetHFW_MTR"];
    }
    else if ( mCat == CatType::VTR ){
      w *= lTreeContent["fnlo_SF_QCD_corr_QCD_proc_VTR"];
      //      w *= lTreeContent["jetemW_VTR"];
      w *= lTreeContent["jetHFW_VTR"];
    }

    w *= lTreeContent["fnlo_SF_QCD_corr_EWK_proc"];
    w *= lTreeContent["fnlo_SF_EWK_corr"];

  }


  //Apply trigger SFs
  //In the SR and QCDCR only apply a weight to MC
  if (mReg==RegionType::SR || mReg==RegionType::QCDCR || mReg==RegionType::HFNoise || mReg==RegionType::HFNoiseCR){
    
    if ( misMC ) {
      if ( mCat == CatType::MTR ){
	w *= lTreeContent["trigger_weight_METMHT"];
      }
      else if ( mCat == CatType::VTR ){
	w *= lTreeContent["trigger_weight_VBF"];
      }
    }
  }
  else{
    //In the other regions we don't have a calculated
    //trigger SF, but need to apply a weight to data
    //to correct for the prescale
    if ( mProc == "DATA" && !misMC ){//DATA is the name for the JetHT dataset
      
      double prescale = 1.;
      if (mYear == "2017"){
	
	if (static_cast<int>(lTreeContent["HLT_PFJet550"])==1 ){
	  prescale = 41.54 /  41.54 ;
	}
	else if (static_cast<int>(lTreeContent["HLT_PFJet500"])==1 ){
	  prescale = 41.54 /  41.54 ;
	}
	else if (static_cast<int>(lTreeContent["HLT_PFJet450"])==1 ){
	  prescale = 10.45 /  41.54 ;
	}
	else if (static_cast<int>(lTreeContent["HLT_PFJet400"])==1 ){
	  prescale = 4.21 /  41.54 ;
	}
	else if (static_cast<int>(lTreeContent["HLT_PFJet320"])==1 ){
	  prescale = 1.4 /  41.54 ;
	}
	else if (static_cast<int>(lTreeContent["HLT_PFJet260"])==1 ){
	  prescale = 0.55 /  41.54 ;
	}
	else if (static_cast<int>(lTreeContent["HLT_PFJet200"])==1 ){
	  prescale = 0.22 /  41.54 ;
	}
	//The following triggers are not used due to the low statistics

	// else if (static_cast<int>(lTreeContent["HLT_PFJet140"])==1 ){
	//   prescale = 0.0397 /  41.54 ;
	// }
	// else if (static_cast<int>(lTreeContent["HLT_PFJet80"])==1 ){
	//   prescale = 0.0042 /  41.54 ;
	// }
	// else if (static_cast<int>(lTreeContent["HLT_PFJet60"])==1 ){
	//   prescale = 0.0010 /  41.54 ;
	// }
	// else if ( static_cast<int>(lTreeContent["HLT_PFJet40"])==1 ){
	//   prescale = 0.0003 /  41.54 ;
	// 	}
      }	
      else if (mYear == "2018"){
	
	if (static_cast<int>(lTreeContent["HLT_PFJet550"])==1 ){
	  prescale = 59.96 /  59.96 ;
	}
	else if (static_cast<int>(lTreeContent["HLT_PFJet500"])==1 ){
	  prescale = 59.96 /  59.96 ;
	}
	else if (static_cast<int>(lTreeContent["HLT_PFJet450"])==1 ){
	  prescale = 7.53 /  59.96 ;
	}
	else if (static_cast<int>(lTreeContent["HLT_PFJet400"])==1 ){
	  prescale = 3.78 /  59.96 ;
	}
	else if (static_cast<int>(lTreeContent["HLT_PFJet320"])==1 ){
	  prescale = 1.83 /  59.96 ;
	}
	else if (static_cast<int>(lTreeContent["HLT_PFJet260"])==1 ){
	  prescale = 0.47 /  59.96 ;
	}
	else if (static_cast<int>(lTreeContent["HLT_PFJet200"])==1 ){
	  prescale = 0.21 /  59.96 ;
	}
	//The following triggers are not used due to the low statistics

	// else if (static_cast<int>(lTreeContent["HLT_PFJet140"])==1 ){
	//   prescale =  0.0486 /  59.96 ;
	// }
	// else if (static_cast<int>(lTreeContent["HLT_PFJet80"])==1 ){
	//   prescale = 0.0051 /  59.96 ;
	// }
	// else if (static_cast<int>(lTreeContent["HLT_PFJet60"])==1 ){
	//   prescale =  0.0008 /  59.96 ;
	// }
	// else if ( static_cast<int>(lTreeContent["HLT_PFJet40"])==1 ){
	//   prescale =  0.0002 /  59.96 ;
	// }
      }	
            
      w /= prescale;
    }
           
  }

  return w;
}

void Events::CalculateAdditionalVariables_Stage1(){
  
  //Calculate the variables relating to the eta of the central and forward jets
  double CentralEtaMTR = 0;
  double ForwardEtaMTR = 0;
  double CentralEtaVTR = 0;
  double ForwardEtaVTR = 0;

  CentralEtaMTR = (std::abs(lTreeContent["Leading_jet_eta"]) < std::abs(lTreeContent["Subleading_jet_eta"])) ? lTreeContent["Leading_jet_eta"] : lTreeContent["Subleading_jet_eta"];
  ForwardEtaMTR = (std::abs(lTreeContent["Leading_jet_eta"]) > std::abs(lTreeContent["Subleading_jet_eta"])) ? lTreeContent["Leading_jet_eta"] : lTreeContent["Subleading_jet_eta"];
  CentralEtaVTR = (std::abs(lTreeContent["lMjj_jet1_eta"]) < std::abs(lTreeContent["lMjj_jet2_eta"])) ? lTreeContent["lMjj_jet1_eta"] : lTreeContent["lMjj_jet2_eta"];
  ForwardEtaVTR = (std::abs(lTreeContent["lMjj_jet1_eta"]) > std::abs(lTreeContent["lMjj_jet2_eta"])) ? lTreeContent["lMjj_jet1_eta"] : lTreeContent["lMjj_jet2_eta"];

  lTreeContent["CentralEtaMTR"] = CentralEtaMTR;
  lTreeContent["ForwardEtaMTR"] = ForwardEtaMTR;
  lTreeContent["CentralEtaVTR"] = CentralEtaVTR;
  lTreeContent["ForwardEtaVTR"] = ForwardEtaVTR;


}

void Events::CalculateAdditionalVariables_Stage2(){

  //Calculate the dijet mass balance
  //plus a couple of other auxiliary variables

  double dijet_met_balance = 0;

  TLorentzVector j1,j2;
  j1.SetPtEtaPhiM(lTreeContent["Leading_jet_pt"],lTreeContent["Leading_pt_eta"],lTreeContent["Leading_pt_phi"],0);
  j2.SetPtEtaPhiM(lTreeContent["Subleading_jet_pt"],lTreeContent["Subleading_pt_eta"],lTreeContent["Subleading_pt_phi"],0);
  dijet_met_balance = ((j1+j2).Pt() - lTreeContent["MET_pt"])/(lTreeContent["MET_pt"]);

  lTreeContent["dijet_met_balance"] = dijet_met_balance;

  lTreeContent["diCleanJet_dEtaCmF"] = lTreeContent["CentralEtaMTR"]-lTreeContent["ForwardEtaMTR"];
  lTreeContent["lMjj_dijet_detaCmF"] = lTreeContent["CentralEtaVTR"]-lTreeContent["ForwardEtaVTR"];

  if ( lTreeContent["Leading_jet_eta"] > 0 ){
    lTreeContent["diCleanJet_M_LeadingPosEta"] = lTreeContent["diCleanJet_M"];
    lTreeContent["diCleanJet_M_LeadingNegEta"] = -999;
  }
  else{
    lTreeContent["diCleanJet_M_LeadingNegEta"] = lTreeContent["diCleanJet_M"];
    lTreeContent["diCleanJet_M_LeadingPosEta"] = -999;
  }


}


///////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////
  //                                                  //
  //   The USER does not need to edit the following   //
  //                                                  //
  //////////////////////////////////////////////////////

//The main loop to calculate the weights, apply the selection,
//calculate auxiliary variables and fill the histograms
Bool_t Events::Process(Long64_t entry)
{

  fReader.SetLocalEntry(entry);
  SetTreeContent(mYear);

  if (entry%100000==0) std::cout << " -- processing entry " << entry/1000. << "k" << std::endl;
  //   if ( entry > 100 ) Abort("maxevents");

  for (unsigned iC(CatType::MTR); iC!=CatType::LastCat; ++iC){//loop on cat
    SetCategory(static_cast<CatType>(iC));

    if (!BaseSelection()){
      continue;
    }

    for (unsigned iR(RegionType::SR); iR!=RegionType::Last; ++iR){//loop on region
      SetRegion(static_cast<RegionType>(iR));

      if (!PassSelection()){
	continue;
      }
      double weight = BaseWeight()*SelWeight();

      if ( mReg == RegionType::HFNoise || mReg == RegionType::HFNoiseCR ){
	weight *= HFNoiseWeight ();
      }

      CalculateAdditionalVariables_Stage2();

      for (unsigned iV(0); iV<mHistVec[iR][iC].size(); ++iV){
	mHistVec[iR][iC][iV]->Fill(lTreeContent[mVarVec[iV]],weight);
      }

      for (unsigned iV(0); iV<mHistVec2D[iR][iC].size(); ++iV){
	mHistVec2D[iR][iC][iV]->Fill(lTreeContent[mVarVec2D[iV].first],lTreeContent[mVarVec2D[iV].second],weight);
      }

      for (unsigned iV(0); iV<mHistVec3D[iR][iC].size(); ++iV){
	mHistVec3D[iR][iC][iV]->Fill(lTreeContent[std::get<0>(mVarVec3D[iV])],lTreeContent[std::get<1>(mVarVec3D[iV])],lTreeContent[std::get<2>(mVarVec3D[iV])],weight);
      }

    }
  }
   
  return kTRUE;
}

void Events::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.

}

void Events::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.
  std::cout << " -- INFO: calling TERMINATE." << std::endl;
  
  mFout->Write();
  mFout->Close();

}
#endif
