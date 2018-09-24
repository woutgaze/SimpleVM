//
// Created by Wouter on 20-9-2018.
//

#include "BasicTests.h"
#include "../ObjectMemory.h"
#include "../testtools/Asserting.h"
#include "../ASTInterpreter.h"
#include "../reader/NodeReader.h"
#include "../Hashing.h"
#include "../BytecodeInterpreter.h"

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
    Class *class = createClass(om, 2, NULL, 0, 0);
    ObjectPointer op = createObject(om, class, values, 0);
    Object *obj = getObject(om, op);

    ObjectPointer v = getInstVar(obj, 0);
    assertEquals(getInt(v), 3);

    ObjectPointer w = getInstVar(obj, 1);
    assertEquals(getInt(w), 4);
}

void test_sendmessage() {
    ObjectMemory *om = createObjectMemory();
    Node *node = newPrimIntAdd(newReadInstVar(0), newReadInstVar(1));
    MethodNode *methods[] = {createMethod("add", node)};
    Class *class = createClass(om, 2, methods, 1, 0);

    ObjectPointer values[] = {registerInt(3), registerInt(4)};
    ObjectPointer op = createObject(om, class, values, 0);

    ObjectPointer v = ast_perform(om, op, "add", NULL);
    assertEquals(getInt(v), 7);
}

void test_dispatch() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {createMethod("add", newPrimIntAdd(newReadInstVar(0), newReadInstVar(1))),
                             createMethod("execute", newUnaryMessage("add", newSelf()))
    };
    Class *class = createClass(om, 2, methods, 2, 0);

    ObjectPointer values[] = {registerInt(3), registerInt(4)};
    ObjectPointer op = createObject(om, class, values, 0);

    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 7);
}

void test_dispatchWithArguments() {
    ObjectMemory *om = createObjectMemory();
    Node *add_args[] = {newReadArg(0), newReadInstVar(1)};
    Node *execute_args[] = {newReadInstVar(0)};
    MethodNode *methods[] = {createMethod("plus", newPrimIntAdd(newReadArg(0), newReadArg(1))),
                             createMethod("add", newNaryMessage("plus", newSelf(), add_args, 2)),
                             createMethod("execute", newNaryMessage("add", newSelf(), execute_args, 1))};
    Class *class = createClass(om, 2, methods, 3, 0);

    ObjectPointer values[] = {registerInt(3), registerInt(4)};
    ObjectPointer op = createObject(om, class, values, 0);

    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 7);
}

void test_int() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {
            createMethod("execute", newInt(3))};
    Class *class = createClass(om, 0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);

    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 3);
}

void test_true() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {
            createMethod("execute", newTrue())};
    Class *class = createClass(om, 0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);

    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertTrue("Bool", getBool(om, v));
}

void test_false() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {
            createMethod("execute", newFalse())};
    Class *class = createClass(om, 0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);

    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertFalse("Bool", getBool(om, v));
}

void test_conditional_true() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {
            createMethod("execute", newConditional(newTrue(), newInt(3), newInt(4)))};
    Class *class = createClass(om, 0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);

    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 3);
}

void test_conditional_false() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {
            createMethod("execute", newConditional(newFalse(), newInt(3), newInt(4)))};
    Class *class = createClass(om, 0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);

    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 4);
}

void test_equals() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {
            createMethod("execute", newPrimEquals(newReadArg(0), newReadArg(1)))};
    Class *class = createClass(om, 0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer op1 = createObject(om, NULL, NULL, 0);
    ObjectPointer v = ast_perform(om, op, "execute", (ObjectPointer[]) {op1, op1});
    assertTrue("Bool", getBool(om, v));
}

void test_not_equals() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {
            createMethod("execute", newPrimEquals(newReadArg(0), newReadArg(1)))};
    Class *class = createClass(om, 0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer op1 = createObject(om, NULL, NULL, 0);
    ObjectPointer op2 = createObject(om, NULL, NULL, 0);
    ObjectPointer v = ast_perform(om, op, "execute", (ObjectPointer[]) {op1, op2});
    assertFalse("Bool", getBool(om, v));
}

void test_write_instvar() {
    ObjectMemory *om = createObjectMemory();
    Node *stmts[] = {newWriteInstVar(0, newInt(3)), newReadInstVar(0)};
    MethodNode *methods[] = {createMethod("execute", newSequence(stmts, 2))};
    Class *class = createClass(om, 1, methods, 1, 0);
    ObjectPointer values[] = {registerInt(23)};

    ObjectPointer op = createObject(om, class, values, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 3);
}

void test_op_zero_is_nil() {
    ObjectMemory *om = createObjectMemory();
    assertTrue("ObjectPointer nil is at pos 0 in ObjectMemory", om->nilValue == 0);
}

