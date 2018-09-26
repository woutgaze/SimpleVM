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

typedef struct ClassFormat ClassFormat;
typedef unsigned int ObjectPointer;

typedef struct Object {
    ClassFormat *class;
} Object;

typedef struct BytesObject {
    ClassFormat *class;
    size_t size;
    char bytes[];
} BytesObject;

typedef struct ObjectTableEntry {
    Object *object;
} ObjectTableEntry;

typedef struct ClassFormat {
    ClassFormat *superClass;
    ObjectPointer *object;
    SizedString name;
    uint32_t indexedType;
    CompiledClassSideNode *side;
} ClassFormat;

typedef struct {
    ObjectTableEntry *entries;
    size_t used;
    size_t size;
} ObjectTable;


typedef struct {
    int objectTableSize;
    ObjectPointer nilValue;
    ObjectPointer trueValue;
    ObjectPointer falseValue;
    ClassFormat *nilClass;
    ClassFormat *arrayClass;
    ClassFormat *stringClass;
    ClassFormat *bytearrayClass;
    ClassFormat *smallintegerClass;
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

ObjectPointer basicNew(ObjectMemory *om, ClassFormat *class);

ObjectPointer basicNew_sz(ObjectMemory *om, ClassFormat *class, size_t indexedSize);

ClassFormat *findClass(ObjectMemory *om, SizedString name);

ClassFormat *createNonMeta(ObjectMemory *om, SizedString name, SizedString superName, uint32_t indexedType,
                     CompiledClassSideNode *side);

ClassFormat *createClassFromNode(ObjectMemory *om, CompiledClassNode *classNode);

void loadClassIfAbsent(ObjectMemory *om, const char *className, const char *bytes);

ClassFormat *createClassNoRegister(ObjectMemory *om, uint32_t indexedType, CompiledClassSideNode *side);

void registerClass(ObjectMemory *om, ClassFormat *class);

ObjectMemory *newObjectMemory();

#endif //SIMPLEVM_OBJECTMEMORY_H
