#!/usr/bin/env tclsh
package require Tk
eval [exec gnuplot << "
	set term tkcanvas
	set xlabel \"Actual Depth(cm)\"
	set ylabel \"Relative Depth(cm)\"

	set key inside left top vertical Right noreverse enhanced autotitles box linetype -1 linewidth 1.000
	plot \[0:700\]\[0:400\] \"../eval/result.txt\" using 1:2 title '17-29' with dots, \
		\"../eval/result.txt\" u 1:3 t '30-49' w dots, \
		\"../eval/result.txt\" u 1:4 t '50-69' w dots, \
		\"../eval/result.txt\" u 1:5 t '70-83' w dots, \
		\"../eval/result.txt\" u 1:6 t 'gen_err' w dots
	"]
pack [canvas .c -width 1000 -height 1000]
gnuplot .c
