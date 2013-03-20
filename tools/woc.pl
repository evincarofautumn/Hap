#!/usr/bin/env perl

my $line = 1;
while (<>) {
    print "$ARGV:$line: $_" if length > 80;
    ++$line;
    $line = 1 if eof;
}
