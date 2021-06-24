//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jan 24 09:51:45 2020 by ROOT version 6.18/00
// from TTree Events/
// found on file: output_skims/Nominal/WJETS.root
//////////////////////////////////////////////////////////

#ifndef Events_h
#define Events_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TH2D.h>
#include <TH3D.h>

// Headers needed by this particular selector

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <string>
#include <utility>
#include <functional>
#include <tuple>

enum RegionType { SR=0, QCDCR=1,  QCDA=2, QCDB=3, HFNoise=4, HFNoiseCR=5, Last};
enum CatType { MTR=0,  VTR=1,  LastCat };

class Events : public TSelector {
  
 public :
  TTreeReader     fReader;  //!the tree reader
  TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain
  

   Events(TTree * /*tree*/ =0);
   virtual ~Events();
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();
   //   virtual void    Initialise();


   void SetOutFileName(const std::string aName);
   std::string GetOutFileName();
   void SetCategory(const CatType & aCat);
   std::string GetRegionStr(const RegionType & aRegion);
   std::string GetCatStr(const CatType & aCat);
   void SetRegion(const RegionType & aRegion);
   void SetProcess(const std::string aProcess);
   void SetYear(const std::string aYear);
   void SetAM(const bool aisAM);
   void SetMC(const bool aisMC);
   void SetSystematic(const std::string aSystematic);
   void SetLumiPb(const double & aLumi, const double & aLumi_VTR);
   void SetTreeContent(std::string year);
   bool CheckValue(ROOT::Internal::TTreeReaderValueBase& value);
   void CalculateAdditionalVariables_Stage1();
   void CalculateAdditionalVariables_Stage2();

   
 private:
   TFile *mFout;
   std::string mOutFile;
   std::vector<std::string> mVarVec;
   std::vector<std::pair<std::string,std::string> > mVarVec2D;
   std::vector<std::tuple<std::string,std::string,std::string> > mVarVec3D;
   std::vector<TH1F*> mHistVec[RegionType::Last][CatType::LastCat];
   std::vector<TH2D*> mHistVec2D[RegionType::Last][CatType::LastCat];
   std::vector<TH3D*> mHistVec3D[RegionType::Last][CatType::LastCat];
   ofstream fileMTR;

   CatType mCat;
   RegionType mReg;
   TString mProc;
   std::string mYear;
   bool misAM;
   bool misMC;
   TString mSyst;
   double mLumiPb;
   double mLumiPb_VTR;
   bool mreweight_met_phi;
   TH1D * Met_phi_SF;

   Double_t MetPhiWeight();
   Double_t HFNoiseWeight();
   Bool_t BaseSelection();
   Bool_t BaseSelectionAM();
   Double_t BaseWeight();
   Bool_t PassSelection();
   Double_t SelWeight();

   /* struct hist1D{ */
   /*   std::string varname; */
   /*   int nBins; */
   /*   double binMin; */
   /*   double binMax; */
   /* }; */
   
   std::map<std::string,double> lTreeContent;
   
   //public:
   ClassDef(Events,0);
   

