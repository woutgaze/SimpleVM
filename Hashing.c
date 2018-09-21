//
// Created by Wouter on 9/19/2018.
//

#include "Hashing.h"

//
// http://www.cse.yorku.ca/~oz/hash.html
//
uint32_t string_hash(const char *str, size_t size) {
    uint32_t hash = 5381;
    for (size_t i = 0; i < size; i++) {
        hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */
    }
    return hash & 0x7FFFFFFF;
}