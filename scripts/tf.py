#!/usr/bin/env python
import sys,os
import subprocess
from array import array
import ROOT
sys.argv.append( '-b' )
ROOT.TH1.SetDefaultSumw2()

directory_name = sys.argv[1]#Input Histograms created using makeHistograms.C
plotname = sys.argv[2]#Output directory name
log = int(sys.argv[3])#Plot the histograms using a log scale
rebin = 0 #Rebin mjj - necessary if splitting data based on eta of forward jet #0 nominal, 1 Central, 2 Forward 

#Make the relevant directories
if os.path.exists(directory_name+"/"+plotname) == False:
    os.mkdir(directory_name+"/"+plotname)
if os.path.exists(directory_name+"/"+plotname+"/A") == False:
    os.mkdir(directory_name+"/"+plotname+"/A")
if os.path.exists(directory_name+"/"+plotname+"/B") == False:
    os.mkdir(directory_name+"/"+plotname+"/B")
if os.path.exists(directory_name+"/"+plotname+"/SR") == False:
    os.mkdir(directory_name+"/"+plotname+"/SR")
if os.path.exists(directory_name+"/"+plotname+"/CR") == False:
    os.mkdir(directory_name+"/"+plotname+"/CR")
if os.path.exists(directory_name+"/"+plotname+"/HFNoise") == False:
    os.mkdir(directory_name+"/"+plotname+"/HFNoise")
if os.path.exists(directory_name+"/"+plotname+"/HFNoiseCR") == False:
    os.mkdir(directory_name+"/"+plotname+"/HFNoiseCR")

#Choose which years to plot
years = ["2017","2018"]
#years = ["2017"]

#Choose which samples to include on the plots
samples = ["MET","VV","TOP","DY","EWKZll","GluGluHtoInv","VBFHtoInv","WH", "qqZH", "ggZH", "ttH", "EWKZNUNU","ZJETS","EWKW","WJETS","QCD","QCDRELAX","DATA"]
#samples = ["MET","VV","TOP","DY","EWKZll","EWKZNUNU","ZJETS","EWKW","WJETS","QCD","QCDRELAX","DATA"]

#Choose which regions to plot
regions = ["MTR","VTR"]


#List of variables to plot

#variables = ["MetNoLep_pt","diCleanJet_M","diCleanJet_M_LeadingPosEta","diCleanJet_M_LeadingNegEta","diCleanJet_dEta","CentralEtaMTR","ForwardEtaMTR","CentralEtaVTR","ForwardEtaVTR","diCleanJet_dPhi","Leading_jet_pt","Subleading_jet_pt","Leading_jet_eta","Subleading_jet_eta","nCleanJet30","MetNoLep_CleanJet_mindPhi","LHE_Vpt","LHE_HT","dijet_met_balance","lMjj_binned","diCleanJet_M_binned","diCleanJet_M_binned_reduced","lMjj_dijet_deta", "lMjj_dijet_dphi"]
variables = ["MetNoLep_pt","diCleanJet_M","diCleanJet_dEta","CentralEtaMTR","ForwardEtaMTR","CentralEtaVTR","ForwardEtaVTR","diCleanJet_dPhi","Leading_jet_pt","Subleading_jet_pt","Leading_jet_eta","Subleading_jet_eta","MetNoLep_CleanJet_mindPhi","lMjj_binned","diCleanJet_M_binned","lMjj_dijet_deta", "lMjj_dijet_dphi"]

