#!/usr/bin/env tclsh
source ../params.tcl

proc exper1 {myargs} {
	set nu [lindex $myargs 0]
	set cnu [lindex $myargs 1]
	set maxd [lindex $myargs 2]
	set f [lindex $myargs 3]
	set T [lindex $myargs 4]
	set IFsgbm [lindex $myargs 5]
	set LR [lindex $myargs 6]

	puts "img: $nu"
	if {$IFsgbm} {
		#puts "${::cal}${cnu} ${::L}${nu} ${::R}${nu} $maxd ${::mask_occ}${nu}"
		if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_occ}${nu}"} msg] } {
			puts "Something seems to have gone wrong:"
			puts "Information about it: $::errorInfo and $msg"
		}
	} else {
		if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T $LR "${::mask_occ}${nu}"} msg] } {
			puts "Something seems to have gone wrong:"
			puts "Information about it: $::errorInfo and $msg"
		}
	}
	puts "Ex1 returned: $msg"
}
