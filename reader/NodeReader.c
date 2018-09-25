//
// Created by Wouter on 19-9-2018.
//

#include <stdio.h>
#include <stdlib.h>
#include "NodeReader.h"
#include "../ASTNodes.h"
#include "NodeReader_V1.h"
#include "../DevTools.h"

Node *readNodeFile(FILE *fileptr) {
    if ((getc(fileptr) != 'S') || (getc(fileptr) != 'V')) {
        panic("Unknown header");
    }
    char version = getc(fileptr);
    if (version == 1) {
        return readNode_V1(fileptr);
    }
    panic("Unknown version");
}

Node *readNodeFromBytes(const char *bytes) {
    FILE *stream;
    stream = fmemopen((char *) bytes, -1, "r");
    Node *node = readNodeFile(stream);
    fclose(stream);
    return node;
}