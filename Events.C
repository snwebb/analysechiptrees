#ifndef Events_cxx
#define Events_cxx

#include "Events.h"
#include <TH2.h>
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
  //  Initialise();
}

Events::~Events(){
  mVarVec.clear();
  mVarVec2D.clear();
  lTreeContent.clear();
  for (unsigned iR(RegionType::SR); iR!=RegionType::Last; ++iR){//loop on region
    for (unsigned iC(CatType::MTR); iC!=CatType::LastCat; ++iC){//loop on region
      mHistVec[iR][iC].clear();
      mHistVec2D[iR][iC].clear();
    }
  }
}

void Events::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).

  fileMTR.open ("MTR.txt");
  
  TString option = GetOption();
  mFout = TFile::Open(mOutFile.c_str(),"RECREATE");
   
  const unsigned nVars = 28;
  //  const unsigned nVars = 2;
  // std::string lVar[nVars] = {"MetNoLep_pt","diCleanJet_M","diCleanJet_dEta","diCleanJet_dPhi","Leading_jet_pt","Subleading_jet_pt","Leading_jet_eta","Subleading_jet_eta","nCleanJet30","MetNoLep_CleanJet_mindPhi","LHE_Vpt","LHE_HT","decayLeptonId","LHE_Nuds","LHE_Nb","LHE_Nc","Pileup_nPU","MetNoLep_phi","diCleanJet_M"};
  std::string lVar[nVars] = {"MetNoLep_pt","diCleanJet_M","lMjj","diCleanJet_dEta","lMjj_dijet_deta","diCleanJet_dPhi","lMjj_dijet_dphi","Leading_jet_pt","Subleading_jet_pt","Leading_jet_eta","Subleading_jet_eta","CentralEtaMTR","ForwardEtaMTR","CentralEtaVTR","ForwardEtaVTR","nCleanJet30","MetNoLep_CleanJet_mindPhi","LHE_Vpt","LHE_HT","decayLeptonId","LHE_Nuds","LHE_Nb","LHE_Nc","Pileup_nPU","dijet_met_balance","lMjj","diCleanJet_M","diCleanJet_M"};

  // std::string lVar[nVars] = {"MetNoLep_CleanJet_mindPhi","diCleanJet_M"};
  for (unsigned iV(0); iV<nVars; ++iV){//loop on variables
    mVarVec.push_back(lVar[iV]);
  }

  
  //   std::string lVarName[nVars] = {"E_{T}^{miss,no #mu} (GeV)","M_{jj} (GeV)","#Delta#eta_{jj}","#Delta#Phi_{jj}","p_{T}^{jet1} (GeV)","p_{T}^{jet2} (GeV)","#eta^{jet1}","#eta^{jet2}","nCleanJet30 (p_{T}>30 GeV)","min#Delta#Phi(jets,E_{T}^{miss,no #mu})","LHE V p_{T} (GeV)","LHE HT (GeV)","decayLeptonId","LHE_Nuds","LHE_Nb","LHE_Nc","N_PU","#Phi E_{T}^{miss,no #mu}","M_{jj} (GeV)"};
  std::string lVarName[nVars] = {"E_{T}^{miss,no #mu} (GeV)","M_{jj} (GeV)","M_{jj} (GeV)","#Delta#eta_{jj}","#Delta#eta_{jj}","#Delta#Phi_{jj}","#Delta#Phi_{jj}","p_{T}^{jet1} (GeV)","p_{T}^{jet2} (GeV)","#eta^{jet1}","#eta^{jet2}","#eta^{central jet}","#eta^{forward jet}","#eta^{central jet}","#eta^{forward jet}","nCleanJet30 (p_{T}>30 GeV)","min#Delta#Phi(jets,E_{T}^{miss,no #mu})","LHE V p_{T} (GeV)","LHE HT (GeV)","decayLeptonId","LHE_Nuds","LHE_Nb","LHE_Nc","N_PU","p_{T}^{jj}-E_{T}^{miss}/E_{T}^{miss}","M_{jj} (GeV)","M_{jj} (GeV)","M_{jj} (GeV)"};
  //  std::string lVarName[nVars] = {"min#Delta#Phi(jets,E_{T}^{miss,no #mu})","M_{jj} (GeV)"};

  const int nBins[nVars] = {350,380,380,120,120,50,50,84,52,50,50,50,50,50,50,10,160,60,60,7,10,10,10,10,42,10,10,6};
  //  const int nBins[nVars] = {500,380,120,50,84,52,100,100,10,180,60,60,7,10,10,10,40,6,180};

  const double binMin[nVars-3] = {100,200 ,200 ,1,1,0  ,0  ,80 ,40 ,-5,-5,-5,-5,-5,-5 ,2  ,0   ,0  ,0   ,0 ,0 ,0 ,0  ,0 ,-1};
  const double binMax[nVars-3] = {600,4000,4000,7,7,1.5,1.5,500,300,5 ,5 ,5 ,5 ,5 ,5  ,12 ,3.2 ,600,2000,7 ,10,10,10 ,80,3.2};

  double mjjbins[11] = {0,  200, 400, 600, 900, 1200, 1500, 2000, 2750, 3500, 5000};
  double mjjbins_reduced[7] = {200,500,800,1250,2000,2800,5000};

  std::vector<std::pair<std::string,std::string> > lVar2D = {std::make_pair("MetNoLep_CleanJet_mindPhi","MetNoLep_pt"),
							     std::make_pair("MetNoLep_CleanJet_mindPhi","diCleanJet_dPhi"),
							     std::make_pair("MetNoLep_CleanJet_mindPhi","lMjj_dijet_dphi"),
							     std::make_pair("MetNoLep_CleanJet_mindPhi","diCleanJet_dEta"),
							     std::make_pair("MetNoLep_CleanJet_mindPhi","Leading_jet_pt"),
							     std::make_pair("MetNoLep_CleanJet_mindPhi","Subleading_jet_pt"),
							     std::make_pair("MetNoLep_CleanJet_mindPhi","dijet_met_balance"),
							     std::make_pair("MetNoLep_CleanJet_mindPhi","Pileup_nPU"),
							     std::make_pair("diCleanJet_dPhi","dijet_met_balance"),
							     std::make_pair("diCleanJet_dEta","diCleanJet_M"),
							     std::make_pair("lMjj_dijet_deta","lMjj"),
							     std::make_pair("diCleanJet_dEta","LHE_HT"),
							     std::make_pair("lMjj_dijet_deta","LHE_HT"),
							     std::make_pair("MetNoLep_CleanJet_mindPhi","diCleanJet_M"),
							     std::make_pair("MetNoLep_CleanJet_mindPhi","lMjj")};


  std::vector<std::pair<int,int> > nBins2D = {std::make_pair(32,500),
					      std::make_pair(32,50),
					      std::make_pair(32,50),
					      std::make_pair(32,60),
					      std::make_pair(32,50),
					      std::make_pair(32,50),
					      std::make_pair(32,42),
					      std::make_pair(32,20),
					      std::make_pair(32,42),
					      std::make_pair(50,20),
					      std::make_pair(50,20),
					      std::make_pair(50,50),
					      std::make_pair(50,50)};
							     
  std::vector<std::pair<double,double> > binMin2D = {std::make_pair(0,100),
						     std::make_pair(0,0),
						     std::make_pair(0,0),
						     std::make_pair(0,1),
						     std::make_pair(0,0),
						     std::make_pair(0,0),
						     std::make_pair(0,-1),
						     std::make_pair(0,0),
						     std::make_pair(0,-1),
						     std::make_pair(0,0),
						     std::make_pair(0,0),
						     std::make_pair(0,0),
						     std::make_pair(0,0)};

  std::vector<std::pair<double,double> > binMax2D = {std::make_pair(3.2,600),
						     std::make_pair(3.2,2.5),
						     std::make_pair(3.2,2.5),
						     std::make_pair(3.2,7),
						     std::make_pair(3.2,500),
						     std::make_pair(3.2,500),
						     std::make_pair(3.2,3.2),
						     std::make_pair(3.2,80),
						     std::make_pair(2.5,3.2),
						     std::make_pair(8,2000),
						     std::make_pair(8,2000),
						     std::make_pair(8,2000),
						     std::make_pair(8,2000)};

  for (unsigned iV(0); iV<lVar2D.size(); ++iV){//loop on variables
    mVarVec2D.push_back(lVar2D[iV]);
  }
  
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
	if ( iV < nVars-3 ){
	  hTmp = new TH1F(label.str().c_str(),(";"+lVarName[iV]).c_str(),nBins[iV],binMin[iV],binMax[iV]);
	}
	else if (iV == nVars-1){
	  hTmp = new TH1F((label.str() + "_binned_reduced").c_str(),(";"+lVarName[iV]).c_str(),nBins[iV],mjjbins_reduced);
	}
	else if (iV == nVars-2){
	  hTmp = new TH1F((label.str() + "_binned").c_str(),(";"+lVarName[iV]).c_str(),nBins[iV],mjjbins);
	}
	else if (iV == nVars-3){
	  hTmp = new TH1F((label.str() + "_binned").c_str(),(";"+lVarName[iV]).c_str(),nBins[iV],mjjbins);
	}
	hTmp->Sumw2();
	mHistVec[iR][iC].push_back(hTmp);
      }

      for (unsigned iV2D = 0;iV2D<lVar2D.size();iV2D++){
	std::ostringstream label;
	label << "h_" << lreg << "_" << lcat << "_" << lVar2D[iV2D].first << "_" << lVar2D[iV2D].second;
	TH2D *hTmp2D = 0;
	if ( iV2D < lVar2D.size()-2 ){
	  hTmp2D = new TH2D(label.str().c_str(),(";"+lVar2D[iV2D].first+";"+lVar2D[iV2D].second).c_str(),nBins2D[iV2D].first,binMin2D[iV2D].first,binMax2D[iV2D].first,nBins2D[iV2D].second,binMin2D[iV2D].second,binMax2D[iV2D].second);
	}
	else {
	  hTmp2D = new TH2D(label.str().c_str(),(";"+lVar2D[iV2D].first+";"+lVar2D[iV2D].second).c_str(),32,0,3.2,10,mjjbins);
	}
	hTmp2D->Sumw2();
	mHistVec2D[iR][iC].push_back(hTmp2D);
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
  std::string reg[4] = {"SR","QCDCR","QCDA","QCDB"};
  if (aRegion==RegionType::Last) return "";
  return reg[static_cast<unsigned>(aRegion)];
}

