#include "data.h"


unsigned char *encrypt(unsigned char *key, unsigned char *txt) {
  unsigned char buff[strlen((const char*)key) + 10];
  AES_KEY ek;
  AES_set_encrypt_key(key, MAX_STR_BUFF, &ek);
  AES_encrypt(txt, buff, &ek);
  return buff;
}


unsigned long hash(char *str) {
  unsigned long v = 0;
  for (int i = 0; str[i]; i++) {
    v ^= (v << (v / 2));
  }
  return v % MAX;
}

list_t *allocate() {
  list_t *list = (list_t*) malloc(sizeof(list_t) + 1);
  return list;
}
list_t *insert(list_t *list, item_t *item) {
  if (!list) {
    list_t *head = allocate();
    head->item = item;
    head->next = NULL;
    list = head;
    return list;
  }
  else if (list->next == NULL) {
    list_t *node = allocate();
    node->item = item;
    node->next = NULL;
    list->next = node;
    return list;
  }
  list_t *tmp = list;
  while (tmp->next) {
    tmp = tmp->next;
  }
  list_t *node = allocate();
  node->item = item;
  node->next = NULL;
  node->next = node;
  return list;
}

item_t *list_rm(list_t *list) {
  if (!list) {
    return NULL;
  }
  if (!list->next) {
    return NULL;
  }
  list_t *node = list->next;
  list_t *tmp = list;
  tmp->next = NULL;
  list = node;
  item_t *item = NULL;
  memcpy(tmp->item, item, sizeof(item_t));
  free(tmp);
  return item;
}

void free_list(list_t *list) {
  if (!list) {
    return;
  }
  list_t *tmp = list;
  while (list) {
    tmp = list;
    list = list->next;
    free(tmp->item->key);
    free(tmp->item->val);
    free(tmp->item);
    free(tmp);
  }
}

item_t *construct_item(char *key, char *val) {
  item_t *item = (item_t*) malloc(sizeof(item_t));
  item->key = (char*) calloc(strlen(key) + 1, sizeof(char));
  item->val = (char*) calloc(strlen(val) + 1, sizeof(char));
  strcpy(item->key, key);
  strcpy(item->val, val);
  return item;
}


list_t **construct_overflow(table_t *table) {
  list_t **flow = (list_t**) calloc((unsigned long)table->size, sizeof(list_t*));
  for (int i = 0; i < table->size; i++) {
    flow[i] = NULL;
  }
  return flow;
}
void free_overflow(table_t *table) {
  list_t **flow = table->overflow;
  for (int i = 0; i < table->size; i++)
    free_list(flow[i]);
  free(flow);
}

void free_hashtable(table_t *table) {
  for (int i = 0; i < table->size; i++) {
    item_t *item = table->items[i];
    if (item != NULL)
      free_item(item);
  }

  free_overflow(table);
  free(table->items);
  free(table);
}

table_t *construct_table(int size) {
  table_t *table = (table_t*) malloc (sizeof(table_t));
  table->size = size;
  table->count = 0;
  table->items = (item_t**) calloc((long unsigned)table->size, sizeof(table_t*));
  for (int i = 0; i < table->size; i++)
    table->items[i] = NULL;
  table->overflow = construct_overflow(table);
  return table;
}
void free_item(item_t *item) {
  free(item->key);
  free(item->val);
  free(item);
}

void handle_collision(table_t *table, unsigned long index, item_t *item) {
  list_t *head = table->overflow[index];

  if (head == NULL) {
    head = allocate();
    head->item = item;
    table->overflow[index] = head;
    return;
  }
  else {
    table->overflow[index] = insert(head, item);
    return;
  }
}

void db_insert(table_t *table, char *key, char *value) {
  item_t *item = construct_item(key, value);
  int index = (int) hash(key);
  item_t *current_item = table->items[index];
  if (current_item == NULL) {
    if (table->count == table->size) {
      free_item(item);
      return;
    }
    table->items[index] = item; 
    table->count++;
  }
  else {
    if (strcmp(current_item->key, key) == 0) {
      free(table->items[index]->val);
      table->items[index]->val = (char*) calloc (strlen(value) + 1, sizeof(char));
      strcpy(table->items[index]->val, value);
      free_item(item);
      return;
    }
    else {
      handle_collision(table, (unsigned) index, item);
      return;
    }
  }
}

char *db_search(table_t *table, char *key) {
  int index = (int) hash(key);
  item_t *item = table->items[index];
  list_t* head = table->overflow[index];
  while (item != NULL) {
    if (strcmp(item->key, key) == 0)
      return item->val;
    if (head == NULL)
      return NULL;
    item = head->item;
    head = head->next;
  }
  return NULL;
}

void db_delete(table_t *table, char *key) {
  int index = (int) hash(key);
  item_t *item = table->items[index];
  list_t *head = table->overflow[index];
  if (item == NULL) {
    return;
  }
  else {
    if (head == NULL && strcmp(item->key, key) == 0) {
      table->items[index] = NULL;
      free_item(item);
      table->count--;
      return;
    }
    else if (head != NULL) {
      if (strcmp(item->key, key) == 0) {
        free_item(item);
        list_t *node = head;
        head = head->next;
        node->next = NULL;
        table->items[index] = construct_item(node->item->key, node->item->val);
        free_list(node);
        table->overflow[index] = head;
        return;
      }
      list_t *curr = head;
      list_t *prev = NULL;
      while (curr) {
        if (strcmp(curr->item->key, key) == 0) {
          if (prev == NULL) {
            free_list(head);
            table->overflow[index] = NULL;
            return;
          }
          else {
            prev->next = curr->next;
            curr->next = NULL;
            free_list(curr);
            table->overflow[index] = head;
            return;
          }
        }
        curr = curr->next;
        prev = curr;
      }
    }
  }
}

const char *get_val(table_t *table, char *key) {
  char *val;
  if ((val = db_search(table, key)) == NULL) {
    return NULL;
  } else {
    return val;
  }
}

