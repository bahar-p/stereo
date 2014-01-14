#!/usr/bim/env tclsh

source ../params.tcl

proc dilate {img is_noc {itr 0}} {
	set nu $img
	if { [catch {exec $::dilate "${::gt_noc}${nu}" $is_noc $itr} msg] } {
		puts "Error: $::errorInfo and $msg"
	}
}

