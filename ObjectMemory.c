#include "ObjectMemory.h"
#include <stdio.h>

const int INTMASK = 1 << ((sizeof(int) * 8) - 1);

void resizeObjectTable(ObjectMemory *om, int toAlloc) {
    printf("resizeObjectTable: %d\n", toAlloc);
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

void setInstVar(Object *obj, int instVarIndex, ObjectPointer a) {
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    firstSlot[instVarIndex] = a;
}

Method *createMethod(const char *selector, Node *node) {
    Method *method = (Method *) malloc(sizeof(Method));

    method->name = selector;
    method->node = node;
    return method;
}

Class *createClass(size_t instVarSize, size_t methodsSize, Method *methods[1]) {
    Class *class = (Class *) malloc(sizeof(Class));
    class->super.class = NULL;
    class->superClass = NULL;
    class->instVarSize = instVarSize;
    class->methodsSize = methodsSize;
    class->methods = (Method **) malloc(sizeof(Method **) * methodsSize);

    for (int i = 0; i < methodsSize; i++) {
        class->methods[i] = methods[i];
    }
    return class;
}


ObjectPointer getInstVar(Object *obj, int instVarIndex) {
    ObjectPointer *firstSlot = (ObjectPointer *) (obj + 1);
    return firstSlot[instVarIndex];
}

ObjectPointer createObject(ObjectMemory *om, Class *class, int instVarCount, ObjectPointer values[]) {
    Object *obj = (Object *) calloc(1, sizeof(Object) + (sizeof(ObjectPointer) * instVarCount));
    obj->class = class;
    for (int i = 0; i < instVarCount; i++) {
        setInstVar(obj, i, values[i]);
    }
    return registerObject(om, obj);
}

ObjectMemory *createObjectMemory() {
    ObjectMemory *om = (ObjectMemory *) malloc(sizeof(ObjectMemory));
    om->objectTable.entries = NULL;
    om->objectTable.used = 0;
    om->objectTable.size = 0;
    growObjectTable(om);
    return om;
}
