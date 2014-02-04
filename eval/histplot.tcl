#!/usr/bin/env tclsh
#package require Tk
source ../params.tcl

proc myhist {myargs} { 
	set noc [lindex $myargs 0]
	set adc [lindex $myargs 1]
	set imglist [lrange $myargs 2 end]
	foreach img $imglist {
		puts "img: $img"
		if {$adc} {
			set rdep_pl "adc_rdep.png"
			set rdep "adcen_rdep${img}.txt"
			set histv "adcen${img}"
		} else {
			set rdep "sgbm_rdep${img}.txt"
			set rdep_pl "sgbm_rdep.png"
			set histv "sgbm${img}"
		}
		if {$noc} {
			set rdep "${::res_noc}${rdep}"
		} else {
			set rdep "${::res_occ}${rdep}"
		}
		set rdep_pl "${img}${rdep_pl}"	
		eval [exec gnuplot << "
		reset
		set term x11
		set xlabel \"Actual Depth(cm)\"
		set ylabel \"Relative Depth(cm)\"
		binwidth=50
		set boxwidth binwidth
		hist(x,width)=width*floor(x/width)+(width/2.0)
		set table \"${::plots}${histv}.dat\"
		plot \"${rdep}\" using (hist(\$1,binwidth)):(1.0) smooth freq with boxes notitle
		unset table
		set term png
		set output \"${::plots}${rdep_pl}\"
		replot
		set term x11
		"]
	}
#pack [canvas .c -width 1000 -height 1000]
#	gnuplot .c
}

myhist $::argv
