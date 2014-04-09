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
# EVAL
set evals "build/eval"
set evals "${mainp}${evals}"
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

set gtm_noc "maskgt/disp_noc/"
set gtm_noc "${mainp}${gtm_noc}"

set gtm_occ "maskgt/disp_occ/"
set gtm_occ "${mainp}${gtm_occ}"

set mask_occ "masks/disp_occ/"
set mask_occ "${mainp}${mask_occ}"

set mask_noc "masks/disp_noc/"
set mask_noc "${mainp}${mask_noc}"

set sg_ms_noc "Ex1/sgbm/dispmasked/noc/"
set sg_ms_noc "${mainp}${sg_ms_noc}"

set sg_noc  "Ex1/sgbm/mydisp/noc/"
set sg_noc "${mainp}${sg_noc}"

set sg_ms_occ "Ex1/sgbm/dispmasked/occ/"
set sg_ms_occ "${mainp}${sg_ms_occ}"

set sg_occ "Ex1/sgbm/mydisp/occ/"
set sg_occ "${mainp}${sg_occ}"

set adc_ms_noc "Ex1/adcensus/dispmasked/noc/"
set adc_ms_noc "${mainp}${adc_ms_noc}"

set adc_noc  "Ex1/adcensus/mydisp/noc/"
set adc_noc "${mainp}${adc_noc}"

set adc_ms_occ "Ex1/adcensus/dispmasked/occ/"
set adc_ms_occ "${mainp}${adc_ms_occ}"

set adc_occ "Ex1/adcensus/mydisp/occ/"
set adc_occ "${mainp}${adc_occ}"

set plots "plots/"
set plots "${mainp}${plots}"

set res_noc "eval/results/noc/"
set res_noc "${mainp}${res_noc}"

set res_occ "eval/results/occ/"
set res_occ "${mainp}${res_occ}"

#discretize
set grad "build/gradient"
set grad "${mainp}${grad}"

#GTminStAc
set gtstac "build/minStAc"
set gtstac "${mainp}${gtstac}"
