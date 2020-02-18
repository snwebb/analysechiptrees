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

// Headers needed by this particular selector

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

enum RegionType { SR=0, We=1, Wmu=2, Zee=3, Zmumu=4, QCDCR=5, QCDA=6, QCDB=7, Last };

//enum CatType { MTR=0, MTRvetoTau=1, MTRvetoB=2, MTRvetoLep=3, MTRveto=4, LastCat };
enum CatType { MTR=0, LastCat, MTRvetoTau=1, MTRvetoB=2, MTRvetoLep=3, MTRveto=4 };

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
   void SetMC(const bool aisMC);
   void SetSystematic(const std::string aSystematic);
   void SetLumiPb(const double & aLumi);
   void SetTreeContent(bool isMC,std::string year);
   bool CheckValue(ROOT::Internal::TTreeReaderValueBase& value);


   
 private:
   TFile *mFout;
   std::string mOutFile;
   std::vector<string> mVarVec;
   std::vector<TH1F*> mHistVec[RegionType::Last][CatType::LastCat];

   CatType mCat;
   RegionType mReg;
   TString mProc;
   std::string mYear;
   bool misMC;
   TString mSyst;
   double mLumiPb;
   
   Bool_t BaseSelection();
   Double_t BaseWeight();
   Bool_t PassSelection();
   Double_t SelWeight();

   std::map<std::string,double> lTreeContent;
   
   //public:
   ClassDef(Events,0);
   



   TTreeReaderValue<Double_t> decayLeptonId;
  TTreeReaderValue<Double_t> Zmumu_flag  ;
  TTreeReaderValue<Double_t> L1PreFiringWeight_Nom  ;
  TTreeReaderValue<Double_t> CRLooseMuon_eventSelW  ;
  TTreeReaderValue<Double_t> CRVetoElectron_eventSelW  ;
  TTreeReaderValue<Double_t> Leading_el_pt  ;
   TTreeReaderValue<Double_t> Subleading_el_phi  ;
   TTreeReaderValue<Double_t> LooseMuon_eventVetoW  ;
   TTreeReaderValue<Double_t> xs_weight  ;
   TTreeReaderValue<Double_t> HLT_Photon200  ;
   TTreeReaderValue<Double_t> Wmunu_flag  ;
   TTreeReaderValue<Double_t> HLT_PFMETNoMu120_PFMHTNoMu120_IDTight  ;
   TTreeReaderValue<Double_t> nMediumBJet  ;
   TTreeReaderValue<Double_t> Leading_el_eta  ;
   TTreeReaderValue<Double_t> GenMET_pt  ;
   TTreeReaderValue<Double_t> Zee_flag  ;
   TTreeReaderValue<Double_t> HLT_Ele32_WPTight_Gsf  ;
   TTreeReaderValue<Double_t> met_filters_2017_mc  ;
   TTreeReaderValue<Double_t> HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60  ;
   TTreeReaderValue<Double_t> Subleading_jet_eta  ;
   TTreeReaderValue<Double_t> Wenu_flag  ;
   TTreeReaderValue<Double_t> Pileup_nTrueInt  ;
   TTreeReaderValue<Double_t> HLT_TripleJet110_35_35_Mjj650_PFMET110  ;
   TTreeReaderValue<Double_t> Met  ;
   TTreeReaderValue<Double_t> dijet_M  ;
   TTreeReaderValue<Double_t> nVLooseTau  ;
   TTreeReaderValue<Double_t> Pileup_nPU  ;
   TTreeReaderValue<Double_t> gen_pt0  ;
   TTreeReaderValue<Double_t> MetPhiNoLep  ;
   TTreeReaderValue<Double_t> VLooseSITTau_eventVetoW  ;
   TTreeReaderValue<Double_t> VLooseTau_eventVetoW  ;
   TTreeReaderValue<Double_t> nVetoElectron  ;
   TTreeReaderValue<Double_t> Leading_jet_pt  ;
   TTreeReaderValue<Double_t> DiVetoElectron_mass  ;
   TTreeReaderValue<Double_t> CRTightElectron_eventSelW  ;
   TTreeReaderValue<Double_t> HLT_DiJet110_35_Mjj650_PFMET110  ;
   TTreeReaderValue<Double_t> gen_mjj  ;
   TTreeReaderValue<Double_t> MetPhi  ;
   TTreeReaderValue<Double_t> MediumBJet_eventVetoW  ;
   TTreeReaderValue<Double_t> LHE_Nuds  ;
   TTreeReaderValue<Double_t> Leading_jet_phi  ;
   TTreeReaderValue<Double_t> LHE_HT  ;
   TTreeReaderValue<Double_t> nloSF_  ;
   TTreeReaderValue<Double_t> Leading_muon_phi  ;
   TTreeReaderValue<Double_t> Subleading_jet_phi  ;
   TTreeReaderValue<Double_t> Subleading_el_pt  ;
   TTreeReaderValue<Double_t> Leading_jet_eta  ;
   TTreeReaderValue<Double_t> met_filters_2017_data  ;
   TTreeReaderValue<Double_t> HLT_IsoMu27  ;
   TTreeReaderValue<Double_t> CRTightMuon_eventSelW  ;
   TTreeReaderValue<Double_t> puWeight  ;
   TTreeReaderValue<Double_t> MetNoLep  ;
   TTreeReaderValue<Double_t> LHE_Vpt  ;
   TTreeReaderValue<Double_t> dijet_dPhi  ;
   TTreeReaderValue<Double_t> Subleading_muon_phi  ;
   TTreeReaderValue<Double_t> Subleading_muon_eta  ;
   TTreeReaderValue<Double_t> Subleading_el_eta  ;
   TTreeReaderValue<Double_t> VetoElectron_eventVetoW  ;
   TTreeReaderValue<Double_t> HLT_Ele35_WPTight_Gsf  ;
   TTreeReaderValue<Double_t> HLT_Ele32_WPTight_Gsf_L1DoubleEG  ;
   TTreeReaderValue<Double_t> PV_npvsGood  ;
   TTreeReaderValue<Double_t> Subleading_muon_pt  ;
   TTreeReaderValue<Double_t> JetMetmindPhi  ;
   TTreeReaderValue<Double_t> isData  ;
   TTreeReaderValue<Double_t> nJets  ;
   TTreeReaderValue<Double_t> DiLooseMuon_mass  ;
   TTreeReaderValue<Double_t> LHE_Nb  ;
   TTreeReaderValue<Double_t> dijet_dEta  ;
   TTreeReaderValue<Double_t> Leading_el_phi  ;
   TTreeReaderValue<Double_t> gen_boson_pt  ;
   TTreeReaderValue<Double_t> nGenDressedLepton  ;
   TTreeReaderValue<Double_t> Leading_muon_pt  ;
   TTreeReaderValue<Double_t> nLoosePhoton  ;
   TTreeReaderValue<Double_t> nLooseMuon  ;
   TTreeReaderValue<Double_t> nCleanJet30  ;
   TTreeReaderValue<Double_t> Subleading_jet_pt  ;
   TTreeReaderValue<Double_t> LHE_Njets  ;
   TTreeReaderValue<Double_t> LHE_Nglu  ;
   TTreeReaderValue<Double_t> Leading_muon_eta  ;
   TTreeReaderValue<Double_t> VetoElectron_eventSelW  ;
   TTreeReaderValue<Double_t> LooseMuon_eventSelW  ;
   TTreeReaderValue<Double_t> GenMET_phi  ;
   TTreeReaderValue<Double_t> LHE_Nc  ;

   TTreeReaderValue<Double_t> LooseMuon_eventVetoW_systID_up  ;
   TTreeReaderValue<Double_t> VetoElectron_eventVetoW_systIDISO_up  ;
   TTreeReaderValue<Double_t> LooseMuon_eventVetoW_systID_down  ;
   TTreeReaderValue<Double_t> VetoElectron_eventVetoW_systIDISO_down  ;
   TTreeReaderValue<Double_t> VLooseSITTau_eventVetoW_up  ;
   TTreeReaderValue<Double_t> VLooseSITTau_eventVetoW_down  ;
   TTreeReaderValue<Double_t> MediumBJet_eventVetoW_up  ;
   TTreeReaderValue<Double_t> MediumBJet_eventVetoW_down  ;
  

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
    Zmumu_flag = {fReader, "Zmumu_flag"};
    decayLeptonId = {fReader, "decayLeptonId"};

    if (tree->GetBranch(   "L1PreFiringWeight_Nom")   !=0 ) L1PreFiringWeight_Nom = {fReader, "L1PreFiringWeight_Nom"};      

    CRLooseMuon_eventSelW = {fReader, "CRLooseMuon_eventSelW"};
    CRVetoElectron_eventSelW = {fReader, "CRVetoElectron_eventSelW"};
    Leading_el_pt = {fReader, "Leading_el_pt"};
    Subleading_el_phi = {fReader, "Subleading_el_phi"};
    LooseMuon_eventVetoW = {fReader, "LooseMuon_eventVetoW"};
    xs_weight = {fReader, "xs_weight"};
    HLT_Photon200 = {fReader, "HLT_Photon200"};
    Wmunu_flag = {fReader, "Wmunu_flag"};
    HLT_PFMETNoMu120_PFMHTNoMu120_IDTight = {fReader, "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"};
    nMediumBJet = {fReader, "nMediumBJet"};
    Leading_el_eta = {fReader, "Leading_el_eta"};
    GenMET_pt = {fReader, "GenMET_pt"};
    Zee_flag = {fReader, "Zee_flag"};
    HLT_Ele32_WPTight_Gsf = {fReader, "HLT_Ele32_WPTight_Gsf"};
    met_filters_2017_mc = {fReader, "met_filters_2017_mc"};
    HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60 = {fReader, "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60"};
    Subleading_jet_eta = {fReader, "Subleading_jet_eta"};
    Wenu_flag = {fReader, "Wenu_flag"};
    Pileup_nTrueInt = {fReader, "Pileup_nTrueInt"};
    HLT_TripleJet110_35_35_Mjj650_PFMET110 = {fReader, "HLT_TripleJet110_35_35_Mjj650_PFMET110"};
    Met = {fReader, "Met"};
    dijet_M = {fReader, "dijet_M"};
    nVLooseTau = {fReader, "nVLooseTau"};
    Pileup_nPU = {fReader, "Pileup_nPU"};
    gen_pt0 = {fReader, "gen_pt0"};
    MetPhiNoLep = {fReader, "MetPhiNoLep"};
    VLooseSITTau_eventVetoW = {fReader, "VLooseSITTau_eventVetoW"};
    VLooseTau_eventVetoW = {fReader, "VLooseTau_eventVetoW"};
    nVetoElectron = {fReader, "nVetoElectron"};
    Leading_jet_pt = {fReader, "Leading_jet_pt"};
    DiVetoElectron_mass = {fReader, "DiVetoElectron_mass"};
    CRTightElectron_eventSelW = {fReader, "CRTightElectron_eventSelW"};
    HLT_DiJet110_35_Mjj650_PFMET110 = {fReader, "HLT_DiJet110_35_Mjj650_PFMET110"};
    gen_mjj = {fReader, "gen_mjj"};
    MetPhi = {fReader, "MetPhi"};
    MediumBJet_eventVetoW = {fReader, "MediumBJet_eventVetoW"};
    LHE_Nuds = {fReader, "LHE_Nuds"};
    Leading_jet_phi = {fReader, "Leading_jet_phi"};
    LHE_HT = {fReader, "LHE_HT"};
    nloSF_ = {fReader, "nloSF_"};
    Leading_muon_phi = {fReader, "Leading_muon_phi"};
    Subleading_jet_phi = {fReader, "Subleading_jet_phi"};
    Subleading_el_pt = {fReader, "Subleading_el_pt"};
    Leading_jet_eta = {fReader, "Leading_jet_eta"};
    met_filters_2017_data = {fReader, "met_filters_2017_data"};
    HLT_IsoMu27 = {fReader, "HLT_IsoMu27"};
    CRTightMuon_eventSelW = {fReader, "CRTightMuon_eventSelW"};
    puWeight = {fReader, "puWeight"};
    MetNoLep = {fReader, "MetNoLep"};
    LHE_Vpt = {fReader, "LHE_Vpt"};
    dijet_dPhi = {fReader, "dijet_dPhi"};
    Subleading_muon_phi = {fReader, "Subleading_muon_phi"};
    Subleading_muon_eta = {fReader, "Subleading_muon_eta"};
    Subleading_el_eta = {fReader, "Subleading_el_eta"};
    VetoElectron_eventVetoW = {fReader, "VetoElectron_eventVetoW"};
    HLT_Ele35_WPTight_Gsf = {fReader, "HLT_Ele35_WPTight_Gsf"};
    HLT_Ele32_WPTight_Gsf_L1DoubleEG = {fReader, "HLT_Ele32_WPTight_Gsf_L1DoubleEG"};
    PV_npvsGood = {fReader, "PV_npvsGood"};
    Subleading_muon_pt = {fReader, "Subleading_muon_pt"};
    JetMetmindPhi = {fReader, "JetMetmindPhi"};
    isData = {fReader, "isData"};
    nJets = {fReader, "nJets"};
    DiLooseMuon_mass = {fReader, "DiLooseMuon_mass"};
    LHE_Nb = {fReader, "LHE_Nb"};
    dijet_dEta = {fReader, "dijet_dEta"};
    Leading_el_phi = {fReader, "Leading_el_phi"};
    gen_boson_pt = {fReader, "gen_boson_pt"};
    nGenDressedLepton = {fReader, "nGenDressedLepton"};
    Leading_muon_pt = {fReader, "Leading_muon_pt"};
    nLoosePhoton = {fReader, "nLoosePhoton"};
    nLooseMuon = {fReader, "nLooseMuon"};
    nCleanJet30 = {fReader, "nCleanJet30"};
    Subleading_jet_pt = {fReader, "Subleading_jet_pt"};
    LHE_Njets = {fReader, "LHE_Njets"};
    LHE_Nglu = {fReader, "LHE_Nglu"};
    Leading_muon_eta = {fReader, "Leading_muon_eta"};
    VetoElectron_eventSelW = {fReader, "VetoElectron_eventSelW"};
    LooseMuon_eventSelW = {fReader, "LooseMuon_eventSelW"};
    GenMET_phi = {fReader, "GenMET_phi"};
    LHE_Nc = {fReader, "LHE_Nc"};

   /*  LooseMuon_eventVetoW_systID_up = {fReader, "LooseMuon_eventVetoW_systID_up"}; */
   /*  VetoElectron_eventVetoW_systIDISO_up = {fReader, "VetoElectron_eventVetoW_systIDISO_up"}; */
   /*  LooseMuon_eventVetoW_systID_down = {fReader, "LooseMuon_eventVetoW_systID_down"}; */
   /*  VetoElectron_eventVetoW_systIDISO_down = {fReader, "VetoElectron_eventVetoW_systIDISO_down"}; */
   /*  VLooseSITTau_eventVetoW_up = {fReader, "VLooseSITTau_eventVetoW_up"}; */
   /*  VLooseSITTau_eventVetoW_down = {fReader, "VLooseSITTau_eventVetoW_down"}; */
   /*  MediumBJet_eventVetoW_up = {fReader, "MediumBJet_eventVetoW_up"}; */
   /*  MediumBJet_eventVetoW_down = {fReader, "MediumBJet_eventVetoW_down"}; */


    //  if ( mYear == "2017" ) L1PreFiringWeight_Nom = {fReader, "L1PreFiringWeight_Nom"};     


    //    std::cout << tree->GetName() << std::endl;

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
