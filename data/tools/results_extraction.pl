#!/usr/bin/perl

$filename = $ARGV[0];
$column = $ARGV[1];

open(FILE,"<$filename") or die "Cannot open $filename for reading.\n";

@data = ();
@sorteddata = ();

while(<FILE>)
{
	@row = split(",",$_);
	push(@data,$row[$column]);
}

@sorteddata = sort { $a <=> $b } @data;
print $sorteddata[0] . "," . $sorteddata[@sorteddata-1] . "\n";
