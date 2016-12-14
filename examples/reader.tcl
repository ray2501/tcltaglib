#!/usr/bin/tclsh

package require tcltaglib

if {$argc > 0} {
    set name [lindex $argv 0]
} else {
    puts "Please input filename."
    exit
}

try {
set filehandle [taglib::file_new $name]

if {[taglib::file_is_valid $filehandle]==0} {
    puts "This file is not valid"
    exit
}

set audioperp [taglib::audioproperties $filehandle]

set length [lindex $audioperp 0]
set bitrate [lindex $audioperp 1]
set samplerate [lindex $audioperp 2]
set channels [lindex $audioperp 3]

puts "####################"
puts "length: "
puts $length
puts "bitrate: "
puts $bitrate
puts "samplerate: "
puts $samplerate
puts "channels: "
puts $channels


puts "####################"
set tag [taglib::file_tag $filehandle]

set title [taglib::tag_title $tag]
puts "title:"
puts $title

set artist [taglib::tag_artist $tag]
puts "Artist:"
puts $artist

set album [taglib::tag_album $tag]
puts "Album:"
puts $album

set year [taglib::tag_year $tag]
puts "Year:"
puts $year

set comment [taglib::tag_comment $tag]
puts "Comment:"
puts $comment

set track [taglib::tag_track $tag]
puts "Track:"
puts $track

set genre [taglib::tag_genre $tag]
puts "Genre:"
puts $genre

taglib::tag_free $tag
taglib::file_free $filehandle
} on error {em} {
    error "Fail to open file: $em"
}
