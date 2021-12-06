#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void _fibo1(int u, int v, int max, int *res)
{
    if (max > 0) {
        res[max] = u;
        _fibo1(v, u + v, max - 1, res);    
    }
}
void fibo1(int max) {
    int *res = (int *) malloc(sizeof(int *) * (max + 1));
    _fibo1(0, 1, max, res);

    while(max > 0) {
        printf("%d ", res[max--]);
    }

}

int main() {
    fibo1(40);

    return 0;
}

