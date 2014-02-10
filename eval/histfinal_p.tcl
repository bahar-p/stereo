#!/usr/bin/env tclsh
source ../params.tcl

proc finalplot {f} {
	puts "$f"
	eval [exec gnuplot << "
		reset
		set term x11
		set xlabel \"Actual Depth(m)\"
		set ylabel \"Stereo Acuity(arcmin)\"
		plot \[:\]\[0:2\] \"$f\" every ::1 using 1:2 with points notitle, \
			\"$f\" every ::1 using 1:3 with lines notitle, \
			\"$f\" every ::1 using 1:4 with lines notitle, \
			\"$f\" every ::1 using 1:5 with lines notitle
		MIN=GPVAL_DATA_Y_MIN
		MAX=GPVAL_DATA_Y_MAX
		set term png
		set output \"${::plots}finalhist.png\"
		replot
		reset
		set term x11
		"]
}

finalplot $::argv
