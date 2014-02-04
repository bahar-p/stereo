#!/usr/bin/env tclsh
source ../params.tcl

proc readt {myargs} {
	set noc [lindex $myargs 0]
	set f [lindex $myargs 1]
	set fp [open "$f" r]
	#set dir [split $f "/"]
	set fname [file tail $f]
	set name [lindex [split $fname "."] 0]
			
	puts "dir: $name"
	set f_data [read $fp]
	set data [split $f_data "\n"]
	set n1 0
	set sum1 0
	foreach line $data {
		set words [split $line " "]
		#puts "$words"
		set num1 [lindex $words 3]
		if {[string length $num1] != 0} {
			incr n1
			set sum1 [expr { $sum1 + $num1 } ]
		}
	}
	set avg1 [expr {$sum1/$n1}]
	puts "avg1: $avg1"
	if {$noc} {
		set fout [open "${::res_noc}avgtime.txt" "a+"]
		puts $fout [list $name "avg_Extime:" $avg1]
	} else {
		set fout [open "${::res_occ}avgtime.txt" "a+"]
		puts $fout [list $name "avg_Extime:" $avg1]
	}
	close $fp
	close $fout
}

readt $::argv