void test_true_not() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {
            createMethod("execute", newPrimNot(newTrue()))};
    Class *class = createClass(om, 0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertFalse("Bool", getBool(om, v));
}

void test_false_not() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {
            createMethod("execute", newPrimNot(newFalse()))};
    Class *class = createClass(om, 0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertTrue("Bool", getBool(om, v));
}

void test_loop() {
    ObjectMemory *om = createObjectMemory();
    Node *stmts[] = {
            newWhileTrue(newPrimIntSmallerThan(newReadInstVar(0), newInt(42)),
                         newWriteInstVar(0, newPrimIntAdd(newReadInstVar(0), newInt(5)))),
            newReadInstVar(0)};
    MethodNode *methods[] = {createMethod("execute", newSequence(stmts, 2))};
    Class *class = createClass(om, 1, methods, 1, 0);
    ObjectPointer values[] = {registerInt(0)};

    ObjectPointer op = createObject(om, class, values, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 45);
}

void test_create_array() {
    ObjectMemory *om = createObjectMemory();
    Node *stmts[] = {
            newWriteIndexed(1, newInt(7)),
            newReadIndexed(1)};
    MethodNode *methods[] = {createMethod("execute", newSequence(stmts, 2))};
    Class *class = createClass(om, 3, methods, 1, true);
    ObjectPointer values[] = {registerInt(0)};

    ObjectPointer op = createObject(om, class, values, 2);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 7);
}

void test_array_construction() {
    ObjectMemory *om = createObjectMemory();
    Node *elements[] = {newInt(1), newInt(2), newInt(3)};
    MethodNode *methods[] = {createMethod("execute", newPrimGetArraySize(newArrayConstruction(elements, 3)))};
    Class *class = createClass(om, 0, methods, 1, false);
    ObjectPointer values[] = {registerInt(0)};

    ObjectPointer op = createObject(om, class, values, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 3);
}

void test_array_construction_last() {
    ObjectMemory *om = createObjectMemory();
    Node *elements[] = {newInt(1), newInt(7), newInt(29)};
    MethodNode *methods[] = {createMethod("execute", newPrimArrayAt(2, newArrayConstruction(elements, 3)))};
    Class *class = createClass(om, 0, methods, 1, false);
    ObjectPointer values[] = {registerInt(0)};

    ObjectPointer op = createObject(om, class, values, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 29);
}

void test_create_string() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {createMethod("execute", newString("Kaas"))};
    Class *class = createClass(om, 0, methods, 1, NONE);
    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    printf("String: %s\n\n", getCString(om, v));
}

void test_read_string_node() {
    ObjectMemory *om = createObjectMemory();
    char bytes[] = {0x53, 0x56, 0x01, 0x15, 0x04, 0x00, 0x4B, 0x61, 0x61, 0x73};
    Node *node = readNodeFromBytes(bytes);
    MethodNode *methods[] = {createMethod("execute", node)};
    Class *class = createClass(om, 0, methods, 1, NONE);
    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    printf("String: %s\n\n", getCString(om, v));
}

void test_read_sequence_node() {
    ObjectMemory *om = createObjectMemory();
    char bytes[] = {83, 86, 1, 14, 3, 0, 13, 0, 0, 0, 3, 0, 0, 0, 13, 1, 0, 0, 4, 0, 0, 0, 1, 2, 0, 0, 2, 1, 0};
    Node *node = readNodeFromBytes(bytes);
    MethodNode *methods[] = {createMethod("execute", node)};
    Class *class = createClass(om, 0, methods, 1, false);
    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertEquals(getInt(v), 7);
}

void test_read_method_node() {
    ObjectMemory *om = createObjectMemory();
    char bytes[] = {83, 86, 1, 23, 7, 0, 101, 120, 101, 99, 117, 116, 101, 24, 0, 0, 1, 0, 22, 3, 0, 102, 111, 111, 14,
                    2, 0, 26, 0, 0, 21, 4, 0, 75, 97, 97, 115, 7, 25, 0, 0};

    MethodNode *methodNode = (MethodNode *) readNodeFromBytes(bytes);
    MethodNode *methods[] = {methodNode};
    Class *class = createClass(om, 2, methods, 1, NONE);
    ObjectPointer values[] = {om->nilValue, om->nilValue};


    ObjectPointer op = createObject(om, class, values, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    printf("String: %s\n\n", getCString(om, v));
}

void test_true_isNil() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {
            createMethod("execute", newUnaryMessage("isNil", newTrue()))};
    Class *class = createClass(om, 0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertFalse("Bool", getBool(om, v));

}
void test_nil_isNil() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {
            createMethod("execute", newUnaryMessage("isNil", newNil()))};
    Class *class = createClass(om, 0, methods, 1, 0);

    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertTrue("Bool", getBool(om, v));

}

