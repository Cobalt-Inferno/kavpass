#ifndef TOP_LEVEL_H
#define TOP_LEVEL_H
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <dirent.h>
#include "hasher.h"
#include "scanner.h"
bool isdir(char *path);
void GetFiles(char *name, int indent, bool testing, char *tbuf);
void GetMD5s(char *name, int indent);
#endif
