#ifndef CURLER_H
#define CURLER_H
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#define MAX_BAR_LEN 30

size_t write_data(void *ptr, size_t size, size_t cmemb, FILE *stream);
void xcurl(const char *url, const char *of);

#endif
