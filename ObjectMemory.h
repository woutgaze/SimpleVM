//
// Created by Wouter on 17-9-2018.
//

#ifndef SIMPLEVM_OBJECTMEMORY_H
#define SIMPLEVM_OBJECTMEMORY_H

#include <stdlib.h>
#include <stdint.h>
#include "ASTNodes.h"

#define NONE 0
#define OBJECT_INDEXED 1
#define BYTE_INDEXED 2

typedef struct Class Class;

typedef struct Object {
    Class *class;
} Object;

typedef struct BytesObject {
    Class *class;
    size_t size;
    char bytes[];
} BytesObject;

typedef struct ObjectTableEntry {
    Object *object;
} ObjectTableEntry;

typedef struct Class {
    Object super;
    ClassNode * classNode;
    Class * superClass;
} Class;

typedef struct {
    ObjectTableEntry *entries;
    size_t used;
    size_t size;
} ObjectTable;

typedef int ObjectPointer;

typedef struct {
    int objectTableSize;
    ObjectPointer nilValue;
    ObjectPointer trueValue;
    ObjectPointer falseValue;
    Class *arrayClass;
    Class *stringClass;
    Class *nilClass;
    size_t nextTableIndex;
    ObjectTable *objectTables;
} ObjectMemory;

void growObjectTable(ObjectTable *objectTable);

ObjectPointer registerObject(ObjectMemory *om, Object *obj);
ObjectPointer registerObjectWithHash(ObjectMemory *om, Object *obj, size_t hash);
ObjectPointer findObjectMatching(ObjectMemory *om, BytesObject *bytes, size_t size, size_t hash);

Object *getObject(ObjectMemory *om, ObjectPointer op);

ObjectPointer registerInt(int value);

int getInt(ObjectPointer p);
bool getBool(ObjectMemory *om, ObjectPointer p);
char * getCString(ObjectMemory *om, ObjectPointer p);

ObjectPointer getInstVar(Object *obj, int instVarIndex);

void setInstVar(Object *obj, int instVarIndex, ObjectPointer a);

ObjectPointer getIndexed(Object *obj, size_t instVarIndex);

void setIndexed(Object *obj, size_t index, ObjectPointer a);

void noCheckSetIndexed(Object *obj, size_t index, size_t instVarSize, ObjectPointer a);

size_t getIndexedSize(Object *obj);

ObjectPointer createObject(ObjectMemory *om, Class *class, ObjectPointer values[], size_t indexedSize);

Object *newObject(Class *class, ObjectPointer values[], size_t indexedSize);

MethodNode *createMethod(const char *selector, Node *node);

Class *createClass(size_t instVarSize, MethodNode **methods, size_t methodsSize, int indexingType);

Class *createClassFromNode(ClassNode *classNode);

ObjectMemory *createObjectMemory();


#endif //SIMPLEVM_OBJECTMEMORY_H
