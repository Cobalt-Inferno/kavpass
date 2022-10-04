#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define LINE_BUFF 2048
#define MIC_COMMENT ';'
enum datatype {
    T_INT,
    T_STR,
    T_DOUBLE,
};
typedef struct {
    void *val;
    char *opt;
    enum datatype data;
    bool comp;
} __attribute__((packed)) conf_t;
void clean(char **str) {
    while(isspace(**str)) {
        (*str)++;
    }
    char *ev = strchr(*str, '\0') - 1;
    while(isspace(*ev)) {
        ev--;
    }
    *(ev+1) = '\0';
}
int find(char *opt, conf_t *opts) {
    int n = 0;
    while(opts[n].opt != NULL) {
        if(opts[n].comp == true) {
            n++;
            continue;
        }
        if(strcmp(opt, opts[n].opt) == 0) {
            return n;
        } else {
            n++;
        }
    }
    return -1;
}
void parse(const char *load, conf_t *opts) {
    char buff[LINE_BUFF];
    char *opt, *val;
    FILE *file = fopen(load, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: File does not exist!\n");
        return;
    }
    while(fgets(buff, LINE_BUFF, file) != NULL) {
        if (buff[0] == MIC_COMMENT || buff[0] == '\0') {
            continue;
        }
        if ((opt = strtok(buff, ":")) != NULL) {
            clean(&opt);
            int v = find(opt, opts);
            if (v < 0) {
                continue;
            }
            val = strtok(NULL, "\n");
            clean(&val);
            switch(opts[v].data) {
                case T_INT:
                    int *p = (int *)opts[v].val;
                    *p = (int) strtol(val, NULL, 0);
                    break;
                case T_STR:
                    char **ps = (char **)opts[v].val;
                    strcpy(*ps, val);
                    break;
                case T_DOUBLE:
                    double *pb = (double *)opts[v].val;
                    *pb = strtod(val, NULL);
                    break;
                default:
                    continue;
            }
            opts[v].comp = true;
        }
    }
    fclose(file);
}
