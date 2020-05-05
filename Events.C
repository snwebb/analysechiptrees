#ifndef Events_cxx
#define Events_cxx
// The class definition in Events.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("Events.C")
// root> T->Process("Events.C","some options")
// root> T->Process("Events.C+")
//


#include "Events.h"
#include <TH2.h>
#include <TStyle.h>

Events::Events(TTree *){
  mFout = 0;
  mOutFile = "Histos.root";
  mCat=CatType::MTR;
  mReg=RegionType::SR;
  mLumiPb=41800.;
  mProc = "QCD";

  lTreeContent.clear();
  //  Initialise();
}

Events::~Events(){
  mVarVec.clear();
  lTreeContent.clear();
  for (unsigned iR(RegionType::SR); iR!=RegionType::Last; ++iR){//loop on region
    for (unsigned iC(CatType::MTR); iC!=CatType::LastCat; ++iC){//loop on region
      mHistVec[iR][iC].clear();
    }
  }
}

void Events::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  TString option = GetOption();
  mFout = TFile::Open(mOutFile.c_str(),"RECREATE");
    
    const unsigned nVars = 19;
  //  const unsigned nVars = 2;
  // std::string lVar[nVars] = {"MetNoLep_pt","diCleanJet_M","diCleanJet_dEta","diCleanJet_dPhi","Leading_jet_pt","Subleading_jet_pt","Leading_jet_eta","Subleading_jet_eta","nCleanJet30","MetNoLep_CleanJet_mindPhi","LHE_Vpt","LHE_HT","decayLeptonId","LHE_Nuds","LHE_Nb","LHE_Nc","Pileup_nPU","MetNoLep_phi","diCleanJet_M"};
    std::string lVar[nVars] = {"MetNoLep_pt","diCleanJet_M","diCleanJet_dEta","diCleanJet_dPhi","Leading_jet_pt","Subleading_jet_pt","Leading_jet_eta","Subleading_jet_eta","nCleanJet30","MetNoLep_CleanJet_mindPhi","LHE_Vpt","LHE_HT","decayLeptonId","LHE_Nuds","LHE_Nb","LHE_Nc","Pileup_nPU","diCleanJet_M","diCleanJet_M"};

    // std::string lVar[nVars] = {"MetNoLep_CleanJet_mindPhi","diCleanJet_M"};
  for (unsigned iV(0); iV<nVars; ++iV){//loop on variables
    mVarVec.push_back(lVar[iV]);
  }
  
  //   std::string lVarName[nVars] = {"E_{T}^{miss,no #mu} (GeV)","M_{jj} (GeV)","#Delta#eta_{jj}","#Delta#Phi_{jj}","p_{T}^{jet1} (GeV)","p_{T}^{jet2} (GeV)","#eta^{jet1}","#eta^{jet2}","nCleanJet30 (p_{T}>30 GeV)","min#Delta#Phi(jets,E_{T}^{miss,no #mu})","LHE V p_{T} (GeV)","LHE HT (GeV)","decayLeptonId","LHE_Nuds","LHE_Nb","LHE_Nc","N_PU","#Phi E_{T}^{miss,no #mu}","M_{jj} (GeV)"};
  std::string lVarName[nVars] = {"E_{T}^{miss,no #mu} (GeV)","M_{jj} (GeV)","#Delta#eta_{jj}","#Delta#Phi_{jj}","p_{T}^{jet1} (GeV)","p_{T}^{jet2} (GeV)","#eta^{jet1}","#eta^{jet2}","nCleanJet30 (p_{T}>30 GeV)","min#Delta#Phi(jets,E_{T}^{miss,no #mu})","LHE V p_{T} (GeV)","LHE HT (GeV)","decayLeptonId","LHE_Nuds","LHE_Nb","LHE_Nc","N_PU","M_{jj} (GeV)","M_{jj} (GeV)"};
  //  std::string lVarName[nVars] = {"min#Delta#Phi(jets,E_{T}^{miss,no #mu})","M_{jj} (GeV)"};

  const int nBins[nVars] = {350,380,120,50,84,52,100,100,10,180,60,60,7,10,10,10,10,10,6};
  //const int nBins[nVars] = {180,6};
  //  const int nBins[nVars] = {500,380,120,50,84,52,100,100,10,180,60,60,7,10,10,10,40,6,180};


  //const double binsX[nBins+1] = {200,400,600,900,1200,1500,2000,2750,3500,5000};
  //const double binsX[nBins+1] = {0,50,100,150,200,250,300,400,500,800};
    const double binMin[nVars-1] = {100,200,1,0,80,40,-5,-5,2,0,0,0,0,0,0,0,0};
    //  const double binMin[nVars-1] = {0};
  const double binMax[nVars-1] = {600,4000,7,1.5,500,300,5,5,12,3.1416,600,2000,7,10,10,10,80};
  //  const double binMax[nVars-1] = {3.1416};
  double mjjbins[11] = {0,  200, 400, 600, 900, 1200, 1500, 2000, 2750, 3500, 5000};

  double mjjbins_reduced[7] = {200,500,800,1250,2000,2800,5000};
  
  for (unsigned iR(RegionType::SR); iR!=RegionType::Last; ++iR){//loop on region
    std::string lreg = GetRegionStr(static_cast<RegionType>(iR));
    for (unsigned iC(CatType::MTR); iC!=CatType::LastCat; ++iC){//loop on cat
      std::string lcat = GetCatStr(static_cast<CatType>(iC));
      mFout->mkdir((lreg+"/"+lcat).c_str());
      mFout->cd((lreg+"/"+lcat).c_str());
      for (unsigned iV(0); iV<nVars; ++iV){//loop on variables
	std::ostringstream label;
	label << "h_" << lreg << "_" << lcat << "_" << lVar[iV];
	TH1F *hTmp = 0;
	if ( iV < nVars-2 ){
	     hTmp = new TH1F(label.str().c_str(),(";"+lVarName[iV]).c_str(),nBins[iV],binMin[iV],binMax[iV]);
	}
	else if (iV == nVars-1){
	  hTmp = new TH1F((label.str() + "_binned_reduced").c_str(),(";"+lVarName[iV]).c_str(),nBins[iV],mjjbins_reduced);
	}
	else if (iV == nVars-2){
	  hTmp = new TH1F((label.str() + "_binned").c_str(),(";"+lVarName[iV]).c_str(),nBins[iV],mjjbins);
	}
	hTmp->Sumw2();
	mHistVec[iR][iC].push_back(hTmp);
      }
    }
  }
   
}

