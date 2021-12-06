#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VALUE 25
#define SIZE 150

#define I_MIN 0
#define I_MAX 1
#define I_MOY 2

int mmm[3] = { MAX_VALUE, 0, 0 };
int array[SIZE] = { };
int result[SIZE] = { };


void generate() {
    int i;
    srand( time( NULL ) );

    for (i = 0; i < SIZE; i++) {
        array[i] = rand() % MAX_VALUE;
    }
}

void draw() {
    int i, j, k, r;
    char graph[MAX_VALUE][SIZE];

    for(j = 0; j < SIZE; j++) {
        for (i = 0; i < MAX_VALUE; i++) {
            r = result[j];
            if ( i < r) { 
                graph[i][j] = '|';
            } else {
                graph[i][j] = ' ';
            }
        }
    }

    for (i = MAX_VALUE - 1; i >= 0; i--) {
        for(j = 0; j < SIZE; j++) {
            printf("%c", graph[i][j]);
        }
        printf("\n");
    }
}

void minmax() {
    int i;
    int cur;
    int *result = mmm;

    for (i = 0; i < SIZE; i++) {
        cur = array[i];
        mmm[I_MOY] += cur;

        if(cur > mmm[I_MAX]) {
            mmm[I_MAX] = cur;
        }
        if(cur < mmm[I_MIN]) {
            mmm[I_MIN] = cur;
        }
    }

    mmm[I_MOY] /= SIZE;
}



void radix() {
    int i, r;
    int bucket[MAX_VALUE] = { 0 };

    for (i = 0; i < SIZE; i++) {
        bucket[array[i]] += 1;
    }

    r = 0;
    for (i = 0; i < MAX_VALUE; i++) {
        /* while(bucket[i]--) { avec doublons */
        if (bucket[i]) { 
            result[r++] = i;
        }
    }
    }

    int main (void) {
        int i;
        generate();
        minmax();
        radix();
        printf("min=%d, max=%d, moy=%d\n", mmm[0], mmm[1], mmm[2]);

        printf("Sans doublon :\n");

        for (i = 0; i < SIZE; i++) {
            if (result[i]) {
                printf("%d ", result[i]);
            }
        }
        printf("\n");

        draw();


        return 0;
    }
