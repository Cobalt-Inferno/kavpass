#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define RESET  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define WHITE  "\x1B[37m"

void switch_color(int col) {
    col = col >= 0 ? col : 0;
    switch(col) {
        case 0:
            printf("%s",RESET);
            break;
        case 1:
            printf("%s",RED);
            break;
        case 2:
            printf("%s",GREEN);
            break;
        case 3:
            printf("%s",YELLOW);
            break;
        case 4:
            printf("%s",BLUE);
            break;
        case 5:
            printf("%s",MAGENTA);
            break;
        case 6:
            printf("%s",CYAN);
            break;
        case 7:
            printf("%s",WHITE);
            break;
        default:
            return;
    }
}


const size_t BUF_SIZE = 2096;
const char *version = "v1.0.0";
typedef struct {
    char *symb;
    char *l_let;
    char *u_let;
    char *ints;
    char *Pass;
    int len;
    char *file;
    bool failed;
    char tmp_c;
    char *test_symb;
    bool test_symb_b;
} Password;

void i_help() {
    printf("Available commands in kavpass interactive mode:\n");
    printf("\tset\t|\tThis command can take any of the following options:\n");
    printf("\t\t•\tverbose\t\t|\t[true/false] sets the status of verbose output.\n");
    printf("\t\t•\tlength\t\t|\t[num] sets the length for generated passwords.\n");
    printf("\t\t•\toutput\t\t|\t[file] sets the file to output to.\n");
    printf("\t\t•\tunsafe\t\t|\t[true/false] sets the status for using an unsafe RNG.\n");
    printf("\t\t•\textra-unicode\t|\t[true/false] sets the status for using extra unicode chars.\n");
    printf("\t\t•\tprompt\t\t|\t[prompt] sets the interactive prompt.\n");
    printf("\tgenerate\t|\tgenerates the prompt with select options.\n");
}

