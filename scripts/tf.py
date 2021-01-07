#!/usr/bin/env python
import sys,os
import subprocess
from array import array
sys.argv.append( '-b' )
# os.mkdir("CR")
# os.mkdir("A")
# os.mkdir("B")
import ROOT
ROOT.TH1.SetDefaultSumw2()

directory_name = sys.argv[1]
plotname = sys.argv[2]
log = int(sys.argv[3])
##directory_name = "Plots/20201016-Histos-DeltaEtaLessThan5"
#directory_name = "Plots/20201016-Histos-DeltaEtaGreaterThan5"
#directory_name = "Plots/20201016-Histos-Full"
#directory_name = "Plots/"


if os.path.exists("Plots/"+plotname) == False:
    os.mkdir("Plots/"+plotname)
if os.path.exists("Plots/"+plotname+"/A") == False:
    os.mkdir("Plots/"+plotname+"/A")
if os.path.exists("Plots/"+plotname+"/B") == False:
    os.mkdir("Plots/"+plotname+"/B")
if os.path.exists("Plots/"+plotname+"/SR") == False:
    os.mkdir("Plots/"+plotname+"/SR")
if os.path.exists("Plots/"+plotname+"/CR") == False:
    os.mkdir("Plots/"+plotname+"/CR")

#listOfSysts = ["Nominal","ElectronVetoUp","ElectronVetoDown","MuonVetoUp","MuonVetoDown","TauVetoUp","TauVetoDown","BjetVetoUp","BjetVetoDown"]
#listOfSysts = ["Nominal","ElectronVetoUp","MuonVetoUp","TauVetoUp","BjetVetoUp"]
#listOfSysts = ["Nominal"]

#samples = ["DATA","QCD","DY","EWKW","EWKZNUNU","EWKZll","GluGluHtoInv","TOP","VV","WJETS","ZJETS","MET"]
years = ["2017","2018"]
#years = ["2017"]
#samples = ["MET","QCD","DY","EWKW","EWKZNUNU","EWKZll","GluGluHtoInv","SingleElectron","TOP","VV","WJETS","ZJETS"]
#samples = ["MET","QCD","QCDRELAX","DY","EWKW","EWKZNUNU","EWKZll","GluGluHtoInv","TOP","VV","WJETS","ZJETS","DATA"]
#samples = ["MET","VV","TOP","DY","EWKZll","GluGluHtoInv","VBFHtoInv","EWKZNUNU","ZJETS","EWKW","WJETS","QCD","QCDRELAX","DATA"]
samples = ["MET","VV","TOP","DY","EWKZll","EWKZNUNU","ZJETS","EWKW","WJETS","QCD","QCDRELAX","DATA"]
regions = ["MTR","VTR"]
#variables = ["diCleanJet_M_binned"]

#variables = ["MetNoLep_pt","diCleanJet_M","diCleanJet_dEta","diCleanJet_dEtaCmF","CentralEtaMTR","ForwardEtaMTR","CentralEtaVTR","ForwardEtaVTR","diCleanJet_dPhi","Leading_jet_pt","Subleading_jet_pt","Leading_jet_eta","Subleading_jet_eta","nCleanJet30","MetNoLep_CleanJet_mindPhi","LHE_Vpt","LHE_HT","dijet_met_balance","lMjj_binned","diCleanJet_M_binned","diCleanJet_M_binned_reduced","lMjj_dijet_deta","lMjj_dijet_detaCmF", "lMjj_dijet_dphi"]

#variables = ["MetNoLep_pt","diCleanJet_M","diCleanJet_M_LeadingPosEta","diCleanJet_M_LeadingNegEta","diCleanJet_dEta","CentralEtaMTR","ForwardEtaMTR","CentralEtaVTR","ForwardEtaVTR","diCleanJet_dPhi","Leading_jet_pt","Subleading_jet_pt","Leading_jet_eta","Subleading_jet_eta","nCleanJet30","MetNoLep_CleanJet_mindPhi","LHE_Vpt","LHE_HT","dijet_met_balance","lMjj_binned","diCleanJet_M_binned","diCleanJet_M_binned_reduced","lMjj_dijet_deta", "lMjj_dijet_dphi"]

