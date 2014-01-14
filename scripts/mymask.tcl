#!/usr/bin/env tclsh
source ../params.tcl
set d_itr 10
set ratio 3

proc mask {myargs} {
	global d_itr ratio
	set nu [lindex $myargs 0]
	set min_thresh [lindex $myargs 1]
	if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
		puts "ERROR: $::errorInfo and $msg"
	}
}
