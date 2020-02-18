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

// Events::Initialise(){

//    Zmumu_flag = {fReader, "Zmumu_flag"};
//    decayLeptonId = {fReader, "decayLeptonId"};
//    L1PreFiringWeight_Nom; = {fReader, "L1PreFiringWeight_Nom"};
//    CRLooseMuon_eventSelW = {fReader, "CRLooseMuon_eventSelW"};
//    CRVetoElectron_eventSelW = {fReader, "CRVetoElectron_eventSelW"};
//    Leading_el_pt = {fReader, "Leading_el_pt"};
//     Subleading_el_phi = {fReader, "Subleading_el_phi"};
//     LooseMuon_eventVetoW = {fReader, "LooseMuon_eventVetoW"};
//     xs_weight = {fReader, "xs_weight"};
//     HLT_Photon200 = {fReader, "HLT_Photon200"};
//     Wmunu_flag = {fReader, "Wmunu_flag"};
//     HLT_PFMETNoMu120_PFMHTNoMu120_IDTight = {fReader, "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"};
//     nMediumBJet = {fReader, "nMediumBJet"};
//     Leading_el_eta = {fReader, "Leading_el_eta"};
//     GenMET_pt = {fReader, "GenMET_pt"};
//     Zee_flag = {fReader, "Zee_flag"};
//     HLT_Ele32_WPTight_Gsf = {fReader, "HLT_Ele32_WPTight_Gsf"};
//     met_filters_2017_mc = {fReader, "met_filters_2017_mc"};
//     HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60 = {fReader, "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"};
//     Subleading_jet_eta = {fReader, "Subleading_jet_eta"};
//     Wenu_flag = {fReader, "Wenu_flag"};
//     Pileup_nTrueInt = {fReader, "Pileup_nTrueInt"};
//     HLT_TripleJet110_35_35_Mjj650_PFMET110 = {fReader, "HLT_TripleJet110_35_35_Mjj650_PFMET110"};
//     Met = {fReader, "Met"};
//     dijet_M = {fReader, "dijet_M"};
//     nVLooseTau = {fReader, "nVLooseTau"};
//     Pileup_nPU = {fReader, "Pileup_nPU"};
//     gen_pt0 = {fReader, "gen_pt0"};
//     MetPhiNoLep = {fReader, "MetPhiNoLep"};
//     VLooseSITTau_eventVetoW = {fReader, "VLooseSITTau_eventVetoW"};
//     VLooseTau_eventVetoW = {fReader, "VLooseTau_eventVetoW"};
//     nVetoElectron = {fReader, "nVetoElectron"};
//     Leading_jet_pt = {fReader, "Leading_jet_pt"};
//     DiVetoElectron_mass = {fReader, "DiVetoElectron_mass"};
//     CRTightElectron_eventSelW = {fReader, "CRTightElectron_eventSelW"};
//     HLT_DiJet110_35_Mjj650_PFMET110 = {fReader, "HLT_DiJet110_35_Mjj650_PFMET110"};
//     gen_mjj = {fReader, "gen_mjj"};
//     MetPhi = {fReader, "MetPhi"};
//     MediumBJet_eventVetoW = {fReader, "MediumBJet_eventVetoW"};
//     LHE_Nuds = {fReader, "LHE_Nuds"};
//     Leading_jet_phi = {fReader, "Leading_jet_phi"};
//     LHE_HT = {fReader, "LHE_HT"};
//     nloSF_ = {fReader, "nloSF_"};
//     Leading_muon_phi = {fReader, "Leading_muon_phi"};
//     Subleading_jet_phi = {fReader, "Subleading_jet_phi"};
//     Subleading_el_pt = {fReader, "Subleading_el_pt"};
//     Leading_jet_eta = {fReader, "Leading_jet_eta"};
//     met_filters_2017_data = {fReader, "met_filters_2017_data"};
//     HLT_IsoMu27 = {fReader, "HLT_IsoMu27"};
//     CRTightMuon_eventSelW = {fReader, "CRTightMuon_eventSelW"};
//     puWeight = {fReader, "puWeight"};
//     MetNoLep = {fReader, "MetNoLep"};
//     LHE_Vpt = {fReader, "LHE_Vpt"};
//     dijet_dPhi = {fReader, "dijet_dPhi"};
//     Subleading_muon_phi = {fReader, "Subleading_muon_phi"};
//     Subleading_muon_eta = {fReader, "Subleading_muon_eta"};
//     Subleading_el_eta = {fReader, "Subleading_el_eta"};
//     VetoElectron_eventVetoW = {fReader, "VetoElectron_eventVetoW"};
//     HLT_Ele35_WPTight_Gsf = {fReader, "HLT_Ele35_WPTight_Gsf"};
//     HLT_Ele32_WPTight_Gsf_L1DoubleEG = {fReader, "HLT_Ele32_WPTight_Gsf_L1DoubleEG"};
//     PV_npvsGood = {fReader, "PV_npvsGood"};
//     Subleading_muon_pt = {fReader, "Subleading_muon_pt"};
//     JetMetmindPhi = {fReader, "JetMetmindPhi"};
//     isData = {fReader, "isData"};
//     nJets = {fReader, "nJets"};
//     DiLooseMuon_mass = {fReader, "DiLooseMuon_mass"};
//     LHE_Nb = {fReader, "LHE_Nb"};
//     dijet_dEta = {fReader, "dijet_dEta"};
//     Leading_el_phi = {fReader, "Leading_el_phi"};
//     gen_boson_pt = {fReader, "gen_boson_pt"};
//     nGenDressedLepton = {fReader, "nGenDressedLepton"};
//     Leading_muon_pt = {fReader, "Leading_muon_pt"};
//     nLoosePhoton = {fReader, "nLoosePhoton"};
//     nLooseMuon = {fReader, "nLooseMuon"};
//     nCleanJet30 = {fReader, "nCleanJet30"};
//     Subleading_jet_pt = {fReader, "Subleading_jet_pt"};
//     LHE_Njets = {fReader, "LHE_Njets"};
//     LHE_Nglu = {fReader, "LHE_Nglu"};
//     Leading_muon_eta = {fReader, "Leading_muon_eta"};
//     VetoElectron_eventSelW = {fReader, "VetoElectron_eventSelW"};
//     LooseMuon_eventSelW = {fReader, "LooseMuon_eventSelW"};
//     GenMET_phi = {fReader, "GenMET_phi"};
//     LHE_Nc = {fReader, "LHE_Nc"};

