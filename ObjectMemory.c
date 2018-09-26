#include "ObjectMemory.h"
#include "reader/NodeReader.h"
#include "DevTools.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const unsigned int INTMASK = 1 << ((sizeof(ObjectPointer) * 8) - 1);
const unsigned int INT_SIGN_MASK = 1 << ((sizeof(ObjectPointer) * 8) - 2);
const unsigned int MAX_OBJECT_TABLES = 1 << OBJECT_TABLES_BITS;
const unsigned int OBJECT_TABLES_MASK = (1 << OBJECT_TABLES_BITS) - 1;

void resizeObjectTable(ObjectTable *objectTable, int toAlloc);

void initializeObjectTable(ObjectTable *table);

void resizeObjectTable(ObjectTable *objectTable, int toAlloc) {
    objectTable->size += toAlloc;
    objectTable->entries = (ObjectTableEntry *) realloc(objectTable->entries,
                                                        sizeof(ObjectTableEntry) * objectTable->size);
}

void growObjectTable(ObjectTable *objectTable) {
    if (objectTable->size == 0) {
        resizeObjectTable(objectTable, 100);
        return;
    }
    resizeObjectTable(objectTable, objectTable->size);
}

size_t registerObjectInTable(ObjectTable *objectTable, Object *obj) {
    if (objectTable->used == objectTable->size) growObjectTable(objectTable);
    size_t index = objectTable->used;
    objectTable->entries[index].object = obj;
    objectTable->used++;
    return index;
}

ObjectPointer registerObjectWithTableIndex(ObjectMemory *om, Object *obj, size_t tables_index) {
    size_t entries_index = registerObjectInTable(&om->objectTables[tables_index], obj);

    size_t result = (entries_index << OBJECT_TABLES_BITS) | tables_index;
    return (ObjectPointer) result;
}

ObjectPointer registerObjectWithHash(ObjectMemory *om, Object *obj, size_t hash) {
    return registerObjectWithTableIndex(om, obj, hash & OBJECT_TABLES_MASK);
}

ObjectPointer registerObject(ObjectMemory *om, Object *obj) {
    size_t tables_index = om->nextTableIndex;
    om->nextTableIndex = (om->nextTableIndex + 1) & OBJECT_TABLES_MASK;
    return registerObjectWithTableIndex(om, obj, tables_index);
}

ObjectPointer findObjectMatching(ObjectMemory *om, BytesObject *bytes, size_t size, size_t hash) {
    size_t tables_index = hash & OBJECT_TABLES_MASK;
    ObjectTable table = om->objectTables[tables_index];
    for (size_t i = 0; i < table.used; i++) {
        BytesObject *obj = (BytesObject *) table.entries[i].object;
        if (!memcmp(obj, bytes, size)) return (i << OBJECT_TABLES_BITS) | tables_index;
    }
    return 0;
}

ObjectPointer getBoolValue(ObjectMemory *om, bool test) {
    return test ? om->trueValue : om->falseValue;
}


bool isSmallInteger(ObjectPointer op) {
    return INTMASK & op;
}

Object *getObject(ObjectMemory *om, ObjectPointer op) {
    if (isSmallInteger(op)) {
        panic("ObjectPointer is immediate");
    }
    size_t tables_index = op & OBJECT_TABLES_MASK;
    size_t entries_index = op >> OBJECT_TABLES_BITS;
    return om->objectTables[tables_index].entries[entries_index].object;
}

ObjectPointer registerInt(int value) {
    if (INTMASK & value) {
        panic("Int too large");
    }
    return INTMASK | value;
}

int getInt(ObjectPointer p) {
    if (!(isSmallInteger(p))) {
        panic("ObjectPointer is not immediate int");
    }
    if (INT_SIGN_MASK & p) {
        // Negative int
        return p;
    }
    return INTMASK ^ p;
}

bool getBool(ObjectMemory *om, ObjectPointer p) {
    if (om->trueValue == p) return true;
    if (om->falseValue == p) return false;
    panic("ObjectPointer is not a boolean");
}

uint32_t getIndexedType(Behavior *class) {
    return getInt(class->format) >> 12;
}

uint32_t getInstSize(Behavior *class) {
    return getInt(class->format) & 0xFFF;
}

char *getCString(ObjectMemory *om, ObjectPointer p) {
    Object *obj = getObject(om, p);
    Behavior *class = obj->class;
    uint32_t indexedType = getIndexedType(class);
    if (indexedType != BYTE_INDEXED) {
        panic("Object is not byte indexed");
    }

    size_t instVarsSize = getInstSize(class);
    size_t offset = sizeof(Object) +
                    (sizeof(ObjectPointer) * instVarsSize) + sizeof(size_t);
    char *src = ((char *) obj) + offset;


    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    size_t size = firstSlot[instVarsSize];
    char *string = malloc(size + 1);
    memcpy(string, src, size);
    string[size] = 0;
    return string;
}

ObjectPointer getInstVar(Object *obj, int instVarIndex) {
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    return firstSlot[instVarIndex];
}

void setInstVar(Object *obj, int instVarIndex, ObjectPointer a) {
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    firstSlot[instVarIndex] = a;
}

