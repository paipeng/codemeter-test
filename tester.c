#include "test.h"
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char** argv) {
    int a, b;
    if (argc >= 3) {
        a = atoi(argv[1]);
        b = atoi(argv[2]);
    } else {
        a = 1;
        b = 23;

    }
    printf("hello world %d+%d=%d\n", a, b, add(a,b));
    return 0;
}
