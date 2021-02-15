#!/usr/bin/env python
import sys,os
import subprocess
from array import array
sys.argv.append( '-b' )
import ROOT
ROOT.TH1.SetDefaultSumw2()
from root_numpy import hist2array
import numpy as np
np.set_printoptions(threshold=sys.maxsize)
import matplotlib.pyplot as pl

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


directory_name = sys.argv[1]
plotname = sys.argv[2]
#log = int(sys.argv[3])

if os.path.exists("Plots/"+plotname) == False:
    os.mkdir("Plots/"+plotname)
if os.path.exists("Plots/"+plotname+"/CR") == False:
    os.mkdir("Plots/"+plotname+"/CR")

years = ["2017","2018"]
years = ["2017"]
#samples = ["MET","VV","TOP","DY","EWKZll","EWKZNUNU","ZJETS","EWKW","WJETS","QCD","QCDRELAX","DATA"]
samples = ["MET","VV","TOP","DY","EWKZll","EWKZNUNU","ZJETS","EWKW","WJETS","QCD","QCDRELAX"]

regions = ["MTR","VTR"]
#regions = ["MTR"]

#variables = ["CentralEtaMTR_ForwardEtaMTR_MetNoLep_CleanJet_mindPhi","CentralEtaVTR_ForwardEtaVTR_MetNoLep_CleanJet_mindPhi"]

variables = ["CentralEtaMTR_ForwardEtaMTR","CentralEtaVTR_ForwardEtaVTR"]
#variables = ["CentralEtaMTR_ForwardEtaMTR_MetNoLep_CleanJet_mindPhi"]


for region in regions:
    for variable in variables:
        if variable == "CentralEtaMTR_ForwardEtaMTR_MetNoLep_CleanJet_mindPhi" and region == "VTR":
            continue
        if variable == "CentralEtaVTR_ForwardEtaVTR_MetNoLep_CleanJet_mindPhi" and region == "MTR":
            continue
        if variable == "CentralEtaMTR_ForwardEtaMTR" and region == "VTR":
            continue
        if variable == "CentralEtaVTR_ForwardEtaVTR" and region == "MTR":
            continue
        for year in years:

            files = []
            SRs = []
            CRs = []

            CRs_projected = []
            CRs_rebinned = []
            CRs_hist1d = []


            for sample in samples:
                files.append( ROOT.TFile.Open("~/invisible/AnalyseTrees/analysechiptrees/"+directory_name+"/Histos_Nominal_"+sample+"_"+year+".root"))
            
                SRs.append(files[-1].Get("SR/"+region+"/h_SR_"+region+"_" + variable))
                CRs.append(files[-1].Get("QCDCR/"+region+"/h_QCDCR_"+region+"_" + variable))
            
            for CR,SR in zip(CRs,SRs):
                CR.Add(SR)

            # for CR in CRs:
            #     CRs_projected.append(CR.Project3D("xy"))

            # print (region, variable, year)

            file_out = ROOT.TFile("Plots/"+ plotname + "/out_" + region + "_" + year+ ".root","RECREATE")
            file_out.mkdir("Projections")
            file_out.cd("Projections")
            #for i,CR in enumerate(CRs_projected):
            for i,CR in enumerate(CRs):
                CR.Write(region +"_"+year + "_"+samples[i])
            file_out.Close()


            # for i,CR in enumerate(CRs_projected):
            #     CRs_rebinned.append( CR.Rebin2D(1,5,region +"_"+year + "_"+variable +"_"+samples[i]) )

            # file_out = ROOT.TFile("Plots/"+ plotname + "/out_" + region + "_" + year+ ".root","Update")
            # file_out.mkdir("Rebinned")
            # file_out.cd("Rebinned")
            # for i,CR in enumerate(CRs_rebinned):
            #     CR.Write(region +"_" +year + "_" + samples[i])       
            # file_out.Close()

            #for j,CR in enumerate(CRs_rebinned):
                #for i in range(1,CR.GetNbinsY()+1):
                    #CRs_hist1d.append ( CR.ProjectionX(region +"_"+year + "_"+samples[j] + "_bin" + str(i),i,i) )

            for i in range(1,CRs[0].GetNbinsX()+1):
                eachbin = []
                for j,CR in enumerate(CRs):
                    eachbin.append ( CR.ProjectionY(region +"_"+year + "_"+samples[j] + "_bin" + str(i),i,i) )
                    eachbin[-1].Scale(1,"width")
                CRs_hist1d.append(eachbin)

            file_out = ROOT.TFile("Plots/"+ plotname + "/out_" + region + "_" + year+ ".root","Update")
            file_out.mkdir("Hist1D")
            file_out.cd("Hist1D")
            for sample in CRs_hist1d:
                for CR in sample:
                    CR.Write()
            file_out.Close()

            # Plotting   
            for e,etabin in enumerate(CRs_hist1d):
                leg = ROOT.TLegend(0.45,0.75,0.87,0.89)
                leg.SetNColumns(2)
                ROOT.gStyle.SetLegendBorderSize(0)
                c = ROOT.TCanvas("stackplot_" + variable)
                stack = ROOT.THStack("hs","")
                #for i,CR in enumerate(CRs):
                for i,CR in enumerate(etabin):

                    if ( samples[i] != "EWKZll"  and samples[i] != "EWKW" and samples[i] != "QCD" and samples[i] != "EWKZNUNU" and samples[i] != "VV" and samples[i] != "TOP" and samples[i] != "DY" and samples[i] != "ZJETS" and samples[i] != "WJETS" and samples[i] != "GluGluHtoInv" and samples[i] != "VBFHtoInv"):
                        continue

                    CR.SetFillColor(ROOT.TColor.GetColor((colors[samples[i]])))
                    CR.SetLineColor(ROOT.TColor.GetColor((colors[samples[i]])))
                    if ( samples[i] == "QCD"):
                        CR.SetLineColor(1)
                    leg.AddEntry(CR,samples[i],"F")
                    stack.Add(CR)

                etabin[0].Draw()
                etabin[0].SetMinimum(0)
                stack.Draw("HISTsame")
                etabin[0].Draw("same")
                leg.Draw()
                if ( etabin[0].Integral()>0 ):
                    etabin[0].SetMinimum(1)
                    etabin[0].SetMaximum(etabin[0].GetMaximum()*2)
                    # ROOT.gPad.SetLogy(log)
                    # if ( log ):
                    #     etabin[0].SetMaximum(etabin[0].GetMaximum()*10)
                    # else:
                    #     etabin[0].SetMaximum(etabin[0].GetMaximum()*1.5)
                c.Draw()
                c.SaveAs("Plots/"+plotname + "/CR/CR_" + variable + "_" + region + "_" + year + "_bin" + str(e).zfill(2) + ".png")
                c.SaveAs("Plots/"+plotname + "/CR/CR_" + variable + "_" + region + "_" + year + "_bin" + str(e).zfill(2) + ".pdf")
                c.Close()
            #file_out.Close()


            #central_forward = 

            # print (type(CRs[0]))
            # array = hist2array(CRs[0])#met
            # print (array)
            
            # # print ("A")
            # # print (np.sum(array,axis=0))
            
            # # print ("B")
            # # print (np.sum(array,axis=1))

            # print ("C")
            # c = np.sum(array,axis=2)

            # # print ("D")
            # # print (np.sum(array,axis=(0,1)))

            # pl.imshow(c)
            # pl.show()


