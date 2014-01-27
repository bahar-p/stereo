#!/usr/bin/env tclsh

set mainp "/home/bahar/Master/stereo/"

# AdCensus #
set adcensus "build/main"
set adcensus "${mainp}${adcensus}"
#SGBM #
set sgbm "build/kitdisp"
set sgbm "${mainp}${sgbm}"
# Canny #
set canny "build/canny"
set canny "${mainp}${canny}"
# Dilate #
set dilate "build/dilate"
set dilate "${mainp}${dilate}"
# MaxGTDisp #
set maxdisp "build/maxdisp"
set maxdisp "${mainp}${maxdisp}"
# L-R Images #
set L "${mainp}simg/L/"
set R "${mainp}simg/R/"
# Calibration Files #
set cal "${mainp}imgcalib/"
# GT_Occlusion Map #
set gt_occ "gt/disp_occ/"
set gt_occ "${mainp}${gt_occ}"

set gt_noc "gt/disp_noc/"
set gt_noc "${mainp}${gt_noc}"

set gtf_occ "gtfill/disp_occ/"
set gtf_occ "${mainp}${gtf_occ}"

set gtf_noc "gtfill/disp_noc/"
set gtf_noc "${mainp}${gtf_noc}"

set mask_occ "masks/disp_occ/"
set mask_occ "${mainp}${mask_occ}"

set mask_noc "masks/disp_noc/"
set mask_noc "${mainp}${mask_noc}"

set plots "plots/"
set plots "${mainp}${plots}"

set evals "eval/"
set evals "${mainp}${evals}"
