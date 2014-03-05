#!/usr/bin/env tclsh
source ../params.tcl

proc finalplot {f} {
	puts "$f"
	eval [exec gnuplot << "
		reset
		set term x11
		set xlabel \"Actual Depth(m)\"
		set ylabel \"Stereo Acuity(arcmin)\"
		set title \"Average disparity error over distance\"
		set key top right
		plot \[:\]\[0:2\] \"$f\" every ::1 using 1:2 with points t \"Avg Error\", \
			\"$f\" every ::1 using 1:3 with lines t \"Age:17-29\", \
			\"$f\" every ::1 using 1:4 with lines t \"Age:30-49\", \
			\"$f\" every ::1 using 1:5 with lines t \"Age:50-69\"
		MIN=GPVAL_DATA_Y_MIN
		MAX=GPVAL_DATA_Y_MAX
		set term png
		set output \"${::plots}finalhist.png\"
		replot
		set term postscript eps enhanced colour dashed lw 1 \"Helvetica\" 14
		set output \"${::plots}test.eps\" 
		replot
		reset
		set term x11
		"]
}

finalplot $::argv
