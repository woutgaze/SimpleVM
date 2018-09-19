//
// Created by Wouter on 19-9-2018.
//

#include <stdio.h>
#include <stdlib.h>
#include "NodeReader.h"
#include "../ASTNodes.h"
#include "NodeReader_V1.h"

void readFailed() {
    fprintf(stderr, "Read failed.\n");
    exit(-1);
}

Node *readNodeFile(FILE *fileptr) {
    if (getc(fileptr) != 'S' || getc(fileptr) != 'V') {
        fprintf(stderr, "Unknown header.\n");
        exit(-1);
    }
    char version = getc(fileptr);
    if (version == 1) {
        return readNode_V1(fileptr);
    }
    fprintf(stderr, "Unknown version.\n");
    exit(-1);
}