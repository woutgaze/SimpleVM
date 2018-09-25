//
// Created by Wouter on 9/25/2018.
//

#include "ImageBuilder.h"
#include "Bootstrap.h"
#include "reader/NodeReader.h"

ObjectMemory *createObjectMemory() {
    ObjectMemory *om = newObjectMemory();

    Class *objClass = createClassNoRegister(om, (CompiledClassNode *) readNodeFromBytes(B_OBJECT));
    om->nilClass = createClassNoRegister(om, (CompiledClassNode *) readNodeFromBytes(B_UNDEFINEDOBJECT));
    om->nilValue = basicNew(om, om->nilClass);
    registerClass(om, objClass);
    registerClass(om, om->nilClass);
    loadClassesIntoObjectMemory(om);

    om->trueValue = basicNew(om, findClass(om, getSizedString("True")));
    om->falseValue = basicNew(om, findClass(om, getSizedString("False")));
    om->arrayClass = findClass(om, getSizedString("Array"));;
    om->stringClass = findClass(om, getSizedString("AsciiString"));;
    om->bytearrayClass = findClass(om, getSizedString("ByteArray"));;
    om->smallintegerClass = findClass(om, getSizedString("SmallInteger"));;
    return om;
}

