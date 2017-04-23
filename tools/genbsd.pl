#! /usr/bin/perl

$BSD = <<'END';
# BSD DEFAULTS
#

OBJS += stdout.o xv65.o

#
END

print <<END;
# File generated by genbsd.pl - DO NOT EDIT

END
$out = 1;
while (<>) {
	chop;
	if (/^# TARGET \/ PLATFORM DEFAULT$/) { $out = 0; print $BSD; }
	elsif (/^# LOCAL OVERRIDE/) { $out = 1; }
	if ($out) { print "$_\n"; }
}