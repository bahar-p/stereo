#!/usr/bim/env tclsh

source ../params.tcl

proc dilate {img is_noc {itr 0}} {
	set nu $img
	if {$is_noc} {
		if { [catch {exec $::dilate "${::gt_noc}${nu}" $is_noc $itr} msg] } {
			puts "Error: $::errorInfo and $msg"
		}
	} else {
		if { [catch {exec $::dilate "${::gt_occ}${nu}" $is_noc $itr} msg] } {
			puts "Error: $::errorInfo and $msg"
		}
	}
}

