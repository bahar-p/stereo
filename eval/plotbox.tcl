#!/usr/bin/env tclsh
source ../params.tcl

proc plot_rdep {myargs} { 
	set noc [lindex $myargs 0]
		set timefile "avgtime.txt"
		if {$noc} {
			set timefile "${::res_noc}${timefile}"
			set timeplot "AvgTimenoc.png"
		} else {
			set timefile "${::res_occ}${timefile}"
			set timeplot "AvgTimeocc.png"
		}
		#puts "$path , $rdep"
		eval [exec gnuplot << "
		reset
		plot \"${timefile}\" u yticlabels(3):xticlabels(1) w boxes lc rgb\"red\" t 'Time'
		set term x11
		set xlabel \"Algorithms\"
		set ylabel \"ExecutionTime(sec)\"
		set boxwidth 0.5 absolute
		set style fill transparent solid 0.5 noborder
		MAX=GPVAL_Y_MAX
		MIN=GPVAL_Y_MIN
		set yrange \[0:MAX+(MAX-MIN)*0.05\]
		set ytics (\"MIN\" 1, \"MAX\" 2)
		plot \"${timefile}\" u 3:xticlabels(1) w boxes lc rgb\"red\" notitle
		set term png
		set output \"${::plots}${timeplot}\"
		replot
		set term x11
		"]
	#pack [canvas .c -width 1000 -height 1000]
	#gnuplot .c
}

plot_rdep $::argv