# def hist2array(hist, include_overflow=False, copy=True, return_edges=False):

#     # Determine dimensionality and shape

#     if isinstance(hist, ROOT.TH3):
#         shape = (hist.GetNbinsZ() + 2,
#                  hist.GetNbinsY() + 2,
#                  hist.GetNbinsX() + 2)
#     elif isinstance(hist, ROOT.TH2):
#         shape = (hist.GetNbinsY() + 2, hist.GetNbinsX() + 2)
#     elif isinstance(hist, ROOT.TH1):
#         shape = (hist.GetNbinsX() + 2,)
#     elif isinstance(hist, ROOT.THnBase):
#         shape = tuple([hist.GetAxis(i).GetNbins() + 2
#                        for i in range(hist.GetNdimensions())])

#     # Determine the corresponding numpy dtype
#     for hist_type in 'DFISC':
#         if isinstance(hist, getattr(ROOT, 'TArray{0}'.format(hist_type))):
#             break
#     else:
#         raise AssertionError(
#             "hist is somehow an instance of TH[1|2|3] "
#             "but not TArray[D|F|I|S|C]")

#     # Constuct a NumPy array viewing the underlying histogram array
#     if hist_type == 'C':
#         array_func = getattr(_librootnumpy,
#                              'array_h{0}c'.format(len(shape)))
#         array = array_func(ROOT.AsCObject(hist))
#         array.shape = shape
#     else:
#         dtype = np.dtype(DTYPE_ROOT2NUMPY[hist_type])
#         array = np.ndarray(shape=shape, dtype=dtype,
#                            buffer=hist.GetArray())

#     if return_edges:
#         ndims = hist.GetDimension()
#         axis_getters = ['GetXaxis', 'GetYaxis', 'GetZaxis'][:ndims]

#         edges = []
#         for idim, axis_getter in zip(range(ndims), axis_getters):
#             # GetXaxis expects 0 parameters while we need the axis in GetAxis
#             ax = getattr(hist, axis_getter)(*(() if simple_hist else (idim,)))
#             # `edges` is Nbins + 1 in order to have the last bin's upper edge as well
#             edges.append(np.empty(ax.GetNbins() + 1, dtype=np.double))
#             # load the lower edges into `edges`
#             ax.GetLowEdge(edges[-1])
#             # Get the upper edge of the last bin
#             edges[-1][-1] = ax.GetBinUpEdge(ax.GetNbins())

#     if not include_overflow:
#         # Remove overflow and underflow bins
#         array = array[tuple([slice(1, -1) for idim in range(array.ndim)])]

#     # Preserve x, y, z -> axis 0, 1, 2 order
#     array = np.transpose(array)
#     if copy:
#         array = np.copy(array)

#     if return_edges:
#         return array, edges
#     return array

bashCommand = "cd Plots; tar -zcf " + plotname + ".tar.gz " + plotname + "; cd ../;"
process = subprocess.Popen(bashCommand, shell=True)
