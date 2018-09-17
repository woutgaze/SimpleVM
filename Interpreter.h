//
// Created by Wouter on 17-9-2018.
//

#ifndef SIMPLEVM_INTERPRETER_H
#define SIMPLEVM_INTERPRETER_H

#include <stdlib.h>
#include "ASTNodes.h"
#include "ObjectMemory.h"

typedef struct Frame {
    Object *self;
    ObjectPointer selfp;
    ObjectMemory *objectmemory;
} Frame;

ObjectPointer evaluate(Frame *frame, Node *node);

Method *lookupSelector(Class *class, const char *selector);

ObjectPointer evaluateNode(ObjectMemory *om, ObjectPointer selfp, Node *node);

ObjectPointer perform(ObjectMemory *om, ObjectPointer op, const char *selector);

#endif //SIMPLEVM_INTERPRETER_H