void Events::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void Events::SetOutFileName(const std::string aName){
  mOutFile = aName;
}

void Events::SetProcess(const std::string aProcess){
  mProc = aProcess;
}
void Events::SetMC(bool aisMC){
  misMC = aisMC;
}
void Events::SetYear(const std::string aYear){
  mYear = aYear;
}
void Events::SetAM(bool aisAM){
  misAM = aisAM;
}
void Events::SetSystematic(const std::string aSyst){
  mSyst = aSyst;
}

std::string Events::GetOutFileName(){
  return mOutFile;
}


void Events::SetCategory(const CatType & aCategory){
  mCat = aCategory;
}

std::string Events::GetRegionStr(const RegionType & aRegion){
  //  std::string reg[8] = {"SR","We","Wmu","Zee","Zmumu","QCDCR","QCDA","QCDB"};
  std::string reg[4] = {"SR","QCDCR","QCDA","QCDB"};
  if (aRegion==RegionType::Last) return "";
  return reg[static_cast<unsigned>(aRegion)];
}

std::string Events::GetCatStr(const CatType & aCat){
  //  std::string cat[5] = {"MTR","MTRvetoTau","MTRvetoB","MTRvetoLep","MTRvetoAll"};
  std::string cat[2] = {"MTR","VTR"};
  if (aCat==CatType::LastCat) return "";
  return cat[static_cast<unsigned>(aCat)];
}

void Events::SetRegion(const RegionType & aRegion){
  mReg = aRegion;
}

void Events::SetLumiPb(const double & aLumi){
  mLumiPb = aLumi;
  std::cout << " -- INFO: setting lumi to " << mLumiPb << " pb " << std::endl;
}


bool Events::CheckValue(ROOT::Internal::TTreeReaderValueBase& value) {
  if (value.GetSetupStatus() < 0) {
    std::cerr << "Error " << value.GetSetupStatus()
	      << "setting up reader for " << value.GetBranchName() << '\n';
    return false;
  }
  return true;
}


