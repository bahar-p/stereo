package require Tk
source ./calleval.tcl
source "../eval/hist.tcl"
#source "../eval/histfinal.tcl"
#source "../eval/histfinal_p.tcl"

set pixthr 3.00
set noc 0
set adc 0
set msk 0
set bins 100
set width 0.5
set imgcnt 52

wm title . "Run Evaluation" 
wm minsize . 300 300
frame .f1
pack .f1

label .f1.l1 -text "Call Evaluation"
spinbox .f1.sp1 -from 0.00 -to 4.00 -increment 0.25 -format %.2f -width 10 \
	     -justify center -text "pixthr" -textvariable pixthr
checkbutton .f1.cb1 -text "noc" -variable noc
checkbutton .f1.cb2 -text "adcensus" -variable adc
checkbutton .f1.cb3 -text "mask" -variable msk
button .f1.b1 -text "Call Eval" -command {myeval $pixthr $noc $adc $msk}

grid .f1.l1 -column 0 -row 0
grid .f1.cb1 -column 1 -row 0
grid .f1.cb2 -column 2 -row 0
grid .f1.cb3 -column 3 -row 0
grid .f1.b1 -column 4 -row 0

label .f1.l2 -text "Avg StAcuity/image"
spinbox .f1.sp2 -from 0 -to 200 -increment 1 -width 10 \
	     -justify center -text "bins" -textvariable bins
spinbox .f1.sp3 -from 0.00 -to 5.00 -increment 0.25 -format %.2f -width 10 \
	     -justify center -text "width" -textvariable width
button .f1.b2 -text "Call hist/image" -command {hist1 $noc $bins $width $adc}
grid .f1.l2 -column 0 -row 1
grid .f1.sp2 -column 1 -row 1
grid .f1.sp3 -column 2 -row 1
grid .f1.b2 -column 4 -row 1

label .f1.l3 -text "Avg StAcuity/all"
spinbox .f1.sp4 -from 0 -to 400 -increment 1 -width 10 \
	     -justify center -text "ImageCount" -textvariable imgcnt
button .f1.b3 -text "Call hist/all" -command {hist2 $bins $width "../eval/results/histimgs.txt" $adc}

grid .f1.l3 -column 0 -row 2
grid .f1.sp4 -column 1 -row 2
grid .f1.b3 -column 2 -row 2

label .f1.l4 -text "Avg StaAcuity plot"
button .f1.b4 -text "Plot" -command {plot "../eval/results/histfinal.txt"}

grid .f1.l4 -column 0 -row 3
grid .f1.b4 -column 1 -row 3

proc myeval {pixthr noc adc msk} {
	puts "$pixthr $noc $adc $msk"
	do_eval [list $pixthr $noc $adc $msk]
}

proc hist1 {noc bins width adc} {
	set adclist "adst000004.txt adst000005.txt adst000007.txt adst000009.txt adst000011.txt adst000015.txt adst000016.txt adst000018.txt adst000019.txt adst000021.txt adst000023.txt adst000026.txt adst000029.txt adst000031.txt adst000032.txt adst000037.txt adst000038.txt adst000039.txt adst000040.txt adst000045.txt adst000050.txt adst000053.txt adst000056.txt adst000059.txt adst000060.txt adst000064.txt adst000068.txt adst000081.txt adst000082.txt adst000086.txt adst000088.txt adst000092.txt adst000098.txt adst000103.txt adst000110.txt adst000116.txt adst000123.txt adst000133.txt adst000138.txt adst000139.txt adst000143.txt adst000147.txt adst000153.txt adst000154.txt adst000164.txt adst000167.txt adst000169.txt adst000173.txt adst000180.txt adst000185.txt adst000191.txt adst000193.txt"
	set sgblist "sgst000004.txt sgst000005.txt sgst000007.txt sgst000009.txt sgst000011.txt sgst000015.txt sgst000016.txt sgst000018.txt sgst000019.txt sgst000021.txt sgst000023.txt sgst000026.txt sgst000029.txt sgst000031.txt sgst000032.txt sgst000037.txt sgst000038.txt sgst000039.txt sgst000040.txt sgst000045.txt sgst000050.txt sgst000053.txt sgst000056.txt sgst000059.txt sgst000060.txt sgst000064.txt sgst000068.txt sgst000081.txt sgst000082.txt sgst000086.txt sgst000088.txt sgst000092.txt sgst000098.txt sgst000103.txt sgst000110.txt sgst000116.txt sgst000123.txt sgst000133.txt sgst000138.txt sgst000139.txt sgst000143.txt sgst000147.txt sgst000153.txt sgst000154.txt sgst000164.txt sgst000167.txt sgst000169.txt sgst000173.txt sgst000180.txt sgst000185.txt sgst000191.txt sgst000193.txt"

	if {$adc} {
		calhist $noc $bins $width $adclist 
	} else {
		calhist $noc $bins $width $sgblist 
	}
}

proc hist1 {bins width f imgcnt} {
	fhist $bins $width $f $imgcnt;
}
proc plot {f} {
	finalplot $f
}

