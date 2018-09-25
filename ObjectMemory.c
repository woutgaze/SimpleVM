#include "ObjectMemory.h"
#include "reader/NodeReader.h"
#include "DevTools.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static const unsigned int INTMASK = 1 << ((sizeof(ObjectPointer) * 8) - 1);
static const unsigned int INT_SIGN_MASK = 1 << ((sizeof(ObjectPointer) * 8) - 2);
static const unsigned int MAX_OBJECT_TABLES = 1 << OBJECT_TABLES_BITS;
static const unsigned int OBJECT_TABLES_MASK = (1 << OBJECT_TABLES_BITS) - 1;

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


Object *getObject(ObjectMemory *om, ObjectPointer op) {
    if (INTMASK & op) {
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
    if (!(INTMASK & p)) {
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

char *getCString(ObjectMemory *om, ObjectPointer p) {
    Object *obj = getObject(om, p);
    Class *class = obj->class;
    uint32_t indexedType = class->classNode->indexedType;
    if (indexedType != BYTE_INDEXED) {
        panic("Object is not byte indexed");
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
        panic("Not indexable");
    }
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    size_t indexedSize = firstSlot[class->classNode->instSide->instVars.size];
    if (index >= indexedSize) {
        panic("Index out of bounds");
    }
    return (firstSlot[class->classNode->instSide->instVars.size + 1 + index]);
}

void setIndexed(Object *obj, size_t index, ObjectPointer a) {
    Class *class = obj->class;
    if (!(class->classNode->indexedType == OBJECT_INDEXED)) {
        panic("Not indexable");
    }
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    size_t indexedSize = firstSlot[class->classNode->instSide->instVars.size];
    if (index >= indexedSize) {
        panic("Index out of bounds");
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
        panic("Not indexable");
    }
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    return firstSlot[class->classNode->instSide->instVars.size];
}

Class *findClass(ObjectMemory *om, SizedString name) {
    for (int i = 0; i < om->classTable.used; i++) {
        Class *class = (Class *) om->classTable.entries[i].object;
        if (sstrcmp(class->classNode->name, name) == 0) return class;
    };
    return NULL;
}

void registerClass(ObjectMemory *om, Class *class) {
    registerObject(om, (Object *) class);
    registerObjectInTable(&om->classTable, (Object *) class);
    class->superClass = findClass(om, class->classNode->superName);
}

Class *createClassNoRegister(ObjectMemory *om, CompiledClassNode *classNode) {
    if ((classNode->indexedType == BYTE_INDEXED) && (classNode->instSide->instVars.size != 0)) {
        panic("Class cannot be both byte indexed and holding instVars");
    }
    Class *class = (Class *) malloc(sizeof(Class));
    class->super.class = NULL;
    class->classNode = classNode;
    return class;
}

Class *createClassFromNode(ObjectMemory *om, CompiledClassNode *classNode) {
    Class *class = createClassNoRegister(om, classNode);
    registerClass(om, class);
    return class;
}

Class *createClass(ObjectMemory *om, size_t instVarSize, CompiledMethodNode **methods, size_t methodsSize,
                   int indexingType) {
    ArgumentNode *instVars[] = {(ArgumentNode *) newArgument(getSizedString("t1")),
                                (ArgumentNode *) newArgument(getSizedString("t2")),
                                (ArgumentNode *) newArgument(getSizedString("t3"))};
    CompiledClassSideNode *instSide = (CompiledClassSideNode *) newCompiledClassSide(instVars, instVarSize, methods,
                                                                                     methodsSize);
    CompiledClassSideNode *classSide = (CompiledClassSideNode *) newCompiledClassSide(NULL, 0, NULL, 0);
    CompiledClassNode *classNode = (CompiledClassNode *) newCompiledClass(getSizedString("Test"),
                                                                          getSizedString("Object"), indexingType,
                                                                          instSide,
                                                                          classSide);
    return createClassFromNode(om, classNode);
}

ObjectPointer basicNew(ObjectMemory *om, Class *class) {
    return basicNew_sz(om, class, 0);
}

ObjectPointer basicNew_sz(ObjectMemory *om, Class *class, size_t indexedSize) {
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
        setInstVar(obj, i, om->nilValue);
    }
    if ((indexedType != NONE)) {
        ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
        (firstSlot[instVarSize]) = indexedSize;
    }
    return registerObject(om, obj);
}

void loadClassIfAbsent(ObjectMemory *om, const char *className, const char *bytes) {
    Class *current = findClass(om, getSizedString(className));
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
    initializeObjectTable(&om->classTable);
    return om;
}