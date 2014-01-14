#!/usr/bin/env tclsh

source ./mymask.tcl

proc run_mask {myargs} {
	set noc [lindex $myargs 0]	
	set imglist [lrange $myargs 1 end]
	foreach img $imglist {
		mask $img $noc
	}
}

run_mask $::argv
