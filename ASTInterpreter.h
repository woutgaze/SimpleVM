//
// Created by Wouter on 17-9-2018.
//
#ifndef SIMPLEVM_INTERPRETER_H
#define SIMPLEVM_INTERPRETER_H

#include <stdlib.h>
#include "ASTNodes.h"
#include "ObjectMemory.h"

typedef struct ASTFrame {
    Object *self;
    ObjectPointer selfp;
    ObjectMemory *objectmemory;
    ObjectPointer *arguments;
    ObjectPointer *temps;
} ASTFrame;

ObjectPointer evaluate(ASTFrame *frame, Node *node);

MethodNode *lookupASTSelector(Class *class, SizedString *selector);

ObjectPointer ast_perform(ObjectMemory *om, ObjectPointer op, char *selector, ObjectPointer *arguments);
ObjectPointer ast_perform_sym(ObjectMemory *om, ObjectPointer selfp, SizedString *selector, ObjectPointer *arguments);

#endif //SIMPLEVM_INTERPRETER_H