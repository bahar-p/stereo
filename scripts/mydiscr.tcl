#!/usr/bin/env tclsh
source ../params.tcl

proc disc {myargs} {

	set nu [lindex $myargs 0]
	set maxd [lindex $myargs 1]
	set pixthr [lindex $myargs 2]
	set f [lindex $myargs 3]
	set b [lindex $myargs 4]
	set w [lindex $myargs 5]
	set h [lindex $myargs 6]
	set adc [lindex $myargs 7]
	puts "img: $nu adc: $adc"
		if {$adc} {
			if { [catch { exec $::grad "${::gt_noc}${nu}" "${::adc_noc}${nu}" $maxd $pixthr $f $b $w $h $adc} msg] } {
				puts "Something seems to have gone wrong:"
				puts "Information about it: $::errorInfo and $msg"
			}
		} else {
			if { [catch { exec $::grad "${::gt_noc}${nu}" "${::sg_noc}${nu}" $maxd $pixthr $f $b $w $h $adc} msg] } {
				puts "Something seems to have gone wrong:"
				puts "Information about it: $::errorInfo and $msg"
			}
		}
}

