#!/usr/bin/env tclsh

source ./mymask.tcl

proc runmask { } {
	mask [list 000005.png 7]
	mask [list 000009.png 4]
	mask [list 000016.png 5]
	mask [list 000037.png 5]
	mask [list 000039.png 4]
	mask [list 000060.png 4]
	mask [list 000064.png 7]
	mask [list 000081.png 5]
	mask [list 000086.png 4]
	mask [list 000110.png 5]
	mask [list 000116.png 5]
	mask [list 000133.png 5]
	mask [list 000138.png 8]
	mask [list 000169.png 15]
}

runmask
