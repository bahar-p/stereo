#!/usr/bin/env tclsh
source ../params.tcl

proc calhist {bnum width args} {
	set fout [open "${::res_noc}histimgs.txt" "a+"]
	puts [list "Name" "binCenter" "binCount" "binSumValue"]
	foreach f $args {
		for {set i 0} {$i<$bnum} {set i [expr {$i+1}]} {
			set bins($i) [expr {($i*$width)+$width/2}];
			set bins(sum,$i) 0;
			set bins(count,$i) 0;
			puts "$i $bins($i) $bins(sum,$i) $bins(count,$i)"
		}
		set fp [open "$f" r]
		#set dir [split $f "/"]
		set fname [file tail $f]
		set name [lindex [split $fname "."] 0]
			
		puts "dir: $name"
		set f_data [read $fp]
		set data [split $f_data "\n"]
		foreach line $data {
			lassign $line depth st_1729 st_3049 st_5069 st_7083 mystAc
			if {[string length $depth] !=0 } {	
				set rounded [expr {$width*(floor($depth/$width))+($width/2)}]
				for {set i 0} {$i<$bnum } {set i [expr {$i+1}]} {
					if {$rounded == $bins($i)} {
						incr bins(count,$i)
						set bins(sum,$i) [expr {$bins(sum,$i)+$mystAc}]
					}
					#puts "$i $bins(sum,$i) $bins(count,$i)"
				}
	
			}
		}
		for {set i 0} {$i<$bnum } {set i [expr {$i+1}]} {
			puts $fout [list $name $bins($i) $bins(count,$i) $bins(sum,$i)]
		}
	}
}

calhist {*}$::argv 
