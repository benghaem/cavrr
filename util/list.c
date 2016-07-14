#include "list.h"
#include <stdlib.h>

struct list* list_create(int v){
    struct list* l = malloc(sizeof(struct list));
    l->v = v;
    l->next = NULL;
    return l;
}

int list_find(struct list* l, int v){
    while( l != NULL ){
        if (l->v == v){
            return 1;
        }
        l = l->next;
    }
    return 0;
}

int list_append(struct list* l, int v){
    if (l != NULL){
        while( l->next != NULL ){
            l = l->next;
        }
        l->next = list_create(v);
        return 1;
    }
    return 0;
}

int list_remove(struct list* l, int v){
    struct list* prev = NULL;
    while (l != NULL ){
        if (l->v == v){
            prev->next = l->next;
            free(l);
            return 1;
        }
        prev = l;
        l = l->next;
    }
    return 0;
}

void list_destroy(struct list *l){
    struct list* prev = NULL;
    while (l != NULL ){
        prev = l;
        l = l->next;
        free(prev);
    }
}
