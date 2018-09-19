//
// Created by Wouter on 19-9-2018.
//

#ifndef SIMPLEVM_NODEREADER_H
#define SIMPLEVM_NODEREADER_H

#include <stdio.h>
#include "../ASTNodes.h"

Node *readNodeFile(FILE *fileptr);
Node *readNodeFromBytes(char * bytes);


#endif //SIMPLEVM_NODEREADER_H