//     LooseMuon_eventVetoW_systID_up = {fReader, "LooseMuon_eventVetoW_systID_up"};
//     VetoElectron_eventVetoW_systIDISO_up = {fReader, "VetoElectron_eventVetoW_systIDISO_up"};
//     LooseMuon_eventVetoW_systID_down = {fReader, "LooseMuon_eventVetoW_systID_down"};
//     VetoElectron_eventVetoW_systIDISO_down = {fReader, "VetoElectron_eventVetoW_systIDISO_down"};
//     VLooseSITTau_eventVetoW_up = {fReader, "VLooseSITTau_eventVetoW_up"};
//     VLooseSITTau_eventVetoW_down = {fReader, "VLooseSITTau_eventVetoW_down"};
//     MediumBJet_eventVetoW_up = {fReader, "MediumBJet_eventVetoW_up"};
//     MediumBJet_eventVetoW_down = {fReader, "MediumBJet_eventVetoW_down"};
// }

void Events::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  TString option = GetOption();
  mFout = TFile::Open(mOutFile.c_str(),"RECREATE");
  

  //std::cout << " Number of branches in tree: " << varName.size() << std::endl;

  
  const unsigned nVars = 18;
  std::string lVar[nVars] = {"MetNoLep","dijet_M","dijet_dEta","dijet_dPhi","Leading_jet_pt","Subleading_jet_pt","Leading_jet_eta","Subleading_jet_eta","nCleanJet30","JetMetmindPhi","LHE_Vpt","LHE_HT","decayLeptonId","LHE_Nuds","LHE_Nb","LHE_Nc","Pileup_nPU","dijet_M"};
  for (unsigned iV(0); iV<nVars; ++iV){//loop on variables
    mVarVec.push_back(lVar[iV]);
  }
  
  std::string lVarName[nVars] = {"E_{T}^{miss,no #mu} (GeV)","M_{jj} (GeV)","#Delta#eta_{jj}","#Delta#Phi_{jj}","p_{T}^{jet1} (GeV)","p_{T}^{jet2} (GeV)","#eta^{jet1}","#eta^{jet2}","nJets (p_{T}>30 GeV)","min#Delta#Phi(jets,E_{T}^{miss,no #mu})","LHE V p_{T} (GeV)","LHE HT (GeV)","decayLeptonId","LHE_Nuds","LHE_Nb","LHE_Nc","N_PU","M_{jj} (GeV)"};

  //  const int nBins[nVars] = {350,380,120,50,84,52,100,100,10,180,60,60,7,10,10,10,10};
  const int nBins[nVars] = {350,380,120,50,84,52,100,100,10,180,60,60,7,10,10,10,40,6};


  //const double binsX[nBins+1] = {200,400,600,900,1200,1500,2000,2750,3500,5000};
  //const double binsX[nBins+1] = {0,50,100,150,200,250,300,400,500,800};
  const double binMin[nVars-1] = {250,200,1,0,80,40,-5,-5,2,0.5,0,0,0,0,0,0,0};
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
  //std::cout << " -- INFO: category set to: " << GetCatStr(mCat)
  //<< std::endl;
  
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
  //std::cout << " -- INFO: region set to: " << GetRegionStr(aRegion) << std::endl;
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


