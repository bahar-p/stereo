#!/usr/bin/env tchsh
source ./maxdisp.tcl

proc run_maxdisp {myargs} {
	set noc [lindex $myargs 0]
	set imglist [lrange $myargs 1 end]	
	foreach img $imglist {
		maxDisp $img $noc
	}
}

run_maxdisp $::argv
