# Process QCD Trees

## Introduction

Here follows a short recipe of how to run the code, in addition to the information given in the code comments.

The purpose of these programs is to take a `ROOT` tree output from the `CHIP` framework and produce the input to the QCD Method B fitting code (produced by Nicholas Wardle). The QCD estimate using Method A will also be produced, as well as various supplementary control plots.

There are two main stages to this routine, `makeHistograms.C` (process trees and get base `ROOT` histograms), and `scripts/tf.py` (make the final plot images and `ROOT` files for input to the next stage).

## `makeHistograms.C`

The code is run like:

`root makeHistograms.C+`

The `makeHistograms.C` file internally compiles the main analysis loop in `Events.C`. Further information on how to modify these files for your use-case are given in the comments within the file.

The output `ROOT` histogram files (one for each sample and year) will be stored in `/Plots` by default.

## `scripts/tf.py`

The code is run like:

`./scripts/tf.py input_directory output_directory log`

where `input_directory` is the path to the `ROOT` histograms in the first stage, `output_directory` is the name of the output directory for the plots (will be stored under `/Plots/output_directory` by default), and `log` is either `1` or `0` (whether the control plots are output using a log-y scale or not, respectively).

The output files relevant for passing to the next stage, in order to evaluate Method B are called:

```
out_MTR_2017.root
out_MTR_2018.root
out_VTR_2017.root
out_VTR_2018.root
```

The plots relevant for Method A are also produced, as well as the control plots for each region, within the appropriate directory (`SR`, `CR`, `A`, and `B`).

## `scripts/tf2d.py`

A supplementary script to produce 2D control distributions from the output of the `makeHistograms.C` stage. The code is run like:

`./scripts/tf.py input_directory output_directory`

where `input_directory` is the path to the `ROOT` histograms in the first stage, and `output_directory` is the name of the output directory for the plots (will be stored under `/Plots/output_directory` by default).