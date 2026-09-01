#!/usr/bin/perl
# SPDX-License-Identifier: BSD-2-Clause
# Copyright 1996-2025 The NASM Authors - All Rights Reserved

#
# doc/Makefile.in is written for GNU make (used unmodified, via
# autoconf substitution, on Unix).  It relies on a handful of GNU
# make-only constructs -- the $^ automatic variable (not valid nmake
# syntax at all: nmake's parser treats a bare "$^" as an illegal macro
# reference and aborts), the $< automatic variable used outside of an
# inference rule (silently unsupported by nmake, since nmake only
# defines $< within .SUFFIXES-style inference rules), and the
# GNU-only "-include *.dep" optional-include directive (nmake only
# understands the "!include" directive and has no equivalent "ignore
# missing file, glob patterns allowed" form).
#
# Mkfiles/msvc.mak needs to feed doc/Makefile.in directly to nmake (it
# doesn't have its own copy of the doc build rules to maintain in
# parallel), so this script produces an nmake-safe copy of the file by
# rewriting just those constructs to their equivalent explicit form.
# doc/Makefile.in itself is left untouched for the Unix/GNU make
# build; only the copy this script writes is affected.
#

use strict;
use warnings;

my ($in, $out) = @ARGV;
die "Usage: $0 <in> <out>\n" unless defined($in) && defined($out);

open(my $ifh, '<', $in) or die "$0: cannot open: $in\n";
open(my $ofh, '>', $out) or die "$0: cannot open: $out\n";

while (my $line = <$ifh>) {
    # insns.src: inslist.pl ../x86/insns.xda
    #	$(RUNPERL) $^ $@
    if (index($line, '$(RUNPERL) $^ $@') >= 0) {
        $line =~ s/\$\(RUNPERL\) \$\^ \$\@/\$(RUNPERL) inslist.pl ..\/x86\/insns.xda \$\@/;
    }

    # $(htmltarget)/nasmdoc.dip/nasmdoc.txt: $< is the first prerequisite
    # of $(ALLSRCS), i.e. $(SRCS) (nasmdoc.src).
    $line =~ s/(\$\(RDSRC\)\s+(?:-ohtml html|dip|txt))\s+\$</$1 \$(SRCS)/;

    # nasmdoc.pdf/nasmdoc-raw.pdf: $< is the first (only) prerequisite
    # of interest, nasmdoc.ps.
    $line =~ s/(\$\(PDFOPT\)\s+)\$<(\s+\$\@\s+fontpath)/$1nasmdoc.ps$2/;

    # nasmdoc.pdf.xz: $< is nasmdoc-raw.pdf.
    $line =~ s/(\$\(XZ\)\s+-9e\s+<\s+)\$<(\s+>\s+\$\@)/$1nasmdoc-raw.pdf$2/;

    # GNU make-only optional wildcard include of auto-dependency files;
    # not parseable by nmake at all, and unneeded for a one-shot build.
    next if $line =~ /^-include\s+\*\.dep\s*$/;

    print $ofh $line;
}

close($ifh);
close($ofh);
