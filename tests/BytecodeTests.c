//
// Created by Wouter on 25-9-2018.
//

#include "BytecodeTests.h"
#include "../ObjectMemory.h"
#include "../reader/NodeReader.h"
#include "../BytecodeInterpreter.h"
#include "../testtools/Asserting.h"

void test_read_compiled_method_node() {
    ObjectMemory *om = createObjectMemory();
    char bytes[] = {83, 86, 1, 39, 7, 0, 101, 120, 101, 99, 117, 116, 101, 38, 0, 0, 1, 0, 1, 0, 5, 0, 15, 0, 21, 4, 0,
                    75, 97, 97, 115, 26, 0, 0, 37, 25, 0, 0, 7};

    char *sel = "execute";

    CompiledMethodNode *methodNode = (CompiledMethodNode *) readNodeFromBytes(bytes);
    CompiledMethodNode *methods[] = {methodNode};
    Class *class = createCompiledClass(om, 2, methods, 1, NONE);
    ObjectPointer values[] = {om->nilValue, om->nilValue};


    ObjectPointer op = createObject(om, class, values, 0);
    ObjectPointer v = perform(om, op, getSizedString(sel), NULL);
    printf("String: %s\n\n", getCString(om, v));
}

void test_read_compiled_class_node() {
    ObjectMemory *om = createObjectMemory();
    char bytes[] = {83, 86, 1, 41, 4, 0, 75, 97, 97, 115, 6, 0, 79, 98, 106, 101, 99, 116, 0, 0, 40, 2, 0, 22, 3, 0,
                    102, 111, 111, 22, 3, 0, 98, 97, 114, 3, 0, 39, 4, 0, 105, 110, 105, 116, 38, 0, 0, 0, 0, 1, 0, 5,
                    0, 11, 0, 0, 3, 0, 0, 0, 13, 0, 0, 37, 5, 7, 39, 9, 0, 99, 97, 108, 99, 117, 108, 97, 116, 101, 38,
                    0, 0, 0, 0, 2, 0, 4, 0, 10, 0, 2, 0, 0, 0, 19, 0, 0, 0, 1, 7, 39, 7, 0, 101, 120, 101, 99, 117, 116,
                    101, 38, 0, 0, 0, 0, 1, 0, 6, 0, 23, 0, 5, 3, 4, 0, 105, 110, 105, 116, 37, 5, 3, 9, 0, 99, 97, 108,
                    99, 117, 108, 97, 116, 101, 7, 40, 0, 0, 0, 0};

    CompiledClassNode *cn = (CompiledClassNode *) readNodeFromBytes(bytes);
    Class *class = createCompiledClassFromNode(om, cn);
    ObjectPointer op = basicNew(om, class);
    ObjectPointer v = perform(om, op, getSizedString("execute"), NULL);
    assertEquals(getInt(v), 22);
}

void run_BytecodeTests() {
    runTest("test_read_compiled_class_node", test_read_compiled_class_node);
}