std::string Events::GetCatStr(const CatType & aCat){
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

  bool isMC = !static_cast<int>(*isData + 0.5);
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
    lTreeContent["LooseMuon_eventVetoW"] = *LooseMuon_eventVetoW;
    lTreeContent["xs_weight"] = *xs_weight;
    lTreeContent["GenMET_pt"] = *GenMET_pt;
    lTreeContent["Pileup_nTrueInt"] = *Pileup_nTrueInt;
    lTreeContent["VLooseSITTau_eventVetoW"] = *VLooseSITTau_eventVetoW;
    lTreeContent["VLooseTauFix_eventVetoW"] = *VLooseTauFix_eventVetoW;
    lTreeContent["VLooseTau_eventVetoW"] = *VLooseTau_eventVetoW;
    lTreeContent["MediumBJet_eventVetoW"] = *MediumBJet_eventVetoW;
    lTreeContent["puWeight"] = *puWeight;
    lTreeContent["LHE_Vpt"] = *LHE_Vpt;
    lTreeContent["LHE_Nb"] = *LHE_Nb;
    lTreeContent["LHE_Njets"] = *LHE_Njets;
    lTreeContent["LHE_Nglu"] = *LHE_Nglu;
    lTreeContent["LHE_Nc"] = *LHE_Nc;
    lTreeContent["LHE_HT"] = *LHE_HT;
    lTreeContent["LHE_Nuds"] = *LHE_Nuds;
    lTreeContent["VetoElectron_eventVetoW"] = *VetoElectron_eventVetoW;
    lTreeContent["VetoElectron_eventSelW"] = *VetoElectron_eventSelW;
    lTreeContent["LooseMuon_eventSelW"] = *LooseMuon_eventSelW;
    lTreeContent["CRTightElectron_eventSelW"] = *CRTightElectron_eventSelW;
    lTreeContent["CRTightMuon_eventSelW"] = *CRTightMuon_eventSelW;
    lTreeContent["Pileup_nPU"] = *Pileup_nPU;
    lTreeContent["fnlo_SF_EWK_corr"] = *fnlo_SF_EWK_corr;
    lTreeContent["fnlo_SF_QCD_corr_QCD_proc_MTR"] = *fnlo_SF_QCD_corr_QCD_proc_MTR;
    lTreeContent["fnlo_SF_QCD_corr_QCD_proc_VTR"] = *fnlo_SF_QCD_corr_QCD_proc_VTR;
    lTreeContent["fnlo_SF_QCD_corr_EWK_proc"] = *fnlo_SF_QCD_corr_EWK_proc;
    if ( year == "2017" ){
      lTreeContent["met_filters"] = *met_filters_2017_mc;
      lTreeContent["hem_weight"] = 1;
    }
    else if  ( year == "2018" ){
      lTreeContent["met_filters"] = *met_filters_2018_mc;
      lTreeContent["hem_weight"] = *hem_weight;
    }
  }

  if ( !isMC ){
    lTreeContent["hem_weight"] = 1;
    if ( year == "2017" )
      lTreeContent["met_filters"] = *met_filters_2017_data;
    else if  ( year == "2018" )
      lTreeContent["met_filters"] = *met_filters_2018_data;
  }

  lTreeContent["event"] = *event;
  lTreeContent["Zmumu_flag"] = *Zmumu_flag;
  lTreeContent["Leading_el_pt"] = *Leading_el_pt;
  lTreeContent["Subleading_el_phi"] = *Subleading_el_phi;
  lTreeContent["HLT_Photon200"] = *HLT_Photon200;
  if ( !misAM ){
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
  lTreeContent["Zee_flag"] = *Zee_flag;
  lTreeContent["HLT_Ele32_WPTight_Gsf"] = *HLT_Ele32_WPTight_Gsf;
  lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"] = *HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60;
  lTreeContent["Subleading_jet_eta"] = *Subleading_jet_eta;
  lTreeContent["lMjj_jet2_eta"] = *lMjj_jet2_eta;
  lTreeContent["Wenu_flag"] = *Wenu_flag;
  lTreeContent["HLT_TripleJet110_35_35_Mjj650_PFMET110"] = *HLT_TripleJet110_35_35_Mjj650_PFMET110;
  lTreeContent["MET_pt"] = *MET_pt;

  if (misAM){
    lTreeContent["diCleanJet_M"] = *dijet_M;
    lTreeContent["MetNoLep_CleanJet_mindPhi"] = *JetMetmindPhi; 
    lTreeContent["MetNoLep_pt"] = *MetNoLep;
    lTreeContent["CaloMET_pt"] = *CaloMET_pt;
    lTreeContent["Leading_jet_chHEF"] = *Leading_jet_chHEF;
    lTreeContent["Leading_jet_neHEF"] = *Leading_jet_neHEF;
    lTreeContent["Subleading_jet_chHEF"] = *Subleading_jet_chHEF;
    lTreeContent["Subleading_jet_neHEF"] = *Subleading_jet_neHEF;
    lTreeContent["jet_chf_nhf_cut"] = 1;
    lTreeContent["jet_chf_nhf_vtr_cut"] = 1;
    lTreeContent["lMjj_dijet_dphi"] = 1;
    lTreeContent["horn_sel"] = 0;
    lTreeContent["horn_sel_vtr"] = 0;

  }
  else{
    lTreeContent["diCleanJet_M"] = *diCleanJet_M;
    lTreeContent["lMjj"] = *lMjj;

    lTreeContent["MetNoLep_CleanJet_mindPhi"] = *MetNoLep_CleanJet_mindPhi;
    lTreeContent["MetNoLep_pt"] = *MetNoLep_pt;
    lTreeContent["jet_chf_nhf_cut"] = *jet_chf_nhf_cut;
    lTreeContent["jet_chf_nhf_vtr_cut"] = *jet_chf_nhf_vtr_cut;
    lTreeContent["lMjj_dijet_dphi"] = *lMjj_dijet_dphi;
    lTreeContent["lMjj_dijet_deta"] = *lMjj_dijet_deta;
    lTreeContent["horn_sel"] = *horn_sel;
    lTreeContent["horn_sel_vtr"] = *horn_sel_vtr;
    lTreeContent["VecBDPhiCutMTR"] = *VecBDPhiCutMTR;
    lTreeContent["VecBDPhiCutVTR"] = *VecBDPhiCutVTR;
  }

  lTreeContent["nVLooseTau"] = *nVLooseTau;
  //  if ( mProc == "MET" )
  lTreeContent["nVLooseTauFix"] = *nVLooseTauFix;
  // else
  //   lTreeContent["nVLooseTauFix"] = *nVLooseTau;
  lTreeContent["nMediumBJet"] = *nMediumBJet;
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
  lTreeContent["lMjj_jet1_eta"] = *lMjj_jet1_eta;


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
    lTreeContent["diCleanJet_dEta"] = *dijet_dEta;
    lTreeContent["diCleanJet_dPhi"] = *dijet_dPhi;
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
    
    lTreeContent["VBF_MTR_QCD_CR_eff_Sel"] = 1;   
    lTreeContent["VBF_VTR_QCD_CR_eff_Sel"] = 1; 
    lTreeContent["VBF_MTR_QCD_SR_eff_Sel"] = 1; 
    lTreeContent["VBF_VTR_QCD_SR_eff_Sel"] = 1; 
    lTreeContent["VBF_MTR_QCD_A_eff_Sel"] = 1; 
    lTreeContent["VBF_VTR_QCD_A_eff_Sel"] = 1; 
    lTreeContent["VBF_MTR_QCD_B_eff_Sel"] = 1; 
    lTreeContent["VBF_VTR_QCD_B_eff_Sel"] = 1; 
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
    lTreeContent["VBF_VTR_QCD_NoDijetDphiOrMetPt_eff_Sel"] = * VBF_VTR_QCD_NoDijetDphiOrMetPt_eff_Sel; 
    lTreeContent["VBF_MTR_QCD_NoDijetDphiOrMetPt_eff_Sel"] = * VBF_MTR_QCD_NoDijetDphiOrMetPt_eff_Sel; 
  }




  
}


