
#include <cstdio>
#include <cstring>
#include <Windows.h>

#include "mcrt_win.h"

namespace utl {
// based on
// https://github.com/wine-mirror/wine/blob/6d801377055911d914226a3c6af8d8637a63fa13/dlls/shell32/shell32_main.c
char** cmdl_to_argv(const char* cmdl, int& nArgs) {
    if (!cmdl)
        return nullptr;

    DWORD argc;
    char** argv;
    LPSTR d;
    LPSTR cmdline;
    int qcount, bcount;

    /* --- First count the arguments */
    argc = 1;
    auto* s = cmdl;
    /* The first argument, the executable path, follows special rules */
    if (*s == '"') {
        /* The executable path ends at the next quote, no matter what */
        s++;
        while (*s)
            if (*s++ == '"')
                break;
    } else {
        /* The executable path ends at the next space, no matter what */
        while (*s && *s != ' ' && *s != '\t')
            s++;
    }
    /* skip to the first argument, if any */
    while (*s == ' ' || *s == '\t')
        s++;
    if (*s)
        argc++;

    /* Analyze the remaining arguments */
    qcount = bcount = 0;
    while (*s) {
        if ((*s == ' ' || *s == '\t') && qcount == 0) {
            /* skip to the next argument and count it if any */
            while (*s == ' ' || *s == '\t')
                s++;
            if (*s)
                argc++;
            bcount = 0;
        } else if (*s == '\\') {
            /* '\', count them */
            bcount++;
            s++;
        } else if (*s == '"') {
            /* '"' */
            if ((bcount & 1) == 0)
                qcount++; /* unescaped '"' */
            s++;
            bcount = 0;
            /* consecutive quotes, see comment in copying code below */
            while (*s == '"') {
                qcount++;
                s++;
            }
            qcount = qcount % 3;
            if (qcount == 2)
                qcount = 0;
        } else {
            /* a regular character */
            bcount = 0;
            s++;
        }
    }

    /* Allocate in a single lump, the string array, and the strings that go
     * with it. This way the caller can make a single LocalFree() call to free
     * both, as per MSDN.
     */

    size_t len = std::strlen(cmdl);
    argv = (char**)LocalAlloc(LMEM_FIXED, (argc + 1) * sizeof(LPSTR) + (len + 1) * sizeof(char));
    if (!argv)
        return NULL;
    cmdline = (LPSTR)(argv + argc + 1);
    std::strncpy(cmdline, cmdl, len);
    // strcpy(cmdline, cmdl);

    /* --- Then split and copy the arguments */
    argv[0] = d = cmdline;
    argc = 1;
    /* The first argument, the executable path, follows special rules */
    if (*d == '"') {
        /* The executable path ends at the next quote, no matter what */
        s = d + 1;
        while (*s) {
            if (*s == '"') {
                s++;
                break;
            }
            *d++ = *s++;
        }
    } else {
        /* The executable path ends at the next space, no matter what */
        while (*d && *d != ' ' && *d != '\t')
            d++;
        s = d;
        if (*s)
            s++;
    }
    /* close the executable path */
    *d++ = 0;
    /* skip to the first argument and initialize it if any */
    while (*s == ' ' || *s == '\t')
        s++;
    if (!*s) {
        /* There are no parameters so we are all done */
        argv[argc] = NULL;
        nArgs = argc;
        return argv;
    }

    /* Split and copy the remaining arguments */
    argv[argc++] = d;
    qcount = bcount = 0;
    while (*s) {
        if ((*s == ' ' || *s == '\t') && qcount == 0) {
            /* close the argument */
            *d++ = 0;
            bcount = 0;

            /* skip to the next one and initialize it if any */
            do {
                s++;
            } while (*s == ' ' || *s == '\t');
            if (*s)
                argv[argc++] = d;
        } else if (*s == '\\') {
            *d++ = *s++;
            bcount++;
        } else if (*s == '"') {
            if ((bcount & 1) == 0) {
                /* Preceded by an even number of '\', this is half that
                 * number of '\', plus a quote which we erase.
                 */
                d -= bcount / 2;
                qcount++;
            } else {
                /* Preceded by an odd number of '\', this is half that
                 * number of '\' followed by a '"'
                 */
                d = d - bcount / 2 - 1;
                *d++ = '"';
            }
            s++;
            bcount = 0;
            /* Now count the number of consecutive quotes. Note that qcount
             * already takes into account the opening quote if any, as well as
             * the quote that lead us here.
             */
            while (*s == '"') {
                if (++qcount == 3) {
                    *d++ = '"';
                    qcount = 0;
                }
                s++;
            }
            if (qcount == 2)
                qcount = 0;
        } else {
            /* a regular character */
            *d++ = *s++;
            bcount = 0;
        }
    }
    *d = '\0';
    argv[argc] = nullptr;
    nArgs = argc;

    return argv;
}
}