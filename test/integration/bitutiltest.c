#include "../util/bitutil.h"
#include <stdio.h>

int main(){
    unsigned int a[] = {0,1,1,0};
    printf("%lX", bit_create(a,4));
    printf("%X", ascii_byte_to_int("AE"));
    return 0;
}
