#!/usr/bin/perl
#
# watch -- watch the oven error log and make a noise when something is added
#
# call as a cgi script
#
# Copyright (C) 2013 by J. M. Hill
# Created at Steward Observatory 
# Argument sets the refresh time in seconds, 20>
# Hacked for linux by Tom Trebisky 7/8/2019
#

$version = "23AUG13";
$version = "08JUL19";

# strip \'s put in by server (apparently)
$ARGV[0] =~ s/\\(.)/$1/g;

# configuration
# $path = "/home/pilot/errors.log";
$path = "./errors.log";
$cmd = "/usr/bin/wc ";

$sound = "/usr/local/bin/train.au";

print "Whistles when something appears in $path \n";

unless (-e $path) {
    print "File: $path doesn't exist.\n";
    exit;
}

# Use wc to check the size of errors.log
@initialmatches = `$cmd $path`;

print "Size $initialmatches[0] \n";

sub toot {
    # system("cat /usr/demo/SOUND/sounds/train.au > /dev/audio");
    system ( "aplay $sound" );
}

toot ();

# Loop forever until a ^C
while ( 1 > 0 ) {
	sleep(30); # wait 30 seconds
	@matches = `$cmd $path`;
	print "Size $matches[0] \n";
	if ($matches[0] > $initialmatches[0]) { 
		toot ();
		sleep(1);
		toot ();
		$initialmatches[0] = $matches[0];
	} # end if
} # end infinite while
