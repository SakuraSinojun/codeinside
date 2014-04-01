
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "getopt.h"

#define no_argument        0
#define required_argument  1
#define optional_argument  2

char *optarg;
int optind = 0;
int optopt = 0;
int opterr = 1;

int getopt_long(int, char * const *, const char *,const struct option *, int *);

#define IGNORE_FIRST (*options == '-' || *options == '+')
#define PRINT_ERROR ((opterr) && ((*options != ':') \
                || (IGNORE_FIRST && options[1] != ':')))

//vc下getenv("POSIXLY_CORRECT") 为NULL,所以IS_POSIXLY_CORRECT为0
#define IS_POSIXLY_CORRECT (getenv("POSIXLY_CORRECT") != NULL)
#define PERMUTE         (!IS_POSIXLY_CORRECT && !IGNORE_FIRST)
/* XXX: GNU ignores PC if *options == '-' */
#define IN_ORDER        (!IS_POSIXLY_CORRECT && *options == '-')

/* return values */
#define BADCH (int)'?'
#define BADARG  ((IGNORE_FIRST && options[1] == ':') \
        || (*options == ':') ? (int)':' : (int)'?')
#define INORDER (int)1

static char    EMSG[1] = {0};

static int getopt_internal (int, char * const *, const char *);
static int gcd (int, int);
static void permute_args (int, int, int, char * const *);

static char *place = EMSG; /* option letter processing */

static int nonopt_start = -1; /* first non option argument (for permute) */
static int nonopt_end = -1;   /* first option after non options (for permute) */

/* Error messages */
static const char recargchar[] = "option requires an argument -- %c\n";
static const char recargstring[] = "option requires an argument -- %s\n";
static const char ambig[] = "ambiguous option -- %.*s\n";
static const char noarg[] = "option doesn't take an argument -- %.*s\n";
static const char illoptchar[] = "unknown option -- %c\n";
static const char illoptstring[] = "unknown option -- %s\n";

/*
 * Compute the greatest common divisor of a and b.
 */
static int
gcd(int a, int b)
{
        int c;

        c = a % b;
        while (c != 0)
        {
                a = b;
                b = c;
                c = a % b;
        }

        return b;
}

/*
 * Exchange the block from nonopt_start to nonopt_end with the block
 * from nonopt_end to opt_end (keeping the same order of arguments
 * in each block).
 */
static void
permute_args(int panonopt_start, int panonopt_end, int opt_end,
        char * const *nargv)
{
        int cstart, cyclelen, i, j, ncycle, nnonopts, nopts, pos;
        char *swap;

        assert(nargv != NULL);

        /*
         * compute lengths of blocks and number and size of cycles
         */
        nnonopts = panonopt_end - panonopt_start;
        nopts = opt_end - panonopt_end;
        ncycle = gcd(nnonopts, nopts);
        cyclelen = (opt_end - panonopt_start) / ncycle;

        for (i = 0; i < ncycle; i++)
        {
                cstart = panonopt_end+i;
                pos = cstart;
                for (j = 0; j < cyclelen; j++)
                {
                        if (pos >= panonopt_end)
                                pos -= nnonopts;
                        else
                                pos += nopts;
                        swap = nargv[pos];
                        /* LINTED const cast */
                        ((char **) nargv)[pos] = nargv[cstart];
                        /* LINTED const cast */
                        ((char **)nargv)[cstart] = swap;
                }
        }
}

/*
 * getopt_internal --
 * Parse argc/argv argument vector.  Called by user level routines.
 *  Returns -2 if -- is found (can be long option or end of options marker).
 */
