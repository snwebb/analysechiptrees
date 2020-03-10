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
    
  const unsigned nVars = 18;
  std::string lVar[nVars] = {"MetNoLep","diCleanJet_M","diCleanJet_dEta","diCleanJet_dPhi","Leading_jet_pt","Subleading_jet_pt","Leading_jet_eta","Subleading_jet_eta","nCleanJet30","MetNoLep_CleanJet_mindPhi","LHE_Vpt","LHE_HT","decayLeptonId","LHE_Nuds","LHE_Nb","LHE_Nc","Pileup_nPU","diCleanJet_M"};
  for (unsigned iV(0); iV<nVars; ++iV){//loop on variables
    mVarVec.push_back(lVar[iV]);
  }
  
  std::string lVarName[nVars] = {"E_{T}^{miss,no #mu} (GeV)","M_{jj} (GeV)","#Delta#eta_{jj}","#Delta#Phi_{jj}","p_{T}^{jet1} (GeV)","p_{T}^{jet2} (GeV)","#eta^{jet1}","#eta^{jet2}","nCleanJet30 (p_{T}>30 GeV)","min#Delta#Phi(jets,E_{T}^{miss,no #mu})","LHE V p_{T} (GeV)","LHE HT (GeV)","decayLeptonId","LHE_Nuds","LHE_Nb","LHE_Nc","N_PU","M_{jj} (GeV)"};

  //  const int nBins[nVars] = {350,380,120,50,84,52,100,100,10,180,60,60,7,10,10,10,10};
  const int nBins[nVars] = {500,380,120,50,84,52,100,100,10,180,60,60,7,10,10,10,40,6};


  //const double binsX[nBins+1] = {200,400,600,900,1200,1500,2000,2750,3500,5000};
  //const double binsX[nBins+1] = {0,50,100,150,200,250,300,400,500,800};
  const double binMin[nVars-1] = {100,200,1,0,80,40,-5,-5,2,0.5,0,0,0,0,0,0,0};
  const double binMax[nVars-1] = {600,4000,7,1.5,500,300,5,5,12,3.1416,600,2000,7,10,10,10,80};
  //  double mjjbins[11] = {0,  200, 400, 600, 900, 1200, 1500, 2000, 2750, 3500, 5000};

  double mjjbins[7] = {200,500,800,1250,2000,2800,5000};
  
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
	if ( iV < nVars-1 ){
	     hTmp = new TH1F(label.str().c_str(),(";"+lVarName[iV]).c_str(),nBins[iV],binMin[iV],binMax[iV]);
	}
	else if (iV == nVars-1){
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
  std::string reg[8] = {"SR","We","Wmu","Zee","Zmumu","QCDCR","QCDA","QCDB"};
  if (aRegion==RegionType::Last) return "";
  return reg[static_cast<unsigned>(aRegion)];
}

