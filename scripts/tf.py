import ROOT



#listOfSysts = ["Nominal","ElectronVetoUp","ElectronVetoDown","MuonVetoUp","MuonVetoDown","TauVetoUp","TauVetoDown","BjetVetoUp","BjetVetoDown"]

#listOfSysts = ["Nominal","ElectronVetoUp","MuonVetoUp","TauVetoUp","BjetVetoUp"]
listOfSysts = ["Nominal"]

files = []

SRs = []
CRs = []

TFs = []

for syst in listOfSysts:
    files.append( ROOT.TFile.Open("../Plots/Histos_"+syst+"_QCD.root"))
    SRs.append(files[-1].Get("SR/MTR/h_SR_MTR_dijet_M_binned"))
    CRs.append(files[-1].Get("QCDCR/MTR/h_QCDCR_MTR_dijet_M_binned"))

for sr,cr in zip(SRs,CRs):
    tf = sr.Clone()
    tf.Divide(cr)
    TFs.append(tf)



#DRAWING

c = ROOT.TCanvas()

for i,tf in enumerate(TFs):
    tf.SetLineColor(i+1)
    tf.Draw("same")

c.Draw()
c.Print("plot.root")