Bool_t Events::BaseSelection(){
  
  bool pass=kTRUE;
  std::string catStr = GetCatStr(mCat);


  if (catStr.find("MTR")!=catStr.npos){


    if ( misAM ){ //Needed if we don't have the flags
      pass = lTreeContent["nCleanJet30"]>=2;
      pass = pass && (lTreeContent["Leading_jet_pt"] > 80);
      pass = pass && (std::abs(lTreeContent["Leading_jet_eta"] < 4.7));
      pass = pass && (lTreeContent["Subleading_jet_pt"] > 40);
      pass = pass && (std::abs(lTreeContent["Subleading_jet_eta"] < 4.7));
      pass = pass && (lTreeContent["Leading_jet_eta"]*lTreeContent["Subleading_jet_eta"] < 0);
      pass = pass && (lTreeContent["diCleanJet_M"]>200);
      pass = pass && (std::abs(lTreeContent["diCleanJet_dPhi"]<1.5));
      pass = pass && (std::abs(lTreeContent["diCleanJet_dEta"]>1));    
      pass = pass && (lTreeContent["nLoosePhoton"]==0);
      pass = pass && (std::abs(lTreeContent["MET_pt"]-lTreeContent["CaloMET_pt"])/lTreeContent["MetNoLep_pt"]<0.5);
      pass = pass && (lTreeContent["met_filters"]==1);
      pass = pass && (static_cast<int>(lTreeContent["nMediumBJet"]) == 0);

      if ( std::abs(lTreeContent["Leading_jet_eta"]) <= 2.5 ){
	pass = pass && lTreeContent["Leading_jet_chHEF"] > 0.1;
	pass = pass && lTreeContent["Leading_jet_neHEF"] < 0.8;	
      }
      if ( std::abs(lTreeContent["Subleading_jet_eta"]) <= 2.5 ){
	pass = pass && lTreeContent["Subleading_jet_chHEF"] > 0.1;
	pass = pass && lTreeContent["Subleading_jet_neHEF"] < 0.8;	
      }

    }
    else{
      pass = pass && (static_cast<int>(lTreeContent["jet_chf_nhf_cut"]));
    }
       
    // if ( misMC ){      
    //   pass = pass 
    // 	&& (lTreeContent["xs_weight"] > 0);
    // }
      
  }
  else if (catStr.find("VTR")!=catStr.npos){
    pass = kTRUE;

    pass = pass && (static_cast<int>(lTreeContent["jet_chf_nhf_vtr_cut"]));

    // if ( misMC ){      
    //   pass = pass 
    // 	&& (lTreeContent["xs_weight"] > 0);
    // }

  }
  
  if ( mProc == "QCD" || mProc == "QCDRELAX" ){

    //    pass = pass && lTreeContent["LHE_HT"]>700; //TEMP

  }


  return pass;

}