void Events::SetTreeContent(bool isMC,std::string year){

  // while (  fReader.Next() )
  //   { 

  //   }
  
  //  fChain->SetBranchStatus("L1PreFiringWeight_Nom",0);
  if ( year == "2017"){
    lTreeContent["L1PreFiringWeight_Nom"] = *L1PreFiringWeight_Nom;
  }
  else if  ( year == "2018"){
    lTreeContent["L1PreFiringWeight_Nom"] = 1;
  }
    //  if ( isMC ){
    lTreeContent["decayLeptonId"] = *decayLeptonId;
    //  }
  // else{
  //   lTreeContent["decayLeptonId"] = 1;
  // }
  lTreeContent["Zmumu_flag"] = *Zmumu_flag;
  lTreeContent["CRLooseMuon_eventSelW"] = *CRLooseMuon_eventSelW;

  lTreeContent["CRVetoElectron_eventSelW"] = *CRVetoElectron_eventSelW;
  lTreeContent["Leading_el_pt"] = *Leading_el_pt;
  lTreeContent["Subleading_el_phi"] = *Subleading_el_phi;
  lTreeContent["LooseMuon_eventVetoW"] = *LooseMuon_eventVetoW;
  // lTreeContent["LooseMuon_eventVetoW_systID_up"] = *LooseMuon_eventVetoW_systID_up;
  // lTreeContent["LooseMuon_eventVetoW_systID_down"] = *LooseMuon_eventVetoW_systID_down;

  lTreeContent["xs_weight"] = *xs_weight;
  lTreeContent["HLT_Photon200"] = *HLT_Photon200;
  lTreeContent["Wmunu_flag"] = *Wmunu_flag;
  lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"] = *HLT_PFMETNoMu120_PFMHTNoMu120_IDTight;
  lTreeContent["nMediumBJet"] = *nMediumBJet;
  lTreeContent["Leading_el_eta"] = *Leading_el_eta;
  lTreeContent["GenMET_pt"] = *GenMET_pt;
  lTreeContent["Zee_flag"] = *Zee_flag;
  lTreeContent["HLT_Ele32_WPTight_Gsf"] = *HLT_Ele32_WPTight_Gsf;
  lTreeContent["met_filters_2017_mc"] = *met_filters_2017_mc;
  lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"] = *HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60;
  lTreeContent["Subleading_jet_eta"] = *Subleading_jet_eta;
  lTreeContent["Wenu_flag"] = *Wenu_flag;
  lTreeContent["Pileup_nTrueInt"] = *Pileup_nTrueInt;
  lTreeContent["HLT_TripleJet110_35_35_Mjj650_PFMET110"] = *HLT_TripleJet110_35_35_Mjj650_PFMET110;
  lTreeContent["Met"] = *Met;
  lTreeContent["dijet_M"] = *dijet_M;
  lTreeContent["nVLooseTau"] = *nVLooseTau;
  lTreeContent["Pileup_nPU"] = *Pileup_nPU;
  lTreeContent["gen_pt0"] = *gen_pt0;
  lTreeContent["MetPhiNoLep"] = *MetPhiNoLep;
  lTreeContent["VLooseSITTau_eventVetoW"] = *VLooseSITTau_eventVetoW;
  // lTreeContent["VLooseSITTau_eventVetoW_up"] = *VLooseSITTau_eventVetoW_up;
  // lTreeContent["VLooseSITTau_eventVetoW_down"] = *VLooseSITTau_eventVetoW_down;
  lTreeContent["VLooseTau_eventVetoW"] = *VLooseTau_eventVetoW;
  lTreeContent["nVetoElectron"] = *nVetoElectron;
  lTreeContent["Leading_jet_pt"] = *Leading_jet_pt;
  lTreeContent["DiVetoElectron_mass"] = *DiVetoElectron_mass;
  lTreeContent["CRTightElectron_eventSelW"] = *CRTightElectron_eventSelW;
  lTreeContent["HLT_DiJet110_35_Mjj650_PFMET110"] = *HLT_DiJet110_35_Mjj650_PFMET110;
  lTreeContent["gen_mjj"] = *gen_mjj;
  lTreeContent["MetPhi"] = *MetPhi;
  lTreeContent["MediumBJet_eventVetoW"] = *MediumBJet_eventVetoW;
  // lTreeContent["MediumBJet_eventVetoW_up"] = *MediumBJet_eventVetoW_up;
  // lTreeContent["MediumBJet_eventVetoW_down"] = *MediumBJet_eventVetoW_down;
  lTreeContent["LHE_Nuds"] = *LHE_Nuds;
  lTreeContent["Leading_jet_phi"] = *Leading_jet_phi;
  lTreeContent["LHE_HT"] = *LHE_HT;

lTreeContent["nloSF_"] = *nloSF_;
lTreeContent["Leading_muon_phi"] = *Leading_muon_phi;
lTreeContent["Subleading_jet_phi"] = *Subleading_jet_phi;
lTreeContent["Subleading_el_pt"] = *Subleading_el_pt;
lTreeContent["Leading_jet_eta"] = *Leading_jet_eta;
lTreeContent["met_filters_2017_data"] = *met_filters_2017_data;
lTreeContent["HLT_IsoMu27"] = *HLT_IsoMu27;
lTreeContent["CRTightMuon_eventSelW"] = *CRTightMuon_eventSelW;
lTreeContent["puWeight"] = *puWeight;
lTreeContent["MetNoLep"] = *MetNoLep;
lTreeContent["LHE_Vpt"] = *LHE_Vpt;
lTreeContent["dijet_dPhi"] = *dijet_dPhi;
lTreeContent["Subleading_muon_phi"] = *Subleading_muon_phi;
lTreeContent["Subleading_muon_eta"] = *Subleading_muon_eta;
lTreeContent["Subleading_el_eta"] = *Subleading_el_eta;
lTreeContent["VetoElectron_eventVetoW"] = *VetoElectron_eventVetoW;
// lTreeContent["VetoElectron_eventVetoW_systIDISO_up"] = *VetoElectron_eventVetoW_systIDISO_up;
// lTreeContent["VetoElectron_eventVetoW_systIDISO_down"] = *VetoElectron_eventVetoW_systIDISO_down;
lTreeContent["HLT_Ele35_WPTight_Gsf"] = *HLT_Ele35_WPTight_Gsf;
lTreeContent["HLT_Ele32_WPTight_Gsf_L1DoubleEG"] = *HLT_Ele32_WPTight_Gsf_L1DoubleEG;
lTreeContent["PV_npvsGood"] = *PV_npvsGood;
lTreeContent["Subleading_muon_pt"] = *Subleading_muon_pt;
lTreeContent["JetMetmindPhi"] = *JetMetmindPhi;
lTreeContent["isData"] = *isData;
lTreeContent["nJets"] = *nJets;
lTreeContent["DiLooseMuon_mass"] = *DiLooseMuon_mass;
lTreeContent["LHE_Nb"] = *LHE_Nb;
lTreeContent["dijet_dEta"] = *dijet_dEta;
lTreeContent["Leading_el_phi"] = *Leading_el_phi;
lTreeContent["gen_boson_pt"] = *gen_boson_pt;
lTreeContent["nGenDressedLepton"] = *nGenDressedLepton;
lTreeContent["Leading_muon_pt"] = *Leading_muon_pt;
lTreeContent["nLoosePhoton"] = *nLoosePhoton;
lTreeContent["nLooseMuon"] = *nLooseMuon;
lTreeContent["nCleanJet30"] = *nCleanJet30;
lTreeContent["Subleading_jet_pt"] = *Subleading_jet_pt;
lTreeContent["LHE_Njets"] = *LHE_Njets;
lTreeContent["LHE_Nglu"] = *LHE_Nglu;
lTreeContent["Leading_muon_eta"] = *Leading_muon_eta;
lTreeContent["VetoElectron_eventSelW"] = *VetoElectron_eventSelW;
lTreeContent["LooseMuon_eventSelW"] = *LooseMuon_eventSelW;
lTreeContent["GenMET_phi"] = *GenMET_phi;
lTreeContent["LHE_Nc"] = *LHE_Nc;
}