void Events::SetTreeContent(std::string year){

  // / bool isMC = !(static_cast*isData);

  bool isMC = !static_cast<int>(*isData + 0.5);
  //  std::cout << "ismc = " << isMC << std::endl;
  SetMC(isMC);
  if ( year == "2017"){
    lTreeContent["L1PreFiringWeight_Nom"] = *L1PreFiringWeight_Nom;
    lTreeContent["trigger_weight_METMHT"] = *trigger_weight_METMHT2017;
    lTreeContent["trigger_weight_VBF"] = *trigger_weight_VBF2017;
    lTreeContent["trigger_weight_SingleEle35"] = *trigger_weight_SingleEle352017;

  }
  else if  ( year == "2018"){
    lTreeContent["L1PreFiringWeight_Nom"] = 1;
    lTreeContent["trigger_weight_METMHT"] = *trigger_weight_METMHT2018;
    lTreeContent["trigger_weight_VBF"] = *trigger_weight_VBF2018;
    lTreeContent["trigger_weight_SingleEle32"] = *trigger_weight_SingleEle322018;
  }

  if ( isMC ){
    lTreeContent["decayLeptonId"] = *decayLeptonId;
    lTreeContent["CRLooseMuon_eventSelW"] = *CRLooseMuon_eventSelW;
    lTreeContent["CRVetoElectron_eventSelW"] = *CRVetoElectron_eventSelW;
    // lTreeContent["LooseMuon_eventVetoW_systID_up"] = *LooseMuon_eventVetoW_systID_up;
    // lTreeContent["LooseMuon_eventVetoW_systID_down"] = *LooseMuon_eventVetoW_systID_down;
    lTreeContent["LooseMuon_eventVetoW"] = *LooseMuon_eventVetoW;
    lTreeContent["xs_weight"] = *xs_weight;
    lTreeContent["GenMET_pt"] = *GenMET_pt;
    lTreeContent["Pileup_nTrueInt"] = *Pileup_nTrueInt;
    lTreeContent["VLooseSITTau_eventVetoW"] = *VLooseSITTau_eventVetoW;
    // lTreeContent["VLooseSITTau_eventVetoW_up"] = *VLooseSITTau_eventVetoW_up;
    // lTreeContent["VLooseSITTau_eventVetoW_down"] = *VLooseSITTau_eventVetoW_down;
    // lTreeContent["gen_pt0"] = *Gen_jet_pt;
    // lTreeContent["gen_mjj"] = *Gen_Mjj;
    lTreeContent["VLooseTau_eventVetoW"] = *VLooseTau_eventVetoW;
    lTreeContent["MediumBJet_eventVetoW"] = *MediumBJet_eventVetoW;
    // lTreeContent["MediumBJet_eventVetoW_up"] = *MediumBJet_eventVetoW_up;
    // lTreeContent["MediumBJet_eventVetoW_down"] = *MediumBJet_eventVetoW_down;
    lTreeContent["puWeight"] = *puWeight;
    lTreeContent["LHE_Vpt"] = *LHE_Vpt;
    lTreeContent["LHE_Nb"] = *LHE_Nb;
    lTreeContent["LHE_Njets"] = *LHE_Njets;
    lTreeContent["LHE_Nglu"] = *LHE_Nglu;
    lTreeContent["LHE_Nc"] = *LHE_Nc;
    lTreeContent["LHE_HT"] = *LHE_HT;
    lTreeContent["LHE_Nuds"] = *LHE_Nuds;
    lTreeContent["VetoElectron_eventVetoW"] = *VetoElectron_eventVetoW;
    // lTreeContent["VetoElectron_eventVetoW_systIDISO_up"] = *VetoElectron_eventVetoW_systIDISO_up;
    // lTreeContent["VetoElectron_eventVetoW_systIDISO_down"] = *VetoElectron_eventVetoW_systIDISO_down;
    //lTreeContent["gen_boson_pt"] = *Gen_boson_pt;
    //    lTreeContent["nGenDressedLepton"] = *nGenDressedLepton;
    lTreeContent["VetoElectron_eventSelW"] = *VetoElectron_eventSelW;
    lTreeContent["LooseMuon_eventSelW"] = *LooseMuon_eventSelW;
    //    lTreeContent["GenMET_phi"] = *GenMET_phi;
    lTreeContent["CRTightElectron_eventSelW"] = *CRTightElectron_eventSelW;
    lTreeContent["CRTightMuon_eventSelW"] = *CRTightMuon_eventSelW;
    lTreeContent["Pileup_nPU"] = *Pileup_nPU;
    lTreeContent["fnlo_SF_EWK_corr"] = *fnlo_SF_EWK_corr;
    lTreeContent["fnlo_SF_QCD_corr_QCD_proc_MTR"] = *fnlo_SF_QCD_corr_QCD_proc_MTR;
    lTreeContent["fnlo_SF_QCD_corr_QCD_proc_VTR"] = *fnlo_SF_QCD_corr_QCD_proc_VTR;
    lTreeContent["fnlo_SF_QCD_corr_EWK_proc"] = *fnlo_SF_QCD_corr_EWK_proc;

    if ( year == "2017" )
      lTreeContent["met_filters"] = *met_filters_2017_mc;
    else if  ( year == "2018" )
      lTreeContent["met_filters"] = *met_filters_2018_mc;
  }
  // else{
  //   lTreeContent["decayLeptonId"] = 0;
  //   lTreeContent["CRLooseMuon_eventSelW"] = 1;
  //   lTreeContent["CRVetoElectron_eventSelW"] = 1;
  //   lTreeContent["LooseMuon_eventVetoW_systID_up"] = 1;
  //   lTreeContent["LooseMuon_eventVetoW_systID_down"] = 1;
  //   lTreeContent["LooseMuon_eventVetoW"] = 1;
  //   lTreeContent["xs_weight"] = 1;
  //   lTreeContent["GenMET_pt"] = 0;
  //   lTreeContent["Pileup_nTrueInt"] = -1;
  //   lTreeContent["VLooseSITTau_eventVetoW"] = 1;
  //   lTreeContent["VLooseSITTau_eventVetoW_up"] = 1;
  //   lTreeContent["VLooseSITTau_eventVetoW_down"] = 1;
  //   lTreeContent["gen_pt0"] = -1;
  //   lTreeContent["VLooseTau_eventVetoW"] = 1;
  //   lTreeContent["gen_mjj"] = -1;
  //   lTreeContent["MediumBJet_eventVetoW"] = 1;
  //   lTreeContent["MediumBJet_eventVetoW_up"] = 1;
  //   lTreeContent["MediumBJet_eventVetoW_down"] = 1;
  //   lTreeContent["puWeight"] = 1;
  //   lTreeContent["LHE_Vpt"] = -1;
  //   lTreeContent["LHE_Nb"] = -1;
  //   lTreeContent["LHE_Njets"] = -1;
  //   lTreeContent["LHE_Nglu"] = -1;
  //   lTreeContent["LHE_Nc"] = -1;
  //   lTreeContent["LHE_HT"] = -1;
  //   lTreeContent["LHE_Nuds"] = -1;
  //   lTreeContent["VetoElectron_eventVetoW"] = 1;
  //   lTreeContent["VetoElectron_eventVetoW_systIDISO_up"] = 1;
  //   lTreeContent["VetoElectron_eventVetoW_systIDISO_down"] = 1;
  //   lTreeContent["gen_boson_pt"] = -1;
  //   lTreeContent["nGenDressedLepton"] = -1;
  //   lTreeContent["VetoElectron_eventSelW"] = 1;
  //   lTreeContent["LooseMuon_eventSelW"] = 1;
  //   lTreeContent["GenMET_phi"] = -1;
  //   lTreeContent["CRTightElectron_eventSelW"] = 1;
  //   lTreeContent["Pileup_nPU"] = -1;
  //   lTreeContent["nloSF_"] = 1;
  //   lTreeContent["CRTightMuon_eventSelW"] = 1;
  if ( !isMC ){
    if ( year == "2017" )
      lTreeContent["met_filters"] = *met_filters_2017_data;
    else if  ( year == "2018" )
      lTreeContent["met_filters"] = *met_filters_2018_data;
  }
  lTreeContent["Zmumu_flag"] = *Zmumu_flag;
  lTreeContent["Leading_el_pt"] = *Leading_el_pt;
  lTreeContent["Subleading_el_phi"] = *Subleading_el_phi;
  lTreeContent["HLT_Photon200"] = *HLT_Photon200;

  //  if (  mProc != "GluGluHtoInv" && mProc != "VBFHtoInv" && mProc != "EWKZNUNU" && mProc != "VV" && mProc != "EWKZll" && mProc != "EWKW" && mProc != "ZJETS"){
  if (  mProc == "DATA" || mProc == "QCD" ){
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
  }
  else{
    lTreeContent["HLT_PFJet40"] = 1;
    lTreeContent["HLT_PFJet60"] = 1;
    lTreeContent["HLT_PFJet80"] = 1;
    lTreeContent["HLT_PFJet140"] = 1;
    lTreeContent["HLT_PFJet200"] = 1;
    lTreeContent["HLT_PFJet260"] = 1;
    lTreeContent["HLT_PFJet320"] = 1;
    lTreeContent["HLT_PFJet400"] = 1;
    lTreeContent["HLT_PFJet450"] = 1;
    lTreeContent["HLT_PFJet500"] = 1;
    lTreeContent["HLT_PFJet550"] = 1;
  }

  lTreeContent["Wmunu_flag"] = *Wmunu_flag;
  lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"] = *HLT_PFMETNoMu120_PFMHTNoMu120_IDTight;
  // lTreeContent["nMediumBJet"] = *nMediumBJet;
  // lTreeContent["Leading_el_eta"] = *Leading_el_eta;
  lTreeContent["Zee_flag"] = *Zee_flag;
  lTreeContent["HLT_Ele32_WPTight_Gsf"] = *HLT_Ele32_WPTight_Gsf;
  lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"] = *HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60;
  lTreeContent["Subleading_jet_eta"] = *Subleading_jet_eta;
  lTreeContent["Wenu_flag"] = *Wenu_flag;
  lTreeContent["HLT_TripleJet110_35_35_Mjj650_PFMET110"] = *HLT_TripleJet110_35_35_Mjj650_PFMET110;
  lTreeContent["MET_pt"] = *MET_pt;
  if (misAM){
    lTreeContent["diCleanJet_M"] = *dijet_M;//Temp for AM
    lTreeContent["MetNoLep_CleanJet_mindPhi"] = *JetMetmindPhi; //Temp for AM
    lTreeContent["MetNoLep_pt"] = *MetNoLep;//Temp for AM
  }
  else{
    lTreeContent["diCleanJet_M"] = *diCleanJet_M;
    lTreeContent["MetNoLep_CleanJet_mindPhi"] = *MetNoLep_CleanJet_mindPhi;
    lTreeContent["MetNoLep_pt"] = *MetNoLep_pt;
  }
  lTreeContent["nVLooseTau"] = *nVLooseTau;

  lTreeContent["nVetoElectron"] = *nVetoElectron;
  lTreeContent["Leading_jet_pt"] = *Leading_jet_pt;
  lTreeContent["DiVetoElectron_mass"] = *DiVetoElectron_mass;
  lTreeContent["HLT_DiJet110_35_Mjj650_PFMET110"] = *HLT_DiJet110_35_Mjj650_PFMET110;
  lTreeContent["MET_phi"] = *MET_phi;
  lTreeContent["Leading_jet_phi"] = *Leading_jet_phi;
  lTreeContent["Leading_muon_phi"] = *Leading_muon_phi;
  lTreeContent["Subleading_jet_phi"] = *Subleading_jet_phi;
  lTreeContent["Subleading_el_pt"] = *Subleading_el_pt;
  lTreeContent["Leading_jet_eta"] = *Leading_jet_eta;
  lTreeContent["HLT_IsoMu27"] = *HLT_IsoMu27;
  //  lTreeContent["MetNoLep_phi"] = *MetNoLep_phi;


  lTreeContent["Subleading_muon_phi"] = *Subleading_muon_phi;

  lTreeContent["Subleading_muon_eta"] = *Subleading_muon_eta;
  lTreeContent["Subleading_el_eta"] = *Subleading_el_eta;
  lTreeContent["HLT_Ele35_WPTight_Gsf"] = *HLT_Ele35_WPTight_Gsf;
  lTreeContent["HLT_Ele32_WPTight_Gsf_L1DoubleEG"] = *HLT_Ele32_WPTight_Gsf_L1DoubleEG;
  lTreeContent["PV_npvsGood"] = *PV_npvsGood;
  lTreeContent["Subleading_muon_pt"] = *Subleading_muon_pt;
  lTreeContent["nCleanJet30"] = *nCleanJet30;
  lTreeContent["DiLooseMuon_mass"] = *DiLooseMuon_mass;
  if (misAM){
    lTreeContent["diCleanJet_dEta"] = *dijet_dEta;//Temp for AM
    lTreeContent["diCleanJet_dPhi"] = *dijet_dPhi;//Temp for AM
  }
  else{
    lTreeContent["diCleanJet_dEta"] = *diCleanJet_dEta;
    lTreeContent["diCleanJet_dPhi"] = *diCleanJet_dPhi;
  }
  lTreeContent["Leading_el_phi"] = *Leading_el_phi;
  lTreeContent["Leading_muon_pt"] = *Leading_muon_pt;
  lTreeContent["nLoosePhoton"] = *nLoosePhoton;
  lTreeContent["nLooseMuon"] = *nLooseMuon;
  lTreeContent["Subleading_jet_pt"] = *Subleading_jet_pt;
  lTreeContent["Leading_muon_eta"] = *Leading_muon_eta;



  if ( misAM ){
    lTreeContent["Met"] = *Met;
    lTreeContent["TkMET_pt"] = *TkMET_pt;
    lTreeContent["SoftActivityJetHT10"] = *SoftActivityJetHT10;
    lTreeContent["softActivityJet1_eta"] = *softActivityJet1_eta;
    lTreeContent["softActivityJet1_phi"] = *softActivityJet1_phi;
    lTreeContent["softActivityJet2_eta"] = *softActivityJet2_eta;
    lTreeContent["softActivityJet2_phi"] = *softActivityJet2_phi;
    lTreeContent["softActivityJet3_eta"] = *softActivityJet3_eta;
    lTreeContent["softActivityJet3_phi"] = *softActivityJet3_phi;
    lTreeContent["softActivityJet4_eta"] = *softActivityJet4_eta;
    lTreeContent["softActivityJet4_phi"] = *softActivityJet4_phi;
    lTreeContent["softActivityJet5_eta"] = *softActivityJet5_eta;
    lTreeContent["softActivityJet5_phi"] = *softActivityJet5_phi;
    lTreeContent["softActivityJet6_eta"] = *softActivityJet6_eta;
    lTreeContent["softActivityJet6_phi"] = *softActivityJet6_phi;
    lTreeContent["isoTrack1_eta"] = *isoTrack1_eta;
    lTreeContent["isoTrack1_phi"] = *isoTrack1_phi;
    lTreeContent["isoTrack2_eta"] = *isoTrack2_eta;
    lTreeContent["isoTrack2_phi"] = *isoTrack2_phi;
    lTreeContent["isoTrack3_eta"] = *isoTrack3_eta;
    lTreeContent["isoTrack3_phi"] = *isoTrack3_phi;
    
    lTreeContent["VBF_MTR_QCD_CR_eff_Sel"] = 1; //Temp for AM  
    lTreeContent["VBF_VTR_QCD_CR_eff_Sel"] = 1; //Temp for AM
    lTreeContent["VBF_MTR_QCD_SR_eff_Sel"] = 1; //Temp for AM
    lTreeContent["VBF_VTR_QCD_SR_eff_Sel"] = 1; //Temp for AM
    lTreeContent["VBF_MTR_QCD_A_eff_Sel"] = 1; //Temp for AM 
    lTreeContent["VBF_VTR_QCD_A_eff_Sel"] = 1; //Temp for AM 
    lTreeContent["VBF_MTR_QCD_B_eff_Sel"] = 1; //Temp for AM 
    lTreeContent["VBF_VTR_QCD_B_eff_Sel"] = 1; //Temp for AM 
  }
  else{
    lTreeContent["VBF_MTR_QCD_CR_eff_Sel"] = * VBF_MTR_QCD_CR_eff_Sel;  
    lTreeContent["VBF_VTR_QCD_CR_eff_Sel"] = * VBF_VTR_QCD_CR_eff_Sel;
    lTreeContent["VBF_MTR_QCD_SR_eff_Sel"] = * VBF_MTR_QCD_SR_eff_Sel;
    lTreeContent["VBF_VTR_QCD_SR_eff_Sel"] = * VBF_VTR_QCD_SR_eff_Sel;
    lTreeContent["VBF_MTR_QCD_A_eff_Sel"] = * VBF_MTR_QCD_A_eff_Sel; 
    lTreeContent["VBF_VTR_QCD_A_eff_Sel"] = * VBF_VTR_QCD_A_eff_Sel; 
    lTreeContent["VBF_MTR_QCD_B_eff_Sel"] = * VBF_MTR_QCD_B_eff_Sel; 
    lTreeContent["VBF_VTR_QCD_B_eff_Sel"] = * VBF_VTR_QCD_B_eff_Sel; 
  }




  
}


