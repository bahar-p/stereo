#!/usr/bin/env tclsh
source ../params.tcl

proc evl {myargs} {

	set nu [lindex $myargs 0]
	set maxd [lindex $myargs 1]
	set pixthr [lindex $myargs 2]
	set noc [lindex $myargs 3]
	set adc [lindex $myargs 4]
	set f [lindex $myargs 5]
	set bs [lindex $myargs 6]
	set msk [lindex $myargs 7]	
	puts "img: $nu"
	if {$noc} {
		if {$adc} {
			if {$msk} {	
				if { [catch { exec $::evals "${::gtm_noc}${nu}" "${::adc_ms_noc}${nu}" $maxd $f $bs $pixthr $adc $noc "${::mask_noc}${nu}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
			} else {
				if { [catch { exec $::evals "${::gt_noc}${nu}" "${::adc_noc}${nu}" $maxd $f $bs $pixthr $adc $noc} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
			}
		} else {
			if {$msk} {
				if { [catch { exec $::evals "${::gtm_noc}${nu}" "${::sg_ms_noc}${nu}" $maxd $f $bs $pixthr $adc $noc "${::mask_noc}${nu}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
			} else {
				#puts "${::gt_noc}${nu} ${::sg_noc}${nu} $maxd $f $bs $adc $noc"
				if { [catch { exec $::evals "${::gt_noc}${nu}" "${::sg_noc}${nu}" $maxd $f $bs $pixthr $adc $noc} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
			}
		}
	} else {
		if {$adc} {
			if {$msk} {
				if { [catch { exec $::evals "${::gtm_occ}${nu}" "${::adc_ms_occ}${nu}" $maxd $f $bs $pixthr $adc $noc "${::mask_occ}${nu}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
			} else {
				if { [catch { exec $::evals "${::gt_occ}${nu}" "${::adc_occ}${nu}" $maxd $f $bs $pixthr $adc $noc} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
			}
		} else {
			if {$msk} {
				if { [catch { exec $::evals "${::gtm_occ}${nu}" "${::sg_ms_occ}${nu}" $maxd $f $bs $pixthr $adc $noc} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
			} else {
				if { [catch { exec $::evals "${::gt_occ}${nu}" "${::sg_occ}${nu}" $maxd $f $bs $pixthr $adc $noc "${::mask_occ}${nu}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
			}
		}
	}
}