variables = ["MetNoLep_pt","diCleanJet_M","diCleanJet_dEta","CentralEtaMTR","ForwardEtaMTR","CentralEtaVTR","ForwardEtaVTR","diCleanJet_dPhi","Leading_jet_pt","Subleading_jet_pt","Leading_jet_eta","Subleading_jet_eta","MetNoLep_CleanJet_mindPhi","lMjj_binned","diCleanJet_M_binned","lMjj_dijet_deta", "lMjj_dijet_dphi"]

# nick = {}
# nick[("MTR","2017")] = ROOT.TFile("out_MTR_2017.root_qcdDD.root","READ")
# nick[("VTR","2017")] = ROOT.TFile("out_VTR_2017.root_qcdDD.root","READ")
# nick[("MTR","2018")] = ROOT.TFile("out_MTR_2018.root_qcdDD.root","READ")
# nick[("VTR","2018")] = ROOT.TFile("out_VTR_2018.root_qcdDD.root","READ")

colors = {}
colors["VV"] = '#7bc345'
colors["TOP"]  =  '#CF3721'
colors["DY"] =  '#9767a3'
colors["EWKZll"]  = '#c46c9c'
colors["EWKZNUNU"]  = '#457bc3'
colors["ZJETS"]  =  '#14dbf9'
colors["EWKW"]   =  '#f28d84'
colors["WJETS"]     =  '#febc67'
colors["QCD"]     =  '#ffffff'
colors["GluGluHtoInv"]     =  '#34eb9f'
colors["VBFHtoInv"]     =  '#ebeb34'


