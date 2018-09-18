#include <stdio.h>
#include <stdlib.h>
#include "Interpreter.h"

void test_addEntryToObjectTable() {
    ObjectMemory *om = createObjectMemory();
    printf("X: %d\n", sizeof(size_t));

    registerObject(om, (Object *) malloc(sizeof(Object)));
    registerObject(om, (Object *) malloc(sizeof(Object)));
    registerObject(om, (Object *) malloc(sizeof(Object)));
    registerObject(om, (Object *) malloc(sizeof(Object)));
}

void test_createObject() {
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

void test_sendmessage() {
    ObjectMemory *om = createObjectMemory();
    Node *node = newPrimAdd(newReadInstVar(0), newReadInstVar(1));
    Method *methods[] = {createMethod("add", node, 0)};
    Class *class = createClass(2, methods, 1);

    ObjectPointer values[] = {registerInt(3), registerInt(4)};
    ObjectPointer op = createObject(om, class, 2, values);

    ObjectPointer v = perform(om, op, "add", NULL);
    printf("v value: %d\n", (getInt(v)));
}

void test_dispatch() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {createMethod("add", newPrimAdd(newReadInstVar(0), newReadInstVar(1)), 0),
                         createMethod("execute", newUnaryMessage(newSelf(), "add"), 0)
    };
    Class *class = createClass(2, methods, 2);

    ObjectPointer values[] = {registerInt(3), registerInt(4)};
    ObjectPointer op = createObject(om, class, 2, values);

    ObjectPointer v = perform(om, op, "execute", NULL);
    printf("v value: %d\n", (getInt(v)));
}

void test_dispatchWithArguments() {
    ObjectMemory *om = createObjectMemory();
    Node *add_args[] = {newReadArg(0), newReadInstVar(1)};
    Node *execute_args[] = {newReadInstVar(0)};
    Method *methods[] = {createMethod("plus", newPrimAdd(newReadArg(0), newReadArg(1)), 2),
                         createMethod("add", newNaryMessage(newSelf(), "plus", add_args, 2), 1),
                         createMethod("execute", newNaryMessage(newSelf(), "add", execute_args, 1), 0)};
    Class *class = createClass(2, methods, 3);

    ObjectPointer values[] = {registerInt(3), registerInt(4)};
    ObjectPointer op = createObject(om, class, 2, values);

    ObjectPointer v = perform(om, op, "execute", NULL);
    printf("v value: %d\n", (getInt(v)));
}

void test_int() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute", newInt(3), 0)};
    Class *class = createClass(2, methods, 1);

    ObjectPointer op = createObject(om, class, 0, NULL);

    ObjectPointer v = perform(om, op, "execute", NULL);
    printf("v value: %d\n", (getInt(v)));
}

void test_true() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute", newBool(true), 0)};
    Class *class = createClass(2, methods, 1);

    ObjectPointer op = createObject(om, class, 0, NULL);

    ObjectPointer v = perform(om, op, "execute", NULL);
    printf("v value: %s\n", (getBool(om, v)) ? "true" : "false");
}

void test_false() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute", newBool(false), 0)};
    Class *class = createClass(2, methods, 1);

    ObjectPointer op = createObject(om, class, 0, NULL);

    ObjectPointer v = perform(om, op, "execute", NULL);
    printf("v value: %s\n", (getBool(om, v)) ? "true" : "false");
}

void test_conditional_true() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute", newConditional(newBool(true), newInt(3), newInt(4)), 0)};
    Class *class = createClass(2, methods, 1);

    ObjectPointer op = createObject(om, class, 0, NULL);

    ObjectPointer v = perform(om, op, "execute", NULL);
    printf("v value: %d\n", (getInt(v)));
}

void test_conditional_false() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute", newConditional(newBool(false), newInt(3), newInt(4)), 0)};
    Class *class = createClass(2, methods, 1);

    ObjectPointer op = createObject(om, class, 0, NULL);

    ObjectPointer v = perform(om, op, "execute", NULL);
    printf("v value: %d\n", (getInt(v)));
}

void test_equals() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute",newPrimEquals(newReadArg(0), newReadArg(1)) , 2)};
    Class *class = createClass(2, methods, 1);

    ObjectPointer op = createObject(om, class, 0, NULL);
    ObjectPointer op1 = createObject(om, NULL, 0, NULL);
    ObjectPointer * args[] = {op1, op1};
    ObjectPointer v = perform(om, op, "execute", args );
    printf("v value: %s\n", (getBool(om, v)) ? "true" : "false");
}

void test_not_equals() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute",newPrimEquals(newReadArg(0), newReadArg(1)) , 2)};
    Class *class = createClass(2, methods, 1);

    ObjectPointer op = createObject(om, class, 0, NULL);
    ObjectPointer op1 = createObject(om, NULL, 0, NULL);
    ObjectPointer op2 = createObject(om, NULL, 0, NULL);
    ObjectPointer * args[] = {op1, op2};
    ObjectPointer v = perform(om, op, "execute", args );
    printf("v value: %s\n", (getBool(om, v)) ? "true" : "false");
}


void runTest(const char *label, void (*testFN)()) {
    printf("Starting %s\n", label);
    (*testFN)();
    printf("Done %s\n\n", label);
}

int main() {
    runTest("test_addEntryToObjectTable", test_addEntryToObjectTable);
    runTest("test_createObject", test_createObject);
    runTest("test_sendmessage", test_sendmessage);
    runTest("test_dispatch", test_dispatch);
    runTest("test_dispatchWithArguments", test_dispatchWithArguments);
    runTest("test_int", test_int);
    runTest("test_true", test_true);
    runTest("test_false", test_false);
    runTest("test_conditional_true", test_conditional_true);
    runTest("test_conditional_false", test_conditional_false);
    runTest("test_equals", test_equals);
    runTest("test_not_equals", test_not_equals);

    printf("Done\n");
}