Bool_t Events::BaseSelection(){
  
  bool pass=kTRUE;
  std::string catStr = GetCatStr(mCat);


  if (catStr.find("MTR")!=catStr.npos){
    //Needed if we don't have the flags //Temp AM

    if ( misAM ){
      pass = lTreeContent["nCleanJet30"]>=2
	&& (lTreeContent["Leading_jet_pt"] > 80)
	&& (lTreeContent["Subleading_jet_pt"] > 40)
	&& (lTreeContent["diCleanJet_M"]>200)
	&& (lTreeContent["diCleanJet_dEta"]>1)     
	&& (lTreeContent["diCleanJet_dPhi"]<1.5)
	&& (lTreeContent["nLoosePhoton"]==0)
	&& (lTreeContent["met_filters"]==1);
	//	&& (lTreeContent["met_filters"]==0);
      ////////////////////////
    } 
       

	 //       std::cout << (lTreeContent["met_filters"]) << std::endl;
    if ( misMC ){      
      pass = pass 
	&& (lTreeContent["xs_weight"] > 0);
    }
      
  }
  else if (catStr.find("VTR")!=catStr.npos){
    pass = kTRUE;

    if ( misMC ){      
      pass = pass 
	&& (lTreeContent["xs_weight"] > 0);
    }

  }
  
  // if (mCat==CatType::MTRvetoTau){
  //   pass = pass && lTreeContent["nVLooseTau"]==0;
  // }
  // else if (mCat==CatType::MTRvetoB){
  //   pass = pass && lTreeContent["nMediumBJet"]==0;
  // }
  // else if (mCat==CatType::MTRveto){
  //   pass = pass && lTreeContent["nVLooseTau"]==0 && lTreeContent["nMediumBJet"]==0;
  // }
  
  if ( mProc == "QCD" ){
    pass = pass && lTreeContent["LHE_HT"]>300;
    if (mYear == "2018"){
      pass = pass && lTreeContent["LHE_HT"]>300;
    }
  }


  return pass;

}

