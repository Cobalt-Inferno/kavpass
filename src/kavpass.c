#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "color.h"
#include "generator.h"
#include "interactive.h"
#include "curler.h"
#include "top_level_scan.h"
#include "hasher.h"
#include "scanner.h"


const char *version = "v1.0.1";
struct option long_options[] = {
    { "length",             required_argument,  0,      'l' },
    { "help",               no_argument,        0,      'h' },
    { "output",             required_argument,  0,      'o' },
    { "force-unsafe-rng",   required_argument,  0,      'F' },
    { "verbose",            no_argument,        0,      'v' },
    { "extra-unicode",      no_argument,        0,      'e' },
    { "interactive",        no_argument,        0,      'i' },
    { "color",              required_argument,  0,      'c' },
    { "prefix",             required_argument,  0,      'p' },
    { "directory",             required_argument,  0,      'd' },
    { "memory-map",             required_argument,  0,      'm' },
    { "update",             no_argument,  0,      'u' },
    { "quick",             required_argument,  0,      'q' },
    { "file",             required_argument,  0,      'f' },
    { "help",               no_argument,        0,      'h' },
    { 0, 0, 0, 0 }
};

Password pass, *p = &pass;


void usage() {
    printf("Program: kavpass | version: %s\n", version);
    printf("Usage: kavpass -[hloveFpci]\n");
    printf("\t-h\t--help\t\t\t|\tDisplays the help message.\n");
    printf("\t-l\t--length NUM\t\t|\tSpecifies the length.\n");
    printf("\t-o\t--output\t\t|\tFile to output to.\n");
    printf("\t-v\t--verbose\t\t|\tVerbose output.\n"); 
    printf("\t-e\t--extra-unicode\t\t|\tAdds extra unicode char support.\n");
    printf("\t-F\t--force-unsafe-rng NUM\t|\tReplaces --length and forces the use of an unsafe RNG.\n");
    printf("\t-p\t--prefix NUM\t\t|\tSets a prefix for the password.\n");
    printf("\t-c\t--color COLOR\t\t|\tSets a color for output.\n");
    printf("\t-i\t--interactive\t\t|\tEnters an interactive mode. (IN DEVELOPMENT)\n");
    printf("\t-d\t--directory\t\t|\tTakes a specified directory to scan.\n");
    printf("\t-f\t--file\t\t\t|\tTakes a single file to scan.\n");
    printf("\t-m\t--memory-map\t\t|\tLike directory, but just shows the md5s of files in the directory.\n");
    printf("\t-q\t--quick\t\t\t|\tUses an experimental way of scanning files with a 3-5x speed increase.\n");
    printf("\t-u\t--update\t\t|\tUpdates our hash list. May take a while.\n");

}

int main(int argc, char **argv) {
    int c, option_index = 0;
    bool unsafe = false, verbose = false, made_pass = false, tmp = false, commence = false;
    if (argc == 1) {
        usage();
        return EXIT_FAILURE;
    }
    while((c = getopt_long(argc, argv, "F:hp:c:ievo:l:d:m:uq:f", long_options, &option_index)) != -1) {
        switch(c) {
            case 'F':
                unsafe = true;
                commence = true;
                made_pass = true;
                if (atoi(optarg) > 2096) {
                    fprintf(stderr, "Length canot be longer than 2096.\n");
                    exit(1);
                }
                p->len = atoi(optarg);
                break;
            case 'h':
                usage(); 
                break;
            case 'p':
                p->prefix = true;
                p->pref = optarg;
                break;
            case 'c': ;
                char *token = optarg;
                if (strncmp(token, "red", 3) == 0) {
                    switch_color(1);
                } else if (strncmp(token, "green", 5) == 0) {
                    switch_color(2);
                } else if (strncmp(token, "yellow", 6) == 0) {
                    switch_color(3);
                } else if (strncmp(token, "blue", 4) == 0) {
                    switch_color(4);
                } else if (strncmp(token, "magenta", 7) == 0) {
                    switch_color(5);
                } else if (strncmp(token, "cyan", 4) == 0) {
                    switch_color(6);
                } else if (strncmp(token, "white", 5) == 0) {
                    switch_color(7);
                } else {
                    fprintf(stderr, "Color: \"%s\" is not recognized.\n", token);
                }
                break;
            case 'i': ;
                // Launch interactive mode
                interactive_mode(p);
                return 0;
                break;
            case 'e':
                p->test_symb_b = true;
                break;
            case 'v':
                verbose = true;
                break;
            case 'o':
                p->file = optarg;
                tmp = true;
                break;
            case 'l':
                if (p->len > 2096) {
                    fprintf(stderr, "Length cannot be longer than 2096.\n");
                    exit(1);
                }
                commence = true;
                made_pass = true;
                p->len = atoi(optarg);
                break;
            case 'd':
                GetFiles(optarg, 0, false, 0);
                break;
            case 'm':
                char *bufe = tmpbuff();
                GetFiles(optarg, 0, true, bufe);
                break;
            case 'u':
                xcurl("https://virusshare.com/hashfiles/unpacked_hashes.md5","/tmp/data");
                break;
            case 'q':
                char *buf = loadbuf();
                GetFiles(optarg, 0, true, buf);
                break;
            case 'f':
                char *ff = md5sum(optarg);
                bool tmp = quickscan(ff);
                if (isdir(optarg)) {
                    fprintf(stderr, "Cannot scan a directory with --file flag.\n");
                }
                else if (tmp) {
                    printf("File: %s \e[37;1m[ \e[31;1mFLAGGED \e[37;1m]\e[0m\n", optarg);
                }
                else {
                    printf("File: %s \e[37;1m[ \e[32;1mOK \e[37;1m] \e[0m\n", optarg);
                }
                break;
 
            case ':':
                usage();
                break;
            case '?':
                usage();
                break;
            default:
                usage();
                break;
        }
    }
    if (!unsafe) {
        if (commence) {
            init(p);
            safe_set_pass(p);
            if (verbose) {
                printf("Password: %s\nWith Length: %zu\n",p->Pass, strlen(p->Pass));
            }
            else {
                printf("%s\n",p->Pass);
            }
        }
        if (tmp) {
            if (made_pass) {
                write_file(p->Pass, p->file);
                if (verbose) {
                    printf("Wrote password to file: %s\n",p->file);
                }
            }
            else {
                fprintf(stderr, "Password not generated. Run $ kavpass -h\n");
            }
        }
    } else {
        if (commence) {
            init(p);
            unsafe_set_pass(p);
            if (verbose) {
                printf("Unsafe password: %s\nWith Length: %zu\n",p->Pass, strlen(p->Pass));
            }
            else {
                printf("%s\n",p->Pass);
            }
        }
        if (tmp) {
            if (made_pass) {
                write_file(p->Pass, p->file);
                if (verbose) {
                    printf("Wrote unsafe password to file: %s\n",p->file);
                }
            }
            else {
                fprintf(stderr, "Password not generated. Run $ kavpass -h\n");
            }
        }
    }

    switch_color(0);
    if (p->Pass) {
        free(p->Pass);
    }
    if (p->file) {
        free(p->file);
    }
}
