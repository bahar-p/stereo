source calleval.tcl
package require Tk

set ::var 3.00

wm title . "Run Evaluation"
frame .f1
pack .f1
pack [canvas .cv -in .f1]

label .l1 -text "Call Evaluation"
label .l2 -text "Call Histogram1"
label .l3 -text "Call Histogram2"

spinbox .sp1 -from 0.00 -to 4.00 -increment 0.25 -format %.2f -width 10 \
	     -font 10 -justify center -textvariable var

#proc myeval { } {
#	do_eval [list  
#}