for region in regions:
    for variable in variables:
        for year in years:

            #samples = ["DATA","QCD","DY"]
            files = []

            SRs = []
            CRs = []
            As = []
            Bs = []
            TFs = []

            #year=sys.argv[1]

            # for syst in listOfSysts:
            #     files.append( ROOT.TFile.Open("../Plots/Histos_"+syst+"_QCD_2017.root"))
            #     SRs.append(files[-1].Get("SR/MTR/h_SR_MTR_dijet_M_binned"))
            #     CRs.append(files[-1].Get("QCDCR/MTR/h_QCDCR_MTR_dijet_M_binned"))


            #for syst in listOfSysts:
            for sample in samples:
                files.append( ROOT.TFile.Open("~/invisible/AnalyseTrees/analysechiptrees/"+directory_name+"/Histos_Nominal_"+sample+"_"+year+".root"))
                Bs.append(files[-1].Get("QCDB/"+region+"/h_QCDB_"+region+"_" + variable))
                As.append(files[-1].Get("QCDA/"+region+"/h_QCDA_"+region+"_" + variable))
                SRs.append(files[-1].Get("SR/"+region+"/h_SR_"+region+"_" + variable))
                CRs.append(files[-1].Get("QCDCR/"+region+"/h_QCDCR_"+region+"_" + variable))

            #rebin if necessary
            # for i,(A,B,SR,CR) in enumerate(zip(As,Bs,SRs,CRs)):
            #     if variable == "diCleanJet_M_binned":
            #         mjjbins=array('d',[0,200,400,600,900,1200,1500,2000,2750,5000])
            #         As[i] = A.Rebin(len(mjjbins)-1,"A_rebinned",mjjbins)
            #         Bs[i] = B.Rebin(len(mjjbins)-1,"B_rebinned",mjjbins)
            #         CRs[i] = CR.Rebin(len(mjjbins)-1,"CR_rebinned",mjjbins)
            #         SRs[i] = SR.Rebin(len(mjjbins)-1,"SR_rebinned",mjjbins)
            #     elif variable == "lMjj_binned":
            #         mjjbins=array('d',[0,900,1200,1500,2000,5000])
            #         As[i] = A.Rebin(len(mjjbins)-1,"A_rebinned",mjjbins)
            #         Bs[i] = B.Rebin(len(mjjbins)-1,"B_rebinned",mjjbins)
            #         CRs[i] = CR.Rebin(len(mjjbins)-1,"CR_rebinned",mjjbins)
            #         SRs[i] = SR.Rebin(len(mjjbins)-1,"SR_rebinned",mjjbins)
                #print (SRs[-1].Integral(),CRs[-1].Integral())

            #Get Data - background subtracted
            #DRAWING


            if (  (variable=="diCleanJet_M_binned" and region == "MTR" ) or (variable=="lMjj_binned" and region == "VTR" )  ):
                
                file_out = ROOT.TFile("Plots/"+plotname + "/out_" + region + "_" + year+ ".root","RECREATE")
                BackgroundSubtractedData_SR = SRs[0].Clone("BackgroundSubtractedData_SR")
                BackgroundSubtractedData_CR = CRs[0].Clone("BackgroundSubtractedData_CR")
                BackgroundSubtractedData_A = As[-1].Clone("BackgroundSubtractedData_A")
                BackgroundSubtractedData_B = Bs[-1].Clone("BackgroundSubtractedData_B")


                for i,SR in enumerate(SRs):
                    if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv"):
                        continue
                    BackgroundSubtractedData_SR.Add(SR,-1)
                for i,CR in enumerate(CRs):
                    if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv"):
                        continue
                    BackgroundSubtractedData_CR.Add(CR,-1)
                for i,A in enumerate(As):
                    if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX"  or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv"):
                        continue
                    BackgroundSubtractedData_A.Add(A,-1)
                for i,B in enumerate(Bs):
                    if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX"  or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv"):
                        continue
                    BackgroundSubtractedData_B.Add(B,-1)

                #Get QCD Transfer factor
                #QCD relax = -2
                #QCD normal = -3

                qcdchoice = -3

                QCDTransferFactor_SR = SRs[qcdchoice].Clone("QCDTransferFactor_SR")
                QCDTransferFactor_B = Bs[qcdchoice].Clone("QCDTransferFactor_B")

                QCDTransferFactor_SR.Divide( CRs[qcdchoice] )
                QCDTransferFactor_B.Divide( As[qcdchoice] )

                #Get Final QCD prediction

                FinalQCDSR = BackgroundSubtractedData_CR.Clone("FinalQCD_SR")
                FinalQCDB = BackgroundSubtractedData_A.Clone("FinalQCD_B")

                FinalQCDSR.Multiply(QCDTransferFactor_SR)
                FinalQCDB.Multiply(QCDTransferFactor_B)

                QCDMC_SR = SRs[-3].Clone("QCDMC_SR")
                QCDMC_CR = CRs[-3].Clone("QCDMC_CR")

                QCDMC_B = Bs[-3].Clone("QCDMC_B")
                JETHT_B = Bs[-1].Clone("JETHT_B")



                #Write all relevant hists:

                for i,(sr,cr,a,b) in enumerate(zip(SRs,CRs,As,Bs)):
                    sr.Write(samples[i]+"_SR")
                    cr.Write(samples[i]+"_CR")
                    a.Write(samples[i]+"_A")
                    b.Write(samples[i]+"_B")

                QCDTransferFactor_SR.Write()
                QCDTransferFactor_B.Write()

                BackgroundSubtractedData_A.Write()
                BackgroundSubtractedData_B.Write()
                BackgroundSubtractedData_CR.Write()
                BackgroundSubtractedData_SR.Write()

                FinalQCDSR.Write()
                FinalQCDB.Write()

                QCDMC_SR.Write()
                QCDMC_CR.Write()

                #QCD TRANSFER FACTORS
                c_1 = ROOT.TCanvas("QCDTransferFactor_SR")
                QCDTransferFactor_SR.Draw()
                QCDTransferFactor_SR.GetYaxis().SetTitle("Transfer factor, r")
                QCDTransferFactor_SR.SetMinimum(-0.0001)
                ROOT.gStyle.SetOptStat(0);
                c_1.Draw()
                c_1.SaveAs("Plots/"+plotname + "/QCDTransferFactor_SR_" + region + "_" + year + ".png" )
                c_1.SaveAs("Plots/"+plotname + "/QCDTransferFactor_SR_" + region + "_" + year + ".pdf" )
                c_1.Close()

                c_2 = ROOT.TCanvas("QCDTransferFactor_B")
                QCDTransferFactor_B.Draw()
                QCDTransferFactor_B.GetYaxis().SetTitle("Transfer factor, r")
                ROOT.gStyle.SetOptStat(0);
                c_2.Draw()
                c_2.SaveAs("Plots/"+plotname + "/QCDTransferFactor_B_" + region + "_" + year + ".png" )
                c_2.SaveAs("Plots/"+plotname + "/QCDTransferFactor_B_" + region + "_" + year + ".pdf" )
                c_2.Close()

                #FINAL QCD
                #nicks = nick[(region,year)].Get("rebin_QCD_hist_counts")
                c_3 = ROOT.TCanvas("FinalQCD")
                FinalQCDSR.Draw()
                FinalQCDSR.GetYaxis().SetTitle("Number of Events")
                FinalQCDSR.GetYaxis().SetRangeUser(0.1,50000)
                QCDMC_SR.SetLineColor(ROOT.kRed)
                QCDMC_SR.Draw("same")
                #nicks.SetLineColor(ROOT.kBlack)
                #nicks.SetMarkerSize(0)
                #nicks.Draw("Esame")
                print ( "Data-Driven MJ prediction in " + region + " in " + year + " is "  + str(FinalQCDSR.Integral()) )
                print ( "MC MJ prediction in " + region + " in " + year + " is "  + str(QCDMC_SR.Integral()) )

                ROOT.gPad.SetLogy(1)
                leg_3 = ROOT.TLegend(0.4,0.6,0.89,0.89)
                leg_3.SetBorderSize(0);
                #leg_3.AddEntry(FinalQCDSR,"Estimate using QCD MC transfer factor","L")
                leg_3.AddEntry(FinalQCDSR,"Data driven method A","L")
                leg_3.AddEntry(QCDMC_SR,"QCD MC","L")
                #leg_3.AddEntry(nicks,"Fully data-driven estimate","L")
                #leg_3.AddEntry(nicks,"Data driven method B","L")
                leg_3.Draw()

                ROOT.gStyle.SetOptStat(0);
                c_3.Draw()
                c_3.SaveAs("Plots/"+plotname + "/FinalQCDSR_" + region + "_" + year + ".png" )
                c_3.SaveAs("Plots/"+plotname + "/FinalQCDSR_" + region + "_" + year + ".pdf" )
                c_3.Close()

                #FINAL QCD CROSS CHECK
                c_4 = ROOT.TCanvas("FinalQCD")
                FinalQCDB.Draw()
                if ( region == "MTR"):
                    FinalQCDB.GetYaxis().SetRangeUser(0,3000)
                if ( region == "VTR"):
                    FinalQCDB.GetYaxis().SetRangeUser(0,1000)
                FinalQCDB.GetYaxis().SetTitle("Number of Events")
                # QCDMC_B.SetLineColor(ROOT.kRed)
                # QCDMC_B.Draw("same")
                JETHT_B.SetLineColor(ROOT.kRed)
                JETHT_B.Draw("same")
                leg_4 = ROOT.TLegend(0.5,0.5,0.89,0.89)
                leg_4.SetBorderSize(0);
                leg_4.AddEntry(FinalQCDB,"Data-driven QCD prediction","L")
                #leg_4.AddEntry(QCDMC_B,"Estimate from using QCD MC","L")
                leg_4.AddEntry(JETHT_B,"Data in \"B\" region","L")
                leg_4.Draw()
                ROOT.gStyle.SetOptStat(0);
                c_4.Draw()
                c_4.SaveAs("Plots/"+plotname + "/FinalQCDB_" + region + "_" + year + ".png" )
                c_4.SaveAs("Plots/"+plotname + "/FinalQCDB_" + region + "_" + year + ".pdf" )
                c_4.Close()

                #FINAL QCD CROSS CHECK Ratio
                c_5 = ROOT.TCanvas("FinalQCDRatio")
                ratio = FinalQCDB.Clone(FinalQCDB.GetName() + "_ratio" )
                ratio.Divide(JETHT_B)
                ratio.Draw()
                if ( region == "MTR"):
                    ratio.GetYaxis().SetRangeUser(0,3)
                if ( region == "VTR"):
                    ratio.GetYaxis().SetRangeUser(0,3)
                ratio.GetYaxis().SetTitle("Ratio")
                ROOT.gStyle.SetOptStat(0);
                c_5.Draw()
                c_5.SaveAs("Plots/"+plotname + "/FinalQCDBRatio_" + region + "_" + year + ".png" )
                c_5.SaveAs("Plots/"+plotname + "/FinalQCDBRatio_" + region + "_" + year + ".pdf" )
                c_5.Close()


                #Data Driven Transfer Factor (B/A)
                c_6 = ROOT.TCanvas("DataDrivenQCDTransferFactor")
                ratio = BackgroundSubtractedData_B.Clone(BackgroundSubtractedData_B.GetName() + "_ratio" )
                ratio.Divide(BackgroundSubtractedData_A)
                ratio.Draw()
                # if ( region == "MTR"):
                #     ratio.GetYaxis().SetRangeUser(0,3)
                ratio.GetYaxis().SetTitle("Ratio")
                ROOT.gStyle.SetOptStat(0);
                c_6.Draw()
                c_6.SaveAs("Plots/"+plotname + "/DataDrivenQCDTransferFactor_" + region + "_" + year + ".png" )
                c_6.SaveAs("Plots/"+plotname + "/DataDrivenQCDTransferFactor_" + region + "_" + year + ".pdf" )
                c_6.Close()

                #For the main analysis
                file_out_2 = ROOT.TFile("QCD_" + region + "_" + year+ ".root","RECREATE")
                FinalQCDSR.Write("QCD")
                file_out_2.Close()
                
                file_out.Close()

            # QCDCR   
            leg = ROOT.TLegend(0.45,0.75,0.87,0.89)
            leg.SetNColumns(2)
            ROOT.gStyle.SetLegendBorderSize(0)
            c = ROOT.TCanvas("stackplot_" + variable)
            stack = ROOT.THStack("hs","")
            for i,CR in enumerate(CRs):
                if ( variable == "MetNoLep_CleanJet_mindPhi" ):
                    CR.Rebin(5)
                if ( variable == "diCleanJet_M_LeadingPosEta" or variable == "diCleanJet_M_LeadingNegEta" ):
                    xbins = array('d', [0,  200, 400, 600, 900, 1200, 1500, 2000, 2750, 3500, 5000])
                    CR = CR.Rebin(len(xbins)-1,"CR_rebinned",xbins)
                    CRs[i] = CR

                if ( samples[i] != "EWKZll"  and samples[i] != "EWKW" and samples[i] != "QCD" and samples[i] != "EWKZNUNU" and samples[i] != "VV" and samples[i] != "TOP" and samples[i] != "DY" and samples[i] != "ZJETS" and samples[i] != "WJETS" and samples[i] != "GluGluHtoInv" and samples[i] != "VBFHtoInv"):
                    continue

                CR.SetFillColor(ROOT.TColor.GetColor((colors[samples[i]])))
                CR.SetLineColor(ROOT.TColor.GetColor((colors[samples[i]])))
                if ( samples[i] == "QCD"):
                    CR.SetLineColor(1)

                # CR.SetFillColor(30+(i*2))
                # CR.SetLineColor(30+(i*2))
                leg.AddEntry(CR,samples[i],"F")
                stack.Add(CR)


            CRs[0].Draw()
            CRs[0].SetMinimum(0)
            stack.Draw("HISTsame")
            CRs[0].Draw("same")
            leg.Draw()
            if ( CRs[0].Integral()>0 ):
                CRs[0].SetMinimum(1)
                ROOT.gPad.SetLogy(log)
                if ( log ):
                    CRs[0].SetMaximum(CRs[0].GetMaximum()*10)
                else:
                    CRs[0].SetMaximum(CRs[0].GetMaximum()*1.5)
            c.Draw()
            c.SaveAs("Plots/"+plotname + "/CR/CR_" + variable + "_" + region + "_" + year + ".png")
            c.SaveAs("Plots/"+plotname + "/CR/CR_" + variable + "_" + region + "_" + year + ".pdf")
            c.Close()
            #file_out.Close()

            # SR   
            leg_SR = ROOT.TLegend(0.45,0.75,0.87,0.89)
            leg_SR.SetNColumns(2)
            ROOT.gStyle.SetLegendBorderSize(0)
            c_SR = ROOT.TCanvas("stackplot_" + variable)
            p1_SR = ROOT.TPad("SRmain","",0,0.3,1,1)
            p2_SR = ROOT.TPad("SRratio","",0,0,1,0.3)
            p1_SR.Draw()
            p1_SR.cd()
            ROOT.gStyle.SetOptStat(0);
            ROOT.gPad.SetBottomMargin(0)
            stack_SR = ROOT.THStack("hs_SR","")
            MCSum = CRs[0].Clone("SR_MCSum")
            MCSum.Reset()
            for i,SR in enumerate(SRs):
                if ( variable == "MetNoLep_CleanJet_mindPhi" ):
                    SR.Rebin(5)

                if ( variable == "diCleanJet_M_LeadingPosEta" or variable == "diCleanJet_M_LeadingNegEta" ):
                    xbins = array('d', [0,  200, 400, 600, 900, 1200, 1500, 2000, 2750, 3500, 5000])
                    SR = SR.Rebin(len(xbins)-1,"SR_rebinned",xbins)
                    SRs[i] = SR

                #if ( samples[i] != "EWKZll"  and samples[i] != "EWKW" and samples[i] != "QCD" and samples[i] != "EWKZNUNU" and samples[i] != "VV" and samples[i] != "TOP" and samples[i] != "DY" and samples[i] != "ZJETS" and samples[i] != "WJETS" and samples[i] != "GluGluHtoInv" and samples[i] != "VBFHtoInv"):
                #Removing QCD from ratio sum as requested by AM
                #if ( samples[i] != "EWKZll"  and samples[i] != "EWKW" and samples[i] != "EWKZNUNU" and samples[i] != "VV" and samples[i] != "TOP" and samples[i] != "DY" and samples[i] != "ZJETS" and samples[i] != "WJETS" and samples[i] != "GluGluHtoInv" and samples[i] != "VBFHtoInv"):
                if ( samples[i] != "EWKZll"  and samples[i] != "EWKW" and samples[i] != "EWKZNUNU" and samples[i] != "VV" and samples[i] != "TOP" and samples[i] != "DY" and samples[i] != "ZJETS" and samples[i] != "WJETS"):
                    continue
                # SR.SetFillColor(30+(i*2))
                # SR.SetLineColor(30+(i*2))
                SR.SetFillColor(ROOT.TColor.GetColor((colors[samples[i]])))
                SR.SetLineColor(ROOT.TColor.GetColor((colors[samples[i]])))
                if ( samples[i] == "QCD"):
                    SR.SetLineColor(1)
                leg_SR.AddEntry(SR,samples[i],"F")
                MCSum.Add(SR)
                stack_SR.Add(SR)
            SRs[0].Draw()
            SR_Ratio = SRs[0].Clone("SR_Ratio")
            SR_Ratio.Divide(MCSum)
            SRs[0].SetMinimum(0)
            stack_SR.Draw("HISTsame")
            stack_SR.SetMinimum(0)
            SRs[0].Draw("same")
            leg_SR.Draw()
            if ( SRs[0].Integral()>0 ):
                SRs[0].SetMinimum(1)
                ROOT.gPad.SetLogy(log)
                if ( log ):
                    SRs[0].SetMaximum(SRs[0].GetMaximum()*10)
                else:
                    SRs[0].SetMaximum(SRs[0].GetMaximum()*1.5)
            c_SR.cd()
            p2_SR.Draw()
            p2_SR.cd()
            p2_SR.SetGrid()
            ROOT.gPad.SetTopMargin(0)
            ROOT.gPad.SetBottomMargin(0.3)
            ROOT.gStyle.SetOptStat(0);
            SR_Ratio.Draw()
            
            SR_Ratio.GetYaxis().SetRangeUser(0,2)
            c_SR.cd()
            c_SR.Draw()
            c_SR.SaveAs("Plots/"+plotname + "/SR/SR_" + variable + "_" + region + "_" + year + ".png")
            c_SR.SaveAs("Plots/"+plotname + "/SR/SR_" + variable + "_" + region + "_" + year + ".C")
            c_SR.SaveAs("Plots/"+plotname + "/SR/SR_" + variable + "_" + region + "_" + year + ".pdf")
            c_SR.Close()
            #file_out.Close()


            #A 
            leg_A = ROOT.TLegend(0.45,0.75,0.87,0.89)
            leg_A.SetNColumns(2)
            ROOT.gStyle.SetLegendBorderSize(0)
            c_A = ROOT.TCanvas("stackplot_" + variable)
            stack_A = ROOT.THStack("hs_A","")
            for i,A in enumerate(As):
                if ( samples[i] != "EWKZll"  and samples[i] != "EWKW" and samples[i] != "QCD" and samples[i] != "EWKZNUNU" and samples[i] != "VV" and samples[i] != "TOP" and samples[i] != "DY" and samples[i] != "ZJETS" and samples[i] != "WJETS" and samples[i] != "GluGluHtoInv" and samples[i] != "VBFHtoInv"):
                    continue

                A.SetFillColor(ROOT.TColor.GetColor((colors[samples[i]])))
                A.SetLineColor(ROOT.TColor.GetColor((colors[samples[i]])))
                if ( samples[i] == "QCD"):
                    A.SetLineColor(1)

                # A.SetFillColor(30+(i*2))
                # A.SetLineColor(30+(i*2))
                leg_A.AddEntry(A,samples[i],"F")
                stack_A.Add(A)
            As[-1].Draw()
            As[-1].SetMinimum(0)
            stack_A.Draw("HISTsame")
            As[-1].Draw("same")
            leg_A.Draw()
            c_A.Draw()
            c_A.SaveAs("Plots/"+plotname + "/A/A_" + variable + "_" + region + "_" + year + ".png")
            c_A.SaveAs("Plots/"+plotname + "/A/A_" + variable + "_" + region + "_" + year + ".pdf")
            c_A.Close()
            #file_out.Close()

            #B
            leg_B = ROOT.TLegend(0.45,0.75,0.87,0.89)
            leg_B.SetNColumns(2)
            #leg_B = ROOT.TLegend(0.65,0.55,0.87,0.89)
            ROOT.gStyle.SetLegendBorderSize(0)
            c_B = ROOT.TCanvas("stackplot_" + variable)
            stack_B = ROOT.THStack("hs_B","")

            for i,B in enumerate(Bs):
                if ( samples[i] != "EWKZll"  and samples[i] != "EWKW" and samples[i] != "QCD" and samples[i] != "EWKZNUNU" and samples[i] != "VV" and samples[i] != "TOP" and samples[i] != "DY" and samples[i] != "ZJETS" and samples[i] != "WJETS" and samples[i] != "GluGluHtoInv" and samples[i] != "VBFHtoInv"):
                    continue

                B.SetFillColor(ROOT.TColor.GetColor((colors[samples[i]])))
                B.SetLineColor(ROOT.TColor.GetColor((colors[samples[i]])))
                if ( samples[i] == "QCD"):
                    B.SetLineColor(1)

                # B.SetFillColor(30+(i*2))
                # B.SetLineColor(30+(i*2))
                leg_B.AddEntry(B,samples[i],"F")
                stack_B.Add(B)
            if Bs[-1].Integral()>0:
                Bs[-1].Draw()
                Bs[-1].SetMinimum(0)
                stack_B.Draw("HISTsame")
                Bs[-1].Draw("same")
            else:
                stack_B.Draw("HIST")
                stack_B.SetMinimum(0)

            leg_B.Draw()
            c_B.Draw()
            c_B.SaveAs("Plots/"+plotname + "/B/B_" + variable + "_" + region + "_" + year + ".png")
            c_B.SaveAs("Plots/"+plotname + "/B/B_" + variable + "_" + region + "_" + year + ".pdf")
            c_B.Close()





            


        # for i,tf in enumerate(TFs):
        #     tf.SetLineColor(i+1)
        #     tf.SaveAs("tf_" + year + ".root")
        #     tf.Draw("same")

        #c.Draw()
        #c.Print("plot_"+year+".root")

