//
// Created by Wouter on 17-9-2018.
//

#ifndef SIMPLEVM_OBJECTMEMORY_H
#define SIMPLEVM_OBJECTMEMORY_H

#include <stdlib.h>
#include <stdint.h>
#include "ASTNodes.h"

typedef struct Class Class;

typedef struct Object {
    Class *class;
} Object;

typedef struct ObjectTableEntry {
    Object *object;
} ObjectTableEntry;

typedef struct {
    const char *name;
    Node *node;
    int numArgs;
} Method;

typedef struct Class {
    Object super;
    Class *superClass;
    size_t instVarSize;
    Method **methods;
    size_t methodsSize;
} Class;

typedef struct {
    ObjectTableEntry *entries;
    size_t used;
    size_t size;
} ObjectTable;

typedef int ObjectPointer;

typedef struct {
    ObjectTable objectTable;
    int objectTableSize;
    ObjectPointer nilValue;
    ObjectPointer trueValue;
    ObjectPointer falseValue;
} ObjectMemory;

void resizeObjectTable(ObjectMemory *om, int toAlloc);

void growObjectTable(ObjectMemory *om);

ObjectPointer registerObject(ObjectMemory *om, Object *obj);

Object *getObject(ObjectMemory *om, ObjectPointer op);

ObjectPointer registerInt(int value);

int getInt(ObjectPointer p);
bool getBool(ObjectMemory * om , ObjectPointer p);

void setInstVar(Object *obj, int instVarIndex, ObjectPointer a);

ObjectPointer getInstVar(Object *obj, int instVarIndex);

ObjectPointer createObject(ObjectMemory *om, Class *class, int instVarCount, ObjectPointer values[]);

Method *createMethod(const char *selector, Node *node, int numArgs);

Class *createClass(size_t instVarSize, Method *methods[1], size_t methodsSize);

ObjectMemory *createObjectMemory();


#endif //SIMPLEVM_OBJECTMEMORY_H
