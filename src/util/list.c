#include "list.h"
#include <stdlib.h>



struct list* list_create(){
    struct list* l = malloc(sizeof(struct list));

    l->next = NULL;
    return l;
}

int list_find(struct list* l, int v){
    if (l != NULL){
        while( l->next != NULL ){
            if (l->v == v){
                return 1;
            }
            l = l->next;
        }
    }
    return 0;
}

int list_append(struct list* l, int v){
    if (l != NULL){
        while ( l->next != NULL ){
            l = l->next;
        }
        l->next = list_create();
        l->v = v;
        return 1;
    }
    return 0;
}


int list_remove(struct list* l, int v){
    struct list* next;
    if (l != NULL){
        while (l->next != NULL){
            if (l->v == v){
                next = l->next;
                l->v = next->v;
                l->next = next->next;
                free(next);
                return 1;
            }
            l = l->next;
        }
    }
    return 0;
}

void list_destroy(struct list *l){
    struct list* prev;
    if (l != NULL){
         while (l->next != NULL){
            prev = l;
            l = l->next;
            free(prev);
         }
         free(l);
    }
}
