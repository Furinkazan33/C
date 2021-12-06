#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int arr_size(char *src) {
    int i = 0;

    while(src[i++] != '\0') {
    }

    return i;
}

char *arr_copy(char *src) {
    int n = arr_size(src);
    char *result = (char *) malloc(sizeof(char *) * (n + 1));

    for (; n >= 0; n--) {
        result[n] = src[n];
    }

    return result;
}


void arr_print(char *src) {
    while(*src != '\0')
    {
        printf("%c", *src++);
    }
    printf("\n");
}

int main(int argv, char *argc) 
{
    char a[] = "Un petit test\0";
    char *p = a;
    char *p2 = arr_copy(p);

    a[2] = 'X';

    arr_print(p);
    arr_print(p2);
}


