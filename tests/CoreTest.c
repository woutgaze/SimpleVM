//
// Created by Wouter on 26-9-2018.
//

#include "CoreTest.h"
#include "../testtools/Asserting.h"
#include "../ObjectMemory.h"
#include "../ImageBuilder.h"
#include "../BytecodeInterpreter.h"

void runCoreTest(const char *sel) {

    printf("Starting #%s\n", sel);

    ObjectMemory *om = createObjectMemory();
    Class *class =  findClass(om, getSizedString("CoreTest"));
    ObjectPointer runner = basicNew(om, class);
    ObjectPointer v = perform(om, runner, getSizedString(sel), NULL);
    assertTrue("Test should return true: ", getBool(om, v));

    printf("Done #%s\n\n", sel);
}

void run_CoreTests() {
    runCoreTest("testAdd");
    runCoreTest("test_ifTrue");
    runCoreTest("test_ifFalse");
    runCoreTest("test_timesRepeat");
    runCoreTest("test_to_do");
    runCoreTest("test_string_size");

}