Double_t Events::BaseWeight(){
  std::string catStr = GetCatStr(mCat);
  double w = (lTreeContent["puWeight"])*(lTreeContent["xs_weight"])*mLumiPb*(lTreeContent["L1PreFiringWeight_Nom"]);

  double tauveto = lTreeContent["VLooseSITTau_eventVetoW"];
  double bjetveto = lTreeContent["MediumBJet_eventVetoW"];
  //  double bjetveto = 1;
  double electronveto = lTreeContent["VetoElectron_eventVetoW"];
  double muonveto = lTreeContent["LooseMuon_eventVetoW"];





  // if ( tauveto < 0 ) tauveto = 0;
  // if ( bjetveto < 0 ) bjetveto =0;
  // if ( electronveto < 0 ) electronveto = 0;
  // if ( muonveto < 0 ) muonveto = 0;
  // if ( tauveto > 0.9 && bjetveto > 0.9 && electronveto > 0.9 && muonveto > 0.9){
    
  // }

  // if ( mSyst == "TauVetoUp"){
  //   tauveto = lTreeContent["VLooseSITTau_eventVetoW_up"];
  // }
  // else if ( mSyst == "TauVetoDown"){
  //   tauveto = lTreeContent["VLooseSITTau_eventVetoW_down"];
  // }
  // else if ( mSyst == "BjetVetoUp"){
  //   bjetveto = lTreeContent["MediumBJet_eventVetoW_up"];
  // }
  // else if ( mSyst == "BjetVetoDown"){
  //   bjetveto = lTreeContent["MediumBJet_eventVetoW_down"];
  // }


  w *= tauveto*bjetveto*electronveto*muonveto;

 

  if ( !misMC ) w = 1.0;//Data

  return w;

}