#For Nick's method
variables = ["MetNoLep_CleanJet_mindPhi"]

for region in regions:
    for variable in variables:
        for year in years:

            files = []
            SRs = []
            CRs = []
            As = []
            Bs = []

            for sample in samples:
                files.append( ROOT.TFile.Open("~/invisible/AnalyseTrees/analysechiptrees/"+directory_name+"/Histos_Nominal_"+sample+"_"+year+".root"))
                SRs.append(files[-1].Get("SR/"+region+"/h_SR_"+region+"_" + variable))
                CRs.append(files[-1].Get("QCDCR/"+region+"/h_QCDCR_"+region+"_" + variable))
                As.append(files[-1].Get("QCDA/"+region+"/h_QCDA_"+region+"_" + variable))
                Bs.append(files[-1].Get("QCDB/"+region+"/h_QCDB_"+region+"_" + variable))

            file_out = ROOT.TFile("Plots/"+ plotname + "/out_" + region + "_" + year+ ".root","UPDATE")
            

            file_out.mkdir("MetNoLep_CleanJet_mindPhi")
            file_out.cd("MetNoLep_CleanJet_mindPhi")

            for CR,SR in zip(CRs,SRs):
                CR.Add(SR)

            for A,B in zip(As,Bs):
                A.Add(B)

            for i,CR in enumerate(CRs):
                CR.Rebin(5)
                CR.Write(samples[i]+"_CR")
            for i,A in enumerate(As):
                A.Rebin(5)
                A.Write(samples[i]+"_A")

            BackgroundSum_CR = CRs[0].Clone("BackgroundSum_CR")
            BackgroundSum_CR.Reset()

            BackgroundSum_A = As[0].Clone("BackgroundSum_A")
            BackgroundSum_A.Reset()

            for i,cr in enumerate(CRs):
                if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX"  or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv"):
                    continue
                BackgroundSum_CR.Add(cr)

            BackgroundSum_CR.Write()

            for i,a in enumerate(As):
                if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX"  or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv"):
                    continue
                BackgroundSum_A.Add(a)

            BackgroundSum_A.Write()

            # BackgroundSubtractedData_SR = SRs[0].Clone("BackgroundSubtractedData_MetNoLep_CleanJet_mindPhi_SR")

            # for i,CR in enumerate(CRs):
            #     if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX" or samples[i] == "MET"):
            #         continue
            #     BackgroundSubtractedData_CR.Add(CR,-1)
            # for i,SR in enumerate(SRs):
            #     if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX" or samples[i] == "MET"):
            #         continue
            #     BackgroundSubtractedData_SR.Add(SR,-1)

            # BackgroundSubtractedData_CR.Add(BackgroundSubtractedData_SR)
            # #Write all relevant hists:
            # BackgroundSubtractedData_CR.Write()

            file_out.Close()


bashCommand = "cd Plots; tar -zcf " + plotname + ".tar.gz " + plotname + "; cd ../;"
process = subprocess.Popen(bashCommand, shell=True)
