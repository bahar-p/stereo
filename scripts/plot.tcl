#!/usr/bin/env tclsh
source ../params.tcl

proc myplot {fsgb fadc} {
	puts "$fsgb $fadc"
	eval [exec gnuplot << "
		reset
		set term x11
		set xlabel \"Detectable threshold in disparity change (arcmin)\"
		set ylabel \"Avg detected pixels\"
		set key top right
		plot \[-0.05:2.1\]\[-0.05:0.15\] \"$fsgb\" using 2:3 with linespoints t \"SGBM\", \"$fadc\" using 2:3 with linespoints t \"ADCensus\"
		set term png
		set output \"${::plots}algthresh.png\"
		replot
		set term postscript eps enhanced colour solid lw 3 \"Arial-Bold\" 15
		set output \"${::plots}algthresh.eps\" 
		replot
		reset
		set term x11
		"]
#set title \"Average pixels detected with change in disparity threshold of stereoacuity  \"
#plot \[0:0.8\]\[0:0.15\] \"$f\" using 2:3 pt 1 ps 1.3 notitle
}

myplot {*}$::argv
