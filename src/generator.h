#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    char *symb;
    char *l_let;
    char *u_let;
    char *ints;
    char *Pass;
    int len;
    bool prefix;
    char *file;
    bool failed;
    char *pref;
    char tmp_c;
    char *test_symb;
    bool test_symb_b;
} __attribute__((packed)) Password;

void init(Password *pass);
int pull_rand();
void safe_return_ran(int line, Password *pass);
void safe_set_pass(Password *pass);
void unsafe_set_pass(Password *pass);

#endif