Double_t Events::BaseWeight(){
  std::string catStr = GetCatStr(mCat);
  double w = (lTreeContent["puWeight"])*(lTreeContent["xs_weight"])*mLumiPb*(lTreeContent["L1PreFiringWeight_Nom"]);

  double tauveto = lTreeContent["VLooseTauFix_eventVetoW"];
  double electronveto = lTreeContent["VetoElectron_eventVetoW"];
  double muonveto = lTreeContent["LooseMuon_eventVetoW"];

  w *= tauveto*electronveto*muonveto;

  w*=lTreeContent["hem_weight"];//1 for data and MC in 2017
  
  if ( !misMC ) w = 1.0;//Data

  return w;

}

Bool_t Events::PassSelection(){

  bool pass = kTRUE;
  
  std::string lcat = "";
    
  bool lCleanCut0 = true;
  bool lCleanCut1 = true;
  bool lCleanCut2 = true;
  bool lCleanCut3 = true;

  double cut_lMjj_dijet_dphi = 2.5;
    
  if ( misAM ){
    //      bool lCleanCut0 = !(std::abs(lTreeContent["Met"] - lTreeContent["TkMET_pt"])/lTreeContent["Met"] >= 0.8 && ((std::abs(lTreeContent["Leading_jet_eta"])>=2.8 && std::abs(lTreeContent["Leading_jet_eta"]) <= 3.2) || (std::abs(lTreeContent["Subleading_jet_eta"])>=2.8 && std::abs(lTreeContent["Subleading_jet_eta"]) <= 3.2)));
    //      lCleanCut0 = !((lTreeContent["Met"] - lTreeContent["TkMET_pt"])/lTreeContent["Met"] >= 0.8 && ((std::abs(lTreeContent["Leading_jet_eta"])>=2.8 && std::abs(lTreeContent["Leading_jet_eta"]) <= 3.2) || (std::abs(lTreeContent["Subleading_jet_eta"])>=2.8 && std::abs(lTreeContent["Subleading_jet_eta"]) <= 3.2)));
    lCleanCut0 = !((lTreeContent["MetNoLep_pt"] - lTreeContent["TkMET_pt"])/lTreeContent["MetNoLep_pt"] >= 0.8 && ((std::abs(lTreeContent["Leading_jet_eta"])>=2.8 && std::abs(lTreeContent["Leading_jet_eta"]) <= 3.2) || (std::abs(lTreeContent["Subleading_jet_eta"])>=2.8 && std::abs(lTreeContent["Subleading_jet_eta"]) <= 3.2)));
    lCleanCut1 = !(lTreeContent["SoftActivityJetHT10"]<100 && ((std::abs(lTreeContent["Leading_jet_eta"])>=2.8 && std::abs(lTreeContent["Leading_jet_eta"]) <= 3.2) || (std::abs(lTreeContent["Subleading_jet_eta"])>=2.8 && std::abs(lTreeContent["Subleading_jet_eta"]) <= 3.2)));
    lCleanCut2 = !(((lTreeContent["softActivityJet1_eta"] > -3 && lTreeContent["softActivityJet1_eta"] < -1.2 && lTreeContent["softActivityJet1_phi"] < -0.5 && lTreeContent["softActivityJet1_phi"] > - 1.9) || (lTreeContent["softActivityJet2_eta"] > -3 && lTreeContent["softActivityJet2_eta"] < -1.2 && lTreeContent["softActivityJet2_phi"] < -0.5 && lTreeContent["softActivityJet2_phi"] > - 1.9) || (lTreeContent["softActivityJet3_eta"] > -3 && lTreeContent["softActivityJet3_eta"] < -1.2 && lTreeContent["softActivityJet3_phi"] < -0.5 && lTreeContent["softActivityJet3_phi"] > - 1.9) || (lTreeContent["softActivityJet4_eta"] > -3 && lTreeContent["softActivityJet4_eta"] < -1.2 && lTreeContent["softActivityJet4_phi"] < -0.5 && lTreeContent["softActivityJet4_phi"] > - 1.9) || (lTreeContent["softActivityJet5_eta"] > -3 && lTreeContent["softActivityJet5_eta"] < -1.2 && lTreeContent["softActivityJet5_phi"] < -0.5 && lTreeContent["softActivityJet5_phi"] > - 1.9) || (lTreeContent["softActivityJet6_eta"] > -3 && lTreeContent["softActivityJet6_eta"] < -1.2 && lTreeContent["softActivityJet6_phi"] < -0.5 && lTreeContent["softActivityJet6_phi"] > - 1.9)) && lTreeContent["MetPhiNoLep"]>-1.6 && lTreeContent["MetPhiNoLep"]<-0.8);
    lCleanCut3 = !(((lTreeContent["isoTrack1_eta"] > -3 && lTreeContent["isoTrack1_eta"] < -1.2 && lTreeContent["isoTrack1_phi"] < -0.5 && lTreeContent["isoTrack1_phi"] > - 1.9) || (lTreeContent["isoTrack2_eta"] > -3 && lTreeContent["isoTrack2_eta"] < -1.2 && lTreeContent["isoTrack2_phi"] < -0.5 && lTreeContent["isoTrack2_phi"] > - 1.9) || (lTreeContent["isoTrack3_eta"] > -3 && lTreeContent["isoTrack3_eta"] < -1.2 && lTreeContent["isoTrack3_phi"] < -0.5 && lTreeContent["isoTrack3_phi"] > - 1.9)) && lTreeContent["MetPhiNoLep"]>-1.6 && lTreeContent["MetPhiNoLep"]<-0.8);
  }  
  pass = pass && lCleanCut0;
  pass = pass && lCleanCut1;
  
  if (mYear == "2018"){
    pass = pass && lCleanCut2;
    pass = pass && lCleanCut3;
  }

  //Circular Cut and HF HF removal
  if (mCat==CatType::MTR){
    pass = pass && (static_cast<int>(lTreeContent["VecBDPhiCutMTR"]));
    pass = pass && ( abs(lTreeContent["Leading_jet_eta"]) < 3.0 || abs(lTreeContent["Subleading_jet_eta"]) < 3.0 );
    //    pass = pass && (std::abs(lTreeContent["diCleanJet_dEta"])<5);    
  }
  else if (mCat==CatType::VTR){
    pass = pass && (static_cast<int>(lTreeContent["VecBDPhiCutVTR"]));
    pass = pass && ( abs(lTreeContent["lMjj_jet1_eta"]) < 3.0 || abs(lTreeContent["lMjj_jet2_eta"]) < 3.0 );
    //    pass = pass && (std::abs(lTreeContent["lMjj_dijet_deta"])<5);    
  }

  //Triggers
  if (mCat==CatType::MTR){
    lcat = "MTR";

    if (mReg==RegionType::SR || (mReg==RegionType::QCDCR) ){
      pass = pass && (static_cast<int>(lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"])==1 || static_cast<int>(lTreeContent["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"])==1);
    }
    else{
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
      
      pass = pass && (lTreeContent["Leading_jet_pt"] > 200);//should it be 250?
    }
  }
  else if  (mCat==CatType::VTR){
    lcat = "VTR";
    if (mReg==RegionType::SR || (mReg==RegionType::QCDCR) ){
      pass = pass && (static_cast<int>(lTreeContent["HLT_DiJet110_35_Mjj650_PFMET110"])==1 || static_cast<int>(lTreeContent["HLT_TripleJet110_35_35_Mjj650_PFMET110"])==1);
    }
    else{
      // if ( mProc == "DATA" || mProc == "QCD" ){  //Already set when defined below
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
  }//End of trigger section
  
  if (mReg==RegionType::SR){
    
    if ( mCat == CatType::MTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;
      pass = pass && lTreeContent["MetNoLep_pt"]>250;
    }
    else if ( mCat == CatType::VTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>1.8;
      pass = pass && lTreeContent["MetNoLep_pt"]<=250;
      pass = pass && lTreeContent["MetNoLep_pt"]>160;
    }
    
    //////////////////////////////////
    //Investigation into dijet dphi
    
    if ( mProc == "QCDRELAX" ){
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_NoDijetDphiOrMetPt_eff_Sel"]));
      
      if ( mCat == CatType::MTR ){
	pass = pass && lTreeContent["diCleanJet_dPhi"]<cut_lMjj_dijet_dphi; 
	//	pass = pass && lTreeContent["diCleanJet_dPhi"]>1.5; 
      }
      else if ( mCat == CatType::VTR ){
	pass = pass && lTreeContent["lMjj_dijet_dphi"]<cut_lMjj_dijet_dphi;
	//	pass = pass && lTreeContent["lMjj_dijet_dphi"]>1.8;
      }
    }
    else{
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_SR_eff_Sel"]));
    }
    
    
    //////////////////////////////////
    
    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseTauFix"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }
    
  }
  else if (mReg==RegionType::QCDCR){
    
    if ( mCat == CatType::MTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]<0.5;
      pass = pass && lTreeContent["MetNoLep_pt"] > 250; 
    }
    else if ( mCat == CatType::VTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]<1.8;
      pass = pass && lTreeContent["MetNoLep_pt"] <= 250;   
      pass = pass && lTreeContent["MetNoLep_pt"] > 160;
    }
    
    //////////////////////////////////
    //Investigation into dijet dphi
    if ( mProc == "QCDRELAX" ){
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_NoDijetDphiOrMetPt_eff_Sel"]));

      if ( mCat == CatType::MTR ){
	pass = pass && lTreeContent["diCleanJet_dPhi"]<cut_lMjj_dijet_dphi; 
	//	pass = pass && lTreeContent["diCleanJet_dPhi"]>1.5; 
      }
      else if ( mCat == CatType::VTR ){
	pass = pass && lTreeContent["lMjj_dijet_dphi"]<cut_lMjj_dijet_dphi; 
	//	pass = pass && lTreeContent["lMjj_dijet_dphi"]>1.8; 
      }
    }
    else{
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_CR_eff_Sel"]));
    }
    

    //////////////////////////////////
    
    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseTauFix"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }
  }
  else if (mReg==RegionType::QCDA){
    
    if ( mCat == CatType::MTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]<0.5;
    }
    else if ( mCat == CatType::VTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]<1.8;
    }
    pass = pass && lTreeContent["MetNoLep_pt"]>100;
    pass = pass && lTreeContent["MetNoLep_pt"]<=160;     
    
    
    //////////////////////////////////
    //Investigation into dijet dphi
    if ( mProc == "QCDRELAX" ){
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_NoDijetDphiOrMetPt_eff_Sel"]));
      if ( mCat == CatType::MTR ){
	pass = pass && lTreeContent["diCleanJet_dPhi"]<cut_lMjj_dijet_dphi;
	//	pass = pass && lTreeContent["diCleanJet_dPhi"]>1.5;
      }
      else if ( mCat == CatType::VTR ){
	pass = pass && lTreeContent["lMjj_dijet_dphi"]<cut_lMjj_dijet_dphi;
	//	pass = pass && lTreeContent["lMjj_dijet_dphi"]>1.8;
      }

    }
    else{
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_A_eff_Sel"]));
    }
    
    
    //////////////////////////////////
    
    if ( !misMC ){
      pass = pass 
	&& static_cast<int>(lTreeContent["nVLooseTauFix"]) == 0
	&& static_cast<int>(lTreeContent["nMediumBJet"]) == 0
	&& static_cast<int>(lTreeContent["nVetoElectron"]) == 0
	&& static_cast<int>(lTreeContent["nLooseMuon"]) == 0;
    }
    
  }
  else if (mReg==RegionType::QCDB){//"Signal-like" region
        
    if ( mCat == CatType::MTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>0.5;
    }
    else if ( mCat == CatType::VTR ){
      pass = pass && lTreeContent["MetNoLep_CleanJet_mindPhi"]>1.8;
    }
    
    pass = pass && lTreeContent["MetNoLep_pt"]>100;
    pass = pass && lTreeContent["MetNoLep_pt"]<=160;     

    //////////////////////////////////
    //Investigation into dijet dphi
    if ( mProc == "QCDRELAX" ){
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_NoDijetDphiOrMetPt_eff_Sel"]));
      if ( mCat == CatType::MTR ){
	pass = pass && lTreeContent["diCleanJet_dPhi"]<cut_lMjj_dijet_dphi;
	//	pass = pass && lTreeContent["diCleanJet_dPhi"]>1.5;
      }
      if ( mCat == CatType::VTR ){
	pass = pass && lTreeContent["lMjj_dijet_dphi"]<cut_lMjj_dijet_dphi; 
	//	pass = pass && lTreeContent["lMjj_dijet_dphi"]>1.8; 
      }
    }
    else{
      pass = pass && (static_cast<int>(lTreeContent["VBF_"+lcat+"_QCD_B_eff_Sel"]));
    }
    
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

Double_t Events::SelWeight(){
  double w = 1.;

  //QCD k-factor SFs
  if ( misMC ) {

    if ( mCat == CatType::MTR ){
      w *= lTreeContent["fnlo_SF_QCD_corr_QCD_proc_MTR"];
    }
    else if ( mCat == CatType::VTR ){
      w *= lTreeContent["fnlo_SF_QCD_corr_QCD_proc_VTR"];
    }

    w *= lTreeContent["fnlo_SF_QCD_corr_EWK_proc"];
    w *= lTreeContent["fnlo_SF_EWK_corr"];

  }

  //Trigger SFs
  if (mReg==RegionType::SR || (mReg==RegionType::QCDCR) ){
    
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

  return w;
}

void Events::CalculateAdditionalVariables(){

  double dijet_met_balance = 0;

  TLorentzVector j1,j2;
  j1.SetPtEtaPhiM(lTreeContent["Leading_jet_pt"],lTreeContent["Leading_pt_eta"],lTreeContent["Leading_pt_phi"],0);
  j2.SetPtEtaPhiM(lTreeContent["Subleading_jet_pt"],lTreeContent["Subleading_pt_eta"],lTreeContent["Subleading_pt_phi"],0);
  dijet_met_balance = ((j1+j2).Pt() - lTreeContent["MetNoLep_pt"])/(lTreeContent["MetNoLep_pt"]);

  lTreeContent["dijet_met_balance"] = dijet_met_balance;


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

Bool_t Events::Process(Long64_t entry)
{

  fReader.SetLocalEntry(entry);
  SetTreeContent(mYear);
  if (entry%10000==0) std::cout << " -- processing entry " << entry/1000. << "k" << std::endl;
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

       

      CalculateAdditionalVariables();


      //Fill CSV file
      if (mCat==CatType::MTR && GetRegionStr(static_cast<RegionType>(iR)) == "QCDA"){
	if ( lTreeContent["LHE_HT"] > 700 && lTreeContent["LHE_HT"] < 1000){
	  fileMTR << std::fixed << std::setprecision(3)<< lTreeContent["LHE_HT"] << ", " << std::setprecision(0)  << lTreeContent["event"] << std::setprecision(4) <<  ", " << lTreeContent["diCleanJet_M"] << ", "<< lTreeContent["MetNoLep_pt"] << ", " << lTreeContent["diCleanJet_dPhi"] <<  ", " << lTreeContent["Leading_jet_pt"] << ", " << lTreeContent["Subleading_jet_pt"] << std::setprecision(9) << ", "  << GetRegionStr(static_cast<RegionType>(iR)) << ", " << weight << std::endl;
	}
      }

      for (unsigned iV(0); iV<mHistVec[iR][iC].size(); ++iV){
	//Blinding
	// if ( mReg==RegionType::SR && !misMC ){
	//   if ( mVarVec[iV] != "MetNoLep_CleanJet_mindPhi" && mVarVec[iV] != "Leading_jet_eta"  && mVarVec[iV] != "dijet_met_balance"){
	//     mHistVec[iR][iC][iV]->Fill(lTreeContent[mVarVec[iV]],0);
	//   }
	//   else{
	//     mHistVec[iR][iC][iV]->Fill(lTreeContent[mVarVec[iV]],weight);
	//   }
	// }
	// else{
	mHistVec[iR][iC][iV]->Fill(lTreeContent[mVarVec[iV]],weight);
	//	 }
      }

      for (unsigned iV(0); iV<mHistVec2D[iR][iC].size(); ++iV){
	//Blinding
	// if ( mReg==RegionType::SR && !misMC ){
	//   if ( mVarVec2D[iV].first != "MetNoLep_CleanJet_mindPhi" && mVarVec2D[iV].first != "Leading_jet_eta" && mVarVec2D[iV].second != "MetNoLep_CleanJet_mindPhi" && mVarVec2D[iV].second != "Leading_jet_eta" &&  mVarVec2D[iV].first != "dijet_met_balance" && mVarVec2D[iV].second != "dijet_met_balance" ){
	//     mHistVec2D[iR][iC][iV]->Fill(lTreeContent[mVarVec2D[iV].first],lTreeContent[mVarVec2D[iV].second],0);
	//   }
	//   else{
	//     mHistVec2D[iR][iC][iV]->Fill(lTreeContent[mVarVec2D[iV].first],lTreeContent[mVarVec2D[iV].second],weight);
	//   }
	// }
	mHistVec2D[iR][iC][iV]->Fill(lTreeContent[mVarVec2D[iV].first],lTreeContent[mVarVec2D[iV].second],weight);
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

  fileMTR.close();
}
#endif
