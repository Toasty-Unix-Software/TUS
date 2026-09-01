/* SPDX-License-Identifier: BSD-2-Clause */
/* Copyright 2026 The NASM Authors - All Rights Reserved */

#include "compiler.h"
#include "nasmlib.h"
#include "files.h"
#include "error.h"

/* These must match the constants in "files.h" */
static const char * const filename_names[FN_NFILES] = {
    "input",

    "output",
    "error",
    "list",
    "dependency",
    "map",

    NULL,                       /* End of "real" filenames */

    "debug mapped input",
    "make mapped output"
};

const char *_filenames[FN_NFILES];

const char *copy_filename(enum filenames fn, const char *src)
{
    return set_filename(fn, nasm_strdup(src));
}

const char *set_filename(enum filenames fn, char *src)
{
    const char **dstp, *dst;
    nasm_assert((size_t)fn < ARRAY_SIZE(_filenames));

    dstp = &_filenames[fn];
    dst = *dstp;

    if (dst) {
        nasm_fatal("more than one %s file specified: %s and %s",
                   filename_names[fn], dst, src);
    }

    return *dstp = src;
}

void check_overwrite_files(void)
{
    enum filenames fi, fo;      /* No fie or fum */

    for (fi = FN_INFILE; fi < FN_OUTFILE; fi++) {
        const char *inname = get_filename(fi);

        if (!inname)
            continue;

        for (fo = FN_OUTFILE; fo < FN_NFILES_REAL; fo++) {
            const char *outname = get_filename(fo);
            if (outname && !nasm_compare_paths(inname, outname)) {
                nasm_nonfatal("%s file would overwrite %s file `%s'",
                              filename_names[fo], filename_names[fi], inname);
            }
        }
    }
}

void cleanup_filenames(void)
{
    enum filenames fn;

    for (fn = 0; fn < FN_NFILES; fn++) {
        nasm_free((char *)_filenames[fn]);
        _filenames[fn] = NULL;
    }
}
