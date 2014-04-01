

#ifndef __GETOPT_H__
#define __GETOPT_H__

#pragma once

#define no_argument             0
#define required_argument       1
#define optional_argument       2

extern char *   optarg;
extern int      optind;
extern int      optopt;
extern int      opterr;

int getopt_long(int, char * const *, const char *, const struct option *, int *);
struct option
{
        /* name of long option */
        const char *name;
        /*
         * one of no_argument, required_argument, and optional_argument:
         * whether option takes an argument
         */
        int has_arg;
        /* if not NULL, set *flag to val when option found */
        int *flag;
        /* if flag not NULL, value to set *flag to; else return value */
        int val;
};

#endif

