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

typedef struct Behavior Behavior;
typedef uint32_t ObjectPointer;

typedef struct Object {
    Behavior *class;
} Object;

typedef struct BytesObject {
    Behavior *class;
    size_t size;
    char bytes[];
} BytesObject;

typedef struct ObjectTableEntry {
    Object *object;
} ObjectTableEntry;

typedef struct Behavior {
    Behavior *class;
    ObjectPointer superClass;
    ObjectPointer methodDict;
    ObjectPointer format;
    ObjectPointer subclasses;
} Behavior;

typedef struct Class {
    Behavior *class;
    ObjectPointer superClass;
    ObjectPointer methodDict;
    ObjectPointer format;
    ObjectPointer subclasses;
    ObjectPointer name;
} Class;

typedef struct CompiledMethod {
    Behavior *class;
    ObjectPointer argumentsSize;
    ObjectPointer temporariesSize;
    ObjectPointer maxStackDepth;
    ObjectPointer instructionsSize;
    ObjectPointer bytecode;
} CompiledMethod;

typedef struct {
    size_t used;
    size_t size;
    ObjectTableEntry *entries;
} ObjectTable;


typedef struct {
    int objectTableSize;
    ObjectPointer nilValue;
    ObjectPointer trueValue;
    ObjectPointer falseValue;
    Behavior *nilClass;
    Behavior *arrayClass;
    Behavior *stringClass;
    Behavior *bytearrayClass;
    Behavior *smallintegerClass;
    size_t nextTableIndex;
    ObjectTable classFormatTable;
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

ObjectPointer basicNew(ObjectMemory *om, Behavior *class);

ObjectPointer basicNew_sz(ObjectMemory *om, Behavior *class, size_t indexedSize);

Behavior *findClass(ObjectMemory *om, ObjectPointer name);

Behavior *createNonMeta(ObjectMemory *om, SizedString name, SizedString superName, uint32_t indexedType,
                     CompiledClassSideNode *side);

Behavior *createClassFromNode(ObjectMemory *om, CompiledClassNode *classNode);

void loadClassIfAbsent(ObjectMemory *om, const char *className, const char *bytes);

Behavior *createClassNoRegister(ObjectMemory *om, uint32_t indexedType, CompiledClassSideNode *side);

void registerClass(ObjectMemory *om, Behavior *class);

ObjectMemory *newObjectMemory();

#endif //SIMPLEVM_OBJECTMEMORY_H
