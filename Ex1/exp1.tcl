#!/usr/bin/env tclsh
source ../params.tcl

#set ::mainp "/home/bahar/Master/stereo/"
set maskp  "Ex1/masks/"
set maskp "${::mainp}${maskp}"
#puts "maskp: $maskp"
#set ::adcensus "build/::mainp"
#set ::adcensus "${::mainp}${::adcensus}"
#set ::sgbm "build/kitdisp"
#set ::sgbm "${::mainp}${::sgbm}"

proc runex1 {myargs} {
	set sgbm [lindex $myargs 0]
	set imglist [lrange $myargs 1 end]
	set LR 0
#	global ::mainp
	global maskp
#	global ::adcensus
		foreach img $imglist {
			puts "img: $img"
			switch -exact $img {
			5 {
				set fL "simg/L/000005.png"
				set fR "simg/R/000005.png"
				set cal "imgcalib/000005.txt"
				set m "mask5.png"
				set maxd 80
				set f 707
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::mainp}${cal}" "${::mainp}${fL}" "${::mainp}${fR}" $maxd "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::mainp}${fL}" "${::mainp}${fR}" $maxd $f $T $LR "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			9 {
				set fL "simg/L/000009.png"
				set fR "simg/R/000009.png"
				set cal "imgcalib/000009.txt"
				set m "mask9.png"
				set maxd 80
				set f 718
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::mainp}${cal}" "${::mainp}${fL}" "${::mainp}${fR}" $maxd "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::mainp}${fL}" "${::mainp}${fR}" $maxd $f $T $LR "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			16 {
				set fL "simg/L/000016.png"
				set fR "simg/R/000016.png"
				set cal "imgcalib/000016.txt"
				set m "mask16.png"
				set maxd 160
				set f 718
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::mainp}${cal}" "${::mainp}${fL}" "${::mainp}${fR}" $maxd "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::mainp}${fL}" "${::mainp}${fR}" $maxd $f $T $LR "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			37 {
				set fL "simg/L/000037.png"
				set fR "simg/R/000037.png"
				set cal "imgcalib/000037.txt"
				set m "mask37.png"
				set maxd 128
				set f 721
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::mainp}${cal}" "${::mainp}${fL}" "${::mainp}${fR}" $maxd "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::mainp}${fL}" "${::mainp}${fR}" $maxd $f $T $LR "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			39 {
				set fL "simg/L/000039.png"
				set fR "simg/R/000039.png"
				set cal "imgcalib/000039.txt"
				set m "mask39.png"
				set maxd 128
				set f 721
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::mainp}${cal}" "${::mainp}${fL}" "${::mainp}${fR}" $maxd "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::mainp}${fL}" "${::mainp}${fR}" $maxd $f $T $LR "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			60 {
				set fL "simg/L/000060.png"
				set fR "simg/R/000060.png"
				set cal "imgcalib/000060.txt"
				set m "mask60.png"
				set maxd 96
				set f 707
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::mainp}${cal}" "${::mainp}${fL}" "${::mainp}${fR}" $maxd "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::mainp}${fL}" "${::mainp}${fR}" $maxd $f $T $LR "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			64 {
				set fL "simg/L/000064.png"
				set fR "simg/R/000064.png"
				set cal "imgcalib/000064.txt"
				set m "mask64.png"
				set maxd 160
				set f 718
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::mainp}${cal}" "${::mainp}${fL}" "${::mainp}${fR}" $maxd "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::mainp}${fL}" "${::mainp}${fR}" $maxd $f $T $LR "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			81 {
				set fL "simg/L/000081.png"
				set fR "simg/R/000081.png"
				set cal "imgcalib/000081.txt"
				set m "mask81.png"
				set maxd 80
				set f 721
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::mainp}${cal}" "${::mainp}${fL}" "${::mainp}${fR}" $maxd "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::mainp}${fL}" "${::mainp}${fR}" $maxd $f $T $LR "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			86 {
				set fL "simg/L/000086.png"
				set fR "simg/R/000086.png"
				set cal "imgcalib/000086.txt"
				set m "mask86.png"
				set maxd 96
				set f 707
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::mainp}${cal}" "${::mainp}${fL}" "${::mainp}${fR}" $maxd "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::mainp}${fL}" "${::mainp}${fR}" $maxd $f $T $LR "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			110 {
				set fL "simg/L/000110.png"
				set fR "simg/R/000110.png"
				set cal "imgcalib/000110.txt"
				set m "mask110.png"
				set maxd 96
				set f 718
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::mainp}${cal}" "${::mainp}${fL}" "${::mainp}${fR}" $maxd "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::mainp}${fL}" "${::mainp}${fR}" $maxd $f $T $LR "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg";
			}
			116 {
				set fL "simg/L/000116.png"
				set fR "simg/R/000116.png"
				set cal "imgcalib/000116.txt"
				set m "mask116.png"
				set maxd 128
				set f 718
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::mainp}${cal}" "${::mainp}${fL}" "${::mainp}${fR}" $maxd "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::mainp}${fL}" "${::mainp}${fR}" $maxd $f $T $LR "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg";
			}
			133 {
				set fL "simg/L/000133.png"
				set fR "simg/R/000133.png"
				set cal "imgcalib/000133.txt"
				set m "mask133.png"
				set maxd 160
				set f 718
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::mainp}${cal}" "${::mainp}${fL}" "${::mainp}${fR}" $maxd "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::mainp}${fL}" "${::mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			138 {
				set fL "simg/L/000138.png"
				set fR "simg/R/000138.png"
				set cal "imgcalib/000138.txt"
				set m "mask138.png"
				set maxd 128
				set f 721
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::mainp}${cal}" "${::mainp}${fL}" "${::mainp}${fR}" $maxd "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::mainp}${fL}" "${::mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			169 {
				set fL "simg/L/000169.png"
				set fR "simg/R/000169.png"
				set cal "imgcalib/000169.txt"
				set m "mask169.png"
				set maxd 160
				set f 718
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::mainp}${cal}" "${::mainp}${fL}" "${::mainp}${fR}" $maxd "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::mainp}${fL}" "${::mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			default {
				puts "no match"
			}
		}
		}
}

runex1 $::argv
#foreach arg $::argv {\
	set t [runex1 $arg]\
}
