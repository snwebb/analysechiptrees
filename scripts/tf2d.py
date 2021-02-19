#!/usr/bin/env python
import sys,os
import subprocess
sys.argv.append( '-b' )
import ROOT
plotname = sys.argv[2]
if os.path.exists("Plots/"+plotname) == False:
    os.mkdir("Plots/"+plotname)

years = ["2017","2018"]
samples = ["MET","QCD","QCDRELAX","DY","EWKW","EWKZNUNU","EWKZll","GluGluHtoInv","TOP","VV","WJETS","ZJETS","DATA"]
regions = ["MTR","VTR"]

variables = ["MetNoLep_CleanJet_mindPhi_MetNoLep_pt","MetNoLep_CleanJet_mindPhi_diCleanJet_dPhi","MetNoLep_CleanJet_mindPhi_diCleanJet_dEta","MetNoLep_CleanJet_mindPhi_Leading_jet_pt","MetNoLep_CleanJet_mindPhi_Subleading_jet_pt","MetNoLep_CleanJet_mindPhi_dijet_met_balance","MetNoLep_CleanJet_mindPhi_Pileup_nPU","MetNoLep_CleanJet_mindPhi_diCleanJet_M","MetNoLep_CleanJet_mindPhi_lMjj","diCleanJet_dPhi_dijet_met_balance","diCleanJet_dEta_diCleanJet_M","lMjj_dijet_deta_lMjj","diCleanJet_dEta_LHE_HT","lMjj_dijet_deta_LHE_HT"]

files = {}
PlotDir = sys.argv[1]
for year in years:
    for sample in samples:
        files[(sample,year)] = ROOT.TFile("~/invisible/AnalyseTrees/analysechiptrees/"+PlotDir+"/Histos_Nominal_"+sample+"_"+year+".root","READ")

def NormalisePlot(hist):

    for y in range (1,hist.GetNbinsY()+1):
        normalisation = hist.Integral(1,hist.GetNbinsX()+1,y,y)
        if normalisation > 0:
            for x in range (1,hist.GetNbinsX()+1):
                hist.SetBinContent(x,y,hist.GetBinContent(x,y)/normalisation)
                hist.SetBinError(x,y,hist.GetBinError(x,y)/normalisation)

    return hist


def newplot(name,hist,xtitle,ytitle,ymin=None,ymax=None,norm=False):
    
    c = ROOT.TCanvas(name)
    if norm:
        NormalisePlot(hist)

    hist.Draw("COLZ")
    ROOT.gPad.SetLogz(True)
    hist.SetTitle(";" + xtitle + ";" + ytitle)
    if ( ymin != None ):
        hist.GetYaxis().SetRangeUser(ymin,ymax)
    ROOT.gStyle.SetOptStat(0)
    return c

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
            if ( variable == "MetNoLep_CleanJet_mindPhi_MetNoLep_pt" or variable == "MetNoLep_CleanJet_mindPhi_Pileup_nPU" or variable == "MetNoLep_CleanJet_mindPhi_diCleanJet_M" or variable == "MetNoLep_CleanJet_mindPhi_lMjj" or  variable == "MetNoLep_CleanJet_mindPhi_dijet_met_balance" or variable == "diCleanJet_dPhi_dijet_met_balance" or variable == "MetNoLep_CleanJet_mindPhi_diCleanJet_dPhi" or variable == "diCleanJet_dEta_diCleanJet_M" or variable == "lMjj_dijet_deta_lMjj" or variable == "diCleanJet_dEta_LHE_HT" or variable == "lMjj_dijet_deta_LHE_HT"):

                if ( variable.find("mindPhi")!=-1):
                    x = variable.split("mindPhi",1)[0]+"mindPhi" 
                    y = variable.split("mindPhi",1)[1][1:]
                elif (variable.find("diCleanJet_dEta")!=-1):
                    x = variable.split("dEta",1)[0]+"dEta" 
                    y = variable.split("dEta",1)[1][1:]
                elif (variable.find("lMjj_dijet_deta")!=-1):
                    x = variable.split("deta",1)[0]+"deta" 
                    y = variable.split("deta",1)[1][1:]
                else:
                    x = variable.split("dPhi",1)[0]+"dPhi" 
                    y = variable.split("dPhi",1)[1][1:]


                if variable == "MetNoLep_CleanJet_mindPhi_lMjj" or variable == "MetNoLep_CleanJet_mindPhi_diCleanJet_M":
                    x = "min#Delta#Phi(jets,E_{T}^{miss,no #mu})"
                    y = "M_{jj} (GeV)"
                ymin = None
                ymax = None
                    
                qcd = newplot(variable + "QCD",CRs[1],x,y,ymin,ymax, norm=True)
                qcd.Draw()
                qcd.SaveAs("Plots/" + plotname + "/QCD_" + variable + "_" + region + "_" + year + ".png" )
                qcd.Close()
                qcdrelax = newplot(variable + "QCDRELAX",CRs[2],x,y,ymin,ymax, norm=True)
                qcdrelax.Draw()
                qcdrelax.SaveAs("Plots/" + plotname + "/RELAX_" + variable + "_" + region + "_" + year + ".png" )
                qcdrelax.SaveAs("Plots/" + plotname + "/RELAX_" + variable + "_" + region + "_" + year + ".pdf" )
                qcdrelax.Close()
                data = newplot(variable + "MET",CRs[0],x,y,ymin,ymax, norm=True)
                data.Draw()
                data.SaveAs("Plots/" + plotname + "/MET_" + variable + "_" + region + "_" + year + ".png" )
                data.Close()



        file_out.Close()

bashCommand = "cd Plots; tar -zcf " + plotname + ".tar.gz " + plotname + "; cd ../;"
process = subprocess.Popen(bashCommand, shell=True)
