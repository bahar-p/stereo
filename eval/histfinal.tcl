#!/usr/bin/env tclsh
source ../params.tcl

proc fhist {bnum width f imgcount} {
	set fp [open "$f" r]
	set fout [open "${::res_noc}histfinal.txt" "w+"]
	set st_1729 0.5333
	set st_3049 0.5625
	set st_5069 0.6458
	set st_7083 1.875
	for {set i 0} {$i<$bnum} {set i [expr {$i+1}]} {
		set bins($i) [expr {($i*$width)+$width/2}];
		set bins(avg,$i) 0;
		set bins(sum,$i) 0;
		set bins(count,$i) 0;
		#puts "$i $bins($i) $bins(sum,$i) $bins(count,$i)"
	}
	set idx 0
	set skip $bnum
	while { $idx < $bnum} {
		seek $fp 0
		for {set c 0} {$c < [expr {$imgcount*$bnum}] } {set c [expr {$c+1}]} {
			if {[gets $fp line] !=-1} {
				if { $c%$skip==$idx } {
					lassign $line img bin_c bin_count bin_sum
					set bins(count,$idx) [expr {$bins(count,$idx)+$bin_count}]
					set bins(sum,$idx) [expr {$bins(sum,$idx)+$bin_sum}]
				}
			}	 
		}
		incr idx
		#puts "idx: $idx"
	}
	puts $fout [list "binCenter" "Avg_stAc" "stAc_1729" "stAc_3049" "st_Ac5069" "stAc_7083"]
	for {set i 0} {$i<$bnum } {set i [expr {$i+1}]} {
		if { $bins(count,$i)!=0} {
			set bins(avg,$i) [expr {$bins(sum,$i)/$bins(count,$i)}]
		}
		puts $fout [list $bins($i) $bins(avg,$i) $st_1729 $st_3049 $st_5069 $st_7083]
		#puts "$i : $bins($i) $bins(count,$i) : $bins(sum,$i) : $bins(avg,$i)" 
	}
}

fhist {*}$::argv
