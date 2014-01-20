#!/usr/bin/env tclsh
source ./ex1.tcl

# Usage: tclsh run1.tcl $sgbm $noc $L #R  

proc run1 {args} {
	set myargs {*}$args
	variable LR
	variable noc
	puts "myargs: [llength $myargs]"
	switch -exact [llength $myargs] {
		3 {
			set sgbm [lindex $myargs 0]
			set noc [lindex $myargs 1]
			set LR [lindex $myargs 2]
		}
		default {
			puts "Wrong number of args, must be 3"
			return -1;
		}
	}
	if {$sgbm} {
		exper1 [list $sgbm 000004.png 80 $noc 000004.txt]
		exper1 [list $sgbm 000005.png 80 $noc 000005.txt]
		exper1 [list $sgbm 000007.png 128 $noc 000007.txt]
		exper1 [list $sgbm 000009.png 80 $noc 000009.txt]
		exper1 [list $sgbm 000011.png 80 $noc 000011.txt]
		exper1 [list $sgbm 000015.png 160 $noc 000015.txt]
		exper1 [list $sgbm 000016.png 160 $noc 000016.txt]
		exper1 [list $sgbm 000018.png 112 $noc 000018.txt]
		exper1 [list $sgbm 000019.png 80 $noc 000019.txt]
		exper1 [list $sgbm 000021.png 144 $noc 000021.txt]
		exper1 [list $sgbm 000023.png 80 $noc 000023.txt]
		exper1 [list $sgbm 000026.png 96 $noc 000026.txt]
		exper1 [list $sgbm 000029.png 112 $noc 000029.txt]
		exper1 [list $sgbm 000031.png 144 $noc 000031.txt]
		exper1 [list $sgbm 000032.png 80 $noc 000032.txt]
		exper1 [list $sgbm 000037.png 128 $noc 000037.txt]
		exper1 [list $sgbm 000038.png 96 $noc 000038.txt]
		exper1 [list $sgbm 000039.png 112 $noc 000039.txt]
		exper1 [list $sgbm 000040.png 144 $noc 000040.txt]
		exper1 [list $sgbm 000045.png 128 $noc 000045.txt]
		exper1 [list $sgbm 000050.png 80 $noc 000050.txt]
		exper1 [list $sgbm 000053.png 160 $noc 000053.txt]
		exper1 [list $sgbm 000056.png 80 $noc 000056.txt]
		exper1 [list $sgbm 000059.png 112 $noc 000059.txt]
		exper1 [list $sgbm 000060.png 80 $noc 000060.txt]
		exper1 [list $sgbm 000064.png 160 $noc 000064.txt]
		exper1 [list $sgbm 000068.png 80 $noc 000068.txt]
		exper1 [list $sgbm 000081.png 80 $noc 000081.txt]
		exper1 [list $sgbm 000082.png 112 $noc 000082.txt]
		exper1 [list $sgbm 000086.png 80 $noc 000086.txt]
		exper1 [list $sgbm 000088.png 80 $noc 000088.txt]
		exper1 [list $sgbm 000092.png 160 $noc 000092.txt]
		exper1 [list $sgbm 000098.png 80 $noc 000098.txt]
		exper1 [list $sgbm 000103.png 128 $noc 000103.txt]
		exper1 [list $sgbm 000110.png 80 $noc 000110.txt]
		exper1 [list $sgbm 000116.png 112 $noc 000116.txt]
		exper1 [list $sgbm 000123.png 96 $noc 000123.txt]
		exper1 [list $sgbm 000133.png 160 $noc 000133.txt]
		exper1 [list $sgbm 000138.png 80 $noc 000138.txt]
		exper1 [list $sgbm 000139.png 80 $noc 000139.txt]
		exper1 [list $sgbm 000143.png 80 $noc 000143.txt]
		exper1 [list $sgbm 000147.png 80 $noc 000147.txt]
		exper1 [list $sgbm 000153.png 80 $noc 000153.txt]
		exper1 [list $sgbm 000154.png 80 $noc 000154.txt]
		exper1 [list $sgbm 000164.png 176 $noc 000164.txt]
		exper1 [list $sgbm 000167.png 128 $noc 000167.txt]
		exper1 [list $sgbm 000169.png 160 $noc 000169.txt]
		exper1 [list $sgbm 000173.png 96 $noc 000173.txt]
		exper1 [list $sgbm 000180.png 112 $noc 000180.txt]
		exper1 [list $sgbm 000185.png 80 $noc 000185.txt]
		exper1 [list $sgbm 000191.png 112 $noc 000191.txt]
		exper1 [list $sgbm 000193.png 96 $noc 000193.txt]
	} else {
		exper1 [list $sgbm 000004.png 80 $noc $LR 707 0.537]
	}
}

run1 $::argv
