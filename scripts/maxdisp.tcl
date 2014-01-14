#!/usr/bin/env tchsh
source ../params.tcl

proc maxDisp {img noc} {
	set nu $img
	set is_noc $noc 
	if {$is_noc} {
		if { [catch {exec $::maxdisp "${::gt_noc}${nu}" $is_noc} msg ] } {
			puts "ERROR: $::errorInfo and $msg"
		}
	} else {
		if { [catch {exec $::maxdisp "${::gt_occ}${nu}" $is_noc} msg ] } {
			puts "ERROR: $::errorInfo and $msg"
		}
	}
	puts "maxgtdisp returned: $msg"
}
