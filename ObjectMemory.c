#include "ObjectMemory.h"
#include <stdio.h>
#include <stdbool.h>

const int INTMASK = 1 << ((sizeof(int) * 8) - 1);

void resizeObjectTable(ObjectMemory *om, int toAlloc) {
//    printf("resizeObjectTable: %d\n", toAlloc);
    om->objectTable.size += toAlloc;
    om->objectTable.entries = (ObjectTableEntry *) realloc(om->objectTable.entries,
                                                           sizeof(ObjectTableEntry) * om->objectTable.size);
}

void growObjectTable(ObjectMemory *om) {
    if (om->objectTable.size == 0) {
        resizeObjectTable(om, 100);
        return;
    }
    resizeObjectTable(om, om->objectTable.size);
}

ObjectPointer registerObject(ObjectMemory *om, Object *obj) {
    if (om->objectTable.used == om->objectTable.size) growObjectTable(om);
    size_t index = om->objectTable.used;
    om->objectTable.entries[index].object = obj;
    om->objectTable.used++;
    return index;
}

Object *getObject(ObjectMemory *om, ObjectPointer op) {
    if (INTMASK & op) {
        fprintf(stderr, "ObjectPointer is immediate.\n");
        exit(-1);
    }
    return om->objectTable.entries[op].object;
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
    if (!(class->indexingType == OBJECT_INDEXED)) {
        fprintf(stderr, "Not indexable.\n");
        exit(-1);
    }
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    size_t indexedSize = firstSlot[class->instVarSize];
    if (index >= indexedSize) {
        fprintf(stderr, "Index out of bounds.\n");
        exit(-1);
    }
    return (firstSlot[class->instVarSize + 1 + index]);
}

void setIndexed(Object *obj, size_t index, ObjectPointer a) {
    Class *class = obj->class;
    if (!(class->indexingType == OBJECT_INDEXED)) {
        fprintf(stderr, "Not indexable.\n");
        exit(-1);
    }
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    size_t indexedSize = firstSlot[class->instVarSize];
    if (index >= indexedSize) {
        fprintf(stderr, "Index out of bounds.\n");
        exit(-1);
    }
    firstSlot[class->instVarSize + 1 + index] = a;
}

void noCheckSetIndexed(Object *obj, size_t index, size_t instVarSize, ObjectPointer a) {
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    size_t indexedSize = firstSlot[instVarSize];
    firstSlot[instVarSize + 1 + index] = a;
}

size_t getIndexedSize(Object *obj) {
    Class *class = obj->class;
    if (!(class->indexingType == OBJECT_INDEXED)) {
        fprintf(stderr, "Not indexable.\n");
        exit(-1);
    }
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    return firstSlot[class->instVarSize];
}

Method *createMethod(const char *selector, Node *node, int numArgs) {
    Method *method = (Method *) malloc(sizeof(Method));

    method->name = selector;
    method->node = node;
    method->numArgs = numArgs;
    return method;
}

Class *createClass(size_t instVarSize, Method *methods[1], size_t methodsSize, int indexingType) {
    Class *class = (Class *) malloc(sizeof(Class));
    class->super.class = NULL;
    class->superClass = NULL;
    class->instVarSize = instVarSize;
    class->methodsSize = methodsSize;
    class->methods = (Method **) malloc(sizeof(Method **) * methodsSize);
    class->indexingType = indexingType;
    for (int i = 0; i < methodsSize; i++) {
        class->methods[i] = methods[i];
    }
    return class;
}

ObjectPointer createObject(ObjectMemory *om, Class *class, ObjectPointer values[], size_t indexedSize) {
    Object *obj = newObject(class, values, indexedSize);
    return registerObject(om, obj);
}

Object *newObject(Class *class, ObjectPointer values[], size_t indexedSize) {
    size_t instVarSize = class ? class->instVarSize : 0;
    size_t varSize = 0;
    if (class && (class->indexingType == OBJECT_INDEXED)) {
        varSize = sizeof(size_t) + (sizeof(ObjectPointer) * indexedSize);
    } else if (class && (class->indexingType == BYTE_INDEXED)) {
        varSize = sizeof(size_t) + indexedSize;

    }
    Object *obj = (Object *) calloc(1, sizeof(Object) +
                                       (sizeof(ObjectPointer) * instVarSize) +
                                       varSize);
    obj->class = class;
    for (int i = 0; i < instVarSize; i++) {
        setInstVar(obj, i, values[i]);
    }
    if (class && (class->indexingType != NONE)) {
        ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
        (firstSlot[instVarSize]) = indexedSize;
    }
    return obj;
}

ObjectMemory *createObjectMemory() {
    ObjectMemory *om = (ObjectMemory *) malloc(sizeof(ObjectMemory));
    om->objectTable.entries = NULL;
    om->objectTable.used = 0;
    om->objectTable.size = 0;
    om->nilValue = createObject(om, NULL, NULL, 0);
    om->trueValue = createObject(om, NULL, NULL, 0);
    om->falseValue = createObject(om, NULL, NULL, 0);
    om->arrayClass = createClass(0, NULL, 0, true);
    return om;
}
