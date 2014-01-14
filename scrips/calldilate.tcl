#!/usr/bin/env tclsh
source ./mydilate.tcl

proc run_dilate {myargs} {
	set noc [lindex $myargs 0]
	set imglist [lrange $myargs 1 end]
	foreach img $imglist {
		dilate $img $noc
	}
}

run_dilate $::argv