Bool_t Events::BaseSelection(){
  
  bool pass=kTRUE;
  std::string catStr = GetCatStr(mCat);
  if (catStr.find("MTR")!=catStr.npos){
    pass = lTreeContent["nCleanJet30"]>=2 && lTreeContent["Leading_jet_pt"] > 80 && lTreeContent["Subleading_jet_pt"] > 40 && lTreeContent["dijet_M"]>200 && lTreeContent["dijet_dEta"]>1 && lTreeContent["dijet_dPhi"]<1.5 && lTreeContent["xs_weight"]>0 && lTreeContent["nLoosePhoton"]==0 && lTreeContent["met_filters_2017_mc"]==1;
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

  //  std::cout << lTreeContent["xs_weight"] <<std::endl;

  double tauveto = lTreeContent["VLooseSITTau_eventVetoW"];
  double bjetveto = lTreeContent["MediumBJet_eventVetoW"];
  double electronveto = lTreeContent["VetoElectron_eventVetoW"];
  double muonveto = lTreeContent["LooseMuon_eventVetoW"];

  // if ( tauveto > 0.9 && bjetveto > 0.9){
  //   tauveto = 0;
  //   bjetveto = 0;
  // }

  if ( tauveto > 0.9 && bjetveto > 0.9 && electronveto > 0.9 && muonveto > 0.9){

    //    w = 0;
    
  }
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
  return w;


}

Bool_t Events::PassSelection(){

  bool pass = kTRUE;
  if (mReg==RegionType::SR){
    pass = (lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"]==1 || lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"]==1);
    pass = pass && lTreeContent["MetNoLep"]>250;
    pass = pass && lTreeContent["JetMetmindPhi"]>0.5;
    if (mCat==CatType::MTRvetoLep || mCat==CatType::MTRveto) pass = pass && lTreeContent["nLooseMuon"]==0 && lTreeContent["nVetoElectron"]==0;
  }
  else if (mReg==RegionType::Wmu ){
    pass = pass && lTreeContent["MetNoLep"]>250;
    pass = pass && lTreeContent["Wmunu_flag"]==1 && (lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"]==1 || lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"] == 1);
    pass = pass && lTreeContent["JetMetmindPhi"]>0.5;
  }
  else if (mReg==RegionType::We){
    pass = pass && lTreeContent["MetNoLep"]>250;
    pass = pass && lTreeContent["Wenu_flag"]==1 && lTreeContent["HLT_Ele35_WPTight_Gsf"]==1 && lTreeContent["Leading_el_pt"]>40;
    pass = pass && lTreeContent["JetMetmindPhi"]>0.5;
  }
  else if (mReg==RegionType::Zmumu){
    pass = pass && lTreeContent["MetNoLep"]>250;
    pass = pass && lTreeContent["Zmumu_flag"]==1 && (lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"]==1 || lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"] == 1);
    pass = pass && lTreeContent["JetMetmindPhi"]>0.5;
  }
  else if (mReg==RegionType::Zee){
    pass = pass && lTreeContent["MetNoLep"]>250;
    pass = pass && lTreeContent["Zee_flag"]==1 && lTreeContent["HLT_Ele35_WPTight_Gsf"]==1 && lTreeContent["Leading_el_pt"]>40;
    pass = pass && lTreeContent["JetMetmindPhi"]>0.5;
  }
  else if (mReg==RegionType::QCDCR){
    pass = pass && lTreeContent["MetNoLep"]>250;
    pass = (lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"]==1 || lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"]==1);
    pass = pass && lTreeContent["JetMetmindPhi"]<0.5;
    if (mCat==CatType::MTRvetoLep || mCat==CatType::MTRveto) pass = pass && lTreeContent["nLooseMuon"]==0 && lTreeContent["nVetoElectron"]==0;
  }
  else if (mReg==RegionType::QCDA){
    pass = pass && lTreeContent["MetNoLep"]>100;
    pass = pass && lTreeContent["MetNoLep"]<160;
    pass = (lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"]==1 || lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"]==1);
    pass = pass && lTreeContent["JetMetmindPhi"]<0.5;
    if (mCat==CatType::MTRvetoLep || mCat==CatType::MTRveto) pass = pass && lTreeContent["nLooseMuon"]==0 && lTreeContent["nVetoElectron"]==0;
  }
  else if (mReg==RegionType::QCDB){
    pass = pass && lTreeContent["MetNoLep"]>100;
    pass = pass && lTreeContent["MetNoLep"]<160;
    pass = (lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"]==1 || lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"]==1);
    pass = pass && lTreeContent["JetMetmindPhi"]>0.5;
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


   SetTreeContent(misMC,mYear);

   if (entry%100000==0) std::cout << " -- processing entry " << entry/1000. << "k" << std::endl;
   
   //std::cout << " -- DEBUG: entry " << entry
   //	     << " METnoLep = " <<  lTreeContent["MetNoLep"]
   //	     << std::endl;
   
   for (unsigned iC(CatType::MTR); iC!=CatType::LastCat; ++iC){//loop on cat
     SetCategory(static_cast<CatType>(iC));
     if (!BaseSelection()){
       continue;
     }
     
     //std::cout << " -- event passes selection." << std::endl;
     
     for (unsigned iR(RegionType::SR); iR!=RegionType::Last; ++iR){//loop on region
       SetRegion(static_cast<RegionType>(iR));
       if (!PassSelection()){
	 continue;
       }
       for (unsigned iV(0); iV<mHistVec[iR][iC].size(); ++iV){

	 double weight = BaseWeight()*SelWeight();
	 // if ( iR == 0 && iC == 0 ) {
	 //   if (weight > 0.5 || weight < 0) weight = 0;
	 //   //	   if (weight < 0) weight = 0;
	 // }

	 mHistVec[iR][iC][iV]->Fill(lTreeContent[mVarVec[iV]],weight);
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
