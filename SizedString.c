//
// Created by Wouter on 24-9-2018.
//

#include "SizedString.h"


SizedString getSizedString(const char *cstring) {
    size_t len = strlen(cstring);
    SizedString str;
    str.elements = malloc(len);
    str.size = len;
    memcpy(str.elements, cstring, len);
    return str;
}

const char * getNullTerminatedString(SizedString str) {
    char *string = malloc(str.size + 1);
    memcpy(string, str.elements, str.size);
    string[str.size] = 0;
    return string;
}

int sstrcmp(SizedString a, SizedString b) {
    if (a.size < b.size) {
        return -1;
    } else if (a.size > b.size) {
        return 1;
    }

    return memcmp(a.elements, b.elements, a.size);
}
