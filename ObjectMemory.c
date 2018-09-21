#include "ObjectMemory.h"
#include "reader/NodeReader.h"
#include "DevTools.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static const int INTMASK = 1 << ((sizeof(int) * 8) - 1);
static const int MAX_OBJECT_TABLES = 1 << OBJECT_TABLES_BITS;
static const int OBJECT_TABLES_MASK = (1 << OBJECT_TABLES_BITS) - 1;


Class *createClassFromNode(ObjectMemory *om, ClassNode *classNode);

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
    if (!(indexedType == BYTE_INDEXED)) {
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

MethodNode *createMethod(const char *selector, Node *node) {
    return (MethodNode *) newMethod(selector, (BlockNode *) newBlock(NULL, 0, NULL, 0, node));
}

Class *createClassNoRegister(ObjectMemory *om, ClassNode *classNode) {
    if ((classNode->indexedType == BYTE_INDEXED) && (classNode->instSide->instVars.size != 0)) {
        panic("Class cannot be both byte indexed and holding instVars");
    }
    Class *class = (Class *) malloc(sizeof(Class));
    class->super.class = NULL;
    class->classNode = classNode;
    return class;
}

Class *findClass(ObjectMemory *om, const char *name) {
    for (int i = 0; i < om->classTable.used; i++) {
        Class *class = (Class *) om->classTable.entries[i].object;
        if (strcmp(class->classNode->name, name) == 0) return class;
    };
    return NULL;
}

void registerClass(ObjectMemory *om, Class *class) {
    registerObject(om, (Object *) class);
    registerObjectInTable(&om->classTable, (Object *) class);
    class->superClass = findClass(om, class->classNode->superName);
}

Class *createClassFromNode(ObjectMemory *om, ClassNode *classNode) {
    Class *class = createClassNoRegister(om, classNode);
    registerClass(om, class);
    return class;
}

Class *createClass(ObjectMemory *om, size_t instVarSize, MethodNode **methods, size_t methodsSize, int indexingType) {
    ArgumentNode *instVars[] = {(ArgumentNode *) newArgument("t1"), (ArgumentNode *) newArgument("t2"),
                                (ArgumentNode *) newArgument("t3")};
    ClassSideNode *instSide = (ClassSideNode *) newClassSide(instVars, instVarSize, methods, methodsSize);
    ClassSideNode *classSide = (ClassSideNode *) newClassSide(NULL, 0, NULL, 0);
    ClassNode *classNode = (ClassNode *) newClass("Test", "", indexingType, instSide, classSide);
    return createClassFromNode(om, classNode);
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

ClassNode *newUndefinedObjectClass() {
    char bytes[] = { 83, 86, 1, 28, 15, 0, 85, 110, 100, 101, 102, 105, 110, 101, 100, 79, 98, 106, 101, 99, 116, 6, 0, 79, 98, 106, 101, 99, 116, 0, 0, 27, 0, 0, 1, 0, 23, 5, 0, 105, 115, 78, 105, 108, 24, 0, 0, 0, 0, 14, 1, 0, 7, 36, 27, 0, 0, 0, 0 };
    return (ClassNode *) readNodeFromBytes(bytes);
}

ClassNode *newObjectClass() {
    char bytes[] = { 83, 86, 1, 28, 6, 0, 79, 98, 106, 101, 99, 116, 0, 0, 0, 0, 27, 0, 0, 1, 0, 23, 5, 0, 105, 115, 78, 105, 108, 24, 0, 0, 0, 0, 14, 1, 0, 7, 10, 27, 0, 0, 0, 0 };
    return (ClassNode *) readNodeFromBytes(bytes);
}

ClassNode *newEmptyClass(const char *name, const char *superName, uint32_t indexedType) {
    return (ClassNode *) newClass(name, superName, indexedType, (ClassSideNode *) newClassSide(NULL, 0, NULL, 0),
                                  (ClassSideNode *) newClassSide(NULL, 0, NULL, 0));
}

ObjectMemory *createObjectMemory() {
    ObjectMemory *om = (ObjectMemory *) malloc(sizeof(ObjectMemory));
    om->nextTableIndex = 0;
    om->objectTables = malloc(sizeof(ObjectTable) * MAX_OBJECT_TABLES);
    for (int i = 0; i < MAX_OBJECT_TABLES; i++) {
        initializeObjectTable(&om->objectTables[i]);
    }
    initializeObjectTable(&om->classTable);
    Class *objClass = createClassNoRegister(om, newObjectClass());
    om->nilClass = createClassNoRegister(om, newUndefinedObjectClass());
    om->nilValue = createObject(om, om->nilClass, NULL, 0);
    registerClass(om, objClass);
    registerClass(om, om->nilClass);
    createClassFromNode(om, newEmptyClass("Boolean", "Object", NONE));
    Class *trueClass = createClassFromNode(om, newEmptyClass("True", "Boolean", NONE));
    Class *falseClass = createClassFromNode(om, newEmptyClass("False", "Boolean", NONE));
    om->trueValue = createObject(om, trueClass, NULL, 0);
    om->falseValue = createObject(om, falseClass, NULL, 0);

    createClassFromNode(om, newEmptyClass("Collection", "Object", NONE));
    createClassFromNode(om, newEmptyClass("SequenceableCollection", "Collection", NONE));
    createClassFromNode(om, newEmptyClass("ArrayedCollection", "SequenceableCollection", NONE));
    om->arrayClass = createClassFromNode(om, newEmptyClass("Array", "ArrayedCollection", OBJECT_INDEXED));
    om->stringClass = createClassFromNode(om, newEmptyClass("String", "ArrayedCollection", BYTE_INDEXED));
    om->bytearrayClass = createClassFromNode(om, newEmptyClass("ByteArray", "ArrayedCollection", BYTE_INDEXED));
    createClassFromNode(om, newEmptyClass("Magnitude", "Object", NONE));
    createClassFromNode(om, newEmptyClass("ArithmeticValue", "Magnitude", NONE));
    createClassFromNode(om, newEmptyClass("Number", "ArithmeticValue", NONE));
    createClassFromNode(om, newEmptyClass("Integer", "ArithmeticValue", NONE));
    om->smallintegerClass = createClassFromNode(om, newEmptyClass("SmallInteger", "Integer", NONE));

    return om;
}

void initializeObjectTable(ObjectTable *table) {
    table->entries = NULL;
    table->used = 0;
    table->size = 0;
}
