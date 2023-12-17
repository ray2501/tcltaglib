#!/usr/bin/tclsh

package require tcltaglib

foreach {key value} $argv {
  switch -- $key {
  "-file" {
        set name $value;
    }
  "-title" {
        set title $value;
    }
  "-artist" {
        set artist $value;
    }
  "-album" {
        set album $value;
    }
  "-year" {
        set year $value;
    }
  "-comment" {
        set comment $value;
    }
  "-track" {
        set track $value;
    }
  "-genre" {
        set genre $value;
    }
  }
}

try {
  set filehandle [taglib::file_new $name]
  
  if {[taglib::file_is_valid $filehandle]==0} {
      puts "This file is not valid"
      exit
  }
  
  set tag [taglib::file_tag $filehandle]
  
  if {[info exists title]} {
      taglib::set_title $tag $title
  }
  
  if {[info exists artist]} {
      taglib::set_artist $tag $artist
  }
  
  if {[info exists album]} {
      taglib::set_album $tag $album
  }
  
  if {[info exists comment]} {
      taglib::set_comment $tag $comment
  }
  
  if {[info exists genre]} {
      taglib::set_genre $tag $genre
  }
  
  if {[info exists year]} {
      taglib::set_year $tag $year
  }
  
  if {[info exists track]} {
      taglib::set_track $tag $track
  }
  
  taglib::tag_free $tag
  taglib::file_save $filehandle
  taglib::file_free $filehandle
} on error {em} {
    error "Fail to open file: $em"
}
