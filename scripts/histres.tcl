#!/usr/bin/env tclsh
source ../params.tcl

proc myhist {bnum f imgcount} {
	set fp [open "$f" r]
	set fout [open "/home/bahar/Master/stereo/eval/results/histres.txt" "w+"]
	set thresh {0.002 0.005 0.008 0.01 0.03 0.05 0.1 0.3 0.5 0.7}
	for {set i 0} {$i<$bnum} {set i [expr {$i+1}]} {
		set bins($i) [lindex $thresh $i];
		set bins(avg,$i) 0;
		set bins(sum,$i) 0;
		set bins(count,$i) 0;
		puts "$i $bins($i) $bins(sum,$i) $bins(count,$i)"
	}
	set idx 0
	set skip $bnum
	while { $idx < $bnum} {
		seek $fp 0
		for {set c 0} {$c < [expr {$imgcount*$bnum}] } {set c [expr {$c+1}]} {
			if {[gets $fp line] !=-1} {
				if { $c%$skip==$idx } {
					lassign $line img indx thr detected pixcount f b w h
					set bins(count,$idx) [expr {$bins(count,$idx)+($pixcount)}]
					set bins(sum,$idx) [expr {$bins(sum,$idx)+($pixcount*$detected)}]
				}
			}	 
		}
		incr idx
		#puts "idx: $idx"
	}
	puts $fout [list "index" "threshold" "Avg_detectedPixs" ]
	for {set i 0} {$i<$bnum } {incr i} {
		if { $bins(count,$i)!=0} {
			set bins(avg,$i) [expr {$bins(sum,$i)/$bins(count,$i)}]
		}
		puts $fout [list $i $bins($i) $bins(avg,$i)]
		#puts "$i : $bins($i) $bins(count,$i) : $bins(sum,$i) : $bins(avg,$i)" 
	}
}

myhist {*}$::argv
