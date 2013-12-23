#!/usr/bin/env tclsh
source ../params.tcl

proc runex1 {myargs} {
	set sgbm [lindex $myargs 0]
	set imglist [lrange $myargs 1 end]
	set LR 0
#	global ::mainp
#	global ::adcensus
		foreach img $imglist {
			puts "img: $img"
			switch -exact $img {
			5 {
				set nu "000005.png"
				set cnu "000005.txt"
				set maxd 80
				set f 707
				set T 537
				if {$sgbm} {
					puts "${::cal}${cnu} ${::L}${nu} ${::R}${nu} $maxd ${::mask_noc}${nu}"
					if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T $LR "${mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			9 {
				set nu "000009.png"
				set cnu "000009.txt"
				set maxd 80
				set f 718
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T $LR "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			16 {
				set nu "000016.png"
				set cnu "000016.txt"
				set maxd 160
				set f 718
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T $LR "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			37 {
				set nu "000037.png"
				set cnu "000037.txt"
				set maxd 128
				set f 721
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T $LR "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			39 {
				set nu "000039.png"
				set cnu "000039.txt"
				set maxd 128
				set f 721
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T $LR "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			60 {
				set nu "000060.png"
				set cnu "000060.txt"
				set maxd 96
				set f 707
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T $LR "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			64 {
				set nu "000064.png"
				set cnu "000064.txt"
				set maxd 160
				set f 718
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T $LR "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			81 {
				set nu "000081.png"
				set cnu "000081.txt"
				set maxd 80
				set f 721
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T $LR "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			86 {
				set nu "000086.png"
				set cnu "000086.txt"
				set maxd 96
				set f 707
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T $LR "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			110 {
				set nu "000110.png"
				set cnu "000110.txt"
				set maxd 96
				set f 718
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T $LR "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg";
			}
			116 {
				set nu "000116.png"
				set cnu "000116.txt"
				set maxd 128
				set f 718
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T $LR "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg";
			}
			133 {
				set nu "000133.png"
				set cnu "000133.txt"
				set maxd 160
				set f 718
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			138 {
				set nu "000138.png"
				set cnu "000138.txt"
				set maxd 128
				set f 721
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				}
				puts "Ex1 returned: $msg"
			}
			169 {
				set nu "000169.png"
				set cnu "000169.txt"
				set maxd 160
				set f 718
				set T 537
				if {$sgbm} {
					if { [catch { exec $::sgbm "${::cal}${cnu}" "${::L}${nu}" "${::R}${nu}" $maxd "${::mask_noc}${nu}"} msg] } {
						puts "Something seems to have gone wrong:"
						puts "Information about it: $::errorInfo and $msg"
					}
				} else {
					if { [catch { exec $::adcensus "${::L}${nu}" "${::R}${nu}" $maxd $f $T "${::mask_noc}${nu}"} msg] } {
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
