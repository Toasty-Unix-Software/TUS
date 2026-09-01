#!/usr/bin/perl
# SPDX-License-Identifier: BSD-2-Clause
# Copyright 1996-2025 The NASM Authors - All Rights Reserved

#
# Scan the source tree for "use Module;" lines in *.pl/*.ph files and
# produce doc/perlbreq.src, the list of Perl module prerequisites
# included in the documentation.  This is a portable (pure Perl)
# equivalent of the find/sed pipeline used by the top-level
# Makefile.in's perlbreq.si/doc/perlbreq.src rules, for use by build
# systems (e.g. Mkfiles/msvc.mak) that cannot rely on POSIX find/sed
# being available.
#

use strict;
use warnings;
use File::Find;

my ($srcdir, $outfile) = @ARGV;
die "Usage: $0 <srcdir> <outfile>\n" unless defined($srcdir) && defined($outfile);

my %mods;

find({
    wanted => sub {
        return unless /\.p[lh]$/;
        open(my $fh, '<', $_) or return;
        while (my $line = <$fh>) {
            if ($line =~ /^\s*use\s+([[:upper:]][^\s;]*)/) {
                my $mod = $1;
                next if $mod =~ /^Win32/;
                $mods{$mod} = 1;
            }
        }
        close($fh);
    },
    no_chdir => 1,
}, $srcdir);

open(my $out, '>', $outfile) or die "$0: cannot open: $outfile\n";
foreach my $mod (sort keys %mods) {
    print $out "\\c      $mod\n";
}
print $out "\\c      Win32 (if building on Windows only)\n";
close($out);
