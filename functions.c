#include <stdio.h>



int for_loop(int init, int max, void (*f)(void)) {

    if (init == max) {
        return 0;
    }

    (*f)();

    for_loop(init + 1, max, f);

}

void f() {
    printf("X");
}

int g() {
    printf("X");

    return 0;
}

int main(int argc, char **argv) {
    int i = 0;

    while(i < argc) {
        printf("%s\n", argv[i++]);
    }

    void (*func)(void) = (void *) &g;

    for_loop(0, 5, *func); 

}