   TTreeReaderValue<Double_t> event ;
   TTreeReaderValue<Double_t> Zee_flag ;
   TTreeReaderValue<Double_t> Zmumu_flag ;
   TTreeReaderValue<Double_t> Wenu_flag ;
   TTreeReaderValue<Double_t> Wmunu_flag ;
   TTreeReaderValue<Double_t> VBF_MTR_QCD_CR_eff_Sel ;
   TTreeReaderValue<Double_t> VBF_VTR_QCD_CR_eff_Sel ;
   TTreeReaderValue<Double_t> VBF_MTR_QCD_SR_eff_Sel ;
   TTreeReaderValue<Double_t> VBF_VTR_QCD_SR_eff_Sel ;
   TTreeReaderValue<Double_t> VBF_MTR_QCD_HFNoise_eff_Sel ;
   TTreeReaderValue<Double_t> VBF_VTR_QCD_HFNoise_eff_Sel ;
   TTreeReaderValue<Double_t> VBF_MTR_QCD_HFNoiseCR_eff_Sel ;
   TTreeReaderValue<Double_t> VBF_VTR_QCD_HFNoiseCR_eff_Sel ;
   TTreeReaderValue<Double_t> VBF_MTR_QCD_A_eff_Sel ;
   TTreeReaderValue<Double_t> VBF_VTR_QCD_A_eff_Sel ;
   TTreeReaderValue<Double_t> VBF_MTR_QCD_B_eff_Sel ;
   TTreeReaderValue<Double_t> VBF_VTR_QCD_B_eff_Sel ;
   TTreeReaderValue<Double_t> VBF_VTR_QCD_NoDijetDphiOrMetPt_eff_Sel ;
   TTreeReaderValue<Double_t> VBF_MTR_QCD_NoDijetDphiOrMetPt_eff_Sel ;
   TTreeReaderValue<Double_t> ContainedHFjet ;
   TTreeReaderValue<Double_t> DiVetoElectron_mass ;
   TTreeReaderValue<Double_t> DiLooseMuon_mass ;
   TTreeReaderValue<Double_t> Leading_el_pt ;
   TTreeReaderValue<Double_t> Leading_el_eta ;
   TTreeReaderValue<Double_t> Leading_el_phi ;
   TTreeReaderValue<Double_t> Subleading_el_pt ;
   TTreeReaderValue<Double_t> Subleading_el_eta ;
   TTreeReaderValue<Double_t> Subleading_el_phi ;
   TTreeReaderValue<Double_t> Leading_muon_pt ;
   TTreeReaderValue<Double_t> Leading_muon_eta ;
   TTreeReaderValue<Double_t> Leading_muon_phi ;
   TTreeReaderValue<Double_t> Subleading_muon_pt ;
   TTreeReaderValue<Double_t> Subleading_muon_eta ;
   TTreeReaderValue<Double_t> Subleading_muon_phi ;
   TTreeReaderValue<Double_t> isData ;
   TTreeReaderValue<Double_t> nVLooseTau ;
   TTreeReaderValue<Double_t> nVLooseTauFix ;
   TTreeReaderValue<Double_t> nVLooseSITTau ;
   TTreeReaderValue<Double_t> nMediumBJet ;
   TTreeReaderValue<Double_t> nLoosePhoton ;
   TTreeReaderValue<Double_t> nLooseMuon ;
   TTreeReaderValue<Double_t> nVetoElectron ;
   TTreeReaderValue<Double_t> met_filters_2017_data ;
   TTreeReaderValue<Double_t> met_filters_2017_mc ;
   TTreeReaderValue<Double_t> met_filters_2018_data ;
   TTreeReaderValue<Double_t> met_filters_2018_mc ;
   TTreeReaderValue<Double_t> MetNoLep_CleanJet_mindPhi ;
   TTreeReaderValue<Double_t> MetNoLep_pt ;
   TTreeReaderValue<Double_t> MetNoLep_phi ;
   TTreeReaderValue<Double_t> min_dphi ;
   TTreeReaderValue<Double_t> min_dphi_MET ;
   TTreeReaderValue<Double_t> CorrectedMetNoLep_pt ;
   TTreeReaderValue<Double_t> CorrectedMetNoLep_phi ;
   TTreeReaderValue<Double_t> CorrectedMET_pt ;
   TTreeReaderValue<Double_t> CorrectedMET_phi ;
   TTreeReaderValue<Double_t> MET_pt ;
   TTreeReaderValue<Double_t> MET_phi ;
   TTreeReaderValue<Double_t> MET_sumEt ;
   TTreeReaderValue<Double_t> CaloMET_pt ;
   TTreeReaderValue<Double_t> CaloMET_phi ;
   TTreeReaderValue<Double_t> CaloMET_sumEt ;
   TTreeReaderValue<Double_t> ChsMET_pt ;
   TTreeReaderValue<Double_t> ChsMET_phi ;
   TTreeReaderValue<Double_t> ChsMET_sumEt ;
   TTreeReaderValue<Double_t> nCleanJet30 ;
   TTreeReaderValue<Double_t> Leading_jet_pt ;
   TTreeReaderValue<Double_t> Leading_jet_eta ;
   TTreeReaderValue<Double_t> Leading_jet_phi ;
   TTreeReaderValue<Double_t> Leading_jet_chHEF ;
   TTreeReaderValue<Double_t> Leading_jet_neHEF ;
   TTreeReaderValue<Double_t> Subleading_jet_pt ;
   TTreeReaderValue<Double_t> Subleading_jet_eta ;
   TTreeReaderValue<Double_t> Subleading_jet_phi ;
   TTreeReaderValue<Double_t> Subleading_jet_chHEF ;
   TTreeReaderValue<Double_t> Subleading_jet_neHEF ;
   TTreeReaderValue<Double_t> lMjj_jet1_pt ;
   TTreeReaderValue<Double_t> lMjj_jet1_eta ;
   TTreeReaderValue<Double_t> lMjj_jet1_phi ;
   TTreeReaderValue<Double_t> lMjj_jet2_pt ;
   TTreeReaderValue<Double_t> lMjj_jet2_eta ;
   TTreeReaderValue<Double_t> lMjj_jet2_phi ;
   TTreeReaderValue<Double_t> diCleanJet_M ;
   TTreeReaderValue<Double_t> lMjj ;
   TTreeReaderValue<Double_t> lMjj_dijet_dphi ;
   TTreeReaderValue<Double_t> lMjj_dijet_deta ;
   TTreeReaderValue<Double_t> DiCleanJet_mass ;
   TTreeReaderValue<Double_t> dijet_M ;
   TTreeReaderValue<Double_t> diCleanJet_dPhi ;
   TTreeReaderValue<Double_t> diCleanJet_dEta ;
   TTreeReaderValue<Double_t> Pileup_nTrueInt ;
   TTreeReaderValue<Double_t> Pileup_nPU ;
   TTreeReaderValue<Double_t> PV_npvsGood ;
   TTreeReaderValue<Double_t> Gen_boson_pt ;
   TTreeReaderValue<Double_t> Gen_Mjj ;
   TTreeReaderValue<Double_t> Gen_jet_pt ;
   TTreeReaderValue<Double_t> decayLeptonId ;
   TTreeReaderValue<Double_t> nGenDressedLepton ;
   TTreeReaderValue<Double_t> LHE_HT ;
   TTreeReaderValue<Double_t> LHE_Vpt ;
   TTreeReaderValue<Double_t> LHE_Njets ;
   TTreeReaderValue<Double_t> LHE_Nb ;
   TTreeReaderValue<Double_t> LHE_Nc ;
   TTreeReaderValue<Double_t> LHE_Nuds ;
   TTreeReaderValue<Double_t> LHE_Nglu ;
   TTreeReaderValue<Double_t> GenMET_pt ;
   TTreeReaderValue<Double_t> GenMET_phi ;
   TTreeReaderValue<Double_t> xs_weight ;
   TTreeReaderValue<Double_t> puWeight ;
   TTreeReaderValue<Double_t> PrefireWeight ;
   TTreeReaderValue<Double_t> fnlo_SF_EWK_corr ;
   TTreeReaderValue<Double_t> fnlo_SF_QCD_corr_QCD_proc_MTR ;
   TTreeReaderValue<Double_t> fnlo_SF_QCD_corr_QCD_proc_VTR ;
   TTreeReaderValue<Double_t> fnlo_SF_QCD_corr_EWK_proc ;
   TTreeReaderValue<Double_t> jetemW_MTR ;
   TTreeReaderValue<Double_t> jetemW_VTR ;
   TTreeReaderValue<Double_t> jetHFW_MTR ;
   TTreeReaderValue<Double_t> jetHFW_VTR ;
   TTreeReaderValue<Double_t> jetHFW_NoiseRateMTR ;
   TTreeReaderValue<Double_t> jetHFW_NoiseRateVTR ;
   TTreeReaderValue<Double_t> hem_weight ;
   TTreeReaderValue<Double_t> trigger_weight_METMHT2018 ;
   TTreeReaderValue<Double_t> trigger_weight_VBF2018 ;
   TTreeReaderValue<Double_t> trigger_weight_SingleEle322018 ;
   TTreeReaderValue<Double_t> trigger_weight_METMHT2017 ;
   TTreeReaderValue<Double_t> trigger_weight_VBF2017 ;
   TTreeReaderValue<Double_t> trigger_weight_SingleEle352017 ;
   TTreeReaderValue<Double_t> VetoElectron_eventSelW ;
   TTreeReaderValue<Double_t> CRVetoElectron_eventSelW ;
   TTreeReaderValue<Double_t> CRTightElectron_eventSelW ;
   TTreeReaderValue<Double_t> VetoElectron_eventVetoW ;
   TTreeReaderValue<Double_t> VetoElectronFix_eventVetoW ;
   TTreeReaderValue<Double_t> VetoElectron_eventVetoW_systRECO_up ;
   TTreeReaderValue<Double_t> VetoElectron_eventVetoW_systRECO_down ;
   TTreeReaderValue<Double_t> VetoElectron_eventVetoW_systIDISO_up ;
   TTreeReaderValue<Double_t> VetoElectron_eventVetoW_systIDISO_down ;
   TTreeReaderValue<Double_t> LooseMuon_eventSelW ;
   TTreeReaderValue<Double_t> CRLooseMuon_eventSelW ;
   TTreeReaderValue<Double_t> CRTightMuon_eventSelW ;
   TTreeReaderValue<Double_t> LooseMuon_eventVetoW ;
   TTreeReaderValue<Double_t> LooseMuonFix_eventVetoW ;
   TTreeReaderValue<Double_t> LooseMuon_eventVetoW_systID_up ;
   TTreeReaderValue<Double_t> LooseMuon_eventVetoW_systID_down ;
   TTreeReaderValue<Double_t> LooseMuon_eventVetoW_systISO_up ;
   TTreeReaderValue<Double_t> LooseMuon_eventVetoW_systISO_down ;
   TTreeReaderValue<Double_t> VLooseTau_eventVetoW ;
   TTreeReaderValue<Double_t> VLooseTau_eventVetoW_up ;
   TTreeReaderValue<Double_t> VLooseTau_eventVetoW_down ;
   TTreeReaderValue<Double_t> VLooseSITTau_eventVetoW ;
   TTreeReaderValue<Double_t> VLooseSITTau_eventVetoW_up ;
   TTreeReaderValue<Double_t> VLooseSITTau_eventVetoW_down ;
   TTreeReaderValue<Double_t> VLooseTauFix_eventVetoW ;
   TTreeReaderValue<Double_t> VLooseTauFix_eventVetoW_up ;
   TTreeReaderValue<Double_t> VLooseTauFix_eventVetoW_down ;
   TTreeReaderValue<Double_t> MediumBJet_eventVetoW ;
   TTreeReaderValue<Double_t> HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60 ;
   TTreeReaderValue<Double_t> HLT_PFMETNoMu120_PFMHTNoMu120_IDTight ;
   TTreeReaderValue<Double_t> HLT_DiJet110_35_Mjj650_PFMET110 ;
   TTreeReaderValue<Double_t> HLT_TripleJet110_35_35_Mjj650_PFMET110 ;
   TTreeReaderValue<Double_t> HLT_Ele35_WPTight_Gsf ;
   TTreeReaderValue<Double_t> HLT_Ele32_WPTight_Gsf ;
   TTreeReaderValue<Double_t> HLT_IsoMu27 ;
   TTreeReaderValue<Double_t> HLT_Ele32_WPTight_Gsf_L1DoubleEG ;
   TTreeReaderValue<Double_t> HLT_Photon200 ;
   TTreeReaderValue<Double_t> HLT_PFJet40;
   TTreeReaderValue<Double_t> HLT_PFJet60;
   TTreeReaderValue<Double_t> HLT_PFJet80;
   TTreeReaderValue<Double_t> HLT_PFJet140;
   TTreeReaderValue<Double_t> HLT_PFJet200;
   TTreeReaderValue<Double_t> HLT_PFJet260;
   TTreeReaderValue<Double_t> HLT_PFJet320;
   TTreeReaderValue<Double_t> HLT_PFJet400;
   TTreeReaderValue<Double_t> HLT_PFJet450;
   TTreeReaderValue<Double_t> HLT_PFJet500;
   TTreeReaderValue<Double_t> HLT_PFJet550;

