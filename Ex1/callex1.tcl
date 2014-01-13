#!/usr/bin/env tclsh
source ./ex1.tcl

proc run1 {args} {
	set myargs {*}$args
	puts "myargs: [llength $myargs]"
	switch -exact [llength $myargs] {
		1 {
			set sgbm [lindex $myargs 0]
			set LR 0
			puts "case 1"
		}
		2 {
			set sgbm [lindex $myargs 0]
			set LR [lindex $myargs 1]
			puts "case 2"
		}
		default {
			set sgbm 0
			set LR 0
			puts "case 0"
		}

	}
exper1 [list 000005.png 000005.txt 80 707 0.537 $sgbm $LR]
exper1 [list 000005.png 000005.txt 80 707 0.537 $sgbm $LR]
exper1 [list 000009.png 000009.txt 80 718.8 0.537 $sgbm $LR]
exper1 [list 000016.png 000016.txt 160 718.8 0.537 $sgbm $LR]
exper1 [list 000037.png 000037.txt 128 721.5 0.537 $sgbm $LR]
exper1 [list 000039.png 000039.txt 128 707 0.537 $sgbm $LR]
exper1 [list 000060.png 000060.txt 96 718.8 0.537 $sgbm $LR]
exper1 [list 000064.png 000064.txt 160 721.5 0.537 $sgbm $LR]
exper1 [list 000081.png 000081.txt 80 707 0.537 $sgbm $LR]
exper1 [list 000086.png 000086.txt 96 718.8 0.537 $sgbm $LR]
exper1 [list 000110.png 000110.txt 96 721.5 0.537 $sgbm $LR]
exper1 [list 000116.png 000116.txt 128 707 0.537 $sgbm $LR]
exper1 [list 000133.png 000133.txt 160 718.8 0.537 $sgbm $LR]
exper1 [list 000138.png 000138.txt 128 718.8 0.537 $sgbm $LR]
exper1 [list 000169.png 000169.txt 160 718.8 0.537 $sgbm $LR]

#exper1 000005.png 000005.txt 128 721.5 0.537 $sgbm\
exper1 000005.png 000005.txt 160 718.8 0.537 $sgbm\
exper1 000005.png 000005.txt 80 707 0.537 $sgbm\
exper1 000005.png 000005.txt 80 707 0.537 $sgbm\
exper1 000005.png 000005.txt 80 707 0.537 $sgbm\
exper1 000005.png 000005.txt 80 707 0.537 $sgbm\
exper1 000005.png 000005.txt 80 707 0.537 $sgbm\
exper1 000005.png 000005.txt 80 707 0.537 $sgbm\
exper1 000005.png 000005.txt 80 707 0.537 $sgbm\
exper1 000005.png 000005.txt 80 707 0.537 $sgbm\
exper1 000005.png 000005.txt 80 707 0.537 $sgbm\
exper1 000005.png 000005.txt 80 707 0.537 $sgbm\
exper1 000005.png 000005.txt 80 707 0.537 $sgbm\
exper1 000005.png 000005.txt 80 707 0.537 $sgbm\
exper1 000005.png 000005.txt 80 707 0.537 $sgbm\
exper1 000005.png 000005.txt 80 707 0.537 $sgbm\

}

run1 $::argv
