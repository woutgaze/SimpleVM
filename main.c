#include <stdio.h>
#include <stdlib.h>
#include "Interpreter.h"

void assertTrue(const char *description, bool value) {
    if (!value) {
        printf("ASSERTION FAILED: %s\n", description);
        exit(-1);
    }
}

void assertFalse(const char *description, bool value) {
    assertTrue(description, !value);
}

void assertEquals(int a, int b) {
    if (!(a == b)) {
        printf("ASSERTION FAILED: %d == %d\n", a, b);
        exit(-1);
    }
}

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
    Class *class = createClass(2, NULL, 0, 0);
    ObjectPointer op = createObject(om, class, values, 0);
    Object *obj = getObject(om, op);

    ObjectPointer v = getInstVar(obj, 0);
    assertEquals(getInt(v), 3);

    ObjectPointer w = getInstVar(obj, 1);
    assertEquals(getInt(w), 4);
}

void test_sendmessage() {
    ObjectMemory *om = createObjectMemory();
    Node *node = newPrimAdd(newReadInstVar(0), newReadInstVar(1));
    Method *methods[] = {createMethod("add", node, 0)};
    Class *class = createClass(2, methods, 1, 0);

    ObjectPointer values[] = {registerInt(3), registerInt(4)};
    ObjectPointer op = createObject(om, class, values, 0);

    ObjectPointer v = perform(om, op, "add", NULL);
    assertEquals(getInt(v), 7);
}

void test_dispatch() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {createMethod("add", newPrimAdd(newReadInstVar(0), newReadInstVar(1)), 0),
                         createMethod("execute", newUnaryMessage(newSelf(), "add"), 0)
    };
    Class *class = createClass(2, methods, 2, 0);

    ObjectPointer values[] = {registerInt(3), registerInt(4)};
    ObjectPointer op = createObject(om, class, values, 0);

    ObjectPointer v = perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 7);
}

void test_dispatchWithArguments() {
    ObjectMemory *om = createObjectMemory();
    Node *add_args[] = {newReadArg(0), newReadInstVar(1)};
    Node *execute_args[] = {newReadInstVar(0)};
    Method *methods[] = {createMethod("plus", newPrimAdd(newReadArg(0), newReadArg(1)), 2),
                         createMethod("add", newNaryMessage(newSelf(), "plus", add_args, 2), 1),
                         createMethod("execute", newNaryMessage(newSelf(), "add", execute_args, 1), 0)};
    Class *class = createClass(2, methods, 3, 0);

    ObjectPointer values[] = {registerInt(3), registerInt(4)};
    ObjectPointer op = createObject(om, class, values, 0);

    ObjectPointer v = perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 7);
}

void test_int() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute", newInt(3), 0)};
    Class *class = createClass(0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);

    ObjectPointer v = perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 3);
}

void test_true() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute", newBool(true), 0)};
    Class *class = createClass(0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);

    ObjectPointer v = perform(om, op, "execute", NULL);
    assertTrue("Bool", getBool(om, v));
}

void test_false() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute", newBool(false), 0)};
    Class *class = createClass(0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);

    ObjectPointer v = perform(om, op, "execute", NULL);
    assertFalse("Bool", getBool(om, v));
}

void test_conditional_true() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute", newConditional(newBool(true), newInt(3), newInt(4)), 0)};
    Class *class = createClass(0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);

    ObjectPointer v = perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 3);
}

void test_conditional_false() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute", newConditional(newBool(false), newInt(3), newInt(4)), 0)};
    Class *class = createClass(0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);

    ObjectPointer v = perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 4);
}

void test_equals() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute", newPrimEquals(newReadArg(0), newReadArg(1)), 2)};
    Class *class = createClass(0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer op1 = createObject(om, NULL, NULL, 0);
    ObjectPointer v = perform(om, op, "execute", (ObjectPointer[]) {op1, op1});
    assertTrue("Bool", getBool(om, v));
}

void test_not_equals() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute", newPrimEquals(newReadArg(0), newReadArg(1)), 2)};
    Class *class = createClass(0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer op1 = createObject(om, NULL, NULL, 0);
    ObjectPointer op2 = createObject(om, NULL, NULL, 0);
    ObjectPointer v = perform(om, op, "execute", (ObjectPointer[]) {op1, op2});
    assertFalse("Bool", getBool(om, v));
}

