#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <stdio.h>
#include <stdbool.h>
#include <strings.h>
#include "color.h"
#include "generator.h"

typedef struct {
    char *prompt;
    //int len;
    char *input;
    bool verbose;
    bool unsafe;
    bool file;
} __attribute__((packed)) kavpass;


void i_help();
void write_file(char *str, char *path);
void k_parse(kavpass *kav, Password *pass);
void k_init(kavpass *kav);
void k_ctl(kavpass *kav, Password *pass);
void interactive_mode(Password * pass);

#endif
