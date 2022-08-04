#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#define ARR_S(arr) (sizeof(arr) / sizeof((arr)[0])) 
int pull_rand() {
    FILE * file = fopen("/dev/random", "r");            
    unsigned char buff[10];
    if (!file) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    size_t retv = fread(buff, sizeof(*buff), ARR_S(buff), file); /* I have to add some error checking */
    int temp = buff[0];
    fclose(file);
    return temp;
}
    

size_t BUF_SIZE = 2096; 
typedef struct {
    char *symb;
    char *l_let;
    char *u_let;
    char *salt;
    char *ints;
    bool test_symb;
    char *TESTING_SYMBOLS;
    char *Pass;
    bool failed;
    char tmp_c;
    bool salted;
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
    if (line >=6) {
        p->failed = true;
    }else {
        switch(line) {
            case 1:
                p->tmp_c = p->symb[pull_rand() % strlen(p->symb)];
                break;
            case 2:
                p->tmp_c = p->l_let[pull_rand() % strlen(p->l_let)];
                break;
            case 3:
                p->tmp_c = p->u_let[pull_rand() % strlen(p->u_let)];
                break;
            case 4:
                p->tmp_c = p->ints[pull_rand() % strlen(p->ints)];
                break;
            case 5:
                p->tmp_c = p->TESTING_SYMBOLS[pull_rand() % strlen(p->TESTING_SYMBOLS)];
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
    printf("\t-t\t--testing\t|\tTesting feature to add your own text.\n");
    printf("\t-v\t--verbose\t|\tVerbose output.\n"); 
}
void set_pass(size_t len) {
    LOOP:
    if (p->salted) {
        p->Pass[0] = '$';
        for(size_t j = 1; j < strlen(p->salt) + 1; j++) {
            p->Pass[j] = p->salt[j];
        }
        p->Pass[strlen(p->Pass) + 1] = '$';
        for (size_t i = strlen(p->Pass); i < len; i++) {
            if (p->test_symb) {
                return_ran(pull_rand() % 5 + 1);
            } else {
                return_ran(pull_rand() % 4 + 1);
            }
            p->Pass[i] = p->tmp_c;
            if (p->failed) {
                return;
            }
        }
    }
    else {
        for(size_t i = 0; i < len; i++) {
            if (p->test_symb) {
                return_ran(pull_rand() % 5 + 1);
            } else {
                return_ran(pull_rand() % 4 + 1);
            }
            p->Pass[i] = p->tmp_c;
            if (p->failed) {
                return;
            }
        }
    }
    if (strlen(p->Pass) != len) {
        goto LOOP;
    }
}

struct option long_options[] = {
    { "length",     required_argument,  0,      'l' },
    { "help",       no_argument,        0,      'h' },
    { "output",     required_argument,  0,      'o' },
    { "testing",    required_argument,  0,      't' },
    { "verbose",    no_argument,        0,      'v' },
    { 0, 0, 0, 0 }
};



void write_file(char *str, char *path) {
    FILE *ptr = fopen(path, "w");
    fputs(str, ptr);
    fclose(ptr);
}

int main(int argc, char **argv) {
    int len, c, option_index = 0;
    bool verbose = false, made_pass = false, tmp = false, commence = false;
    char *file;
    while((c = getopt_long(argc, argv, "ht:vs:o:l:", long_options, &option_index)) != -1) {
        switch(c) {
            case 'h':
                usage(); 
                break;
            case 'v':
                verbose = true;
                break;
            case 't':
                p->test_symb = true;
                p->TESTING_SYMBOLS = malloc(BUF_SIZE * sizeof(char*));
                p->TESTING_SYMBOLS = optarg;
                break;
            case 's':
                strcat(p->salt, optarg);
                p->salted = true;
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
        init(p,BUF_SIZE);
        set_pass(len);
        if (verbose) {
            printf("Password: %s\nWith Length: %zu\n",p->Pass, strlen(p->Pass));
        }
        else {
            printf("%s\n",p->Pass);
        }
    }
    if (tmp) {
        init(p,BUF_SIZE);
        if (made_pass) {
            write_file(p->Pass, file);
            if (verbose) {
                printf("Wrote password to file: %s\n",file);
            }
        }
        else {
            fprintf(stderr, "Password not generated. Run $ kavpass -h\n");
        }
    }
    if (p->Pass) {
        free(p->Pass);
    }
}
