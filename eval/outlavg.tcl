#!/usr/bin/env tclsh
source ../params.tcl

proc avgoutl {noc f imgcount} {
	set fp [open "$f" r]
	#set dir [split $f "/"]
	set fname [file tail $f]
	set name [lindex [split $fname "."] 0]		
	puts "dir: $name"
	array set stAc {0 0.53 1 0.56 2 0.64 3 1.87}
	for {set i 0} {$i < 4} {incr i} {
		set stAc(Toutl,$i) 0 
		set stAc(Voutl,$i) 0 
	}
	set idx 0
	set skip 4
	while {$idx < 4} {
		seek $fp 0
		for {set c 0} {$c < [expr {4*$imgcount}] } {set c [expr {$c+1}]} {
			if {[gets $fp line] !=-1} {
				if { $c%$skip==$idx } {
					set words [split $line " "]
					set t_outl [lindex $words 10]
					set v_outl [lindex $words 14]
					set stAc(Toutl,$idx) [expr {$stAc(Toutl,$idx)+$t_outl}] 
					set stAc(Voutl,$idx) [expr {$stAc(Voutl,$idx)+$v_outl}] 
				}
			}
		}
		incr idx
	}
	for {set i 0} {$i<4} {incr i} {
		set stAc(Toutl,$i) [expr {$stAc(Toutl,$i)/$imgcount}]
		set stAc(Voutl,$i) [expr {$stAc(Voutl,$i)/$imgcount}]
		puts "TotalOutl: $stAc(Toutl,$i) ValidOutl: $stAc(Voutl,$i)"
	}
	if {$noc} {
		set fout [open "${::res_noc}avgOutl.txt" "a+"]
	} else {
		set fout [open "${::res_occ}avgOutl.txt" "a+"]
	}
	set fm "%s %s %.2f %.2f %s %.2f %.2f %s %.2f %.2f %s %.2f %.2f"
	puts $fout [format $fm $name "stAc_1729" $stAc(Toutl,0) $stAc(Voutl,0) "stAc_3049" $stAc(Toutl,1) $stAc(Voutl,1) "stAc_5069" \
		$stAc(Toutl,2) $stAc(Voutl,2) "stAc_7083" $stAc(Toutl,3) $stAc(Voutl,3)]
}

avgoutl {*}$::argv
