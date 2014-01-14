#!/usr/bin/env tclsh
source ../params.tcl
set d_itr 10
set ratio 3

proc mask {img is_noc {min_thresh 0}} {
	global d_itr ratio
	set nu $img
	if {$is_noc} {
		if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $is_noc $min_thresh} msg ] } {
			puts "ERROR: $::errorInfo and $msg"
		}
	} else {
		if { [catch {exec $::canny "${::gtf_occ}${nu}" "${::gt_occ}${nu}" $ratio $d_itr $is_noc $min_thresh} msg ] } {
			puts "ERROR: $::errorInfo and $msg"
		}
	}
}
