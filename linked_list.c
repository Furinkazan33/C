#include <stdio.h>
#include <stdlib.h>
//#include "/usr/include/string.h"
#include <string.h>

#define DATA_LEN 50


typedef struct node {
    char *data;
    struct node *son;
} node_t;

node_t *head = NULL;


node_t *new(char *data) {
    node_t *node = (node_t *) malloc(sizeof(node_t));
    node->data = (char *) malloc (sizeof(char *) * strlen(data) + 1);
    strcpy(node->data, data);

    return node;
}


int main(void) {
    node_t *head = new("Head node");
    node_t *s1 = new("Son 1");
    node_t *s11 = new("Son 11");

    s1->son = s11;
    head->son = s1;

    printf("%s\n", head->data);
    printf("%s\n", head->son->data);
    printf("%s\n", head->son->son->data);

    return 0;
}




