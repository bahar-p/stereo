#!/usr/bin/env tclsh
source ../params.tcl

proc readf {myargs} {
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
	set n2 0
	set sum1 0
	set sum2 0
	foreach line $data {
		set words [split $line " "]
		set num1 [lindex $words 6]
		set num2 [lindex $words 10]
		if {[string length $num1] != 0} {
			incr n1
			set sum1 [expr { $sum1 + $num1 } ]
		}
		if {[string length $num2] != 0} {
			incr n2
			set sum2 [expr { $sum2 + $num2 } ]
		}
	}
	set avg1 [expr {$sum1/$n1}]
	set avg2 [expr {$sum2/$n2}]
	puts "sum1: $sum1 sum2: $sum2 avg1: $avg1 avg2: $avg2"
	if {$noc} {
		set fout [open "${::res_noc}avgerr.txt" "a+"]
		puts $fout [list $name "avg_t_err:" $avg1 "avg_v_err:" $avg2]
	} else {
		set fout [open "${::res_occ}avgerr.txt" "a+"]
		puts $fout [list $name "avg_t_err:" $avg1 "avg_v_err:" $avg2]
	}
	close $fp
	close $fout
}

readf $::argv