   TTreeReaderValue<Double_t>  Met ;
   TTreeReaderValue<Double_t>  TkMET_pt ;
   TTreeReaderValue<Double_t>  SoftActivityJetHT10;
   TTreeReaderValue<Double_t>  softActivityJet1_eta;
   TTreeReaderValue<Double_t>  softActivityJet1_phi;
   TTreeReaderValue<Double_t>  softActivityJet2_eta;
   TTreeReaderValue<Double_t>  softActivityJet2_phi;
   TTreeReaderValue<Double_t>  softActivityJet3_eta;
   TTreeReaderValue<Double_t>  softActivityJet3_phi;
   TTreeReaderValue<Double_t>  softActivityJet4_eta;
   TTreeReaderValue<Double_t>  softActivityJet4_phi;
   TTreeReaderValue<Double_t>  softActivityJet5_eta;
   TTreeReaderValue<Double_t>  softActivityJet5_phi;
   TTreeReaderValue<Double_t>  softActivityJet6_eta;
   TTreeReaderValue<Double_t>  softActivityJet6_phi;
   TTreeReaderValue<Double_t>  isoTrack1_eta;
   TTreeReaderValue<Double_t>  isoTrack1_phi;
   TTreeReaderValue<Double_t>  isoTrack2_eta;
   TTreeReaderValue<Double_t>  isoTrack2_phi;
   TTreeReaderValue<Double_t>  isoTrack3_eta;
   TTreeReaderValue<Double_t>  isoTrack3_phi;        

   TTreeReaderValue<Double_t>  jet_chf_nhf_cut;        
   TTreeReaderValue<Double_t>  jet_chf_nhf_vtr_cut;        

   TTreeReaderValue<Double_t>  horn_sel;        
   TTreeReaderValue<Double_t>  horn_sel_vtr;        

   TTreeReaderValue<Double_t>  VecBDPhiCutMTR;        
   TTreeReaderValue<Double_t>  VecBDPhiCutVTR;        

   //AM's variables
   TTreeReaderValue<Double_t>  JetMetmindPhi;        
   TTreeReaderValue<Double_t>  MetNoLep;        
   TTreeReaderValue<Double_t>  dijet_dPhi;
   TTreeReaderValue<Double_t>  dijet_dEta;
};

#endif

