#ifndef SCANNER_H
#define SCANNER_H
#include <stdbool.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

bool quickscan(const char *md5);
char *loadbuf();
char *tmpbuff();
bool testscan(const char *md5, const char *buf);
#endif
