//
// Created by Wouter on 20-9-2018.
//

#ifndef SIMPLEVM_ASSERTING_H
#define SIMPLEVM_ASSERTING_H
#include <stdbool.h>

void assertTrue(char *description, bool value);
void assertFalse(char *description, bool value);
void assertEquals(int a, int b);

#endif //SIMPLEVM_ASSERTING_H
