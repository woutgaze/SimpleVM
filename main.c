#include <stdio.h>
#include <stdlib.h>
#include "Interpreter.h"

void test_addEntryToObjectTable() {
    printf("test_addEntryToObjectTable\n");
    ObjectMemory *om = createObjectMemory();
    printf("X: %d\n", sizeof(size_t));

    registerObject(om, (Object *) malloc(sizeof(Object)));
    registerObject(om, (Object *) malloc(sizeof(Object)));
    registerObject(om, (Object *) malloc(sizeof(Object)));
    registerObject(om, (Object *) malloc(sizeof(Object)));
}

void test_createObject() {
    printf("test_createObject\n");
    ObjectMemory *om = createObjectMemory();
    printf("X: %d\n", sizeof(size_t));

    ObjectPointer values[] = {registerInt(3), registerInt(4)};

    ObjectPointer op = createObject(om, NULL, 2, values);
    Object *obj = getObject(om, op);
    ObjectPointer v = getInstVar(obj, 0);
    printf("v value: %d\n", (getInt(v)));
    ObjectPointer w = getInstVar(obj, 1);
    printf("w value: %d\n", (getInt(w)));
}

void test_evaluate() {
    printf("test_evaluate\n");
    ObjectMemory *om = createObjectMemory();
    ObjectPointer values[] = {registerInt(3), registerInt(4)};

    ObjectPointer op = createObject(om, NULL, 2, values);

    Node *addMethod = newPrimAdd(newReadInstVar(0), newReadInstVar(1));
    ObjectPointer v = evaluateNode(om, op, addMethod);
    printf("v value: %d\n", (getInt(v)));
}

void test_sendmessage() {
    printf("test_sendmessage\n");
    ObjectMemory *om = createObjectMemory();
    Node *node = newPrimAdd(newReadInstVar(0), newReadInstVar(1));
    Method *methods[] = {createMethod("add", node)};
    Class *class = createClass(2, 1, methods);

    ObjectPointer values[] = {registerInt(3), registerInt(4)};
    ObjectPointer op = createObject(om, class, 2, values);

    ObjectPointer v = perform(om, op, "add");
    printf("v value: %d\n", (getInt(v)));
}

void test_dispatch() {
    printf("test_dispatch\n");
    ObjectMemory *om = createObjectMemory();
    Method *method = createMethod("add", newPrimAdd(newReadInstVar(0), newReadInstVar(1)));
    Method *methods[] = {createMethod("add", newPrimAdd(newReadInstVar(0), newReadInstVar(1))),
                         createMethod("execute", newUnaryMessage(newSelf(), "add"))
    };
    Class *class = createClass(2, 2, methods);

    ObjectPointer values[] = {registerInt(3), registerInt(4)};
    ObjectPointer op = createObject(om, class, 2, values);

    ObjectPointer v = perform(om, op, "execute");
    printf("v value: %d\n", (getInt(v)));
}

int main() {
    test_addEntryToObjectTable();
    test_createObject();
    test_evaluate();
    test_sendmessage();
    test_dispatch();

    printf("Done\n");
}