#!/usr/bin/env python
import sys,os
sys.argv.append( '-b' )
import ROOT
#years = ["2017","2018"]
years = ["2017"]
samples = ["MET","QCD","QCDRELAX","DY","EWKW","EWKZNUNU","EWKZll","GluGluHtoInv","TOP","VV","WJETS","ZJETS","DATA"]
regions = ["MTR","VTR"]

variables = ["MetNoLep_CleanJet_mindPhi_MetNoLep_pt","MetNoLep_CleanJet_mindPhi_diCleanJet_dPhi","MetNoLep_CleanJet_mindPhi_diCleanJet_dEta","MetNoLep_CleanJet_mindPhi_Leading_jet_pt","MetNoLep_CleanJet_mindPhi_Subleading_jet_pt","MetNoLep_CleanJet_mindPhi_dijet_met_balance","MetNoLep_CleanJet_mindPhi_Pileup_nPU","MetNoLep_CleanJet_mindPhi_diCleanJet_M","MetNoLep_CleanJet_mindPhi_lMjj","diCleanJet_dPhi_dijet_met_balance","diCleanJet_dEta_diCleanJet_M"]

files = {}
PlotDir = "Plots"
for year in years:
    for sample in samples:
        files[(sample,year)] = ROOT.TFile("~/invisible/AnalyseTrees/analysechiptrees/"+PlotDir+"/Histos_Nominal_"+sample+"_"+year+".root","READ")

def newplot(name,hist,xtitle,ytitle,ymin=None,ymax=None):
    
    c = ROOT.TCanvas(name)
    hist.Draw("COLZ")
    ROOT.gPad.SetLogz(True)
    hist.SetTitle(";" + xtitle + ";" + ytitle)
    if ( ymin != None ):
        hist.GetYaxis().SetRangeUser(ymin,ymax)
    ROOT.gStyle.SetOptStat(0)
    return c

#For Nick's method
#variables = ["MetNoLep_CleanJet_mindPhi_MetNoLep_pt"]
for region in regions:
    for year in years:

        file_out = ROOT.TFile("out2D_" + region + "_" + year+ ".root","RECREATE")

        for variable in variables:

            file_out.cd()
            #files = []
            SRs = []
            CRs = []
            As = []
            Bs = []

            for sample in samples:
                SRs.append(files[(sample,year)].Get("SR/"+region+"/h_SR_"+region+"_" + variable))
                CRs.append(files[(sample,year)].Get("QCDCR/"+region+"/h_QCDCR_"+region+"_" + variable))
                As.append(files[(sample,year)].Get("QCDA/"+region+"/h_QCDA_"+region+"_" + variable))
                Bs.append(files[(sample,year)].Get("QCDB/"+region+"/h_QCDB_"+region+"_" + variable))
            

            file_out.cd()
            file_out.mkdir(variable)
            file_out.cd(variable)

            for CR,SR in zip(CRs,SRs):
                CR.Add(SR)

            for A,B in zip(As,Bs):
                A.Add(B)

            for i,CR in enumerate(CRs):
                if ( samples[i] == "DATA" or  samples[i] == "QCD"):
                    CR.Write(samples[i]+"_CR")
            for i,A in enumerate(As):
                if ( samples[i] == "DATA" or  samples[i] == "QCD"):
                    A.Write(samples[i]+"_A")

            BackgroundSum_CR = CRs[0].Clone("BackgroundSum_CR")
            BackgroundSum_CR.Reset()

            BackgroundSum_A = As[0].Clone("BackgroundSum_A")
            BackgroundSum_A.Reset()

            for i,cr in enumerate(CRs):
                if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX" or samples[i] == "MET"):
                    continue
                BackgroundSum_CR.Add(cr)

            BackgroundSum_CR.Write()

            for i,a in enumerate(As):
                if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX"  or samples[i] == "MET"):
                    continue
                BackgroundSum_A.Add(a)

            BackgroundSum_A.Write()

            #Interesting Plots
            if ( variable == "MetNoLep_CleanJet_mindPhi_MetNoLep_pt" or variable == "MetNoLep_CleanJet_mindPhi_Pileup_nPU" or variable == "MetNoLep_CleanJet_mindPhi_diCleanJet_M" or variable == "MetNoLep_CleanJet_mindPhi_lMjj" or  variable == "MetNoLep_CleanJet_mindPhi_dijet_met_balance" or variable == "diCleanJet_dPhi_dijet_met_balance" or variable == "MetNoLep_CleanJet_mindPhi_diCleanJet_dPhi"):

                if ( variable.find("mindPhi")!=-1):
                    x = variable.split("mindPhi",1)[0]+"mindPhi" 
                    y = variable.split("mindPhi",1)[1][1:]
                elif (variable.find("diCleanJet_dEta_diCleanJet_M")!=-1):
                    x = variable.split("dEta",1)[0]+"dEta" 
                    y = variable.split("dEta",1)[1][1:]
                else:
                    x = variable.split("dPhi",1)[0]+"dPhi" 
                    y = variable.split("dPhi",1)[1][1:]

                print (x)
                print (y)
                ymin = None
                ymax = None
                if ( variable == "MetNoLep_CleanJet_mindPhi_MetNoLep_pt" ):
                    ymin = 100
                    ymax = 160
                    
                qcd = newplot(variable + "QCD",CRs[1],x,y,ymin,ymax)
                qcd.Draw()
                qcd.SaveAs("QCD_" + variable + "_" + region + "_" + year + ".png" )
                qcd.Close()
                data = newplot(variable + "MET",CRs[0],x,y,ymin,ymax)
                data.Draw()
                data.SaveAs("MET_" + variable + "_" + region + "_" + year + ".png" )
                data.Close()




        file_out.Close()

