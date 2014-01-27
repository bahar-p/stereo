#!/usr/bin/env tclsh
#package require Tk
source ../params.tcl

proc plot {imglist} {
	set outlier "outlier.png"
	
	foreach img $imglist {
		set rdep_pl "rdep.png"
		set rdep "rdep${img}.txt"
		set rdep "${::evals}${rdep}"
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

plot $::argv
