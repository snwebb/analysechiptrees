#!/usr/bin/env python
import ROOT
import sys,os
import subprocess
sys.argv.append( '-b' )
ROOT.TH1.SetDefaultSumw2()

years = ["2017","2018"]
samples = ["MET","VV","TOP","DY","EWKZll","EWKZNUNU","ZJETS","EWKW","WJETS","QCD","QCDRELAX","DATA"]
regions = ["MTR","VTR"]
variables = ["diCleanJet_M_binned","Leading_jet_eta"]
#variables = ["MetNoLep_pt","diCleanJet_M","diCleanJet_dEta","diCleanJet_dPhi","Leading_jet_pt","Subleading_jet_pt","Leading_jet_eta","Subleading_jet_eta","nCleanJet30","MetNoLep_CleanJet_mindPhi","LHE_Vpt","LHE_HT","dijet_met_balance","lMjj_binned","diCleanJet_M_binned","diCleanJet_M_reduced"]
SRs = {}
CRs = {}
files = []
            
for region in regions:
    for variable in variables:
        for year in years:

            #samples = ["DATA","QCD","DY"]



            for sample in samples:
                files.append( ROOT.TFile.Open("~/invisible/AnalyseTrees/analysechiptrees/Plots/WITHHFHF/Histos_Nominal_"+sample+"_"+year+".root"))
                SRs[region + "_" + variable + "_" + year + "_WITHHFHF"] = files[-1].Get("SR/"+region+"/h_SR_"+region+"_" + variable)
                CRs[region + "_" + variable + "_" + year + "_WITHHFHF"] = files[-1].Get("QCDCR/"+region+"/h_QCDCR_"+region+"_" + variable)

            for sample in samples:
                files.append( ROOT.TFile.Open("~/invisible/AnalyseTrees/analysechiptrees/Plots/NOHFHF/Histos_Nominal_"+sample+"_"+year+".root"))
                SRs[region + "_" + variable + "_" + year + "_NOHFHF"] = files[-1].Get("SR/"+region+"/h_SR_"+region+"_" + variable)
                CRs[region + "_" + variable + "_" + year + "_NOHFHF"] = files[-1].Get("QCDCR/"+region+"/h_QCDCR_"+region+"_" + variable)



def draw(var = "diCleanJet_M_binned"):
    c1 = ROOT.TCanvas("c1")
    leg1 = ROOT.TLegend(0.6,0.7,0.89,0.89)
    leg1.SetBorderSize(0);
    CRs["MTR_"+var+"_2017_WITHHFHF"].Draw()
    CRs["MTR_"+var+"_2017_WITHHFHF"].SetMinimum(1)
    CRs["MTR_"+var+"_2017_NOHFHF"].SetLineColor(ROOT.kRed)
    CRs["MTR_"+var+"_2017_NOHFHF"].Draw("same")
    ROOT.gStyle.SetOptStat(0);
    ROOT.gPad.SetLogy(1);
    leg1.AddEntry(CRs["MTR_"+var+"_2017_WITHHFHF"],"With HFHF","LP")
    leg1.AddEntry(CRs["MTR_"+var+"_2017_NOHFHF"],"No HFHF","LP")
    leg1.Draw()
    c1.Draw()
    c1.SaveAs("compare_"+var+".png")
    c1.SaveAs("compare_"+var+".pdf")

draw("diCleanJet_M_binned")
draw("Leading_jet_eta")