Bool_t Events::PassSelection(){

  bool pass = kTRUE;
  
  std::string lreg = "";

  bool lCleanCut0 = true;
  bool lCleanCut1 = true;
  bool lCleanCut2 = true;
  bool lCleanCut3 = true;

  if ( misAM ){
    bool lCleanCut0 = !(std::abs(lTreeContent["Met"] - lTreeContent["TkMET_pt"])/lTreeContent["Met"] >= 0.8 && ((std::abs(lTreeContent["Leading_jet_eta"])>=2.8 && std::abs(lTreeContent["Leading_jet_eta"]) <= 3.2) || (std::abs(lTreeContent["Subleading_jet_eta"])>=2.8 && std::abs(lTreeContent["Subleading_jet_eta"]) <= 3.2)));
    bool lCleanCut1 = !(lTreeContent["SoftActivityJetHT10"]<100 && ((std::abs(lTreeContent["Leading_jet_eta"])>=2.8 && std::abs(lTreeContent["Leading_jet_eta"]) <= 3.2) || (std::abs(lTreeContent["Subleading_jet_eta"])>=2.8 && std::abs(lTreeContent["Subleading_jet_eta"]) <= 3.2)));
    bool lCleanCut2 = !(((lTreeContent["softActivityJet1_eta"] > -3 && lTreeContent["softActivityJet1_eta"] < -1.2 && lTreeContent["softActivityJet1_phi"] < -0.5 && lTreeContent["softActivityJet1_phi"] > - 1.9) || (lTreeContent["softActivityJet2_eta"] > -3 && lTreeContent["softActivityJet2_eta"] < -1.2 && lTreeContent["softActivityJet2_phi"] < -0.5 && lTreeContent["softActivityJet2_phi"] > - 1.9) || (lTreeContent["softActivityJet3_eta"] > -3 && lTreeContent["softActivityJet3_eta"] < -1.2 && lTreeContent["softActivityJet3_phi"] < -0.5 && lTreeContent["softActivityJet3_phi"] > - 1.9) || (lTreeContent["softActivityJet4_eta"] > -3 && lTreeContent["softActivityJet4_eta"] < -1.2 && lTreeContent["softActivityJet4_phi"] < -0.5 && lTreeContent["softActivityJet4_phi"] > - 1.9) || (lTreeContent["softActivityJet5_eta"] > -3 && lTreeContent["softActivityJet5_eta"] < -1.2 && lTreeContent["softActivityJet5_phi"] < -0.5 && lTreeContent["softActivityJet5_phi"] > - 1.9) || (lTreeContent["softActivityJet6_eta"] > -3 && lTreeContent["softActivityJet6_eta"] < -1.2 && lTreeContent["softActivityJet6_phi"] < -0.5 && lTreeContent["softActivityJet6_phi"] > - 1.9)) && lTreeContent["MetPhiNoLep"]>-1.6 && lTreeContent["MetPhiNoLep"]<-0.8);
    bool lCleanCut3 = !(((lTreeContent["isoTrack1_eta"] > -3 && lTreeContent["isoTrack1_eta"] < -1.2 && lTreeContent["isoTrack1_phi"] < -0.5 && lTreeContent["isoTrack1_phi"] > - 1.9) || (lTreeContent["isoTrack2_eta"] > -3 && lTreeContent["isoTrack2_eta"] < -1.2 && lTreeContent["isoTrack2_phi"] < -0.5 && lTreeContent["isoTrack2_phi"] > - 1.9) || (lTreeContent["isoTrack3_eta"] > -3 && lTreeContent["isoTrack3_eta"] < -1.2 && lTreeContent["isoTrack3_phi"] < -0.5 && lTreeContent["isoTrack3_phi"] > - 1.9)) && lTreeContent["MetPhiNoLep"]>-1.6 && lTreeContent["MetPhiNoLep"]<-0.8);
  }  
  pass = pass && lCleanCut0;
  pass = pass && lCleanCut1;
  
  if (mYear == "2018"){
    pass = pass && lCleanCut2;
    pass = pass && lCleanCut3;
  }

  if (mCat==CatType::MTR){
    lreg = "MTR";

    if (mReg==RegionType::SR || (mReg==RegionType::QCDCR) ){
      //    if ( mProc != "DATA")//JetHT
      pass = pass && (static_cast<int>(lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"])==1 || static_cast<int>(lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"])==1);
    }
    else{
      //      if ( mProc == "DATA" || mProc == "QCD" ){
	pass = pass && (
			// static_cast<int>(lTreeContent["HLT_PFJet40"])==1 
			// || static_cast<int>(lTreeContent["HLT_PFJet60"])==1
			// || static_cast<int>(lTreeContent["HLT_PFJet80"])==1
			// || static_cast<int>(lTreeContent["HLT_PFJet140"])==1
			static_cast<int>(lTreeContent["HLT_PFJet200"])==1
			|| static_cast<int>(lTreeContent["HLT_PFJet260"])==1
			|| static_cast<int>(lTreeContent["HLT_PFJet320"])==1
			|| static_cast<int>(lTreeContent["HLT_PFJet400"])==1
			|| static_cast<int>(lTreeContent["HLT_PFJet450"])==1
			|| static_cast<int>(lTreeContent["HLT_PFJet500"])==1
			|| static_cast<int>(lTreeContent["HLT_PFJet550"])==1
			);

	pass = pass && (lTreeContent["Leading_jet_pt"] > 200);
	//   }
    }
  }
  else if  (mCat==CatType::VTR){
    lreg = "VTR";
    if (mReg==RegionType::SR || (mReg==RegionType::QCDCR) ){
      //    if ( mProc != "DATA")//JetHT
      pass = pass && (static_cast<int>(lTreeContent["HLT_DiJet110_35_Mjj650_PFMET110"])==1 || static_cast<int>(lTreeContent["HLT_TripleJet110_35_35_Mjj650_PFMET110"])==1);
    }
    else{
      // if ( mProc == "DATA" || mProc == "QCD" ){
	pass = pass && (
			// static_cast<int>(lTreeContent["HLT_PFJet40"])==1 
			// || static_cast<int>(lTreeContent["HLT_PFJet60"])==1
			// || static_cast<int>(lTreeContent["HLT_PFJet80"])==1
			// || static_cast<int>(lTreeContent["HLT_PFJet140"])==1
			static_cast<int>(lTreeContent["HLT_PFJet200"])==1
			|| static_cast<int>(lTreeContent["HLT_PFJet260"])==1
			|| static_cast<int>(lTreeContent["HLT_PFJet320"])==1
			|| static_cast<int>(lTreeContent["HLT_PFJet400"])==1
			|| static_cast<int>(lTreeContent["HLT_PFJet450"])==1
			|| static_cast<int>(lTreeContent["HLT_PFJet500"])==1
			|| static_cast<int>(lTreeContent["HLT_PFJet550"])==1
			);

	pass = pass && (lTreeContent["Leading_jet_pt"] > 200);
      }
    //    }
  }

  if (mReg==RegionType::SR){
    pass = pass && (static_cast<int>(lTreeContent["VBF_"+lreg+"_QCD_SR_eff_Sel"]));


    //Temp if no flag
    if ( misAM ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;
      pass = pass && lTreeContent["MetNoLep_pt"]>250;
    }


    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseSITTau"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }

  }
  // else if (mReg==RegionType::Wmu ){
  //   pass = pass && lTreeContent["MetNoLep_pt"]>250;
  //   pass = pass && lTreeContent["Wmunu_flag"]==1 && (lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"]==1 || lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"] == 1);
  //   pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;
  // }
  // else if (mReg==RegionType::We){
  //   pass = pass && lTreeContent["MetNoLep_pt"]>250;
  //   pass = pass && lTreeContent["Wenu_flag"]==1 && lTreeContent["HLT_Ele35_WPTight_Gsf"]==1 && lTreeContent["Leading_el_pt"]>40;
  //   pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;
  // }
  // else if (mReg==RegionType::Zmumu){
  //   pass = pass && lTreeContent["MetNoLep_pt"]>250;
  //   pass = pass && lTreeContent["Zmumu_flag"]==1 && (lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"]==1 || lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"] == 1);
  //   pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;
  // }
  // else if (mReg==RegionType::Zee){
  //   pass = pass && lTreeContent["MetNoLep_pt"]>250;
  //   pass = pass && lTreeContent["Zee_flag"]==1 && lTreeContent["HLT_Ele35_WPTight_Gsf"]==1 && lTreeContent["Leading_el_pt"]>40;
  //   pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;
  // }
  else if (mReg==RegionType::QCDCR){
    //    std::cout << "PASS1 = " << pass << std::endl;
    pass = pass && (static_cast<int>(lTreeContent["VBF_"+lreg+"_QCD_CR_eff_Sel"]));


    // std::cout << "P1 " << static_cast<int>(lTreeContent["VBF_"+lreg+"_QCD_CR_eff_Sel"]) << " - ";
    // std::cout << lTreeContent["VBF_VTR_QCD_CR_eff_Sel"] << " - " << lTreeContent["VBF_MTR_QCD_CR_eff_Sel"] <<std::endl;
    //    std::cout << "PASS2 = " << pass << std::endl;
      
    //pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.3;
    //    if (mCat==CatType::MTR){
      //      std::cout << "PASS = " << pass << std::endl;
    //    }


    if ( misAM ){
    // //Temp if no flag
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]<0.5;
      pass = pass && lTreeContent["MetNoLep_pt"]>250;
    /////
    }
    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseSITTau"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }

    if (mCat==CatType::MTR){
      //  std::cout << "PASS = " << pass << std::endl;
    }

    //    if (mCat==CatType::MTRvetoLep || mCat==CatType::MTRveto) pass = pass && lTreeContent["nLooseMuon"]==0 && lTreeContent["nVetoElectron"]==0;
  }
  else if (mReg==RegionType::QCDA){
    pass = pass && (static_cast<int>(lTreeContent["VBF_"+lreg+"_QCD_A_eff_Sel"]));
    // pass = pass && (lTreeContent["MetNoLep_pt"]>100);
    // pass = pass && (lTreeContent["MetNoLep_pt"]<160);
    //    pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]<0.5;

    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseSITTau"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }

    //    if (mCat==CatType::MTRvetoLep || mCat==CatType::MTRveto) pass = pass && lTreeContent["nLooseMuon"]==0 && lTreeContent["nVetoElectron"]==0;
  }
  else if (mReg==RegionType::QCDB){
    pass = pass && (static_cast<int>(lTreeContent["VBF_"+lreg+"_QCD_B_eff_Sel"]));
    //    pass = pass && lTreeContent["MetNoLep_pt"]>100;
    //    pass = pass && lTreeContent["MetNoLep_pt"]<160;
    //    pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;

    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseSITTau"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }

    //    if (mCat==CatType::MTRvetoLep || mCat==CatType::MTRveto) pass = pass && lTreeContent["nLooseMuon"]==0 && lTreeContent["nVetoElectron"]==0;
  }


  return pass;
}