int pull_rand();
void init(Password *pass, size_t BUFFER);
void safe_return_ran(int line);
void unsafe_return_ran(int line);
void usage();
void safe_set_pass(size_t len);
void unsafe_set_pass(size_t len);
void write_file(char *sr, char *path);
Password pass, *p = &pass;
typedef struct {
    char *prompt;
    int len;
    char *input;
    bool verbose;
    bool unsafe;
    bool file;
} kavpass;
void k_parse(char *msg, kavpass *kav) {
    char *file = (char*) malloc(256*(sizeof(char*)));
    char *token = strtok(msg, " ");
    if (strncmp(token, "\n", 1) == 0 || strncmp(token, "\0", 1) == 0) {
        return;
    }
    else if (strncmp(token,"set", 3) == 0) {
        token = strtok(NULL, " ");
        if (token == NULL) {
            fprintf(stderr, "No prompt -set- value provided.\n");
            return;
        }
        if (token != NULL) {
            if (strncmp(token, "prompt", 6) == 0) {
                token = strtok(NULL, " ");
                if (token == NULL) {
                    fprintf(stderr, "No prompt provided.\n");
                    return;
                }
                int len = strlen(token);
                if (token[len - 1] == '\n') {
                    token[len - 1] = '\0';
                }
                strcat(token, " ");
                kav->prompt = token;
            }
            else if (strncmp(token, "extra-unicode", 13) == 0) {
                token = strtok(NULL, " ");
                if (token[strlen(token) - 1] == '\n') {
                    token[strlen(token) - 1] = '\0';
                }
                if (token == NULL) {
                    fprintf(stderr, "No setting specified.\n");
                    return;
                }
                if (strncmp(token, "true", 4) == 0) {
                    p->test_symb_b= true;
                }
                else if (strncmp(token, "false", 5) == 0) {
                    p->test_symb_b = false;
                }    
                else {
                    fprintf(stderr, "Option: \"%s\" is not valid.\n",token);
                }
            }
            else if (strncmp(token, "unsafe", 6) == 0) {
                token = strtok(NULL, " ");
                if (token == NULL) {
                    fprintf(stderr, "No setting specified.\n");
                    return;
                }
                if (token[strlen(token) - 1] == '\n') {
                    token[strlen(token) - 1] = '\0';
                }
                if (strncmp(token, "true", 4) == 0) {
                    kav->unsafe = true;
                }
                else if (strncmp(token, "false", 5) == 0) {
                    kav->unsafe = false;
                }    
                else {
                    fprintf(stderr, "Option: \"%s\" is not valid.\n",token);
                }
            }
            else if (strncmp(token, "color", 5) == 0) {
                token = strtok(NULL, " ");
                if (token == NULL) {
                    fprintf(stderr, "No color specified.\n");
                    return;
                }
                if (token[strlen(token - 1)] == '\n') {
                    token[strlen(token) - 1] = '\0';
                }
                if (strncmp(token, "reset", 5) == 0) {
                    switch_color(0);
                } else if (strncmp(token, "red", 3) == 0) {
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
                    fprintf(stderr, "Color: \"%s\" is not recognized.\n",token);
                }
            }
            else if (strncmp(token, "length", 6) == 0) {
                token = strtok(NULL, " ");
                if (token == NULL) {
                    fprintf(stderr, "No length specified.\n");
                    return;
                }
                if (atoi(token) > 2096) {
                    fprintf(stderr, "Length cannot be over 2096.\n");
                    return;
                }
                kav->len = atoi(token);
            }
            else if (strncmp(token, "output", 6) == 0) {
                token = strtok(NULL, " ");
                if (token == NULL) {
                    fprintf(stderr, "No output file specified.\n");
                }
                if (token[strlen(token) - 1] == '\n') {
                    token[strlen(token) - 1] = '\0';
                }
                file = token;
                kav->file = true;
            }
            else if (strncmp(token, "verbose", 7) == 0) {
                token = strtok(NULL, " ");
                if (token == NULL) {
                    fprintf(stderr, "No setting specified.\n");
                    return;
                }
                if (token[strlen(token) - 1] == '\n') {
                    token[strlen(token) - 1] = '\0';
                }
                if (strncmp(token, "true", 4) == 0) {
                    kav->verbose = true;
                }
                else if (strncmp(token, "false", 5) == 0) {
                    kav->verbose = false;
                }    
                else {
                    fprintf(stderr, "Option: \"%s\" is not valid.\n",token);
                }
            }
            else {
                fprintf(stderr, "Setting not recognized.\n");
            }
        }
    }
    else if (strncmp(token, "help", 4) == 0) {
        i_help();
    }
    else if (strncmp(token, "generate", 8) == 0) {
        if (!kav->unsafe) {
            safe_set_pass(kav->len);
            if (kav->verbose) {
                if (kav->file) {
                    write_file(p->Pass, file);
                    printf("Password: %s\nWith length: %d written to file: %s\n",p->Pass, kav->len, file);
                }
                else {
                    printf("Password: %s\nWith length: %d\n",p->Pass, kav->len);
                }
            }
            else {
                printf("%s\n",p->Pass);
            }
        } else {
            unsafe_set_pass(kav->len);
            if (kav->verbose) {
                if (kav->file) {
                    write_file(p->Pass, file);
                    printf("Unsafe password: %s\nWith lengh: %d written to file: %s\n",p->Pass, kav->len, file); 
                }
                else {
                    printf("Unsafe password: %s\nWith lengh: %d\n",p->Pass, kav->len);
                }
            }
            else {
                printf("%s\n",p->Pass);
            }
        }
    }
    else if (strncmp(token, "exit", 4) == 0) {
        return;
    }
    else if (strncmp(token, "clear", 5) == 0) {
        #define ESC    "\x1b"
        printf(ESC"[2J"ESC"[?6h");
    }
    else {
        printf("Command not recognized!\n");
    }
    return;
    free(file);
}
void k_init(kavpass *kav) {
    kav->prompt = "> ";
    kav->input = ";";
}
void k_ctl(kavpass *kav) { 
    char buff[256];
    k_init(kav);
    while((strncmp(kav->input, "exit",4))) {
        printf("%s",kav->prompt);
        fgets(buff, 256, stdin);
        kav->input = buff;
        k_parse(kav->input, kav);
    }
}
#define ARR_S(arr) (sizeof(arr) / sizeof((arr)[0])) 
int pull_rand() {
    FILE * file = fopen("/dev/random", "r");            
    unsigned char buff[10];
    if (!file) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    size_t retv = fread(buff, sizeof(*buff), ARR_S(buff), file); /* I have to add some error checking */
    if (retv != ARR_S(buff)) {
        perror("fread(): failed.");
        return EXIT_FAILURE;
    }
    int temp = buff[0];
    fclose(file);
    return temp;
}
void init(Password *pass, size_t BUFFER) {
    pass->symb = "!@#$%^&*()-=+_][{}";
    pass->l_let = "qwertyuiopasdfghjklzxcvbnm";
    pass->u_let = "QWERTYUIOPASDFGHJKLZXCVBNM";
    pass->ints = "1234567890";
    pass->test_symb = "™€‰—®©☺⚛";
    pass->Pass = (char *) malloc(BUFFER * sizeof(char));
}
void safe_return_ran(int line) {
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
                p->tmp_c = p->test_symb[pull_rand() % strlen(p->test_symb)];
            default:
                p->failed = true;
        }
    }
}
void unsafe_return_ran(int line) {
    if (line >=6) {
        p->failed = true;
    }else {
        switch(line) {
            case 1:
                p->tmp_c = p->symb[rand() % strlen(p->symb)];
                break;
            case 2:
                p->tmp_c = p->l_let[rand() % strlen(p->l_let)];
                break;
            case 3:
                p->tmp_c = p->u_let[rand() % strlen(p->u_let)];
                break;
            case 4:
                p->tmp_c = p->ints[rand() % strlen(p->ints)];
                break;
            case 5:
                p->tmp_c = p->test_symb[rand() % strlen(p->test_symb)];
            default:
                p->failed = true;
        }
    }
}
void usage() {
    printf("Program: kavpass | version: %s\n", version);
    printf("Usage: kavpass -[hloveF]\n");
    printf("\t-h\t--help\t\t\t|\tDisplays the help message.\n");
    printf("\t-l\t--length NUM\t\t|\tSpecifies the length.\n");
    printf("\t-o\t--output\t\t|\tFile to output to.\n");
    printf("\t-v\t--verbose\t\t|\tVerbose output.\n"); 
    printf("\t-e\t--extra-unicode\t\t|\tAdds extra unicode char support.\n");
    printf("\t-F\t--force-unsafe-rng NUM\t|\tReplaces --length and forces the use of an unsafe RNG.\n");
    printf("\t-i\t--interactive\t\t|\tEnters an interactive mode. (IN DEVELOPMENT)\n");
}
void safe_set_pass(size_t len) {
    LOOP:
    for(size_t i = 0; i < len; i++) {
        if (p->test_symb_b) {
            safe_return_ran(pull_rand() % 5 + 1);
        } else {
            safe_return_ran(pull_rand() % 4 + 1);
        }
        p->Pass[i] = p->tmp_c;
        if (p->failed) {
            return;
        }
    }
    if (strlen(p->Pass) != len) {
        goto LOOP;
    }
}
void unsafe_set_pass(size_t len) {
    LOOP:
    srand(time(NULL));
    for(size_t i = 0; i < len; i++) {
        if (p->test_symb_b) {
            unsafe_return_ran(rand() % 5 + 1);
        } else {
            unsafe_return_ran(rand() % 4 + 1);
        }
        p->Pass[i] = p->tmp_c;
        if(p->failed) {
            return;
        }
    }
    if (strlen(p->Pass) != len) {
        goto LOOP;
    }
}
struct option long_options[] = {
    { "length",             required_argument,  0,      'l' },
    { "help",               no_argument,        0,      'h' },
    { "output",             required_argument,  0,      'o' },
    { "force-unsafe-rng",   required_argument,  0,      'F' },
    { "verbose",            no_argument,        0,      'v' },
    { "extra-unicode",      no_argument,        0,      'e' },
    { "interactive",        no_argument,        0,      'i' },
    { 0, 0, 0, 0 }
};
void write_file(char *str, char *path) {
    FILE *ptr = fopen(path, "w");
    fputs(str, ptr);
    fclose(ptr);
}
int main(int argc, char **argv) {
    int c, option_index = 0;
    bool unsafe = false, verbose = false, made_pass = false, tmp = false, commence = false;
    while((c = getopt_long(argc, argv, "F:hievo:l:", long_options, &option_index)) != -1) {
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
            case 'i':
                kavpass *kav = malloc(256 * sizeof(kavpass));
                init(p,BUF_SIZE);
                k_ctl(kav);
                free(kav);
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
                p->len = atoi(optarg);
                if (p->len > 2096) {
                    fprintf(stderr, "Length cannot be longer than 2096.\n");
                    exit(1);
                }
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
    if (!unsafe) {
        if (commence) {
            init(p,BUF_SIZE);
            safe_set_pass(p->len);
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
            init(p,BUF_SIZE);
            unsafe_set_pass(p->len);
            if (verbose) {
                printf("Unsafe password: %s\nWith Length: %zu\n",p->Pass, strlen(p->Pass));
            }
            else {
                printf("%s\n",p->Pass);
            }
        }
        if (tmp) {
            init(p,BUF_SIZE);
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
    if (p->Pass) {
        free(p->Pass);
    }
}
