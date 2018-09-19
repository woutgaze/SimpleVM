#include "ObjectMemory.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static const int INTMASK = 1 << ((sizeof(int) * 8) - 1);
static const int MAX_OBJECT_TABLES = 1 << OBJECT_TABLES_BITS;
static const int OBJECT_TABLES_MASK = (1 << OBJECT_TABLES_BITS) - 1;


Class *createClassFromNode(ClassNode *classNode);

void resizeObjectTable(ObjectTable *objectTable, int toAlloc);

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
    return result;
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
        BytesObject *obj = table.entries[i].object;
        if (!memcmp(obj, bytes, size)) return (i << OBJECT_TABLES_BITS) | tables_index;
    }
    return 0;
}


Object *getObject(ObjectMemory *om, ObjectPointer op) {
    if (INTMASK & op) {
        fprintf(stderr, "ObjectPointer is immediate.\n");
        exit(-1);
    }
    size_t tables_index = op & OBJECT_TABLES_MASK;
    size_t entries_index = op >> OBJECT_TABLES_BITS;
    return om->objectTables[tables_index].entries[entries_index].object;
}

ObjectPointer registerInt(int value) {
    if (INTMASK & value) {
        fprintf(stderr, "Int too large.\n");
        exit(-1);
    }
    return INTMASK | value;
}

int getInt(ObjectPointer p) {
    if (!(INTMASK & p)) {
        fprintf(stderr, "ObjectPointer is not immediate int.\n");
        exit(-1);
    }
    return INTMASK ^ p;
}

bool getBool(ObjectMemory *om, ObjectPointer p) {
    if (om->trueValue == p) return true;
    if (om->falseValue == p) return false;
    fprintf(stderr, "ObjectPointer is not a boolean.\n");
    exit(-1);
}

char *getCString(ObjectMemory *om, ObjectPointer p) {
    Object *obj = getObject(om, p);
    Class *class = obj->class;
    uint32_t indexedType = class->classNode->indexedType;
    if (!(indexedType == BYTE_INDEXED)) {
        fprintf(stderr, "Object is not byte indexed.\n");
        exit(-1);
    }

    size_t instVarsSize = class->classNode->instSide->instVars.size;
    size_t offset = sizeof(Object) +
                    (sizeof(ObjectPointer) * instVarsSize) + sizeof(size_t);
    char *src = ((char *) obj) + offset;


    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    size_t size = firstSlot[class->classNode->instSide->instVars.size];
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
    Class *class = obj->class;
    uint32_t indexedType = class->classNode->indexedType;
    if (!(indexedType == OBJECT_INDEXED)) {
        fprintf(stderr, "Not indexable.\n");
        exit(-1);
    }
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    size_t indexedSize = firstSlot[class->classNode->instSide->instVars.size];
    if (index >= indexedSize) {
        fprintf(stderr, "Index out of bounds.\n");
        exit(-1);
    }
    return (firstSlot[class->classNode->instSide->instVars.size + 1 + index]);
}

void setIndexed(Object *obj, size_t index, ObjectPointer a) {
    Class *class = obj->class;
    if (!(class->classNode->indexedType == OBJECT_INDEXED)) {
        fprintf(stderr, "Not indexable.\n");
        exit(-1);
    }
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    size_t indexedSize = firstSlot[class->classNode->instSide->instVars.size];
    if (index >= indexedSize) {
        fprintf(stderr, "Index out of bounds.\n");
        exit(-1);
    }
    firstSlot[class->classNode->instSide->instVars.size + 1 + index] = a;
}

void noCheckSetIndexed(Object *obj, size_t index, size_t instVarSize, ObjectPointer a) {
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    size_t indexedSize = firstSlot[instVarSize];
    firstSlot[instVarSize + 1 + index] = a;
}

size_t getIndexedSize(Object *obj) {
    Class *class = obj->class;
    if ((class->classNode->indexedType == NONE)) {
        fprintf(stderr, "Not indexable.\n");
        exit(-1);
    }
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    return firstSlot[class->classNode->instSide->instVars.size];
}

MethodNode *createMethod(const char *selector, Node *node) {
    return (MethodNode *) newMethod(selector, (BlockNode *) newBlock(NULL, 0, NULL, 0, node));
}

Class *createClassFromNode(ClassNode *classNode) {
    Class *class = (Class *) malloc(sizeof(Class));
    class->super.class = NULL;
    class->classNode = classNode;
    return class;
}

Class *createClass(size_t instVarSize, MethodNode **methods, size_t methodsSize, int indexingType) {
    ArgumentNode *instVars[] = {newArgument("t1"), newArgument("t2"), newArgument("t3")};
    ClassSideNode *instSide = (ClassSideNode *) newClassSide(instVars, instVarSize, methods, methodsSize);
    ClassSideNode *classSide = (ClassSideNode *) newClassSide(NULL, 0, NULL, 0);
    ClassNode *classNode = (ClassNode *) newClass("Test", "", indexingType, instSide, classSide);
    return createClassFromNode(classNode);
}

ObjectPointer createObject(ObjectMemory *om, Class *class, ObjectPointer values[], size_t indexedSize) {
    Object *obj = newObject(class ? class : om->nilClass, values, indexedSize);
    return registerObject(om, obj);
}

Object *newObject(Class *class, ObjectPointer values[], size_t indexedSize) {
    size_t instVarSize = class->classNode->instSide->instVars.size;

    size_t varSize = 0;
    uint32_t indexedType = class->classNode->indexedType;
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
        setInstVar(obj, i, values[i]);
    }
    if ((indexedType != NONE)) {
        ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
        (firstSlot[instVarSize]) = indexedSize;
    }
    return obj;
}

ObjectMemory *createObjectMemory() {
    ObjectMemory *om = (ObjectMemory *) malloc(sizeof(ObjectMemory));
    om->nextTableIndex = 0;
    om->objectTables = malloc(sizeof(ObjectTable) * MAX_OBJECT_TABLES);
    for (int i = 0; i < MAX_OBJECT_TABLES; i++) {
        om->objectTables[i].entries = NULL;
        om->objectTables[i].used = 0;
        om->objectTables[i].size = 0;
    }
    om->nilClass = createClass(0, NULL, 0, NONE);
    om->nilValue = createObject(om, om->nilClass, NULL, 0);
    om->trueValue = createObject(om, NULL, NULL, 0);
    om->falseValue = createObject(om, NULL, NULL, 0);
    om->arrayClass = createClass(0, NULL, 0, OBJECT_INDEXED);
    om->stringClass = createClass(0, NULL, 0, BYTE_INDEXED);
    return om;
}
