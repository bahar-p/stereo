#!/usr/bin/env tclsh
source ../params.tcl

proc evl {myargs} {

	set nu [lindex $myargs 0]
	set maxd [lindex $myargs 1]
	set noc [lindex $myargs 2]
	set adc [lindex $myargs 3]
	set f [lindex $myargs 4]
	set bs [lindex $myargs 5]
	
	if {$noc} {
		if {$adc} {
			if { [catch { exec $::evals "${::gtm_noc}${nu}" "${::adc_ms_noc}${nu}" $maxd $f $bs $adc $noc "${::mask_noc}${nu}"} msg] } {
				puts "Something seems to have gone wrong:"
				puts "Information about it: $::errorInfo and $msg"
			}
		} else {
			if { [catch { exec $::evals "${::gtm_noc}${nu}" "${::sg_ms_noc}${nu}" $maxd $f $bs $adc $noc "${::mask_noc}${nu}"} msg] } {
				puts "Something seems to have gone wrong:"
				puts "Information about it: $::errorInfo and $msg"
			}
		} 
	} else {
		if {$adc} {
			if { [catch { exec $::evals "${::gtm_occ}${nu}" "${::adc_ms_occ}${nu}" $maxd $f $bs $adc $noc "${::mask_occ}${nu}"} msg] } {
				puts "Something seems to have gone wrong:"
				puts "Information about it: $::errorInfo and $msg"
			} 
		} else {
			if { [catch { exec $::evals "${::gtm_occ}${nu}" "${::sg_ms_occ}${nu}" $maxd $f $bs $adc $noc "${::mask_occ}${nu}"} msg] } {
				puts "Something seems to have gone wrong:"
				puts "Information about it: $::errorInfo and $msg"
			}
		}
	}
}


