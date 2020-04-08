#!/usr/bin/env python
import ROOT
import sys

#listOfSysts = ["Nominal","ElectronVetoUp","ElectronVetoDown","MuonVetoUp","MuonVetoDown","TauVetoUp","TauVetoDown","BjetVetoUp","BjetVetoDown"]
#listOfSysts = ["Nominal","ElectronVetoUp","MuonVetoUp","TauVetoUp","BjetVetoUp"]
#listOfSysts = ["Nominal"]

#samples = ["DATA","QCD","DY","EWKW","EWKZNUNU","EWKZll","GluGluHtoInv","TOP","VV","WJETS","ZJETS","MET"]
years = ["2017","2018"]
samples = ["MET","QCD","DY","EWKW","EWKZNUNU","EWKZll","GluGluHtoInv","SingleElectron","TOP","VV","WJETS","ZJETS"]

#variables = ["diCleanJet_M_binned"]

variables = ["MetNoLep_pt","diCleanJet_M","diCleanJet_dEta","diCleanJet_dPhi","Leading_jet_pt","Subleading_jet_pt","Leading_jet_eta","Subleading_jet_eta","nCleanJet30","MetNoLep_CleanJet_mindPhi","LHE_Vpt","LHE_HT","decayLeptonId","LHE_Nuds","LHE_Nb","LHE_Nc","Pileup_nPU","diCleanJet_M_binned"]


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
            files.append( ROOT.TFile.Open("~/invisible/AnalyseTrees/analysechiptrees/Plots/Histos_Nominal_"+sample+"_"+year+".root"))
            Bs.append(files[-1].Get("QCDB/MTR/h_QCDB_MTR_" + variable))
            As.append(files[-1].Get("QCDA/MTR/h_QCDA_MTR_" + variable))
            SRs.append(files[-1].Get("SR/MTR/h_SR_MTR_" + variable))
            CRs.append(files[-1].Get("QCDCR/MTR/h_QCDCR_MTR_" + variable))


            #print (SRs[-1].Integral(),CRs[-1].Integral())

        #Get Data - background subtracted

        BackgroundSubtractedData_CR = CRs[0].Clone("BackgroundSubtractedData_CR")
        BackgroundSubtractedData_A = As[0].Clone("BackgroundSubtractedData_A")

        for i,CR in enumerate(CRs):
            if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "MET"):
                continue
                BackgroundSubtractedData_CR.Add(CR,-1)
                for i,A in enumerate(As):
                    if ( samples[i] == "DATA" or samples[i] == "QCD" or samples[i] == "MET"):
                        continue
                        BackgroundSubtractedData_A.Add(A,-1)

        #Get QCD Tranfer factor

        QCDTransferFactor_SR = SRs[1].Clone("QCDTransferFactor_SR")
        QCDTransferFactor_B = Bs[1].Clone("QCDTransferFactor_B")

        QCDTransferFactor_SR.Divide( CRs[1] )
        QCDTransferFactor_B.Divide( As[1] )

        #Get Final QCD prediction

        FinalQCDSR = BackgroundSubtractedData_CR.Clone("FinalQCD_SR")
        FinalQCDB = BackgroundSubtractedData_A.Clone("FinalQCD_B")

        FinalQCDSR.Multiply(QCDTransferFactor_SR)
        FinalQCDB.Multiply(QCDTransferFactor_B)

        QCDMC_SR = SRs[1].Clone("QCDMC_SR")
        QCDMC_CR = CRs[1].Clone("QCDMC_CR")

        #DRAWING
        file_out = ROOT.TFile("out_" + year+ ".root","RECREATE")




        #Write all relevant hists:

        for i,(sr,cr,a,b) in enumerate(zip(SRs,CRs,As,Bs)):
            sr.Write(samples[i]+"_SR")
            cr.Write(samples[i]+"_CR")
            a.Write(samples[i]+"_A")
            b.Write(samples[i]+"_B")

        QCDTransferFactor_SR.Write()
        QCDTransferFactor_B.Write()

        BackgroundSubtractedData_A.Write()
        BackgroundSubtractedData_CR.Write()

        FinalQCDSR.Write()
        FinalQCDB.Write()

        QCDMC_SR.Write()
        QCDMC_CR.Write()



        leg = ROOT.TLegend(0.7,0.7,0.9,0.9)
        c = ROOT.TCanvas("stackplot_" + variable)
        stack = ROOT.THStack("hs","")
        for i,CR in enumerate(CRs):
            if ( samples[i] == "DATA" or samples[i] == "MET"):
                continue
            CR.SetFillColor(30+i)
            CR.SetLineColor(30+i)
            leg.AddEntry(CR,samples[i],"F")
            stack.Add(CR)
        CRs[0].Draw()
        CRs[0].SetMinimum(0)
        stack.Draw("HISTsame")
        leg.Draw()
        c.Draw()
        c.SaveAs("Henning/" + variable + "_" + year + ".png")
        c.Close()
        file_out.Close()





    # for i,tf in enumerate(TFs):
    #     tf.SetLineColor(i+1)
    #     tf.SaveAs("tf_" + year + ".root")
    #     tf.Draw("same")

    #c.Draw()
    #c.Print("plot_"+year+".root")
