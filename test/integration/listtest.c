#include "list.h"
#include <stdio.h>
#include <assert.h>

void printlist(struct list* iter){
    printf("[");
    while(iter != NULL){
        printf("%i, ",iter->v);
        iter = iter->next;
    }
    printf("]\n");
}

int main(void){
    struct list* l = list_create(0);
    list_append(l, 1);
    list_append(l, 2);
    list_append(l, 3);
    list_append(l, 4);

    printlist(l);

    list_remove(l, 3);

    printlist(l);

    assert(list_find(l,1) == 1);

    list_destroy(l);
}