std::string Events::GetCatStr(const CatType & aCat){
  std::string cat[5] = {"MTR","MTRvetoTau","MTRvetoB","MTRvetoLep","MTRvetoAll"};
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
  SetMC(isMC);

  if ( year == "2017"){
    lTreeContent["L1PreFiringWeight_Nom"] = *L1PreFiringWeight_Nom;
  }
  else if  ( year == "2018"){
    lTreeContent["L1PreFiringWeight_Nom"] = 1;
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
    lTreeContent["gen_pt0"] = *Gen_jet_pt;
    lTreeContent["gen_mjj"] = *Gen_Mjj;
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
    lTreeContent["gen_boson_pt"] = *Gen_boson_pt;
    lTreeContent["nGenDressedLepton"] = *nGenDressedLepton;
    lTreeContent["VetoElectron_eventSelW"] = *VetoElectron_eventSelW;
    lTreeContent["LooseMuon_eventSelW"] = *LooseMuon_eventSelW;
    lTreeContent["GenMET_phi"] = *GenMET_phi;
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
  //   if ( year == "2017" )
  //     lTreeContent["met_filters"] = *met_filters_2017_data;
  //   else if  ( year == "2018" )
  //     lTreeContent["met_filters"] = *met_filters_2018_data;




  lTreeContent["Zmumu_flag"] = *Zmumu_flag;
  lTreeContent["Leading_el_pt"] = *Leading_el_pt;
  lTreeContent["Subleading_el_phi"] = *Subleading_el_phi;
  lTreeContent["HLT_Photon200"] = *HLT_Photon200;
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
  lTreeContent["diCleanJet_M"] = *diCleanJet_M;
  lTreeContent["nVLooseTau"] = *nVLooseTau;
  lTreeContent["MetNoLep_CleanJet_mindPhi"] = *MetNoLep_CleanJet_mindPhi;
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
  lTreeContent["MetNoLep_pt"] = *MetNoLep_pt;
  lTreeContent["diCleanJet_dPhi"] = *diCleanJet_dPhi;
  lTreeContent["Subleading_muon_phi"] = *Subleading_muon_phi;
  lTreeContent["Subleading_muon_eta"] = *Subleading_muon_eta;
  lTreeContent["Subleading_el_eta"] = *Subleading_el_eta;
  lTreeContent["HLT_Ele35_WPTight_Gsf"] = *HLT_Ele35_WPTight_Gsf;
  lTreeContent["HLT_Ele32_WPTight_Gsf_L1DoubleEG"] = *HLT_Ele32_WPTight_Gsf_L1DoubleEG;
  lTreeContent["PV_npvsGood"] = *PV_npvsGood;
  lTreeContent["Subleading_muon_pt"] = *Subleading_muon_pt;
  lTreeContent["MetNoLep_CleanJet_mindPhi"] = *MetNoLep_CleanJet_mindPhi;
  lTreeContent["nCleanJet30"] = *nCleanJet30;
  lTreeContent["DiLooseMuon_mass"] = *DiLooseMuon_mass;
  lTreeContent["diCleanJet_dEta"] = *diCleanJet_dEta;
  lTreeContent["Leading_el_phi"] = *Leading_el_phi;
  lTreeContent["Leading_muon_pt"] = *Leading_muon_pt;
  lTreeContent["nLoosePhoton"] = *nLoosePhoton;
  lTreeContent["nLooseMuon"] = *nLooseMuon;
  lTreeContent["Subleading_jet_pt"] = *Subleading_jet_pt;
  lTreeContent["Leading_muon_eta"] = *Leading_muon_eta;
}


Bool_t Events::BaseSelection(){
  
  bool pass=kTRUE;
  std::string catStr = GetCatStr(mCat);
  if (catStr.find("MTR")!=catStr.npos){
    pass = lTreeContent["nCleanJet30"]>=2
      && (lTreeContent["Leading_jet_pt"] > 80)
      && (lTreeContent["Subleading_jet_pt"] > 40)
      && (lTreeContent["diCleanJet_M"]>200)
      && (lTreeContent["diCleanJet_dEta"]>1)     
      && (lTreeContent["diCleanJet_dPhi"]<1.5)
      && (lTreeContent["nLoosePhoton"]==0)
      && (lTreeContent["met_filters"]==0);
    if ( misMC ){      
      pass = pass 
	&& (lTreeContent["xs_weight"] > 0);
    }
      
  }
  else if (catStr.find("VTR")!=catStr.npos){
    pass = kTRUE;
  }
  
  if (mCat==CatType::MTRvetoTau){
    pass = pass && lTreeContent["nVLooseTau"]==0;
  }
  else if (mCat==CatType::MTRvetoB){
    pass = pass && lTreeContent["nMediumBJet"]==0;
  }
  else if (mCat==CatType::MTRveto){
    pass = pass && lTreeContent["nVLooseTau"]==0 && lTreeContent["nMediumBJet"]==0;
  }
  
  if ( mProc == "QCD" ){
    //    pass = pass && lTreeContent["LHE_HT"]>1000;
  }


  return pass;
  
}

Double_t Events::BaseWeight(){
  std::string catStr = GetCatStr(mCat);
  double w = (lTreeContent["puWeight"])*(lTreeContent["xs_weight"])*mLumiPb*(lTreeContent["L1PreFiringWeight_Nom"]);

  double tauveto = lTreeContent["VLooseSITTau_eventVetoW"];
  double bjetveto = lTreeContent["MediumBJet_eventVetoW"];
  double electronveto = lTreeContent["VetoElectron_eventVetoW"];
  double muonveto = lTreeContent["LooseMuon_eventVetoW"];

  // if ( tauveto > 0.9 && bjetveto > 0.9 && electronveto > 0.9 && muonveto > 0.9){
    
  // }

  if ( mSyst == "TauVetoUp"){
    tauveto = lTreeContent["VLooseSITTau_eventVetoW_up"];
  }
  else if ( mSyst == "TauVetoDown"){
    tauveto = lTreeContent["VLooseSITTau_eventVetoW_down"];
  }
  else if ( mSyst == "BjetVetoUp"){
    bjetveto = lTreeContent["MediumBJet_eventVetoW_up"];
  }
  else if ( mSyst == "BjetVetoDown"){
    bjetveto = lTreeContent["MediumBJet_eventVetoW_down"];
  }

  if (catStr.find("veto")==catStr.npos){
    w *= tauveto*bjetveto;
  }
  else if (mCat==CatType::MTRvetoTau) w *= (lTreeContent["MediumBJet_eventVetoW"]);
  else if (mCat==CatType::MTRvetoB) w *= (lTreeContent["VLooseSITTau_eventVetoW"]);
  else if (mCat==CatType::MTRvetoLep) w *= (lTreeContent["VLooseSITTau_eventVetoW"])*(lTreeContent["MediumBJet_eventVetoW"]);
  //  w =(lTreeContent["puWeight"])*(lTreeContent["xs_weight"])*mLumiPb;

  if ( !misMC ) w = 1.0;//Data

  return w;

}

Bool_t Events::PassSelection(){

  bool pass = kTRUE;
  if (mReg==RegionType::SR){
    pass = pass && (lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"]==1 || lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"]==1);
    pass = pass && lTreeContent["MetNoLep"]>250;
    pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;

    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseSITTau"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }

    if (mCat==CatType::MTRvetoLep || mCat==CatType::MTRveto) pass = pass && lTreeContent["nLooseMuon"]==0 && lTreeContent["nVetoElectron"]==0;
  }
  else if (mReg==RegionType::Wmu ){
    pass = pass && lTreeContent["MetNoLep"]>250;
    pass = pass && lTreeContent["Wmunu_flag"]==1 && (lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"]==1 || lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"] == 1);
    pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;
  }
  else if (mReg==RegionType::We){
    pass = pass && lTreeContent["MetNoLep"]>250;
    pass = pass && lTreeContent["Wenu_flag"]==1 && lTreeContent["HLT_Ele35_WPTight_Gsf"]==1 && lTreeContent["Leading_el_pt"]>40;
    pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;
  }
  else if (mReg==RegionType::Zmumu){
    pass = pass && lTreeContent["MetNoLep"]>250;
    pass = pass && lTreeContent["Zmumu_flag"]==1 && (lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"]==1 || lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"] == 1);
    pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;
  }
  else if (mReg==RegionType::Zee){
    pass = pass && lTreeContent["MetNoLep"]>250;
    pass = pass && lTreeContent["Zee_flag"]==1 && lTreeContent["HLT_Ele35_WPTight_Gsf"]==1 && lTreeContent["Leading_el_pt"]>40;
    pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;
  }
  else if (mReg==RegionType::QCDCR){
    pass = pass && lTreeContent["MetNoLep"]>250;
    pass = pass && (lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"]==1 || lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"]==1);
    pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]<0.5;

    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseSITTau"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }


    if (mCat==CatType::MTRvetoLep || mCat==CatType::MTRveto) pass = pass && lTreeContent["nLooseMuon"]==0 && lTreeContent["nVetoElectron"]==0;
  }
  else if (mReg==RegionType::QCDA){
    pass = pass && (lTreeContent["MetNoLep"]>100);
    pass = pass && (lTreeContent["MetNoLep"]<160);
    pass = pass && (lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"]==1 || lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"]==1);
    pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]<0.5;

    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseSITTau"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }

    if (mCat==CatType::MTRvetoLep || mCat==CatType::MTRveto) pass = pass && lTreeContent["nLooseMuon"]==0 && lTreeContent["nVetoElectron"]==0;
  }
  else if (mReg==RegionType::QCDB){
    pass = pass && lTreeContent["MetNoLep"]>100;
    pass = pass && lTreeContent["MetNoLep"]<160;

    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseSITTau"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }

    pass = pass && (lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"]==1 || lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"]==1);
    pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;
    if (mCat==CatType::MTRvetoLep || mCat==CatType::MTRveto) pass = pass && lTreeContent["nLooseMuon"]==0 && lTreeContent["nVetoElectron"]==0;
  }


  return pass;
}

