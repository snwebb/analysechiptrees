#!/usr/bin/env python
import ROOT
import sys

#listOfSysts = ["Nominal","ElectronVetoUp","ElectronVetoDown","MuonVetoUp","MuonVetoDown","TauVetoUp","TauVetoDown","BjetVetoUp","BjetVetoDown"]
#listOfSysts = ["Nominal","ElectronVetoUp","MuonVetoUp","TauVetoUp","BjetVetoUp"]
#listOfSysts = ["Nominal"]

samples = ["DATA","QCD","TOP"]


files = []

SRs = []
CRs = []
As = []
Bs = []
TFs = []

year=sys.argv[1]

# for syst in listOfSysts:
#     files.append( ROOT.TFile.Open("../Plots/Histos_"+syst+"_QCD_2017.root"))
#     SRs.append(files[-1].Get("SR/MTR/h_SR_MTR_dijet_M_binned"))
#     CRs.append(files[-1].Get("QCDCR/MTR/h_QCDCR_MTR_dijet_M_binned"))

#for syst in listOfSysts:
for sample in samples:
    
    files.append( ROOT.TFile.Open("~/invisible/AnalyseTrees/analysechiptrees/Plots/Histos_Nominal_"+sample+"_"+year+".root"))
    Bs.append(files[-1].Get("QCDB/MTR/h_QCDB_MTR_diCleanJet_M_binned"))
    As.append(files[-1].Get("QCDA/MTR/h_QCDA_MTR_diCleanJet_M_binned"))
    SRs.append(files[-1].Get("SR/MTR/h_SR_MTR_diCleanJet_M_binned"))
    CRs.append(files[-1].Get("QCDCR/MTR/h_QCDCR_MTR_diCleanJet_M_binned"))


    #print (SRs[-1].Integral(),CRs[-1].Integral())

#Get Data - background subtracted

BackgroundSubtractedData_CR = CRs[0].Clone("BackgroundSubtractedData_CR")
BackgroundSubtractedData_A = As[0].Clone("BackgroundSubtractedData_A")

for i,CR in enumerate(CRs):
    if ( samples[i] == "Data" or samples[i] == "QCD" ):
        continue
    BackgroundSubtractedData_CR.Add(CR,-1)
for i,A in enumerate(As):
    if ( samples[i] == "Data" or samples[i] == "QCD" ):
        continue
    BackgroundSubtractedData_A.Add(A,-1)

#Get QCD Tranfer factor

QCDTransferFactor_SR = SRs[1].Clone("QCDTransferFactor_SR")
QCDTransferFactor_B = Bs[1].Clone("QCDTransferFactor_B")

QCDTransferFactor_SR.Divide( CRs[1] )
QCDTransferFactor_B.Divide( As[1] )

# for sr,cr in zip(SRs,CRs):
#     tf = sr.Clone()
#     tf.Divide(cr)
#     TFs.append(tf)


#Get Final QCD prediction

FinalQCDSR = BackgroundSubtractedData_CR.Clone("FinalQCD_SR")
FinalQCDB = BackgroundSubtractedData_A.Clone("FinalQCD_B")

FinalQCDSR.Multiply(QCDTransferFactor_SR)
FinalQCDB.Multiply(QCDTransferFactor_B)

#DRAWING
file_out = ROOT.TFile("out.root","RECREATE")

#c = ROOT.TCanvas()

QCDTransferFactor_SR.Write()
QCDTransferFactor_B.Write()

BackgroundSubtractedData_A.Write()
BackgroundSubtractedData_CR.Write()

FinalQCDSR.Write()
FinalQCDB.Write()

file_out.Close()

# for i,tf in enumerate(TFs):
#     tf.SetLineColor(i+1)
#     tf.SaveAs("tf_" + year + ".root")
#     tf.Draw("same")

#c.Draw()
#c.Print("plot_"+year+".root")
