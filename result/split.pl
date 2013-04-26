#!/usr/bin/perl

$line = 1;
while(<>){
    @data = split /,/;
    print "line $line : " , $#data;
    $line ++;
    print "\n";
}