static int
getopt_internal(int nargc, char * const *nargv, const char *options)
{
        char *oli;    /* option letter list index */
        int optchar;

        assert(nargv != NULL);
        assert(options != NULL);

        optarg = NULL;

        /*
         * XXX Some programs (like rsyncd) expect to be able to
         * XXX re-initialize optind to 0 and have getopt_long(3)
         * XXX properly function again.  Work around this braindamage.
         */
        if (optind == 0)
                optind = 1;

start:
        if (!*place) {                  /* update scanning pointer */
                if (optind >= nargc) {          /* end of argument vector */
                        place = EMSG;
                        if (nonopt_end != -1) {
                                /* do permutation, if we have to */
                                permute_args(nonopt_start, nonopt_end,
                                        optind, nargv);
                                optind -= nonopt_end - nonopt_start;
                        }
                        else if (nonopt_start != -1) {
                                /*
                                 * If we skipped non-options, set optind
                                 * to the first of them.
                                 */
                                optind = nonopt_start;
                        }
                        nonopt_start = nonopt_end = -1;
                        return -1;
                }
                if ((*(place = nargv[optind]) != '-')
                        || (place[1] == '\0')) {    /* found non-option */
                        place = EMSG;
                        if (IN_ORDER) {
                                /*
                                 * GNU extension:
                                 * return non-option as argument to option 1
                                 */
                                optarg = nargv[optind++];
                                return INORDER;
                        }
                        if (!PERMUTE) {
                                /*
                                 * if no permutation wanted, stop parsing
                                 * at first non-option
                                 */
                                return -1;
                        }
                        /* do permutation */
                        if (nonopt_start == -1)
                                nonopt_start = optind;
                        else if (nonopt_end != -1) {
                                permute_args(nonopt_start, nonopt_end,
                                        optind, nargv);
                                nonopt_start = optind -
                                        (nonopt_end - nonopt_start);
                                nonopt_end = -1;
                        }
                        optind++;
                        /* process next argument */
                        goto start;
                }
                if (nonopt_start != -1 && nonopt_end == -1)
                        nonopt_end = optind;
                if (place[1] && *++place == '-') { /* found "--" */
                        place++;
                        return -2;
                }
        }
        if ((optchar = (int)*place++) == (int)':' ||
                (oli = (char *)strchr(options + (IGNORE_FIRST ? 1 : 0), optchar)) == NULL) {
                /* option letter unknown or ':' */
                if (!*place)
                        ++optind;
                if (PRINT_ERROR)
                        fprintf(stderr, illoptchar, optchar);
                optopt = optchar;
                return BADCH;
        }
        if (optchar == 'W' && oli[1] == ';') {  /* -W long-option */
                /* XXX: what if no long options provided (called by getopt)? */
                if (*place)
                        return -2;

                if (++optind >= nargc) { /* no arg */
                        place = EMSG;
                        if (PRINT_ERROR)
                                fprintf(stderr, recargchar, optchar);
                        optopt = optchar;
                        return BADARG;
                } else    /* white space */
                        place = nargv[optind];
                /*
                 * Handle -W arg the same as --arg (which causes getopt to
                 * stop parsing).
                 */
                return -2;
        }
        if (*++oli != ':') {   /* doesn't take argument */
                if (!*place)
                        ++optind;
        } else {    /* takes (optional) argument */
                optarg = NULL;
                if (*place)   /* no white space */
                        optarg = place;
                /* XXX: disable test for :: if PC? (GNU doesn't) */
                else if (oli[1] != ':') { /* arg not optional */
                        if (++optind >= nargc) { /* no arg */
                                place = EMSG;
                                if (PRINT_ERROR)
                                        fprintf(stderr, recargchar, optchar);
                                optopt = optchar;
                                return BADARG;
                        } else
                                optarg = nargv[optind];
                }
                place = EMSG;
                ++optind;
        }
        /* dump back option letter */
        return optchar;
}

/*
 * getopt_long --
 *Parse argc/argv argument vector.
 */
int
getopt_long(int nargc, char * const *nargv, const char *options,
        const struct option *long_options, int *idx)
{
        int retval;

        assert(nargv != NULL);
        assert(options != NULL);
        assert(long_options != NULL);
        /* idx may be NULL */

        if ((retval = getopt_internal(nargc, nargv, options)) == -2) {
                char *current_argv, *has_equal;
                size_t current_argv_len;
                int i, match;

                current_argv = place;
                match = -1;

                optind++;
                place = EMSG;

                if (*current_argv == '\0') {  /* found "--" */
                        /*
                         * We found an option (--), so if we skipped
                         * non-options, we have to permute.
                         */
                        if (nonopt_end != -1) {
                                permute_args(nonopt_start, nonopt_end,
                                        optind, nargv);
                                optind -= nonopt_end - nonopt_start;
                        }
                        nonopt_start = nonopt_end = -1;
                        return -1;
                }
                if ((has_equal = strchr(current_argv, '=')) != NULL) {
                        /* argument found (--option=arg) */
                        current_argv_len = has_equal - current_argv;
                        has_equal++;
                } else
                        current_argv_len = strlen(current_argv);

                for (i = 0; long_options[i].name; i++) {
                        /* find matching long option */
                        if (strncmp(current_argv, long_options[i].name,
                                        current_argv_len))
                                continue;

                        if (strlen(long_options[i].name) ==
                                (unsigned)current_argv_len) {
                                /* exact match */
                                match = i;
                                break;
                        }
                        if (match == -1)  /* partial match */
                                match = i;
                        else {
                                /* ambiguous abbreviation */
                                if (PRINT_ERROR)
                                        fprintf(stderr, ambig, (int)current_argv_len,
                                                current_argv);
                                optopt = 0;
                                return BADCH;
                        }
                }
                if (match != -1) {   /* option found */
                        if (long_options[match].has_arg == no_argument
                                && has_equal) {
                                if (PRINT_ERROR)
                                        fprintf(stderr, noarg, (int)current_argv_len,
                                                current_argv);
                                /*
                                 * XXX: GNU sets optopt to val regardless of
                                 * flag
                                 */
                                if (long_options[match].flag == NULL)
                                        optopt = long_options[match].val;
                                else
                                        optopt = 0;
                                return BADARG;
                        }
                        if (long_options[match].has_arg == required_argument ||
                                long_options[match].has_arg == optional_argument) {
                                if (has_equal)
                                        optarg = has_equal;
                                else if (long_options[match].has_arg ==
                                        required_argument) {
                                        /*
                                         * optional argument doesn't use
                                         * next nargv
                                         */
                                        optarg = nargv[optind++];
                                }
                        }
                        if ((long_options[match].has_arg == required_argument)
                                && (optarg == NULL)) {
                                /*
                                 * Missing argument; leading ':'
                                 * indicates no error should be generated
                                 */
                                if (PRINT_ERROR)
                                        fprintf(stderr, recargstring, current_argv);
                                /*
                                 * XXX: GNU sets optopt to val regardless
                                 * of flag
                                 */
                                if (long_options[match].flag == NULL)
                                        optopt = long_options[match].val;
                                else
                                        optopt = 0;
                                --optind;
                                return BADARG;
                        }
                } else {   /* unknown option */
                        if (PRINT_ERROR)
                                fprintf(stderr, illoptstring, current_argv);
                        optopt = 0;
                        return BADCH;
                }
                if (long_options[match].flag) {
                        *long_options[match].flag = long_options[match].val;
                        retval = 0;
                } else
                        retval = long_options[match].val;
                if (idx)
                        *idx = match;
        }
        return retval;
}