Double_t Events::SelWeight(){
  double w = 1.;

  if ( mCat == CatType::MTR ){

    if (mReg==RegionType::SR || (mReg==RegionType::QCDCR) ){
      if ( misMC ) w *= lTreeContent["trigger_weight_METMHT"];
    }
    else{
      
      if ( mProc == "DATA" ){

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
    if ( misMC ) w *= lTreeContent["fnlo_SF_QCD_corr_QCD_proc_MTR"];

  }
  else if ( mCat == CatType::VTR ){

    if (mReg==RegionType::SR || (mReg==RegionType::QCDCR) ){
      if ( misMC ) w *= lTreeContent["trigger_weight_VBF"];
    }
    if ( misMC ) w *= lTreeContent["fnlo_SF_QCD_corr_QCD_proc_VTR"];

  }

  if ( misMC ){
    w *= lTreeContent["fnlo_SF_QCD_corr_EWK_proc"];
    w *= lTreeContent["fnlo_SF_EWK_corr"];
  }


  /*  
  if (mReg==RegionType::SR || mReg==RegionType::QCDCR ||  mReg==RegionType::QCDA ||  mReg==RegionType::QCDB){
    if (mCat!=CatType::MTRvetoLep && mCat!=CatType::MTRveto){
     
      double electronveto = lTreeContent["VetoElectron_eventVetoW"];
      double muonveto = lTreeContent["LooseMuon_eventVetoW"];

      // if ( electronveto > 0.9 &&  muonveto > 0.9){
      // 	electronveto = 0;
      // 	muonveto = 0;
      // }

      if ( mSyst == "ElectronVetoUp" ){
	electronveto = lTreeContent["VetoElectron_eventVetoW_systIDISO_up"];
      }
      else if ( mSyst == "ElectronVetoDown" ){
	electronveto = lTreeContent["VetoElectron_eventVetoW_systIDISO_down"];
      }
      else if ( mSyst == "MuonVetoUp" ){
	muonveto =     lTreeContent["LooseMuon_eventVetoW_systID_up"];
      }
      else if ( mSyst == "MuonVetoDown" ){
	muonveto =     lTreeContent["LooseMuon_eventVetoW_systID_down"];
      }
      w*=electronveto*muonveto;
    }
    //    if (mCat!=CatType::MTRvetoLep && mCat!=CatType::MTRveto) w *= (lTreeContent["LooseMuon_eventVetoW"]);
  }
  */
  // else if (mReg==RegionType::Wmu){
  //   w *= (lTreeContent["CRTightMuon_eventSelW"]);
  // }
  // else if (mReg==RegionType::We){
  //   w *= (lTreeContent["CRTightElectron_eventSelW"]);
  // }
  // else if (mReg==RegionType::Zmumu){
  //   w *= (lTreeContent["CRLooseMuon_eventSelW"])*(lTreeContent["CRTightMuon_eventSelW"]);
  // }
  // else if (mReg==RegionType::Zee){
  //   w *= (lTreeContent["CRVetoElectron_eventSelW"])*(lTreeContent["CRTightElectron_eventSelW"]);
  // }

  //  if ( !misMC ) w = 1.0;//Data

//  std::cout << w << std::endl;
  return w;
}

Bool_t Events::Process(Long64_t entry)
{

   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.


  fReader.SetLocalEntry(entry);
  SetTreeContent(mYear);
   if (entry%10000==0) std::cout << " -- processing entry " << entry/1000. << "k" << std::endl;
   //   if ( entry > 100 ) Abort("maxevents");
   //std::cout << " -- DEBUG: entry " << entry
   //	     << " METnoLep = " <<  lTreeContent["MetNoLep_pt"]
   //	     << std::endl;

   //   std::cout << "new event" << std::endl;   
   for (unsigned iC(CatType::MTR); iC!=CatType::LastCat; ++iC){//loop on cat
     SetCategory(static_cast<CatType>(iC));
     if (!BaseSelection()){
       continue;
     }
     //     std::cout << " -- event passes selection." << std::endl;     
     for (unsigned iR(RegionType::SR); iR!=RegionType::Last; ++iR){//loop on region
       SetRegion(static_cast<RegionType>(iR));

       if (!PassSelection()){
	 continue;
       }
       for (unsigned iV(0); iV<mHistVec[iR][iC].size(); ++iV){
	 //	 std::cout << BaseWeight() << " - " << SelWeight() << std::endl;
	 double weight = BaseWeight()*SelWeight();

	 if ( weight > 10 && lTreeContent["diCleanJet_M"] > 3000)
	   //	   std::cout << "w = " << weight << " - " << lTreeContent["diCleanJet_M"]<<std::endl;
	   weight = 1;



	 // if ( iR == 0 && iC == 0 ) {
	 //   if (weight > 0.5 || weight < 0) weight = 0;
	 //   //	   if (weight < 0) weight = 0;
	 // }
	 //	 std::cout << "FILL" << std::endl;
	 mHistVec[iR][iC][iV]->Fill(lTreeContent[mVarVec[iV]],weight);


	 //	 if ( mVarVec[iV] == "MetNoLep_pt" ) 	 std::cout << iR << " - " << iC << " - " << iV << " - " << mHistVec[iR][iC][iV]->Integral() << std::endl;
	 //	 mHistVec[iR][iC][iV]->Fill(lTreeContent[mVarVec[iV]],BaseWeight());
	 // mHistVec[iR][iC][iV]->Fill(lTreeContent[mVarVec[iV]],1);
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
