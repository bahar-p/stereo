#!/usr/bin/env tclsh
source ../params.tcl

set d_itr 10
set ratio 3

proc build_mask {imglist} {
	global d_itr ratio
#global ratio

	foreach img $imglist {
		puts "img: $img"
		switch -exact $img {
			5 {
				set min_thresh 7
				set nu "000005.png"
				if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
					puts "ERROR: $::errorInfo and $msg"
				}
			}
			9 {
				set min_thresh 4
				set nu "000009.png"
				if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
					puts "ERROR: $::errorInfo and $msg"
				}
			}
			16 {
				set min_thresh 5
				set nu "000016.png"
				if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
					puts "ERROR: $::errorInfo and $msg"
				}
			}
			37 {
				set min_thresh 5
				set nu "000037.png"
				if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
					puts "ERROR: $::errorInfo and $msg"
				}
			}
			39 {
				set min_thresh 4
				set nu "000039.png"
				if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
					puts "ERROR: $::errorInfo and $msg"
				}
			}
			60 {
				set min_thresh 4
				set nu "000060.png"
				if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
					puts "ERROR: $::errorInfo and $msg"
				}
			}
			64 {
				set min_thresh 7
				set nu "000064.png"
				if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
					puts "ERROR: $::errorInfo and $msg"
				}
			}
			81 {
				set min_thresh 5
				set nu "000081.png"
				if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
					puts "ERROR: $::errorInfo and $msg"
				}
			}
			86 {
				set min_thresh 4
				set nu "000086.png"
				if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
					puts "ERROR: $::errorInfo and $msg"
				}
			}
			110 {
				set min_thresh 5
				set nu "000110.png"
				if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
					puts "ERROR: $::errorInfo and $msg"
				}
			}
			116 {
				set min_thresh 5
				set nu "000116.png"
				if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
					puts "ERROR: $::errorInfo and $msg"
				}
			}
			133 {
				set min_thresh 5
				set nu "000133.png"
				if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
					puts "ERROR: $::errorInfo and $msg"
				}
			}
			138 {
				set min_thresh 8
				set nu "000138.png"
				if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
					puts "ERROR: $::errorInfo and $msg"
				}
			}
			169 {
				set min_thresh 15
				set nu "000169.png"
				if { [catch {exec $::canny "${::gtf_noc}${nu}" "${::gt_noc}${nu}" $ratio $d_itr $min_thresh} msg ] } {
					puts "ERROR: $::errorInfo and $msg"
				}
			}
			default {
				puts "No match found"
			}
		}
	}
}

build_mask $::argv
