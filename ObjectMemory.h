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
#define OBJECT_TABLES_BITS 8

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
    CompiledClassNode *classNode;
    Class *superClass;
} Class;

typedef struct {
    ObjectTableEntry *entries;
    size_t used;
    size_t size;
} ObjectTable;

typedef unsigned int ObjectPointer;

typedef struct {
    int objectTableSize;
    ObjectPointer nilValue;
    ObjectPointer trueValue;
    ObjectPointer falseValue;
    Class *nilClass;
    Class *arrayClass;
    Class *stringClass;
    Class *bytearrayClass;
    Class *smallintegerClass;
    size_t nextTableIndex;
    ObjectTable classTable;
    ObjectTable *objectTables;
} ObjectMemory;

void growObjectTable(ObjectTable *objectTable);

ObjectPointer registerObject(ObjectMemory *om, Object *obj);

ObjectPointer registerObjectWithHash(ObjectMemory *om, Object *obj, size_t hash);

ObjectPointer findObjectMatching(ObjectMemory *om, BytesObject *bytes, size_t size, size_t hash);

Object *getObject(ObjectMemory *om, ObjectPointer op);

ObjectPointer registerInt(int value);

int getInt(ObjectPointer p);
bool isSmallInteger(ObjectPointer op);

bool getBool(ObjectMemory *om, ObjectPointer p);

char *getCString(ObjectMemory *om, ObjectPointer p);

ObjectPointer getBoolValue(ObjectMemory *om, bool test);

ObjectPointer getInstVar(Object *obj, int instVarIndex);

void setInstVar(Object *obj, int instVarIndex, ObjectPointer a);

ObjectPointer getIndexed(Object *obj, size_t instVarIndex);

void setIndexed(Object *obj, size_t index, ObjectPointer a);

void noCheckSetIndexed(Object *obj, size_t index, size_t instVarSize, ObjectPointer a);

size_t getIndexedSize(Object *obj);

ObjectPointer basicNew(ObjectMemory *om, Class *class);

ObjectPointer basicNew_sz(ObjectMemory *om, Class *class, size_t indexedSize);

Class *findClass(ObjectMemory *om, SizedString name);

Class *createClass(ObjectMemory *om, size_t instVarSize, CompiledMethodNode **methods, size_t methodsSize,
                   int indexingType);

Class *createClassFromNode(ObjectMemory *om, CompiledClassNode *classNode);

void loadClassIfAbsent(ObjectMemory *om, const char *className, const char *bytes);

Class *createClassNoRegister(ObjectMemory *om, CompiledClassNode *classNode);

void registerClass(ObjectMemory *om, Class *class);

ObjectMemory *newObjectMemory();

#endif //SIMPLEVM_OBJECTMEMORY_H