Double_t Events::SelWeight(){
  double w = 1.;
  
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
  else if (mReg==RegionType::Wmu){
    w *= (lTreeContent["CRTightMuon_eventSelW"]);
  }
  else if (mReg==RegionType::We){
    w *= (lTreeContent["CRTightElectron_eventSelW"]);
  }
  else if (mReg==RegionType::Zmumu){
    w *= (lTreeContent["CRLooseMuon_eventSelW"])*(lTreeContent["CRTightMuon_eventSelW"]);
  }
  else if (mReg==RegionType::Zee){
    w *= (lTreeContent["CRVetoElectron_eventSelW"])*(lTreeContent["CRTightElectron_eventSelW"]);
  }

  if ( !misMC ) w = 1.0;//Data
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

   if (entry%100000==0) std::cout << " -- processing entry " << entry/1000. << "k" << std::endl;
   if ( entry > 100 ) Abort("maxevents");
     std::cout << __LINE__ << std::endl;
   //std::cout << " -- DEBUG: entry " << entry
   //	     << " METnoLep = " <<  lTreeContent["MetNoLep"]
   //	     << std::endl;
   
   for (unsigned iC(CatType::MTR); iC!=CatType::LastCat; ++iC){//loop on cat
     SetCategory(static_cast<CatType>(iC));
     if (!BaseSelection()){
       continue;
     }
       std::cout << __LINE__ << std::endl;
     //     std::cout << " -- event passes selection." << std::endl;
     
     for (unsigned iR(RegionType::SR); iR!=RegionType::Last; ++iR){//loop on region
       SetRegion(static_cast<RegionType>(iR));
       if (!PassSelection()){
	 continue;
       }
  std::cout << __LINE__ << std::endl;
       for (unsigned iV(0); iV<mHistVec[iR][iC].size(); ++iV){

	 double weight = BaseWeight()*SelWeight();
	 // if ( iR == 0 && iC == 0 ) {
	 //   if (weight > 0.5 || weight < 0) weight = 0;
	 //   //	   if (weight < 0) weight = 0;
	 // }
  std::cout << __LINE__ << std::endl;
	 mHistVec[iR][iC][iV]->Fill(lTreeContent[mVarVec[iV]],weight);


	 //	 if ( mVarVec[iV] == "MetNoLep" ) 	 std::cout << iR << " - " << iC << " - " << iV << " - " << mHistVec[iR][iC][iV]->Integral() << std::endl;
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
