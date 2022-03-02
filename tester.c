#include <stdlib.h>
#include <stdio.h>
#include "test.h"
#include "test_codemeter.h"


void test_codemeter(int state) {
    set_codemeter_led(state);
}

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
    test_codemeter(add(a,b)%4);
    return 0;
}
