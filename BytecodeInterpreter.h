//
// Created by Wouter on 24-9-2018.
//

#ifndef SIMPLEVM_BYTECODEINTERPRETER_H
#define SIMPLEVM_BYTECODEINTERPRETER_H

#include "ObjectMemory.h"

typedef struct Frame Frame;
typedef struct Process Process;

typedef struct Frame {
    Process *process;
    Frame * sender;
    Object *self;
    size_t ip;
    size_t sp;
    CompiledCodeNode *code;
    ObjectPointer selfp;
    ObjectPointer **arguments;
    ObjectPointer **temps;
    ObjectPointer **stack;
} Frame;

typedef struct Process {
    ObjectMemory *objectmemory;
    Frame *currentFrame;
    ObjectPointer returnValue;
} Process;

ObjectPointer perform(ObjectMemory *om, ObjectPointer selfp, SizedString selector, ObjectPointer **arguments);


#endif //SIMPLEVM_BYTECODEINTERPRETER_H
