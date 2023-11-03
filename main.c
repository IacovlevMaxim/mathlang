#include <stdio.h>
#include <stdlib.h>
//
// Created by maxim on 03.11.2023.
//
char* code = NULL;
int depth;
long r;
size_t size;

int main(void) {
    code = malloc(1);
    depth = 0;

    while((r = getline(&code, &size, stdin)) != -1) {

    }

    return 0;
}