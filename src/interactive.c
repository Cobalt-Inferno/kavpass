#include "interactive.h"

void i_help() {
    printf("Available commands in xArtix interactive mode:\n");
    printf("\t\t•\tset verbose\t\t|\t[true/false] sets the status of verbose output.\n");
    printf("\t\t•\tset length\t\t|\t[num] sets the length for generated passwords.\n");
    printf("\t\t•\tset output\t\t|\t[file] sets the file to output to.\n");
    printf("\t\t•\tset unsafe\t\t|\t[true/false] sets the status for using an unsafe RNG.\n");
    printf("\t\t•\tset extra-unicode\t|\t[true/false] sets the status for using extra unicode chars.\n");
    printf("\t\t•\tset prompt\t\t|\t[prompt] sets the interactive prompt.\n");
    printf("\t\t•\tgenerate\t\t|\tgenerates the prompt with select options.\n");
}

void write_file(char *str, char *path) {
    printf("writing to file %s\n", path);
    FILE *ptr = fopen(path, "w");
    fputs(str, ptr);
    fclose(ptr);
}
void write_file2(Password *pass) {
    printf("Writing pass: %s\nIn file: %s\n", pass->Pass, pass->file);
}

void k_parse(xArtix *kav, Password *pass) {
    //char *file = (char*) malloc(256*(sizeof(char*)));
    char *token = strtok(kav->input, " ");
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
                printf("PROMPT\n");
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
            else if (strncmp(token, "prefix", 6) == 0) {
                token = strtok(NULL, " ");
                if (token == NULL) {
                    fprintf(stderr, "No prefix provided.\n");
                    return;
                }
                if (token[strlen(token - 1)] == '\n') {
                    token[strlen(token - 1)] = '\0';
                }
                pass->prefix = true;
                pass->pref = token;
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
                    pass->test_symb_b= true;
                }
                else if (strncmp(token, "false", 5) == 0) {
                    pass->test_symb_b = false;
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
                if (token[strlen(token) - 1] == '\n') {
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
                if (token[strlen(token - 1)] == '\n') {
                    token[strlen(token - 1)] = '\0';
                }
                if (atoi(token) > 2096) {
                    fprintf(stderr, "Length cannot be over 2096.\n");
                    return;
                }
                //kav->len = atoi(token);
                pass->len = atoi(token);
            }
            else if (strncmp(token, "output", 6) == 0) {
                printf("OUTPUT\n");
                token = strtok(NULL, " ");
                if (token == NULL) {
                    fprintf(stderr, "No output file specified.\n");
                }
                if (token[strlen(token) - 1] == '\n') {
                    token[strlen(token) - 1] = '\0';
                }
                /*file = token;
                printf("file:%s|\n", file);*/
                //pass->file =token;
                printf("asd\n");
                strcpy(pass->file, token);
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
            safe_set_pass(pass);
            if (kav->verbose) {
                if (kav->file) {
                    write_file(pass->Pass, pass->file);
                    printf("Password: %s\nWith length: %d written to file: %s\n",pass->Pass, pass->len, pass->file);
                }
                else {
                    printf("Password: %s\nWith length: %d\n",pass->Pass, pass->len);
                }
            }
            else {
                printf("%s\n",pass->Pass);
            }
        } else {
            unsafe_set_pass(pass);
            if (kav->verbose) {
                if (kav->file) {
                    write_file(pass->Pass, pass->file);
                    printf("Unsafe password: %s\nWith lengh: %d written to file: %s\n",pass->Pass, pass->len, pass->file); 
                }
                else {
                    printf("Unsafe password: %s\nWith lengh: %d\n",pass->Pass, pass->len);
                }
            }
            else {
                printf("%s\n",pass->Pass);
            }
        }
        return;
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
    //free(file);
}

void k_init(xArtix *kav) {
    kav->prompt = "> ";
    kav->input = ";";

}

void k_ctl(xArtix *kav, Password *pass) { 
    char buff[256];
    k_init(kav);
    // debug stuff
    pass->len=12;
    kav->verbose = true;
    // end debug stuff

    while((strncmp(kav->input, "exit",4))) {
        // debug
        printf("\nfile: %s\n", pass->file);
        //end debug
        printf("%s",kav->prompt);
        fgets(buff, 256, stdin);
        kav->input = buff;
        k_parse(kav, pass);
    }
}

void interactive_mode(Password *pass){
    xArtix *kav = malloc(256 * sizeof(xArtix));

    init(pass);
    k_ctl(kav, pass);

    free(kav);
}