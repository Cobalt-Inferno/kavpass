#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
size_t BUF_SIZE;
typedef struct {
    char *symb;
    char *l_let;
    char *u_let;
    char *ints;
    char *Pass;
    bool failed;
    char tmp_c;
} Password;

void init(Password *pass, size_t BUFFER) {
    pass->symb = "!@#$%^&*()-=+_][{}";
    pass->l_let = "qwertyuiopasdfghjklzxcvbnm";
    pass->u_let = "QWERTYUIOPASDFGHJKLZXCVBNM";
    pass->ints = "1234567890";
    pass->Pass = malloc(BUFFER * sizeof(char*));
}

Password pass, *p = &pass;
void return_ran(int line) {
    if (line >=5) {
        p->failed = true;
    }else {
        switch(line) {
            case 1:
                p->tmp_c = p->symb[rand() % strlen(p->symb) + 1];
                break;
            case 2:
                p->tmp_c = p->l_let[rand() % strlen(p->l_let) + 1];
                break;
            case 3:
                p->tmp_c = p->u_let[rand() % strlen(p->u_let) + 1];
                break;
            case 4:
                p->tmp_c = p->ints[rand() % strlen(p->ints) + 1];
                break;
            default:
                p->failed = true;
        }
    }
}

void usage() {
    printf("Program: Kavpass\n");
    printf("Usage:\n");
    printf("\t-l\t--length\t|\tSpecifies the length.\n");
    printf("\t-o\t--output\t|\tFile to output to.\n");
}
void set_pass(size_t len) {
    srand(time(NULL));
    void set_q() {
        for(size_t i = 0; i < len; i++) {
            return_ran(rand() % 4 + 1);
            p->Pass[i] = p->tmp_c;
            if (p->failed) {
                return;
            }
        }
    }
    while (strlen(p->Pass) != len) {
        set_q();
    }
}

int help;

struct option long_options[] = {
    { "length",     required_argument,  0,      'l' },
    { "help",       no_argument,        0,      'h' },
    { "output",     required_argument,  0,      'o' },
    { "verbose",    no_argument,        0,      'v' },
    { 0, 0, 0, 0 }
};

bool commence;


void write_file(char *str, char *path) {
    FILE *ptr = fopen(path, "w");
    fputs(str, ptr);
    fclose(ptr);
}

int main(int argc, char **argv) {
    init(p,20);
    int len, c, option_index = 0;
    bool verbose = false, made_pass = false, tmp = false;
    char *file;
    while((c = getopt_long(argc, argv, "hvo:l:", long_options, &option_index)) != -1) {
        switch(c) {
            case 'h':
                usage(); 
                break;
            case 'v':
                verbose = true;
                break;
            case 'o':
                file = optarg;
                tmp = true;
                break;
            case 'l':
                len = atoi(optarg);
                commence = true;
                made_pass = true;
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
    if (commence) {
        set_pass(len);
        if (verbose) {
            printf("Password: %s\nWith Length: %ld\n",p->Pass, strlen(p->Pass));
        }
        else {
            printf("%s\n",p->Pass);
        }
    }
    if (tmp) {
        if (made_pass) {
            write_file(p->Pass, file);
            if (verbose) {
                printf("Wrote password to file: %s\n",file);
            }
        }
        else {
            printf("Password not generated. Run $ kavpass -h\n");
        }
    }
    free(p->Pass);
}
