#!/usr/bin/env tclsh
source ../params.tcl

proc mstac {myargs} {

	set nu [lindex $myargs 0]
	set f [lindex $myargs 1]
	set b [lindex $myargs 2]
	set w [lindex $myargs 3]
	set h [lindex $myargs 4]
	puts "img: $nu"
		if { [catch { exec $::gtstac "${::gt_noc}${nu}" $f $b $w $h} msg] } {
			puts "Something seems to have gone wrong:"
			puts "Information about it: $::errorInfo and $msg"
		}
	
}

