//
// Created by Wouter on 19-9-2018.
//

#ifndef SIMPLEVM_NODEREADER_V1_TOOLS_H
#define SIMPLEVM_NODEREADER_V1_TOOLS_H

#include <stdio.h>
#include "../ASTNodes.h"

Node * readFailed_V1();

SizedString * readString_V1(FILE *fileptr);
int readInt_V1(FILE *fileptr);

uint16_t readIndex_V1(FILE *fileptr);

Node *readNode_V1(FILE *fileptr);

NodeArray readNodeArray_V1(FILE *fileptr);
ArgumentNodeArray readArgumentNodeArray_V1(FILE *fileptr);
MethodNodeArray readMethodNodeArray_V1(FILE *fileptr);
CompiledMethodNodeArray readCompiledMethodNodeArray_V1(FILE *fileptr);
ByteArray readByteArray_V1(FILE *fileptr);




#endif //SIMPLEVM_NODEREADER_V1_TOOLS_H