void test_hash_string() {
    assertEquals(string_hash("Kaas", 4), 2089277829);
    assertEquals(string_hash("Worst", 5), 241444420);
    assertEquals(string_hash("", 0), 5381);
}

void test_string_intern() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {createMethod("execute", newPrimEquals(newString("Kaas"), newString("Kaas")))};
    Class *class = createClass(om, 0, methods, 1, NONE);
    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertTrue("Strings are pointer-equal", getBool(om, v));

}

void test_string_intern_ne() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {createMethod("execute", newPrimEquals(newString("Kaas1"), newString("Kaas2")))};
    Class *class = createClass(om, 0, methods, 1, NONE);
    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertFalse("Strings are pointer-equal", getBool(om, v));
}

void test_prim_string_concat() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {createMethod("execute",
                                          newPrimEquals(
                                                  newString("Wortel"),
                                                  newPrimStringConcat(newString("Wort"), newString("el"))))};
    Class *class = createClass(om, 0, methods, 1, NONE);
    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertFalse("Strings are pointer-equal", getBool(om, v));
}

void test_prim_string_concat_intern() {
    ObjectMemory *om = createObjectMemory();
    MethodNode *methods[] = {createMethod("execute",
                                          newPrimEquals(newString("Wortel"), newPrimStringIntern(
                                                  newPrimStringConcat(newString("Wort"), newString("el")))
                                          ))};
    Class *class = createClass(om, 0, methods, 1, NONE);
    ObjectPointer op = createObject(om, class, NULL, 0);
    ObjectPointer v = ast_perform(om, op, "execute", NULL);
    assertTrue("Strings are pointer-equal", getBool(om, v));
}

void test_read_compiled_method_node() {
    ObjectMemory *om = createObjectMemory();
    char bytes[] = { 83, 86, 1, 39, 7, 0, 101, 120, 101, 99, 117, 116, 101, 38, 0, 0, 1, 0, 1, 0, 5, 0, 15, 0, 21, 4, 0, 75, 97, 97, 115, 26, 0, 0, 37, 25, 0, 0, 7 };

    CompiledMethodNode *methodNode = (CompiledMethodNode *) readNodeFromBytes(bytes);
    CompiledMethodNode *methods[] = {methodNode};
    Class *class = createCompiledClass(om, 2, methods, 1, NONE);
    ObjectPointer values[] = {om->nilValue, om->nilValue};


    ObjectPointer op = createObject(om, class, values, 0);
    ObjectPointer v = perform(om, op, getSizedString("execute"), NULL);
    printf("String: %s\n\n", getCString(om, v));
}

void runTest(const char *label, void (*testFN)()) {
    printf("Starting %s\n", label);
    (*testFN)();
    printf("Done %s\n\n", label);
}

void run_BasicTests() {
//    runTest("test_addEntryToObjectTable", test_addEntryToObjectTable);
//    runTest("test_createObject", test_createObject);
//    runTest("test_sendmessage", test_sendmessage);
//    runTest("test_dispatch", test_dispatch);
//    runTest("test_dispatchWithArguments", test_dispatchWithArguments);
//    runTest("test_int", test_int);
//    runTest("test_true", test_true);
//    runTest("test_false", test_false);
//    runTest("test_conditional_true", test_conditional_true);
//    runTest("test_conditional_false", test_conditional_false);
//    runTest("test_equals", test_equals);
//    runTest("test_not_equals", test_not_equals);
//    runTest("test_write_instvar", test_write_instvar);
//    runTest("test_op_zero_is_nil", test_op_zero_is_nil);
//    runTest("test_true_not", test_true_not);
//    runTest("test_false_not", test_false_not);
//    runTest("test_loop", test_loop);
//    runTest("test_create_array", test_create_array);
//    runTest("test_array_construction", test_array_construction);
//    runTest("test_array_construction_last", test_array_construction_last);
//    runTest("test_create_string", test_create_string);
//    runTest("test_read_string_node", test_read_string_node);
//    runTest("test_read_sequence_node", test_read_sequence_node);
//    runTest("test_read_method_node", test_read_method_node);
//    runTest("test_hash_string", test_hash_string);
//    runTest("test_string_intern", test_string_intern);
//    runTest("test_prim_string_concat", test_prim_string_concat);
//    runTest("test_prim_string_concat_intern", test_prim_string_concat_intern);
//    runTest("test_string_intern_ne", test_string_intern_ne);
//    runTest("test_true_isNil", test_true_isNil);
//    runTest("test_nil_isNil", test_nil_isNil);
    runTest("test_read_compiled_method_node", test_read_compiled_method_node);
}