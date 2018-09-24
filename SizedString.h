//
// Created by Wouter on 24-9-2018.
//

#ifndef SIMPLEVM_SIZEDSTRING_H
#define SIMPLEVM_SIZEDSTRING_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct SizedString {
    size_t size;
    char elements[];
} SizedString;

SizedString *getSizedString(const char *cstring);
const char * getNullTerminatedString(SizedString * str);
int sstrcmp(SizedString * a, SizedString * b);

#endif //SIMPLEVM_SIZEDSTRING_H
