#!/usr/bin/env tclsh
source ./mydilate.tcl

proc run_dilate { } {
	dilate [list 000005.png 1]
	dilate [list 000009.png 2]
	dilate [list 000016.png 4]
	dilate [list 000037.png 3]
	dilate [list 000039.png 2]
	dilate [list 000060.png 2]
	dilate [list 000064.png 3]
	dilate [list 000081.png 2]
	dilate [list 000086.png 2]
	dilate [list 000110.png 3]
	dilate [list 000116.png 2]
	dilate [list 000133.png 4]
	dilate [list 000138.png 3]
	dilate [list 000169.png 3]
}

run_dilate