ObjectPointer getIndexed(Object *obj, size_t index) {
    Behavior *class = obj->class;
    uint32_t indexedType = getIndexedType(class);
    if (!(indexedType == OBJECT_INDEXED)) {
        panic("Not indexable");
    }
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    size_t indexedSize = firstSlot[getInstSize(class)];
    if (index >= indexedSize) {
        panic("Index out of bounds");
    }
    return (firstSlot[getInstSize(class) + 1 + index]);
}

void setIndexed(Object *obj, size_t index, ObjectPointer a) {
    Behavior *class = obj->class;
    if (!(getIndexedType(class) == OBJECT_INDEXED)) {
        panic("Not indexable");
    }
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    size_t indexedSize = firstSlot[getInstSize(class)];
    if (index >= indexedSize) {
        panic("Index out of bounds");
    }
    firstSlot[getInstSize(class) + 1 + index] = a;
}

void noCheckSetIndexed(Object *obj, size_t index, size_t instVarSize, ObjectPointer a) {
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    size_t indexedSize = firstSlot[instVarSize];
    firstSlot[instVarSize + 1 + index] = a;
}

size_t getIndexedSize(Object *obj) {
    Behavior *class = obj->class;
    if ((getIndexedType(class) == NONE)) {
        panic("Not indexable");
    }
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    return firstSlot[getInstSize(class)];
}

Behavior *findClass(ObjectMemory *om, ObjectPointer name) {
    for (int i = 0; i < om->classFormatTable.used; i++) {
        Class *class = (Class *) om->classFormatTable.entries[i].object;
        if (class->name == name) return (Behavior *) class;
    };
    return NULL;
}

void registerClass(ObjectMemory *om, Behavior *class) {
    registerObjectInTable(&om->classFormatTable, (Object *) class);
}

Behavior *createClassNoRegister(ObjectMemory *om, uint32_t indexedType, CompiledClassSideNode *side) {
    if ((indexedType == BYTE_INDEXED) && (side->instVars.size != 0)) {
        panic("Class cannot be both byte indexed and holding instVars");
    }
    Class *class = (Class *) malloc(sizeof(Class));
    class->class = NULL;
    class->superClass = 0;
    class->methodDict = 0;
    class->format = 0;
    class->subclasses = 0;
    class->name = 0;
    getIndexedType(class) = indexedType;
    class->side = side;
    class->name.size = 0;
    class->name.elements = NULL;

    Behavior *class;
    ObjectPointer superClass;
    ObjectPointer methodDict;
    ObjectPointer format;
    ObjectPointer subclasses;
    ObjectPointer name;

    return class;
}


Behavior *createNonMeta(ObjectMemory *om, SizedString name, SizedString superName, uint32_t indexedType,
                     CompiledClassSideNode *side) {
    Behavior *class = createClassNoRegister(om, indexedType, side);
    class->name = name;

    registerClass(om, class);
    class->superClass = findClass(om, superName);

    return class;
}

Behavior *createClassFromNode(ObjectMemory *om, CompiledClassNode *classNode) {

    return createNonMeta(om, classNode->name, classNode->superName, classNode->indexedType, classNode->instSide);

}

ObjectPointer basicNew(ObjectMemory *om, Behavior *class) {
    return basicNew_sz(om, class, 0);
}

ObjectPointer basicNew_sz(ObjectMemory *om, Behavior *class, size_t indexedSize) {
    size_t instVarSize = getInstSize(class);

    size_t varSize = 0;
    uint32_t indexedType = getIndexedType(class);
    if ((indexedType == OBJECT_INDEXED)) {
        varSize = sizeof(size_t) + (sizeof(ObjectPointer) * indexedSize);
    } else if ((indexedType == BYTE_INDEXED)) {
        varSize = sizeof(size_t) + indexedSize;

    }
    Object *obj = (Object *) calloc(1, sizeof(Object) +
                                       (sizeof(ObjectPointer) * instVarSize) +
                                       varSize);
    obj->class = class;
    for (int i = 0; i < instVarSize; i++) {
        setInstVar(obj, i, om->nilValue);
    }
    if ((indexedType != NONE)) {
        ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
        (firstSlot[instVarSize]) = indexedSize;
    }
    return registerObject(om, obj);
}

void loadClassIfAbsent(ObjectMemory *om, const char *className, const char *bytes) {
    Behavior *current = findClass(om, getSizedString(className));
    if (current == NULL) {
        createClassFromNode(om, (CompiledClassNode *) readNodeFromBytes(bytes));
    }
}

void initializeObjectTable(ObjectTable *table) {
    table->entries = NULL;
    table->used = 0;
    table->size = 0;
}

ObjectMemory *newObjectMemory() {
    ObjectMemory *om = (ObjectMemory *) malloc(sizeof(ObjectMemory));
    om->nextTableIndex = 0;
    om->objectTables = malloc(sizeof(ObjectTable) * MAX_OBJECT_TABLES);
    for (int i = 0; i < MAX_OBJECT_TABLES; i++) {
        initializeObjectTable(&om->objectTables[i]);
    }
    initializeObjectTable(&om->classFormatTable);
    return om;
}