// 单元测试
#if 0

static char    NULL_STRING[] = "(null)";
#include <ctype.h>

void print_usage(const char *program_name)
{
        printf("%s 1.0.0 (2010-06-13)\n", program_name);
        printf("This is a program decoding a BER encoded CDR file\n");
        printf("Usage: %s -f <file_name> -o <output_name> [-c <config_name>] [-k <keyword>]\n", program_name);
        printf("    -f --file       the CDR file to be decoded\n");
        printf("    -o --output     the output file in plain text format\n");
        printf("    -c --config     the description file of the CDR file, if not given, use default configuration\n");
        printf("    -k --keyword    the keyword to search, if not given, all records will be written into output file\n");
}

/*
   输入项
   -f aaa -o aaa.txt -c ccc -k 222
   --file aaa --output aaa.txt -c ccc -k 222
   -c ccc -o aaa.txt -k 222 --file aaa
   */
int main(int argc, char *argv[])
{
        char *file_name = NULL;
        char *output_name = NULL;
        char *config_name = NULL;
        char *keyword = NULL;

        const char *short_opts = "hf:o:c:k:";
        const struct option long_opts[] =
        {
                {"help", no_argument, NULL, 'h'},
                {"file", required_argument, NULL, 'f'},
                {"output", required_argument, NULL, 'o'},
                {"config", required_argument, NULL, 'c'},
                {"keyword", required_argument, NULL, 'k'},
                {0, 0, 0, 0}
        };
        int hflag = 0;
        int c;

        opterr = 0;
        while ( (c = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1 )
        {
                switch ( c )
                {
                case 'h' :
                        hflag = 1;
                        break;
                case 'f' :
                        file_name = optarg;
                        break;
                case 'o' :
                        output_name = optarg;
                        break;
                case 'c' :
                        config_name = optarg;
                        break;
                case 'k' :
                        keyword = optarg;
                        break;
                case '?' :
                        if ( optopt == 'f' || optopt == 'o' || optopt == 'c' || optopt == 'k' )
                                printf("Error: option -%c requires an argument\n", optopt);
                        else if ( isprint(optopt) )
                                printf("Error: unknown option '-%c'\n", optopt);
                        else
                                printf("Error: unknown option character '\\x%x'\n", optopt);
                        return 1;
                default :
                        abort();
                }
        }

        if ( hflag || argc == 1 )
        {
                print_usage(argv[0]);
                return 0;
        }
        if ( !file_name )
        {
                printf("Error: file name must be specified\n");
                return 1;
        }
        if ( !output_name )
        {
                printf("Error: output name must be specified\n");
                return 1;
        }

        // if not setting default, Linux OK, but SunOS core dump
        if ( !config_name ) config_name = NULL_STRING;
        if ( !keyword ) keyword = NULL_STRING;
        printf("Parameters got: file_name = %s, output_name = %s, config_name = %s, keyword = %s\n", file_name, output_name, config_name, keyword);
        return 0;
}

#endif

