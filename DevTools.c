//
// Created by Wouter on 9/17/2018.
//
#include "DevTools.h"
#include <stdio.h>
#include <stdlib.h>

void panic(char *message) {
    panic_a(message, NULL);
}

void panic_a(char *message, char *argument) {
    fflush(stdout);
    if (argument == NULL) {
        fprintf(stderr, "PANIC: %s \n", message);
    } else {
        fprintf(stderr, "PANIC: %s : %s\n", message, argument);
    }
    exit(-1);
}

void niy() {
    panic("Not Implemented Yet");
}

