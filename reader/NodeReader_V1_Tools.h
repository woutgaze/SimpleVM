//
// Created by Wouter on 19-9-2018.
//

#ifndef SIMPLEVM_NODEREADER_V1_TOOLS_H
#define SIMPLEVM_NODEREADER_V1_TOOLS_H

#include <stdio.h>
#include "../ASTNodes.h"

Node * readFailed_V1();

char * readString_V1(FILE *fileptr);
bool readBool_V1(FILE *fileptr);
int readInt_V1(FILE *fileptr);

uint16_t readIndex_V1(FILE *fileptr);

Node *readNode_V1(FILE *fileptr);

NodeArray readNodeArray_V1(FILE *fileptr);



#endif //SIMPLEVM_NODEREADER_V1_TOOLS_H
