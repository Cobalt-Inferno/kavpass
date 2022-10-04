#ifndef HASHER_H
#define HASHER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>


char *md5sum(char *filename);
#endif
