#!/usr/bin/env tclsh

set mainp "/home/bahar/Master/stereo/"
set maskp  "Ex1/masks/"
set maskp "${mainp}${maskp}"
#puts "maskp: $maskp"
set exep "build/main"
set exep "${mainp}${exep}"
#puts "execname: $exep";

proc runex1 {img} {
	global mainp
	global maskp
	global exep
		puts "img: $img"
		switch -exact $img {
			5 {
				set fL "simg/L/000005.png"
				set fR "simg/R/000005.png"
				set m "mask5.png"
				set maxd 80
				set f 707
				set T 537
				if { [catch { exec $exep "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
				#set result [exec $exec_p "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"]
				puts "exp1 returned: $msg";
			}
			9 {
				set fL "simg/L/000009.png"
				set fR "simg/R/000009.png"
				set m "mask9.png"
				set maxd 80
				set f 718
				set T 537
				if { [catch { exec $exep "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
				#set result [exec $exec_p "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"]
				puts "exp1 returned: $msg";
			}
			16 {
				set fL "simg/L/000016.png"
				set fR "simg/R/000016.png"
				set m "mask16.png"
				set maxd 160
				set f 718
				set T 537
				if { [catch { exec $exep "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
				#set result [exec $exec_p "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"]
				puts "exp1 returned: $msg";
			}
			37 {
				set fL "simg/L/000037.png"
				set fR "simg/R/000037.png"
				set m "mask37.png"
				set maxd 128
				set f 721
				set T 537
				if { [catch { exec $exep "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
				#set result [exec $exec_p "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"]
				puts "exp1 returned: $msg";
			}
			39 {
				set fL "simg/L/000039.png"
				set fR "simg/R/000039.png"
				set m "mask39.png"
				set maxd 128
				set f 721
				set T 537
				if { [catch { exec $exep "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
				#set result [exec $exec_p "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"]
				puts "exp1 returned: $msg";
			}
			60 {
				set fL "simg/L/000060.png"
				set fR "simg/R/000060.png"
				set m "mask60.png"
				set maxd 96
				set f 707
				set T 537
				if { [catch { exec $exep "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
				#set result [exec $exec_p "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"]
				puts "exp1 returned: $msg";
			}
			64 {
				set fL "simg/L/000064.png"
				set fR "simg/R/000064.png"
				set m "mask64.png"
				set maxd 160
				set f 718
				set T 537
				if { [catch { exec $exep "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
				#set result [exec $exec_p "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"]
				puts "exp1 returned: $msg";
			}
			81 {
				set fL "simg/L/000081.png"
				set fR "simg/R/000081.png"
				set m "mask81.png"
				set maxd 80
				set f 721
				set T 537
				if { [catch { exec $exep "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
				#set result [exec $exec_p "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"]
				puts "exp1 returned: $msg";
			}
			86 {
				set fL "simg/L/000086.png"
				set fR "simg/R/000086.png"
				set m "mask86.png"
				set maxd 96
				set f 707
				set T 537
				if { [catch { exec $exep "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
				#set result [exec $exec_p "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"]
				puts "exp1 returned: $msg";
			}
			110 {
				set fL "simg/L/000110.png"
				set fR "simg/R/000110.png"
				set m "mask110.png"
				set maxd 96
				set f 718
				set T 537
				if { [catch { exec $exep "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
				#set result [exec $exec_p "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"]
				puts "exp1 returned: $msg";
			}
			116 {
				set fL "simg/L/000116.png"
				set fR "simg/R/000116.png"
				set m "mask116.png"
				set maxd 128
				set f 718
				set T 537
				if { [catch { exec $exep "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
				#set result [exec $exec_p "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"]
				puts "exp1 returned: $msg";
			}
			133 {
				set fL "simg/L/000133.png"
				set fR "simg/R/000133.png"
				set m "mask133.png"
				set maxd 160
				set f 718
				set T 537
				if { [catch { exec $exep "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
				#set result [exec $exec_p "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"]
				puts "exp1 returned: $msg";
			}
			138 {
				set fL "simg/L/000138.png"
				set fR "simg/R/000138.png"
				set m "mask138.png"
				set maxd 128
				set f 721
				set T 537
				if { [catch { exec $exep "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
				#set result [exec $exec_p "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"]
				puts "exp1 returned: $msg";
			}
			169 {
				set fL "simg/L/000169.png"
				set fR "simg/R/000169.png"
				set m "mask169.png"
				set maxd 160
				set f 718
				set T 537
				if { [catch { exec $exep "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"} msg] } {
					puts "Something seems to have gone wrong:"
					puts "Information about it: $::errorInfo and $msg"
				}
				#set result [exec $exec_p "${mainp}${fL}" "${mainp}${fR}" $maxd $f $T "${maskp}${m}"]
				puts "exp1 returned: $msg";
			}
			default {
				puts "no match"
			}
		}	
}

foreach arg $::argv {
	set t [runex1 $arg]
}
