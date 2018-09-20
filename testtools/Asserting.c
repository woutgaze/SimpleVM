//
// Created by Wouter on 20-9-2018.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Asserting.h"
#include "../DevTools.h"

void assertTrue(const char *description, bool value) {
    if (!value) {
        panic_a("ASSERTION FAILED", description);
    }
}

void assertFalse(const char *description, bool value) {
    assertTrue(description, !value);
}

void assertEquals(int a, int b) {
    if (!(a == b)) {
        printf("ASSERTION FAILED: %d == %d\n", a, b);
        panic("ASSERTION FAILED");
        exit(-1);
    }
}