#ifdef Events_cxx
void Events::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

  // Readers to access the data (delete the ones you do not need).

  //Uncomment if you want to access those variables that are commented.
  //They are commented to avoid spending time running over unused variables

  if (tree->GetBranch("event") !=0 ) event = {  fReader,"event"};
  /* if (tree->GetBranch("Zee_flag") !=0 ) Zee_flag = {  fReader,"Zee_flag"}; */
  /* if (tree->GetBranch("Zmumu_flag") !=0 ) Zmumu_flag = {fReader,"Zmumu_flag"}; */
  /* if (tree->GetBranch("Wenu_flag") !=0 ) Wenu_flag = { fReader,"Wenu_flag"}; */
  /* if (tree->GetBranch("Wmunu_flag") !=0 ) Wmunu_flag = {fReader,"Wmunu_flag"}; */
  if (tree->GetBranch("VBF_MTR_QCD_CR_eff_Sel") !=0 ) VBF_MTR_QCD_CR_eff_Sel = { fReader,"VBF_MTR_QCD_CR_eff_Sel"};
  if (tree->GetBranch("VBF_VTR_QCD_CR_eff_Sel") !=0 ) VBF_VTR_QCD_CR_eff_Sel = { fReader,"VBF_VTR_QCD_CR_eff_Sel"};
  if (tree->GetBranch("VBF_MTR_QCD_SR_eff_Sel") !=0 ) VBF_MTR_QCD_SR_eff_Sel = { fReader,"VBF_MTR_QCD_SR_eff_Sel"};
  if (tree->GetBranch("VBF_VTR_QCD_SR_eff_Sel") !=0 ) VBF_VTR_QCD_SR_eff_Sel = { fReader,"VBF_VTR_QCD_SR_eff_Sel"};
  if (tree->GetBranch("VBF_MTR_QCD_HFNoise_eff_Sel") !=0 ) VBF_MTR_QCD_HFNoise_eff_Sel = { fReader,"VBF_MTR_QCD_HFNoise_eff_Sel"};
  if (tree->GetBranch("VBF_VTR_QCD_HFNoise_eff_Sel") !=0 ) VBF_VTR_QCD_HFNoise_eff_Sel = { fReader,"VBF_VTR_QCD_HFNoise_eff_Sel"};
  if (tree->GetBranch("VBF_MTR_QCD_HFNoiseCR_eff_Sel") !=0 ) VBF_MTR_QCD_HFNoiseCR_eff_Sel = { fReader,"VBF_MTR_QCD_HFNoiseCR_eff_Sel"};
  if (tree->GetBranch("VBF_VTR_QCD_HFNoiseCR_eff_Sel") !=0 ) VBF_VTR_QCD_HFNoiseCR_eff_Sel = { fReader,"VBF_VTR_QCD_HFNoiseCR_eff_Sel"};
  if (tree->GetBranch("VBF_MTR_QCD_A_eff_Sel") !=0 ) VBF_MTR_QCD_A_eff_Sel = {  fReader,"VBF_MTR_QCD_A_eff_Sel"};
  if (tree->GetBranch("VBF_VTR_QCD_A_eff_Sel") !=0 ) VBF_VTR_QCD_A_eff_Sel = {  fReader,"VBF_VTR_QCD_A_eff_Sel"};
  if (tree->GetBranch("VBF_MTR_QCD_B_eff_Sel") !=0 ) VBF_MTR_QCD_B_eff_Sel = {  fReader,"VBF_MTR_QCD_B_eff_Sel"};
  if (tree->GetBranch("VBF_VTR_QCD_B_eff_Sel") !=0 ) VBF_VTR_QCD_B_eff_Sel = {  fReader,"VBF_VTR_QCD_B_eff_Sel"};
  if (tree->GetBranch("VBF_VTR_QCD_NoDijetDphiOrMetPt_eff_Sel") !=0 ) VBF_VTR_QCD_NoDijetDphiOrMetPt_eff_Sel = {  fReader,"VBF_VTR_QCD_NoDijetDphiOrMetPt_eff_Sel"};
  if (tree->GetBranch("VBF_MTR_QCD_NoDijetDphiOrMetPt_eff_Sel") !=0 ) VBF_MTR_QCD_NoDijetDphiOrMetPt_eff_Sel = {  fReader,"VBF_MTR_QCD_NoDijetDphiOrMetPt_eff_Sel"};
  if (tree->GetBranch("ContainedHFjet") !=0 ) ContainedHFjet = {  fReader,"ContainedHFjet"};

  //  if (tree->GetBranch("DiVetoElectron_mass") !=0 ) DiVetoElectron_mass = {    fReader,"DiVetoElectron_mass"};
  //  if (tree->GetBranch("DiLooseMuon_mass") !=0 ) DiLooseMuon_mass = {  fReader,"DiLooseMuon_mass"};
  /* if (tree->GetBranch("Leading_el_pt") !=0 ) Leading_el_pt = {fReader,"Leading_el_pt"}; */
  /* if (tree->GetBranch("Leading_el_eta") !=0 ) Leading_el_eta = { fReader,"Leading_el_eta"}; */
  /* if (tree->GetBranch("Leading_el_phi") !=0 ) Leading_el_phi = { fReader,"Leading_el_phi"}; */
  /* if (tree->GetBranch("Subleading_el_pt") !=0 ) Subleading_el_pt = {  fReader,"Subleading_el_pt"}; */
  /* if (tree->GetBranch("Subleading_el_eta") !=0 ) Subleading_el_eta = { fReader,"Subleading_el_eta"}; */
  /* if (tree->GetBranch("Subleading_el_phi") !=0 ) Subleading_el_phi = { fReader,"Subleading_el_phi"}; */
  /* if (tree->GetBranch("Leading_muon_pt") !=0 ) Leading_muon_pt = {fReader,"Leading_muon_pt"}; */
  /* if (tree->GetBranch("Leading_muon_eta") !=0 ) Leading_muon_eta = {  fReader,"Leading_muon_eta"}; */
  /* if (tree->GetBranch("Leading_muon_phi") !=0 ) Leading_muon_phi = {  fReader,"Leading_muon_phi"}; */
  /* if (tree->GetBranch("Subleading_muon_pt") !=0 ) Subleading_muon_pt = {fReader,"Subleading_muon_pt"}; */
  /* if (tree->GetBranch("Subleading_muon_eta") !=0 ) Subleading_muon_eta = {    fReader,"Subleading_muon_eta"}; */
  /* if (tree->GetBranch("Subleading_muon_phi") !=0 ) Subleading_muon_phi = {    fReader,"Subleading_muon_phi"}; */
  if (tree->GetBranch("isData") !=0 ) isData = {    fReader,"isData"};
  //  if (tree->GetBranch("nVLooseTau") !=0 ) nVLooseTau = {fReader,"nVLooseTau"};
  if (tree->GetBranch("nVLooseTauFix") !=0 ) nVLooseTauFix = {fReader,"nVLooseTauFix"};
  //  if (tree->GetBranch("nVLooseSITTau") !=0 ) nVLooseSITTau = {fReader,"nVLooseSITTau"};
  if (tree->GetBranch("nMediumBJet") !=0 ) nMediumBJet = {  fReader,"nMediumBJet"};
  if (tree->GetBranch("nLoosePhoton") !=0 ) nLoosePhoton = { fReader,"nLoosePhoton"};
  if (tree->GetBranch("nLooseMuon") !=0 ) nLooseMuon = {fReader,"nLooseMuon"};
  if (tree->GetBranch("nVetoElectron") !=0 ) nVetoElectron = {fReader,"nVetoElectron"};
  /* if (tree->GetBranch("met_filters_2017_data") !=0 ) met_filters_2017_data = {  fReader,"met_filters_2017_data"}; */
  /* if (tree->GetBranch("met_filters_2017_mc") !=0 ) met_filters_2017_mc = {    fReader,"met_filters_2017_mc"}; */
  /* if (tree->GetBranch("met_filters_2018_data") !=0 ) met_filters_2018_data = {  fReader,"met_filters_2018_data"}; */
  /* if (tree->GetBranch("met_filters_2018_mc") !=0 ) met_filters_2018_mc = {    fReader,"met_filters_2018_mc"}; */
  if (tree->GetBranch("MetNoLep_CleanJet_mindPhi") !=0 ) MetNoLep_CleanJet_mindPhi = { fReader,"MetNoLep_CleanJet_mindPhi"};
  if (tree->GetBranch("MetNoLep_pt") !=0 ) MetNoLep_pt = {  fReader,"MetNoLep_pt"};
  if (tree->GetBranch("MetNoLep_phi") !=0 ) MetNoLep_phi = {  fReader,"MetNoLep_phi"};

  if (tree->GetBranch("min_dphi") !=0 ) min_dphi = { fReader,"min_dphi"};
  if (tree->GetBranch("min_dphi_MET") !=0 ) min_dphi_MET = { fReader,"min_dphi_MET"};
  if (tree->GetBranch("CorrectedMetNoLep_pt") !=0 ) CorrectedMetNoLep_pt = {  fReader,"CorrectedMetNoLep_pt"};
  if (tree->GetBranch("CorrectedMetNoLep_phi") !=0 ) CorrectedMetNoLep_phi = {  fReader,"CorrectedMetNoLep_phi"};
  if (tree->GetBranch("CorrectedMET_pt") !=0 ) CorrectedMET_pt = {  fReader,"CorrectedMET_pt"};
  if (tree->GetBranch("CorrectedMET_phi") !=0 ) CorrectedMET_phi = {  fReader,"CorrectedMET_phi"};

  /* if (tree->GetBranch("MET_pt") !=0 ) MET_pt = {    fReader,"MET_pt"}; */
  /* if (tree->GetBranch("MET_phi") !=0 ) MET_phi = {   fReader,"MET_phi"}; */
  /* if (tree->GetBranch("MET_sumEt") !=0 ) MET_sumEt = { fReader,"MET_sumEt"}; */
  /* if (tree->GetBranch("CaloMET_pt") !=0 ) CaloMET_pt = {fReader,"CaloMET_pt"}; */
  /* if (tree->GetBranch("CaloMET_phi") !=0 ) CaloMET_phi = {  fReader,"CaloMET_phi"}; */
  /* if (tree->GetBranch("CaloMET_sumEt") !=0 ) CaloMET_sumEt = {fReader,"CaloMET_sumEt"}; */
  /* if (tree->GetBranch("ChsMET_pt") !=0 ) ChsMET_pt = { fReader,"ChsMET_pt"}; */
  /* if (tree->GetBranch("ChsMET_phi") !=0 ) ChsMET_phi = {fReader,"ChsMET_phi"}; */
  /* if (tree->GetBranch("ChsMET_sumEt") !=0 ) ChsMET_sumEt = { fReader,"ChsMET_sumEt"}; */
  if (tree->GetBranch("nCleanJet30") !=0 ) nCleanJet30 = {  fReader,"nCleanJet30"};
  if (tree->GetBranch("Leading_jet_pt") !=0 ) Leading_jet_pt = { fReader,"Leading_jet_pt"};
  if (tree->GetBranch("Leading_jet_eta") !=0 ) Leading_jet_eta = {fReader,"Leading_jet_eta"};
  if (tree->GetBranch("Leading_jet_phi") !=0 ) Leading_jet_phi = {fReader,"Leading_jet_phi"};
  if (tree->GetBranch("Leading_jet_chHEF") !=0 ) Leading_jet_chHEF = {fReader,"Leading_jet_chHEF"};
  if (tree->GetBranch("Leading_jet_neHEF") !=0 ) Leading_jet_neHEF = {fReader,"Leading_jet_neHEF"};
  if (tree->GetBranch("Subleading_jet_pt") !=0 ) Subleading_jet_pt = { fReader,"Subleading_jet_pt"};
  if (tree->GetBranch("Subleading_jet_eta") !=0 ) Subleading_jet_eta = {fReader,"Subleading_jet_eta"};
  if (tree->GetBranch("Subleading_jet_phi") !=0 ) Subleading_jet_phi = {fReader,"Subleading_jet_phi"};
  if (tree->GetBranch("Subleading_jet_chHEF") !=0 ) Subleading_jet_chHEF = {fReader,"Subleading_jet_chHEF"};
  if (tree->GetBranch("Subleading_jet_neHEF") !=0 ) Subleading_jet_neHEF = {fReader,"Subleading_jet_neHEF"};
  if (tree->GetBranch("lMjj_jet1_pt") !=0 ) lMjj_jet1_pt = { fReader,"lMjj_jet1_pt"};
  if (tree->GetBranch("lMjj_jet1_eta") !=0 ) lMjj_jet1_eta = {fReader,"lMjj_jet1_eta"};
  if (tree->GetBranch("lMjj_jet1_phi") !=0 ) lMjj_jet1_phi = {fReader,"lMjj_jet1_phi"};
  if (tree->GetBranch("lMjj_jet2_pt") !=0 ) lMjj_jet2_pt = { fReader,"lMjj_jet2_pt"};
  if (tree->GetBranch("lMjj_jet2_eta") !=0 ) lMjj_jet2_eta = {fReader,"lMjj_jet2_eta"};
  if (tree->GetBranch("lMjj_jet2_phi") !=0 ) lMjj_jet2_phi = {fReader,"lMjj_jet2_phi"};
  if (tree->GetBranch("diCleanJet_M") !=0 ) diCleanJet_M = { fReader,"diCleanJet_M"};
  if (tree->GetBranch("lMjj") !=0 ) lMjj = { fReader,"lMjj"};
  if (tree->GetBranch("lMjj_dijet_dphi") !=0 ) lMjj_dijet_dphi = { fReader,"lMjj_dijet_dphi"};
  if (tree->GetBranch("lMjj_dijet_deta") !=0 ) lMjj_dijet_deta = { fReader,"lMjj_dijet_deta"};
  //  if (tree->GetBranch("DiCleanJet_mass") !=0 ) DiCleanJet_mass = { fReader,"DiCleanJet_mass"};
  if (tree->GetBranch("dijet_M") !=0 ) dijet_M = { fReader,"dijet_M"};
  if (tree->GetBranch("diCleanJet_dPhi") !=0 ) diCleanJet_dPhi = {fReader,"diCleanJet_dPhi"};
  if (tree->GetBranch("diCleanJet_dEta") !=0 ) diCleanJet_dEta = {fReader,"diCleanJet_dEta"};
  //  if (tree->GetBranch("Pileup_nTrueInt") !=0 ) Pileup_nTrueInt = {fReader,"Pileup_nTrueInt"};
  if (tree->GetBranch("Pileup_nPU") !=0 ) Pileup_nPU = {fReader,"Pileup_nPU"};
  //  if (tree->GetBranch("PV_npvsGood") !=0 ) PV_npvsGood = {  fReader,"PV_npvsGood"};
  //  if (tree->GetBranch("Gen_boson_pt") !=0 ) Gen_boson_pt = { fReader,"Gen_boson_pt"};
  //  if (tree->GetBranch("Gen_Mjj") !=0 ) Gen_Mjj = {   fReader,"Gen_Mjj"};
  //  if (tree->GetBranch("Gen_jet_pt") !=0 ) Gen_jet_pt = {fReader,"Gen_jet_pt"};
  //  if (tree->GetBranch("decayLeptonId") !=0 ) decayLeptonId = {fReader,"decayLeptonId"};
  //  if (tree->GetBranch("nGenDressedLepton") !=0 ) nGenDressedLepton = { fReader,"nGenDressedLepton"};
  if (tree->GetBranch("LHE_HT") !=0 ) LHE_HT = {    fReader,"LHE_HT"};
  if (tree->GetBranch("LHE_Vpt") !=0 ) LHE_Vpt = {   fReader,"LHE_Vpt"};
  if (tree->GetBranch("LHE_Njets") !=0 ) LHE_Njets = { fReader,"LHE_Njets"};
  //  if (tree->GetBranch("LHE_Nb") !=0 ) LHE_Nb = {    fReader,"LHE_Nb"};
  //  if (tree->GetBranch("LHE_Nc") !=0 ) LHE_Nc = {    fReader,"LHE_Nc"};
  //  if (tree->GetBranch("LHE_Nuds") !=0 ) LHE_Nuds = {  fReader,"LHE_Nuds"};
  //  if (tree->GetBranch("LHE_Nglu") !=0 ) LHE_Nglu = {  fReader,"LHE_Nglu"};
  if (tree->GetBranch("GenMET_pt") !=0 ) GenMET_pt = { fReader,"GenMET_pt"};
  if (tree->GetBranch("GenMET_phi") !=0 ) GenMET_phi = {fReader,"GenMET_phi"};
  if (tree->GetBranch("xs_weight") !=0 ) xs_weight = { fReader,"xs_weight"};
  if (tree->GetBranch("puWeight") !=0 ) puWeight = {  fReader,"puWeight"};
  if (tree->GetBranch("PrefireWeight") !=0 ) PrefireWeight = {  fReader,"PrefireWeight"};
  if (tree->GetBranch("fnlo_SF_EWK_corr") !=0 ) fnlo_SF_EWK_corr = {  fReader,"fnlo_SF_EWK_corr"};
  if (tree->GetBranch("fnlo_SF_QCD_corr_QCD_proc_MTR") !=0 ) fnlo_SF_QCD_corr_QCD_proc_MTR = {  fReader,"fnlo_SF_QCD_corr_QCD_proc_MTR"};
  if (tree->GetBranch("fnlo_SF_QCD_corr_QCD_proc_VTR") !=0 ) fnlo_SF_QCD_corr_QCD_proc_VTR = {  fReader,"fnlo_SF_QCD_corr_QCD_proc_VTR"};
  if (tree->GetBranch("fnlo_SF_QCD_corr_EWK_proc") !=0 ) fnlo_SF_QCD_corr_EWK_proc = { fReader,"fnlo_SF_QCD_corr_EWK_proc"};
  if (tree->GetBranch("jetemW_MTR") !=0 ) jetemW_MTR = {  fReader,"jetemW_MTR"};
  if (tree->GetBranch("jetemW_VTR") !=0 ) jetemW_VTR = {  fReader,"jetemW_VTR"};
  if (tree->GetBranch("jetHFW_MTR") !=0 ) jetHFW_MTR = {  fReader,"jetHFW_MTR"};
  if (tree->GetBranch("jetHFW_VTR") !=0 ) jetHFW_VTR = {  fReader,"jetHFW_VTR"};
  if (tree->GetBranch("jetHFW_NoiseRateMTR") !=0 ) jetHFW_NoiseRateMTR = {  fReader,"jetHFW_NoiseRateMTR"};
  if (tree->GetBranch("jetHFW_NoiseRateVTR") !=0 ) jetHFW_NoiseRateVTR = {  fReader,"jetHFW_NoiseRateVTR"};
  if (tree->GetBranch("hem_weight") !=0 ) hem_weight = {  fReader,"hem_weight"};
  if (tree->GetBranch("trigger_weight_METMHT2018") !=0 ) trigger_weight_METMHT2018 = { fReader,"trigger_weight_METMHT2018"};
  if (tree->GetBranch("trigger_weight_VBF2018") !=0 ) trigger_weight_VBF2018 = { fReader,"trigger_weight_VBF2018"};
  //  if (tree->GetBranch("trigger_weight_SingleEle322018") !=0 ) trigger_weight_SingleEle322018 = { fReader,"trigger_weight_SingleEle322018"};
  if (tree->GetBranch("trigger_weight_METMHT2017") !=0 ) trigger_weight_METMHT2017 = { fReader,"trigger_weight_METMHT2017"};
  if (tree->GetBranch("trigger_weight_VBF2017") !=0 ) trigger_weight_VBF2017 = { fReader,"trigger_weight_VBF2017"};
  //  if (tree->GetBranch("trigger_weight_SingleEle352017") !=0 ) trigger_weight_SingleEle352017 = { fReader,"trigger_weight_SingleEle352017"};
  if (tree->GetBranch("VetoElectron_eventSelW") !=0 ) VetoElectron_eventSelW = { fReader,"VetoElectron_eventSelW"};
  /* if (tree->GetBranch("CRVetoElectron_eventSelW") !=0 ) CRVetoElectron_eventSelW = {    fReader,"CRVetoElectron_eventSelW"}; */
  /* if (tree->GetBranch("CRTightElectron_eventSelW") !=0 ) CRTightElectron_eventSelW = { fReader,"CRTightElectron_eventSelW"}; */
  if (tree->GetBranch("VetoElectron_eventVetoW") !=0 ) VetoElectron_eventVetoW = {fReader,"VetoElectron_eventVetoW"};
  if (tree->GetBranch("VetoElectronFix_eventVetoW") !=0 ) VetoElectronFix_eventVetoW = {fReader,"VetoElectronFix_eventVetoW"};
  /* if (tree->GetBranch("VetoElectron_eventVetoW_systRECO_up") !=0 ) VetoElectron_eventVetoW_systRECO_up = { fReader,"VetoElectron_eventVetoW_systRECO_up"}; */
  /* if (tree->GetBranch("VetoElectron_eventVetoW_systRECO_down") !=0 ) VetoElectron_eventVetoW_systRECO_down = {  fReader,"VetoElectron_eventVetoW_systRECO_down"}; */
  /* if (tree->GetBranch("VetoElectron_eventVetoW_systIDISO_up") !=0 ) VetoElectron_eventVetoW_systIDISO_up = {fReader,"VetoElectron_eventVetoW_systIDISO_up"}; */
  /* if (tree->GetBranch("VetoElectron_eventVetoW_systIDISO_down") !=0 ) VetoElectron_eventVetoW_systIDISO_down = {   fReader,"VetoElectron_eventVetoW_systIDISO_down"}; */
  if (tree->GetBranch("LooseMuon_eventSelW") !=0 ) LooseMuon_eventSelW = {    fReader,"LooseMuon_eventSelW"};
  /* if (tree->GetBranch("CRLooseMuon_eventSelW") !=0 ) CRLooseMuon_eventSelW = {  fReader,"CRLooseMuon_eventSelW"}; */
  /* if (tree->GetBranch("CRTightMuon_eventSelW") !=0 ) CRTightMuon_eventSelW = {  fReader,"CRTightMuon_eventSelW"}; */
  if (tree->GetBranch("LooseMuon_eventVetoW") !=0 ) LooseMuon_eventVetoW = {   fReader,"LooseMuon_eventVetoW"};
  if (tree->GetBranch("LooseMuonFix_eventVetoW") !=0 ) LooseMuonFix_eventVetoW = {   fReader,"LooseMuonFix_eventVetoW"};
  //  if (tree->GetBranch("LooseMuon_eventVetoW_systID_up") !=0 ) LooseMuon_eventVetoW_systID_up = { fReader,"LooseMuon_eventVetoW_systID_up"};
  //  if (tree->GetBranch("LooseMuon_eventVetoW_systID_down") !=0 ) LooseMuon_eventVetoW_systID_down = {    fReader,"LooseMuon_eventVetoW_systID_down"};
  //  if (tree->GetBranch("LooseMuon_eventVetoW_systISO_up") !=0 ) LooseMuon_eventVetoW_systISO_up = {fReader,"LooseMuon_eventVetoW_systISO_up"};
  //  if (tree->GetBranch("LooseMuon_eventVetoW_systISO_down") !=0 ) LooseMuon_eventVetoW_systISO_down = {   fReader,"LooseMuon_eventVetoW_systISO_down"};
  //  if (tree->GetBranch("VLooseTau_eventVetoW") !=0 ) VLooseTau_eventVetoW = {   fReader,"VLooseTau_eventVetoW"};
  /* if (tree->GetBranch("VLooseTau_eventVetoW_up") !=0 ) VLooseTau_eventVetoW_up = {fReader,"VLooseTau_eventVetoW_up"}; */
  /* if (tree->GetBranch("VLooseTau_eventVetoW_down") !=0 ) VLooseTau_eventVetoW_down = { fReader,"VLooseTau_eventVetoW_down"}; */
  //  if (tree->GetBranch("VLooseSITTau_eventVetoW") !=0 ) VLooseSITTau_eventVetoW = {fReader,"VLooseSITTau_eventVetoW"};
  /* if (tree->GetBranch("VLooseSITTau_eventVetoW_up") !=0 ) VLooseSITTau_eventVetoW_up = {fReader,"VLooseSITTau_eventVetoW_up"}; */
  /* if (tree->GetBranch("VLooseSITTau_eventVetoW_down") !=0 ) VLooseSITTau_eventVetoW_down = {   fReader,"VLooseSITTau_eventVetoW_down"}; */
  if (tree->GetBranch("VLooseTauFix_eventVetoW") !=0 ) VLooseTauFix_eventVetoW = {   fReader,"VLooseTauFix_eventVetoW"};
  /* if (tree->GetBranch("VLooseTauFix_eventVetoW_up") !=0 ) VLooseTauFix_eventVetoW_up = {fReader,"VLooseTauFix_eventVetoW_up"}; */
  /* if (tree->GetBranch("VLooseTauFix_eventVetoW_down") !=0 ) VLooseTauFix_eventVetoW_down = { fReader,"VLooseTauFix_eventVetoW_down"}; */

  if (tree->GetBranch("MediumBJet_eventVetoW") !=0 ) MediumBJet_eventVetoW = {  fReader,"MediumBJet_eventVetoW"};
  if (tree->GetBranch("HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60") !=0 ) HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60 = {fReader,"HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"};
  if (tree->GetBranch("HLT_PFMETNoMu120_PFMHTNoMu120_IDTight") !=0 ) HLT_PFMETNoMu120_PFMHTNoMu120_IDTight = {  fReader,"HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"};
  if (tree->GetBranch("HLT_DiJet110_35_Mjj650_PFMET110") !=0 ) HLT_DiJet110_35_Mjj650_PFMET110 = {fReader,"HLT_DiJet110_35_Mjj650_PFMET110"};
  if (tree->GetBranch("HLT_TripleJet110_35_35_Mjj650_PFMET110") !=0 ) HLT_TripleJet110_35_35_Mjj650_PFMET110 = {   fReader,"HLT_TripleJet110_35_35_Mjj650_PFMET110"};
  /* if (tree->GetBranch("HLT_Ele35_WPTight_Gsf") !=0 ) HLT_Ele35_WPTight_Gsf = {  fReader,"HLT_Ele35_WPTight_Gsf"}; */
  /* if (tree->GetBranch("HLT_Ele32_WPTight_Gsf") !=0 ) HLT_Ele32_WPTight_Gsf = {  fReader,"HLT_Ele32_WPTight_Gsf"}; */
  /* if (tree->GetBranch("HLT_IsoMu27") !=0 ) HLT_IsoMu27 = {  fReader,"HLT_IsoMu27"}; */
  /* if (tree->GetBranch("HLT_Ele32_WPTight_Gsf_L1DoubleEG") !=0 ) HLT_Ele32_WPTight_Gsf_L1DoubleEG = {    fReader,"HLT_Ele32_WPTight_Gsf_L1DoubleEG"}; */
  /* if (tree->GetBranch("HLT_Photon200") !=0 ) HLT_Photon200 = {    fReader,"HLT_Photon200"}; */

  if (tree->GetBranch("HLT_PFJet40") !=0 ) HLT_PFJet40 = {    fReader,"HLT_PFJet40"};
  if (tree->GetBranch("HLT_PFJet60") !=0 ) HLT_PFJet60 = {    fReader,"HLT_PFJet60"};
  if (tree->GetBranch("HLT_PFJet80") !=0 ) HLT_PFJet80 = {    fReader,"HLT_PFJet80"};
  if (tree->GetBranch("HLT_PFJet140") !=0 ) HLT_PFJet140 = {    fReader,"HLT_PFJet140"};
  if (tree->GetBranch("HLT_PFJet200") !=0 ) HLT_PFJet200 = {    fReader,"HLT_PFJet200"};
  if (tree->GetBranch("HLT_PFJet260") !=0 ) HLT_PFJet260 = {    fReader,"HLT_PFJet260"};
  if (tree->GetBranch("HLT_PFJet320") !=0 ) HLT_PFJet320 = {    fReader,"HLT_PFJet320"};
  if (tree->GetBranch("HLT_PFJet400") !=0 ) HLT_PFJet400 = {    fReader,"HLT_PFJet400"};
  if (tree->GetBranch("HLT_PFJet450") !=0 ) HLT_PFJet450 = {    fReader,"HLT_PFJet450"};
  if (tree->GetBranch("HLT_PFJet500") !=0 ) HLT_PFJet500 = {    fReader,"HLT_PFJet500"};
  if (tree->GetBranch("HLT_PFJet550") !=0 ) HLT_PFJet550 = {    fReader,"HLT_PFJet550"};


 if (tree->GetBranch("Met") !=0 )		 Met = { fReader, "Met"};		 
 if (tree->GetBranch("TkMET_pt") !=0 )	 TkMET_pt = { fReader, "TkMET_pt"};	 

 /* if (tree->GetBranch("SoftActivityJetHT10") !=0 ) SoftActivityJetHT10= { fReader, "SoftActivityJetHT10"};  */
 /* if (tree->GetBranch("softActivityJet1_eta") !=0 ) softActivityJet1_eta= { fReader,"softActivityJet1_eta"};  */
 /* if (tree->GetBranch("softActivityJet1_phi") !=0 ) softActivityJet1_phi= { fReader,"softActivityJet1_phi"};  */
 /* if (tree->GetBranch("softActivityJet2_eta") !=0 ) softActivityJet2_eta= { fReader,"softActivityJet2_eta"};  */
 /* if (tree->GetBranch("softActivityJet2_phi") !=0 ) softActivityJet2_phi= { fReader,"softActivityJet2_phi"};  */
 /* if (tree->GetBranch("softActivityJet3_eta") !=0 ) softActivityJet3_eta= { fReader,"softActivityJet3_eta"};  */
 /* if (tree->GetBranch("softActivityJet3_phi") !=0 ) softActivityJet3_phi= { fReader,"softActivityJet3_phi"};  */
 /* if (tree->GetBranch("softActivityJet4_eta") !=0 ) softActivityJet4_eta= { fReader,"softActivityJet4_eta"};  */
 /* if (tree->GetBranch("softActivityJet4_phi") !=0 ) softActivityJet4_phi= { fReader,"softActivityJet4_phi"};  */
 /* if (tree->GetBranch("softActivityJet5_eta") !=0 ) softActivityJet5_eta= { fReader,"softActivityJet5_eta"};  */
 /* if (tree->GetBranch("softActivityJet5_phi") !=0 ) softActivityJet5_phi= { fReader,"softActivityJet5_phi"};  */
 /* if (tree->GetBranch("softActivityJet6_eta") !=0 ) softActivityJet6_eta= { fReader,"softActivityJet6_eta"};  */
 /* if (tree->GetBranch("softActivityJet6_phi") !=0 ) softActivityJet6_phi= { fReader,"softActivityJet6_phi"};  */
 /* if (tree->GetBranch("isoTrack1_eta") !=0 )	 isoTrack1_eta= { fReader, "isoTrack1_eta"};	  */
 /* if (tree->GetBranch("isoTrack1_phi") !=0 )	 isoTrack1_phi= { fReader, "isoTrack1_phi"};	  */
 /* if (tree->GetBranch("isoTrack2_eta") !=0 )	 isoTrack2_eta= { fReader, "isoTrack2_eta"};	  */
 /* if (tree->GetBranch("isoTrack2_phi") !=0 )	 isoTrack2_phi= { fReader, "isoTrack2_phi"};	  */
 /* if (tree->GetBranch("isoTrack3_eta") !=0 )	 isoTrack3_eta= { fReader, "isoTrack3_eta"};	  */
 /* if (tree->GetBranch("isoTrack3_phi") !=0 ) isoTrack3_phi= { fReader, "isoTrack3_phi"};    */


 if (tree->GetBranch("jet_chf_nhf_cut") !=0 ) jet_chf_nhf_cut= { fReader, "jet_chf_nhf_cut"};   
 if (tree->GetBranch("jet_chf_nhf_vtr_cut") !=0 ) jet_chf_nhf_vtr_cut= { fReader, "jet_chf_nhf_vtr_cut"};   

 if (tree->GetBranch("horn_sel") !=0 ) horn_sel= { fReader, "horn_sel"};   
 if (tree->GetBranch("horn_sel_vtr") !=0 ) horn_sel_vtr= { fReader, "horn_sel_vtr"};  

 if (tree->GetBranch("VecBDPhiCutMTR") !=0 ) VecBDPhiCutMTR = { fReader, "VecBDPhiCutMTR"};   
 if (tree->GetBranch("VecBDPhiCutVTR") !=0 ) VecBDPhiCutVTR= { fReader, "VecBDPhiCutVTR"};  

 if (tree->GetBranch("MetNoLep") !=0 ) MetNoLep = {fReader, "MetNoLep"}; 
 if (tree->GetBranch("dijet_dPhi") !=0 ) dijet_dPhi = {fReader, "dijet_dPhi"}; 
 if (tree->GetBranch("JetMetmindPhi") !=0 ) JetMetmindPhi = {fReader, "JetMetmindPhi"}; 
 if (tree->GetBranch("dijet_dEta") !=0 ) dijet_dEta = {fReader, "dijet_dEta"}; 
 
   fReader.SetTree(tree);
}

Bool_t Events::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef Events_cxx
