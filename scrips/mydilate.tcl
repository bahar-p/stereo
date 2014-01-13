#!/usr/bim/env tclsh

source ../params.tcl

proc dilate {myargs} {
	set nu [lindex $myargs 0]
	set itr [lindex $myargs 1]
	if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
		puts "Error: $::errorInfo and $msg"
	}
}

