#ifndef DATA_H
#define DATA_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#define MAX 5096
#define MAX_STR_BUFF 5096
typedef struct __item item_t;
struct __item {
  char *key;
  char *val;
};

typedef struct __list list_t;

struct __list {
  item_t *item;
  list_t *next;
};

typedef struct __table table_t;

struct __table {
  item_t **items;
  list_t **overflow;
  int size;
  int count;
};

list_t *allocate();
list_t *insert(list_t *list, item_t *item);
item_t *list_rm(list_t *list);
void free_list(list_t *list);
item_t *construct_item(char *key, char *val);
list_t **construct_overflow(table_t *table);
void free_overflow(table_t *table);
void free_hashtable(table_t *table);
table_t *construct_table(int size);
void free_item(item_t *item);
void handle_collision(table_t *table, unsigned long index, item_t *item);
void db_insert(table_t *table, char *key, char *value);
char *db_search(table_t *table, char *key);
void db_delete(table_t *table, char *key);
const char *get_val(table_t *table, char *key);

#endif
