# -*- tcl -*-
# Tcl package index file, version 1.1
#
if {[package vsatisfies [package provide Tcl] 9.0-]} {
    package ifneeded tcltaglib 1.2 \
	    [list load [file join $dir libtcl9tcltaglib1.2.so] [string totitle tcltaglib]]
} else {
    package ifneeded tcltaglib 1.2 \
	    [list load [file join $dir libtcltaglib1.2.so] [string totitle tcltaglib]]
}