void test_write_instvar() {
    ObjectMemory *om = createObjectMemory();
    Node *stmts[] = {newWriteInstVar(0, newInt(3)), newReadInstVar(0)};
    Method *methods[] = {createMethod("execute", newSequence(stmts, 2), 0)};
    Class *class = createClass(1, methods, 1, 0);
    ObjectPointer values[] = {registerInt(23)};

    ObjectPointer op = createObject(om, class, values, 0);
    ObjectPointer v = perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 3);
}

void test_op_zero_is_nil() {
    ObjectMemory *om = createObjectMemory();
    assertTrue("ObjectPointer nil is at pos 0 in ObjectMemory", om->nilValue == 0);
}

void test_true_not() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute", newPrimNot(newBool(true)), 2)};
    Class *class = createClass(0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer v = perform(om, op, "execute", NULL);
    assertFalse("Bool", getBool(om, v));
}

void test_false_not() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {
            createMethod("execute", newPrimNot(newBool(false)), 2)};
    Class *class = createClass(0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer v = perform(om, op, "execute", NULL);
    assertTrue("Bool", getBool(om, v));
}

void test_loop() {
    ObjectMemory *om = createObjectMemory();
    Node *stmts[] = {
            newWhileTrue(newPrimSmallerThan(newReadInstVar(0), newInt(42)),
                         newWriteInstVar(0, newPrimAdd(newReadInstVar(0), newInt(5)))),
            newReadInstVar(0)};
    Method *methods[] = {createMethod("execute", newSequence(stmts, 2), 0)};
    Class *class = createClass(1, methods, 1, 0);
    ObjectPointer values[] = {registerInt(0)};

    ObjectPointer op = createObject(om, class, values, 0);
    ObjectPointer v = perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 45);
}

void test_create_array() {
    ObjectMemory *om = createObjectMemory();
    Node *stmts[] = {
            newWriteIndexed(1, newInt(7)),
            newReadIndexed(1)};
    Method *methods[] = {createMethod("execute", newSequence(stmts, 2), 0)};
    Class *class = createClass(3, methods, 1, true);
    ObjectPointer values[] = {registerInt(0)};

    ObjectPointer op = createObject(om, class, values, 2);
    ObjectPointer v = perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 7);
}

void test_array_construction() {
    ObjectMemory *om = createObjectMemory();
    Node * elements[] = {newInt(1), newInt(2), newInt(3)};
    Method *methods[] = {createMethod("execute", newPrimGetArraySize(newArrayConstruction(elements, 3)), 0)};
    Class *class = createClass(0, methods, 1, false);
    ObjectPointer values[] = {registerInt(0)};

    ObjectPointer op = createObject(om, class, values, 0);
    ObjectPointer v = perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 3);
}

void test_array_construction_last() {
    ObjectMemory *om = createObjectMemory();
    Node * elements[] = {newInt(1), newInt(7), newInt(29)};
    Method *methods[] = {createMethod("execute", newPrimArrayAt(newArrayConstruction(elements, 3), 2), 0)};
    Class *class = createClass(0, methods, 1, false);
    ObjectPointer values[] = {registerInt(0)};

    ObjectPointer op = createObject(om, class, values, 0);
    ObjectPointer v = perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 29);
}

void test_create_string() {
    ObjectMemory *om = createObjectMemory();
    Method *methods[] = {createMethod("execute", newString("Kaas"), 0)};
    Class *class = createClass(0, methods, 1, false);
    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer v = perform(om, op, "execute", NULL);
    printf("String: %s\n\n", getCString(om, v));
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
    runTest("test_write_instvar", test_write_instvar);
    runTest("test_op_zero_is_nil", test_op_zero_is_nil);
    runTest("test_true_not", test_true_not);
    runTest("test_false_not", test_false_not);
    runTest("test_loop", test_loop);
    runTest("test_create_array", test_create_array);
    runTest("test_array_construction", test_array_construction);
    runTest("test_array_construction_last", test_array_construction_last);
    runTest("test_create_string", test_create_string);

    printf("Done\n");
}