#Set the colours of the plots
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

            files = []

            SRs = []
            CRs = []
            As = []
            Bs = []
            HFs = []
            HFCRs = []
            TFs = []

            #Load all input histograms to memory
            for sample in samples:
                files.append( ROOT.TFile.Open(directory_name+"/Histos_Nominal_"+sample+"_"+year+".root"))
                Bs.append(files[-1].Get("QCDB/"+region+"/h_QCDB_"+region+"_" + variable))
                As.append(files[-1].Get("QCDA/"+region+"/h_QCDA_"+region+"_" + variable))
                SRs.append(files[-1].Get("SR/"+region+"/h_SR_"+region+"_" + variable))
                HFs.append(files[-1].Get("HFNoise/"+region+"/h_HFNoise_"+region+"_" + variable))
                HFCRs.append(files[-1].Get("HFNoiseCR/"+region+"/h_HFNoiseCR_"+region+"_" + variable))
                CRs.append(files[-1].Get("QCDCR/"+region+"/h_QCDCR_"+region+"_" + variable))

            #Processing of Method A, i.e. calculating the background-subtracted data and the 
            #transfer-factors in the mjj distribution

            #Rebin the mjj distribution if necessary
            for i,(A,B,SR,HF,HFCR,CR) in enumerate(zip(As,Bs,SRs,HFs,HFCRs,CRs)):
                if variable == "diCleanJet_M_binned":
                    if rebin == 0:
                        mjjbins=array('d',[0,200,400,600,900,1200,1500,2000,2750,3500,5000])
                    elif rebin == 1:
                        mjjbins=array('d',[0,200,400,600,900,1200,1500,2000,2750,5000])
                    elif rebin == 2:
                        mjjbins=array('d',[0,200,900,1200,1500,2000,2750,3500,5000])
                    As[i] = A.Rebin(len(mjjbins)-1,"A_rebinned",mjjbins)
                    Bs[i] = B.Rebin(len(mjjbins)-1,"B_rebinned",mjjbins)
                    CRs[i] = CR.Rebin(len(mjjbins)-1,"CR_rebinned",mjjbins)
                    SRs[i] = SR.Rebin(len(mjjbins)-1,"SR_rebinned",mjjbins)
                    HFs[i] = HF.Rebin(len(mjjbins)-1,"HF_rebinned",mjjbins)
                    HFCRs[i] = HFCR.Rebin(len(mjjbins)-1,"HFCR_rebinned",mjjbins)
                elif variable == "lMjj_binned":
                    if rebin == 0:
                        mjjbins=array('d',[0,900,1200,1500,2000,2750,5000])
                    elif rebin == 1:
                        mjjbins=array('d',[0,900,1200,1500,5000])
                    elif rebin == 2:
                        mjjbins=array('d',[0,900,1200,1500,2000,2750,5000])

                    As[i] = A.Rebin(len(mjjbins)-1,"A_rebinned",mjjbins)
                    Bs[i] = B.Rebin(len(mjjbins)-1,"B_rebinned",mjjbins)
                    CRs[i] = CR.Rebin(len(mjjbins)-1,"CR_rebinned",mjjbins)
                    SRs[i] = SR.Rebin(len(mjjbins)-1,"SR_rebinned",mjjbins)
                    HFs[i] = HF.Rebin(len(mjjbins)-1,"HF_rebinned",mjjbins)
                    HFCRs[i] = HFCR.Rebin(len(mjjbins)-1,"HFCR_rebinned",mjjbins)



            if (  (variable=="diCleanJet_M_binned" and region == "MTR" ) or (variable=="lMjj_binned" and region == "VTR" )  ):
                
                #Get Data - background subtracted for the mjj distribution                
                file_out = ROOT.TFile(directory_name+"/"+plotname + "/out_" + region + "_" + year+ ".root","RECREATE")
                BackgroundSubtractedData_SR = SRs[0].Clone("BackgroundSubtractedData_SR")
                BackgroundSubtractedData_HF = HFs[0].Clone("HFTemplate")
                BackgroundSubtractedData_HFCR = HFCRs[0].Clone("HFTemplate_CR")
                BackgroundSubtractedData_CR = CRs[0].Clone("BackgroundSubtractedData_CR")
                BackgroundSubtractedData_A = As[-1].Clone("BackgroundSubtractedData_A")
                BackgroundSubtractedData_B = Bs[-1].Clone("BackgroundSubtractedData_B")


                for i,SR in enumerate(SRs):
                    if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                        continue
                    BackgroundSubtractedData_SR.Add(SR,-1)
                BackgroundSubtractedData_SR.Add(BackgroundSubtractedData_HF,-1)
                for i,HF in enumerate(HFs):
                    if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                        continue
                    BackgroundSubtractedData_HF.Add(HF,-1)
                for i,HFCR in enumerate(HFCRs):
                    if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                        continue
                    BackgroundSubtractedData_HFCR.Add(HFCR,-1)
                for i,CR in enumerate(CRs):
                    if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                        continue
                    BackgroundSubtractedData_CR.Add(CR,-1)
                BackgroundSubtractedData_CR.Add(BackgroundSubtractedData_HFCR,-1)
                for i,A in enumerate(As):
                    if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                        continue
                    BackgroundSubtractedData_A.Add(A,-1)
                for i,B in enumerate(Bs):
                    if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                        continue
                    BackgroundSubtractedData_B.Add(B,-1)

                #Get QCD Transfer factors
                #For QCD relaxed, qcdchoice= -2
                #For QCD normal, qcdchoice = -3
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
                QCDMC_HF = SRs[-3].Clone("QCDMC_HF")
                QCDMC_HFCR = SRs[-3].Clone("QCDMC_HFCR")
                QCDMC_CR = CRs[-3].Clone("QCDMC_CR")

                QCDMC_B = Bs[-3].Clone("QCDMC_B")
                JETHT_B = Bs[-1].Clone("JETHT_B")

                #Write all relevant hists to output root file:

                for i,(sr,cr,a,b,hf,hfcr) in enumerate(zip(SRs,CRs,As,Bs,HFs,HFCRs)):
                    sr.Write(samples[i]+"_SR")
                    cr.Write(samples[i]+"_CR")
                    a.Write(samples[i]+"_A")
                    b.Write(samples[i]+"_B")
                    hf.Write(samples[i]+"_HFNoise")
                    hfcr.Write(samples[i]+"_HFNoiseCR")
                
                QCDTransferFactor_SR.Write()
                QCDTransferFactor_B.Write()

                BackgroundSubtractedData_A.Write()
                BackgroundSubtractedData_B.Write()
                BackgroundSubtractedData_CR.Write()
                BackgroundSubtractedData_SR.Write()
                BackgroundSubtractedData_HF.Write()
                BackgroundSubtractedData_HFCR.Write()

                FinalQCDSR.Write()
                FinalQCDB.Write()

                QCDMC_SR.Write()
                QCDMC_HF.Write()
                QCDMC_HFCR.Write()
                QCDMC_CR.Write()

                #Plotting relevant histograms:

                #QCD TRANSFER FACTORS
                c_1 = ROOT.TCanvas("QCDTransferFactor_SR")
                QCDTransferFactor_SR.Draw()
                QCDTransferFactor_SR.GetYaxis().SetTitle("Transfer factor, r")
                QCDTransferFactor_SR.SetMinimum(-0.0001)
                QCDTransferFactor_SR.SetMaximum(0.5)
                ROOT.gStyle.SetOptStat(0);
                c_1.Draw()
                c_1.SaveAs(directory_name+"/"+plotname + "/QCDTransferFactor_SR_" + region + "_" + year + ".png" )
                c_1.SaveAs(directory_name+"/"+plotname + "/QCDTransferFactor_SR_" + region + "_" + year + ".pdf" )
                c_1.Close()

                c_2 = ROOT.TCanvas("QCDTransferFactor_B")
                QCDTransferFactor_B.Draw()
                QCDTransferFactor_B.GetYaxis().SetTitle("Transfer factor, r")
                ROOT.gStyle.SetOptStat(0);
                c_2.Draw()
                c_2.SaveAs(directory_name+"/"+plotname + "/QCDTransferFactor_B_" + region + "_" + year + ".png" )
                c_2.SaveAs(directory_name+"/"+plotname + "/QCDTransferFactor_B_" + region + "_" + year + ".pdf" )
                c_2.Close()

                #FINAL QCD
                c_3 = ROOT.TCanvas("FinalQCD")
                FinalQCDSR.Draw()
                FinalQCDSR.GetYaxis().SetTitle("Number of Events")
                FinalQCDSR.GetYaxis().SetRangeUser(0.1,50000)
                QCDMC_SR.SetLineColor(ROOT.kRed)
                QCDMC_SR.Draw("same")
                print ( "Data-Driven MJ prediction in " + region + " in " + year + " is "  + str(FinalQCDSR.Integral()) )
                print ( "MC MJ prediction in " + region + " in " + year + " is "  + str(QCDMC_SR.Integral()) )

                ROOT.gPad.SetLogy(1)
                leg_3 = ROOT.TLegend(0.4,0.6,0.89,0.89)
                leg_3.SetBorderSize(0);
                leg_3.AddEntry(FinalQCDSR,"Data driven method A","L")
                leg_3.AddEntry(QCDMC_SR,"QCD MC","L")
                leg_3.Draw()

                ROOT.gStyle.SetOptStat(0);
                c_3.Draw()
                c_3.SaveAs(directory_name+"/"+plotname + "/FinalQCDSR_" + region + "_" + year + ".png" )
                c_3.SaveAs(directory_name+"/"+plotname + "/FinalQCDSR_" + region + "_" + year + ".pdf" )
                c_3.Close()

                #FINAL QCD CROSS CHECK
                c_4 = ROOT.TCanvas("FinalQCD")
                FinalQCDB.Draw()
                if ( region == "MTR"):
                    FinalQCDB.GetYaxis().SetRangeUser(0,3000)
                if ( region == "VTR"):
                    FinalQCDB.GetYaxis().SetRangeUser(0,1000)
                FinalQCDB.GetYaxis().SetTitle("Number of Events")

                JETHT_B.SetLineColor(ROOT.kRed)
                JETHT_B.Draw("same")
                leg_4 = ROOT.TLegend(0.5,0.5,0.89,0.89)
                leg_4.SetBorderSize(0);
                leg_4.AddEntry(FinalQCDB,"Data-driven QCD prediction","L")
                leg_4.AddEntry(JETHT_B,"Data in \"B\" region","L")
                leg_4.Draw()
                ROOT.gStyle.SetOptStat(0);
                c_4.Draw()
                c_4.SaveAs(directory_name+"/"+plotname + "/FinalQCDB_" + region + "_" + year + ".png" )
                c_4.SaveAs(directory_name+"/"+plotname + "/FinalQCDB_" + region + "_" + year + ".pdf" )
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
                c_5.SaveAs(directory_name+"/"+plotname + "/FinalQCDBRatio_" + region + "_" + year + ".png" )
                c_5.SaveAs(directory_name+"/"+plotname + "/FinalQCDBRatio_" + region + "_" + year + ".pdf" )
                c_5.Close()

                #Data Driven Transfer Factor (B/A)
                c_6 = ROOT.TCanvas("DataDrivenQCDTransferFactor")
                ratio = BackgroundSubtractedData_B.Clone(BackgroundSubtractedData_B.GetName() + "_ratio" )
                ratio.Divide(BackgroundSubtractedData_A)
                ratio.Draw()
                ratio.GetYaxis().SetTitle("Ratio")
                ROOT.gStyle.SetOptStat(0);
                c_6.Draw()
                c_6.SaveAs(directory_name+"/"+plotname + "/DataDrivenQCDTransferFactor_" + region + "_" + year + ".png" )
                c_6.SaveAs(directory_name+"/"+plotname + "/DataDrivenQCDTransferFactor_" + region + "_" + year + ".pdf" )
                c_6.Close()

                #For the main analysis
                file_out_2 = ROOT.TFile("QCD_" + region + "_" + year+ ".root","RECREATE")
                FinalQCDSR.Write("QCD")
                file_out_2.Close()
                file_out.Close()



            #Plot the other variables as a cross-check:


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
                    
                if ( samples[i] == "DATA" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                    continue

                CR.SetFillColor(ROOT.TColor.GetColor((colors[samples[i]])))
                CR.SetLineColor(ROOT.TColor.GetColor((colors[samples[i]])))
                if ( samples[i] == "QCD"):
                    CR.SetLineColor(1)

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
            c.SaveAs(directory_name+"/"+plotname + "/CR/CR_" + variable + "_" + region + "_" + year + ".png")
            c.SaveAs(directory_name+"/"+plotname + "/CR/CR_" + variable + "_" + region + "_" + year + ".pdf")
            c.Close()



            #SR   
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

                if ( samples[i] == "DATA" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                    continue

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
            c_SR.SaveAs(directory_name+"/"+plotname + "/SR/SR_" + variable + "_" + region + "_" + year + ".png")
            c_SR.SaveAs(directory_name+"/"+plotname + "/SR/SR_" + variable + "_" + region + "_" + year + ".C")
            c_SR.SaveAs(directory_name+"/"+plotname + "/SR/SR_" + variable + "_" + region + "_" + year + ".pdf")
            c_SR.Close()





            #HF   
            leg_HF = ROOT.TLegend(0.45,0.75,0.87,0.89)
            leg_HF.SetNColumns(2)
            ROOT.gStyle.SetLegendBorderSize(0)
            c_HF = ROOT.TCanvas("stackplot_" + variable)
            p1_HF = ROOT.TPad("HFmain","",0,0.3,1,1)
            p2_HF = ROOT.TPad("HFratio","",0,0,1,0.3)
            p1_HF.Draw()
            p1_HF.cd()
            ROOT.gStyle.SetOptStat(0);
            ROOT.gPad.SetBottomMargin(0)
            stack_HF = ROOT.THStack("hs_HF","")
            MCSum = CRs[0].Clone("HF_MCSum")
            MCSum.Reset()
            for i,HF in enumerate(HFs):
                if ( variable == "MetNoLep_CleanJet_mindPhi" ):
                    HF.Rebin(5)

                if ( variable == "diCleanJet_M_LeadingPosEta" or variable == "diCleanJet_M_LeadingNegEta" ):
                    xbins = array('d', [0,  200, 400, 600, 900, 1200, 1500, 2000, 2750, 3500, 5000])
                    HF = HF.Rebin(len(xbins)-1,"HF_rebinned",xbins)
                    HFs[i] = HF

                if ( samples[i] == "DATA" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                    continue

                HF.SetFillColor(ROOT.TColor.GetColor((colors[samples[i]])))
                HF.SetLineColor(ROOT.TColor.GetColor((colors[samples[i]])))
                if ( samples[i] == "QCD"):
                    HF.SetLineColor(1)
                leg_HF.AddEntry(HF,samples[i],"F")
                MCSum.Add(HF)
                stack_HF.Add(HF)
            HFs[0].Draw()
            HF_Ratio = HFs[0].Clone("HF_Ratio")
            HF_Ratio.Divide(MCSum)
            HFs[0].SetMinimum(0)
            stack_HF.Draw("HISTsame")
            stack_HF.SetMinimum(0)
            HFs[0].Draw("same")
            leg_HF.Draw()
            if ( HFs[0].Integral()>0 ):
                HFs[0].SetMinimum(1)
                ROOT.gPad.SetLogy(log)
                if ( log ):
                    HFs[0].SetMaximum(HFs[0].GetMaximum()*10)
                else:
                    HFs[0].SetMaximum(HFs[0].GetMaximum()*1.5)
            c_HF.cd()
            p2_HF.Draw()
            p2_HF.cd()
            p2_HF.SetGrid()
            ROOT.gPad.SetTopMargin(0)
            ROOT.gPad.SetBottomMargin(0.3)
            ROOT.gStyle.SetOptStat(0);
            HF_Ratio.Draw()
            
            HF_Ratio.GetYaxis().SetRangeUser(0,2)
            c_HF.cd()
            c_HF.Draw()
            c_HF.SaveAs(directory_name+"/"+plotname + "/HFNoise/HFNoise_" + variable + "_" + region + "_" + year + ".png")
            c_HF.SaveAs(directory_name+"/"+plotname + "/HFNoise/HFNoise_" + variable + "_" + region + "_" + year + ".C")
            c_HF.SaveAs(directory_name+"/"+plotname + "/HFNoise/HFNoise_" + variable + "_" + region + "_" + year + ".pdf")
            c_HF.Close()




            #HF CR
            leg_HFCR = ROOT.TLegend(0.45,0.75,0.87,0.89)
            leg_HFCR.SetNColumns(2)
            ROOT.gStyle.SetLegendBorderSize(0)
            c_HFCR = ROOT.TCanvas("stackplot_" + variable)
            p1_HFCR = ROOT.TPad("HFCRmain","",0,0.3,1,1)
            p2_HFCR = ROOT.TPad("HFCRratio","",0,0,1,0.3)
            p1_HFCR.Draw()
            p1_HFCR.cd()
            ROOT.gStyle.SetOptStat(0);
            ROOT.gPad.SetBottomMargin(0)
            stack_HFCR = ROOT.THStack("hs_HFCR","")
            MCSum = CRs[0].Clone("HFCR_MCSum")
            MCSum.Reset()
            for i,HFCR in enumerate(HFCRs):
                if ( variable == "MetNoLep_CleanJet_mindPhi" ):
                    HFCR.Rebin(5)

                if ( variable == "diCleanJet_M_LeadingPosEta" or variable == "diCleanJet_M_LeadingNegEta" ):
                    xbins = array('d', [0,  200, 400, 600, 900, 1200, 1500, 2000, 2750, 3500, 5000])
                    HFCR = HFCR.Rebin(len(xbins)-1,"HFCR_rebinned",xbins)
                    HFCRs[i] = HFCR

                if ( samples[i] == "DATA" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                    continue

                HFCR.SetFillColor(ROOT.TColor.GetColor((colors[samples[i]])))
                HFCR.SetLineColor(ROOT.TColor.GetColor((colors[samples[i]])))
                if ( samples[i] == "QCD"):
                    HFCR.SetLineColor(1)
                leg_HFCR.AddEntry(HFCR,samples[i],"F")
                MCSum.Add(HFCR)
                stack_HFCR.Add(HFCR)
            HFCRs[0].Draw()
            HFCR_Ratio = HFCRs[0].Clone("HFCR_Ratio")
            HFCR_Ratio.Divide(MCSum)
            HFCRs[0].SetMinimum(0)
            stack_HFCR.Draw("HISTsame")
            stack_HFCR.SetMinimum(0)
            HFCRs[0].Draw("same")
            leg_HFCR.Draw()
            if ( HFCRs[0].Integral()>0 ):
                HFCRs[0].SetMinimum(1)
                ROOT.gPad.SetLogy(log)
                if ( log ):
                    HFCRs[0].SetMaximum(HFCRs[0].GetMaximum()*10)
                else:
                    HFCRs[0].SetMaximum(HFCRs[0].GetMaximum()*1.5)
            c_HFCR.cd()
            p2_HFCR.Draw()
            p2_HFCR.cd()
            p2_HFCR.SetGrid()
            ROOT.gPad.SetTopMargin(0)
            ROOT.gPad.SetBottomMargin(0.3)
            ROOT.gStyle.SetOptStat(0);
            HFCR_Ratio.Draw()
            
            HFCR_Ratio.GetYaxis().SetRangeUser(0,2)
            c_HFCR.cd()
            c_HFCR.Draw()
            c_HFCR.SaveAs(directory_name+"/"+plotname + "/HFNoiseCR/HFNoiseCR_" + variable + "_" + region + "_" + year + ".png")
            c_HFCR.SaveAs(directory_name+"/"+plotname + "/HFNoiseCR/HFNoiseCR_" + variable + "_" + region + "_" + year + ".C")
            c_HFCR.SaveAs(directory_name+"/"+plotname + "/HFNoiseCR/HFNoiseCR_" + variable + "_" + region + "_" + year + ".pdf")
            c_HFCR.Close()



            #Region A 
            leg_A = ROOT.TLegend(0.45,0.75,0.87,0.89)
            leg_A.SetNColumns(2)
            ROOT.gStyle.SetLegendBorderSize(0)
            c_A = ROOT.TCanvas("stackplot_" + variable)
            stack_A = ROOT.THStack("hs_A","")
            for i,A in enumerate(As):
                if ( samples[i] == "DATA" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                    continue

                A.SetFillColor(ROOT.TColor.GetColor((colors[samples[i]])))
                A.SetLineColor(ROOT.TColor.GetColor((colors[samples[i]])))
                if ( samples[i] == "QCD"):
                    A.SetLineColor(1)

                leg_A.AddEntry(A,samples[i],"F")
                stack_A.Add(A)
            As[-1].Draw()
            As[-1].SetMinimum(0)
            stack_A.Draw("HISTsame")
            As[-1].Draw("same")
            leg_A.Draw()
            c_A.Draw()
            c_A.SaveAs(directory_name+"/"+plotname + "/A/A_" + variable + "_" + region + "_" + year + ".png")
            c_A.SaveAs(directory_name+"/"+plotname + "/A/A_" + variable + "_" + region + "_" + year + ".pdf")
            c_A.Close()




            #Region B
            leg_B = ROOT.TLegend(0.45,0.75,0.87,0.89)
            leg_B.SetNColumns(2)
            ROOT.gStyle.SetLegendBorderSize(0)
            c_B = ROOT.TCanvas("stackplot_" + variable)
            stack_B = ROOT.THStack("hs_B","")

            for i,B in enumerate(Bs):
                if ( samples[i] == "DATA" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                    continue

                B.SetFillColor(ROOT.TColor.GetColor((colors[samples[i]])))
                B.SetLineColor(ROOT.TColor.GetColor((colors[samples[i]])))
                if ( samples[i] == "QCD"):
                    B.SetLineColor(1)

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
            c_B.SaveAs(directory_name+"/"+plotname + "/B/B_" + variable + "_" + region + "_" + year + ".png")
            c_B.SaveAs(directory_name+"/"+plotname + "/B/B_" + variable + "_" + region + "_" + year + ".pdf")
            c_B.Close()




#Output to be used in the calculation of Method B
variables = ["MetNoLep_CleanJet_mindPhi"]

for region in regions:
    for variable in variables:
        for year in years:

            files = []
            SRs = []
            CRs = []
            As = []
            Bs = []
            HFs = []
            HFCRs = []

            for sample in samples:
                files.append( ROOT.TFile.Open(directory_name+"/Histos_Nominal_"+sample+"_"+year+".root"))
                SRs.append(files[-1].Get("SR/"+region+"/h_SR_"+region+"_" + variable))
                CRs.append(files[-1].Get("QCDCR/"+region+"/h_QCDCR_"+region+"_" + variable))
                As.append(files[-1].Get("QCDA/"+region+"/h_QCDA_"+region+"_" + variable))
                Bs.append(files[-1].Get("QCDB/"+region+"/h_QCDB_"+region+"_" + variable))
                HFs.append(files[-1].Get("HFNoise/"+region+"/h_HFNoise_"+region+"_" + variable))
                HFCRs.append(files[-1].Get("HFNoiseCR/"+region+"/h_HFNoiseCR_"+region+"_" + variable))

            file_out = ROOT.TFile(directory_name+"/"+ plotname + "/out_" + region + "_" + year+ ".root","UPDATE")
            
            file_out.mkdir("MetNoLep_CleanJet_mindPhi")
            file_out.cd("MetNoLep_CleanJet_mindPhi")

            for CR,SR in zip(CRs,SRs):
                CR.Add(SR)

            for HFCR,HF in zip(HFCRs,HFs):
                HFCR.Add(HF)

            for A,B in zip(As,Bs):
                A.Add(B)

            for i,CR in enumerate(CRs):
                CR.Rebin(5)
                CR.Write(samples[i]+"_CR")

            for i,HFCR in enumerate(HFCRs):
                HFCR.Rebin(5)
                HFCR.Write(samples[i]+"_HFCR")

            for i,A in enumerate(As):
                A.Rebin(5)
                A.Write(samples[i]+"_A")

            BackgroundSum_CR = CRs[0].Clone("BackgroundSum_CR")
            BackgroundSum_CR.Reset()

            BackgroundSum_HFCR = HFCRs[0].Clone("BackgroundSum_HFCR")
            HFTemplate_CR = HFCRs[0].Clone("HFNoise_CR")
            BackgroundSum_HFCR.Reset()

            BackgroundSum_A = As[0].Clone("BackgroundSum_A")
            BackgroundSum_A.Reset()

            for i,cr in enumerate(CRs):
                if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                    continue
                BackgroundSum_CR.Add(cr)

            for i,hfcr in enumerate(HFCRs):
                if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                    continue
                BackgroundSum_HFCR.Add(hfcr)
            BackgroundSum_HFCR.Write()

            HFTemplate_CR.Add(BackgroundSum_HFCR,-1)
            HFTemplate_CR.Write()

            BackgroundSum_CR.Add(HFTemplate_CR)
            BackgroundSum_CR.Write()

            for i,a in enumerate(As):
                if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "QCDRELAX" or samples[i] == "MET" or samples[i] == "GluGluHtoInv" or samples[i] == "VBFHtoInv" or samples[i] == "WH" or samples[i] == "qqZH" or samples[i] == "ggZH" or samples[i] == "ttH"):
                    continue
                BackgroundSum_A.Add(a)
            BackgroundSum_A.Write()

            file_out.Close()


bashCommand = "cd "+directory_name+"; tar -zcf " + plotname + ".tar.gz " + plotname + "; cd ../;"
process = subprocess.Popen(bashCommand, shell=True)
