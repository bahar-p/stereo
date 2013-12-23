#!/usr/bin/env tclsh

source ../params.tcl

proc do_dilate {imglist} {

	foreach img $imglist {
		puts "img: $img"
		switch -exact $img {
			5 {
				set nu "000005.png"
				set itr 1
				if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
					puts "Error: $::errorInfo and $msg"
				}
			}
			9 {
				set nu "000009.png"
				set itr 2
				if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
					puts "Error: $::errorInfo and $msg"
				}
			}
			16 {
				set nu "000016.png"
				set itr 4
				if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
					puts "Error: $::errorInfo and $msg"
				}
			}
			37 {
				set nu "000037.png"
				set itr 3
				if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
					puts "Error: $::errorInfo and $msg"
				}
			}
			39 {
				set nu "000039.png"
				set itr 2
				if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
					puts "Error: $::errorInfo and $msg"
				}
			}
			60 {
				set nu "000060.png"
				set itr 2
				if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
					puts "Error: $::errorInfo and $msg"
				}
			}
			64 {
				set nu "000064.png"
				set itr 3
				if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
					puts "Error: $::errorInfo and $msg"
				}
			}
			81 {
				set nu "000081.png"
				set itr 2
				if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
					puts "Error: $::errorInfo and $msg"
				}
			}
			86 {
				set nu "000086.png"
				set itr 2
				if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
					puts "Error: $::errorInfo and $msg"
				}
			}
			110 {
				set nu "000110.png"
				set itr 3
				if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
					puts "Error: $::errorInfo and $msg"
				}
			}
			116 {
				set nu "000116.png"
				set itr 2
				if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
					puts "Error: $::errorInfo and $msg"
				}
			}
			133 {
				set nu "000133.png"
				set itr 4
				if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
					puts "Error: $::errorInfo and $msg"
				}
			}
			138 {
				set nu "000138.png"
				set itr 3
				if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
					puts "Error: $::errorInfo and $msg"
				}
			}
			169 {
				set nu "000169.png"
				set itr 3
				if { [catch {exec $::dilate "${::gt_noc}${nu}" $itr} msg] } {
					puts "Error: $::errorInfo and $msg"
				}
			}
			default {
				puts "no match found"
			}
		}
	}
}

do_dilate $::argv
