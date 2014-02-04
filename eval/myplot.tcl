#!/usr/bin/env tclsh
#package require Tk
source ../params.tcl

proc plot_rdep {myargs} { 
	set noc [lindex $myargs 0]
	set adc [lindex $myargs 1]
	set imglist [lrange $myargs 2 end]
	foreach img $imglist {
		if {$adc} {
			set rdep_pl "adc_rdep.png"
			set rdep "adcen_rdep${img}.txt"
		} else {
			set rdep "sgbm_rdep${img}.txt"
			set rdep_pl "sgbm_rdep.png"
		}
		if {$noc} {
			set rdep "${::res_noc}${rdep}"
		} else {
			set rdep "${::res_occ}${rdep}"
		}
		set rdep_pl "${img}${rdep_pl}"	
		#puts "$path , $rdep"
		eval [exec gnuplot << "
		set term x11
		set xlabel \"Actual Depth(cm)\"
		set ylabel \"Relative Depth(cm)\"
		set key inside left top vertical Right noreverse enhanced autotitles box linetype -1 linewidth 1.000
		plot \[0:1000\]\[0:700\] \"${rdep}\" using 1:2 title '17-29' with dots, \
			\"${rdep}\" u 1:3 t '30-49' w dots, \
			\"${rdep}\" u 1:4 t '50-69' w dots, \
			\"${rdep}\" u 1:5 t '70-83' w dots, \
			\"${rdep}\" u 1:6 t 'gen_err' w dots
		set term png
		set output \"${::plots}${rdep_pl}\"
		replot
		set term x11
		"]
	}
#pack [canvas .c -width 1000 -height 1000]
#	gnuplot .c
}

plot_rdep $::argv
