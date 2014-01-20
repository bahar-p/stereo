#!/usr/bin/env tclsh
source ../params.tcl

proc exper1 {myargs} {
	set IfSGBM [lindex $myargs 0]
	set nu [lindex $myargs 1]
	set maxd [lindex $myargs 2]
	set noc [lindex $myargs 3]

	puts "img: $nu"
	if {$IfSGBM} {
		set cnu [lindex $myargs 4]
		#puts "${::cal}${cnu} ${::L}${nu} ${::R}${nu} $maxd ${::mask_occ}${nu}"
		if {$noc} {
			if { [catch { exec $::sgbm "${::L}${nu}" "${::R}${nu}" $maxd $noc "${::mask_noc}${nu}" "${::cal}${cnu}"} msg] } {
		 		puts "Something seems to have gone wrong:"
				puts "Information about it: $::errorInfo and $msg"
			}

		} else {
			if { [catch { exec $::sgbm "${::L}${nu}" "${::R}${nu}" $maxd $noc "${::mask_occ}${nu}" "${::cal}${cnu}"} msg] } {
				puts "Something seems to have gone wrong:"
				puts "Information about it: $::errorInfo and $msg"
			}
		}
	} else {
		set LR [lindex $myargs 4]
		set f [lindex $myargs 5]
		set T [lindex $myargs 6]
		if {$noc} {
			if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}" $LR $f $T } msg] } {
				puts "Something seems to have gone wrong:"
				puts "Information about it: $::errorInfo and $msg"
			}
		} else {
			if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_occ}${nu}" $LR $f $T } msg] } {
				puts "Something seems to have gone wrong:"
				puts "Information about it: $::errorInfo and $msg"
			}
		}
	}
	puts "Ex1 returned: $msg"
}
