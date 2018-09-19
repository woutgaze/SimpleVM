//
// Created by Wouter on 19-9-2018.
//

#include <stdio.h>
#include <stdlib.h>
#include "NodeReader_V1_Tools.h"
#include "../ASTNodes.h"

Node * readFailed_V1() {
    fprintf(stderr, "Read failed.\n");
    exit(-1);
}

char * readString_V1(FILE *fileptr){
    uint16_t size;
    if (!fread(&size, sizeof(uint16_t), 1, fileptr) == 1)  readFailed_V1();
    char * string = malloc(size);
    if (!fread(string, 1, size, fileptr) == size)  readFailed_V1();
    return string;
}

bool readBool_V1(FILE *fileptr){
    bool value;
    if (!fread(&value, sizeof(bool), 1, fileptr) == 1)  readFailed_V1();
    return value;
}

int readInt_V1(FILE *fileptr){
    int value;
    if (!fread(&value, sizeof(int32_t), 1, fileptr) == 1)  readFailed_V1();
    return value;
}

uint16_t readIndex_V1(FILE *fileptr){
    uint16_t value;
    if (!fread(&value, sizeof(uint16_t), 1, fileptr) == 1)  readFailed_V1();
    return value;
}

NodeArray readNodeArray_V1(FILE *fileptr) {
    NodeArray value;
    uint16_t size;
    if (!fread(&size, sizeof(uint16_t), 1, fileptr) == 1) readFailed_V1();
    value.size =size;
    value.elements = malloc(size * sizeof(Node *));
    for (int i = 0; i < size; i++) {
        value.elements[i] = readNode_V1(fileptr);
    }
    return value;
}

ArgumentNodeArray readArgumentNodeArray_V1(FILE *fileptr) {
    ArgumentNodeArray value;
    uint16_t size;
    if (!fread(&size, sizeof(uint16_t), 1, fileptr) == 1) readFailed_V1();
    value.size =size;
    value.elements = malloc(size * sizeof(Node *));
    for (int i = 0; i < size; i++) {
        value.elements[i] = (ArgumentNode*) readNode_V1(fileptr);
    }
    return value;
}

MethodNodeArray readMethodNodeArray_V1(FILE *fileptr) {
    MethodNodeArray value;
    uint16_t size;
    if (!fread(&size, sizeof(uint16_t), 1, fileptr) == 1) readFailed_V1();
    value.size =size;
    value.elements = malloc(size * sizeof(Node *));
    for (int i = 0; i < size; i++) {
        value.elements[i] = (MethodNode*) readNode_V1(fileptr);
    }
    return value;
}


