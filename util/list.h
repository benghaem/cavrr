#ifndef LIST_H
#define LIST_H

struct list{
    int v;
    struct list* next;
} list;

int list_find(struct list* l, int v);

int list_append(struct list* l, int v);

int list_remove(struct list* l, int v);

struct list* list_create(int v);

void list_destroy(struct list *l);

#endif /* list.h */
