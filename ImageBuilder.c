//
// Created by Wouter on 9/25/2018.
//

#include "ImageBuilder.h"
#include "Bootstrap.h"
#include "reader/NodeReader.h"

ClassFormat *createNonMetaFromNode(ObjectMemory *om, CompiledClassNode *classNode) {

    return createNonMeta(om, classNode->name, classNode->superName, classNode->indexedType, classNode->instSide);

}

ObjectMemory *createObjectMemory() {
    ObjectMemory *om = newObjectMemory();

    CompiledClassNode *objNode = (CompiledClassNode *) readNodeFromBytes(B_OBJECT);
    ClassFormat *objClass = createClassNoRegister(om, NONE, objNode->instSide);
    objClass->name = objNode->name;
    CompiledClassNode *undefinedObjectNode = (CompiledClassNode *) readNodeFromBytes(B_UNDEFINEDOBJECT);
    om->nilClass = createClassNoRegister(om, NONE, undefinedObjectNode->instSide);
    om->nilClass->name = undefinedObjectNode->name;
    om->nilValue = basicNew(om, om->nilClass);
    registerClass(om, objClass);
    registerClass(om, om->nilClass);
    om->nilClass->superClass = objClass;

    CompiledClassNode *behaviorNode = (CompiledClassNode *) readNodeFromBytes(B_BEHAVIOR);
    ClassFormat *behaviorC = createNonMetaFromNode(om, behaviorNode);
    CompiledClassNode *classDescriptionNode = (CompiledClassNode *) readNodeFromBytes(B_CLASSDESCRIPTION);
    ClassFormat *classDescriptionC = createNonMetaFromNode(om, classDescriptionNode);
    CompiledClassNode *classNode = (CompiledClassNode *) readNodeFromBytes(B_CLASS);
    ClassFormat *classC = createNonMetaFromNode(om, classNode);
    CompiledClassNode *metaclassNode = (CompiledClassNode *) readNodeFromBytes(B_METACLASS);
    ClassFormat *metaclassC = createNonMetaFromNode(om, metaclassNode);


    loadClassesIntoObjectMemory(om);

    om->trueValue = basicNew(om, findClass(om, getSizedString("True")));
    om->falseValue = basicNew(om, findClass(om, getSizedString("False")));
    om->arrayClass = findClass(om, getSizedString("Array"));;
    om->stringClass = findClass(om, getSizedString("AsciiString"));;
    om->bytearrayClass = findClass(om, getSizedString("ByteArray"));;
    om->smallintegerClass = findClass(om, getSizedString("SmallInteger"));;
    return om;
}

