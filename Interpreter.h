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
    ObjectPointer *arguments;
    ObjectPointer *temps;
} Frame;

ObjectPointer evaluate(Frame *frame, Node *node);

MethodNode *lookupSelector(Class *class, const char *selector);

ObjectPointer perform(ObjectMemory *om, ObjectPointer op, const char *selector, ObjectPointer *arguments);

#endif //SIMPLEVM_INTERPRETER_H