// Generated by SimpleVM.GenerateASTNodes

#include <stdio.h>
#include <stdlib.h>
#include "NodeReader_V1.h"
#include "NodeReader_V1_Tools.h"
#include "../ASTNodes.h"

Node *readArgumentNode_V1(FILE *fileptr) {
    const char * name = readString_V1(fileptr);

    return newArgument(name);
}

Node *readArrayConstructionNode_V1(FILE *fileptr) {
    NodeArray elements = readNodeArray_V1(fileptr);

    return newArrayConstruction(elements.elements, elements.size);
}

Node *readBoolNode_V1(FILE *fileptr) {
    bool value = readBool_V1(fileptr);

    return newBool(value);
}

Node *readConditionalNode_V1(FILE *fileptr) {
    Node * condition = readNode_V1(fileptr);

    Node * trueBranch = readNode_V1(fileptr);

    Node * falseBranch = readNode_V1(fileptr);

    return newConditional(condition, trueBranch, falseBranch);
}

Node *readIntNode_V1(FILE *fileptr) {
    int value = readInt_V1(fileptr);

    return newInt(value);
}

Node *readNaryMessageNode_V1(FILE *fileptr) {
    Node * receiver = readNode_V1(fileptr);

    const char * selector = readString_V1(fileptr);

    NodeArray arguments = readNodeArray_V1(fileptr);

    return newNaryMessage(receiver, selector, arguments.elements, arguments.size);
}

Node *readPrimAddNode_V1(FILE *fileptr) {
    Node * left = readNode_V1(fileptr);

    Node * right = readNode_V1(fileptr);

    return newPrimAdd(left, right);
}

Node *readPrimArrayAtNode_V1(FILE *fileptr) {
    Node * value = readNode_V1(fileptr);

    uint32_t index = readIndex_V1(fileptr);

    return newPrimArrayAt(value, index);
}

Node *readPrimEqualsNode_V1(FILE *fileptr) {
    Node * left = readNode_V1(fileptr);

    Node * right = readNode_V1(fileptr);

    return newPrimEquals(left, right);
}

Node *readPrimGetArraySizeNode_V1(FILE *fileptr) {
    Node * value = readNode_V1(fileptr);

    return newPrimGetArraySize(value);
}

Node *readPrimNotNode_V1(FILE *fileptr) {
    Node * value = readNode_V1(fileptr);

    return newPrimNot(value);
}

Node *readPrimSmallerThanNode_V1(FILE *fileptr) {
    Node * left = readNode_V1(fileptr);

    Node * right = readNode_V1(fileptr);

    return newPrimSmallerThan(left, right);
}

Node *readReadArgNode_V1(FILE *fileptr) {
    uint32_t index = readIndex_V1(fileptr);

    return newReadArg(index);
}

Node *readReadIndexedNode_V1(FILE *fileptr) {
    uint32_t index = readIndex_V1(fileptr);

    return newReadIndexed(index);
}

Node *readReadInstVarNode_V1(FILE *fileptr) {
    uint32_t index = readIndex_V1(fileptr);

    return newReadInstVar(index);
}

Node *readReadTempNode_V1(FILE *fileptr) {
    uint32_t index = readIndex_V1(fileptr);

    return newReadTemp(index);
}

Node *readReturnNode_V1(FILE *fileptr) {
    Node * value = readNode_V1(fileptr);

    return newReturn(value);
}

Node *readSelfNode_V1(FILE *fileptr) {
    return newSelf();
}

Node *readSequenceNode_V1(FILE *fileptr) {
    NodeArray statements = readNodeArray_V1(fileptr);

    return newSequence(statements.elements, statements.size);
}

Node *readStringNode_V1(FILE *fileptr) {
    const char * value = readString_V1(fileptr);

    return newString(value);
}

Node *readUnaryMessageNode_V1(FILE *fileptr) {
    Node * receiver = readNode_V1(fileptr);

    const char * selector = readString_V1(fileptr);

    return newUnaryMessage(receiver, selector);
}

Node *readWhileTrueNode_V1(FILE *fileptr) {
    Node * condition = readNode_V1(fileptr);

    Node * body = readNode_V1(fileptr);

    return newWhileTrue(condition, body);
}

Node *readWriteIndexedNode_V1(FILE *fileptr) {
    uint32_t index = readIndex_V1(fileptr);

    Node * value = readNode_V1(fileptr);

    return newWriteIndexed(index, value);
}

Node *readWriteInstVarNode_V1(FILE *fileptr) {
    uint32_t index = readIndex_V1(fileptr);

    Node * value = readNode_V1(fileptr);

    return newWriteInstVar(index, value);
}

Node *readWriteTempNode_V1(FILE *fileptr) {
    uint32_t index = readIndex_V1(fileptr);

    Node * value = readNode_V1(fileptr);

    return newWriteTemp(index, value);
}

Node *readBlockNode_V1(FILE *fileptr) {
    ArgumentNodeArray arguments = readArgumentNodeArray_V1(fileptr);

    ArgumentNodeArray temporaries = readArgumentNodeArray_V1(fileptr);

    Node * body = readNode_V1(fileptr);

    return newBlock(arguments.elements, arguments.size, temporaries.elements, temporaries.size, body);
}

Node *readMethodNode_V1(FILE *fileptr) {
    const char * selector = readString_V1(fileptr);

    BlockNode * block = readNode_V1(fileptr);

    return newMethod(selector, block);
}

Node *readNode_V1(FILE *fileptr) {
    char type = getc(fileptr);

    switch (type) {

        case ARGUMENT_NODE:
            return readArgumentNode_V1(fileptr);
        case ARRAY_CONSTRUCTION_NODE:
            return readArrayConstructionNode_V1(fileptr);
        case BOOL_NODE:
            return readBoolNode_V1(fileptr);
        case CONDITIONAL_NODE:
            return readConditionalNode_V1(fileptr);
        case INT_NODE:
            return readIntNode_V1(fileptr);
        case NARY_MESSAGE_NODE:
            return readNaryMessageNode_V1(fileptr);
        case PRIM_ADD_NODE:
            return readPrimAddNode_V1(fileptr);
        case PRIM_ARRAY_AT_NODE:
            return readPrimArrayAtNode_V1(fileptr);
        case PRIM_EQUALS_NODE:
            return readPrimEqualsNode_V1(fileptr);
        case PRIM_GET_ARRAY_SIZE_NODE:
            return readPrimGetArraySizeNode_V1(fileptr);
        case PRIM_NOT_NODE:
            return readPrimNotNode_V1(fileptr);
        case PRIM_SMALLER_THAN_NODE:
            return readPrimSmallerThanNode_V1(fileptr);
        case READ_ARG_NODE:
            return readReadArgNode_V1(fileptr);
        case READ_INDEXED_NODE:
            return readReadIndexedNode_V1(fileptr);
        case READ_INST_VAR_NODE:
            return readReadInstVarNode_V1(fileptr);
        case READ_TEMP_NODE:
            return readReadTempNode_V1(fileptr);
        case RETURN_NODE:
            return readReturnNode_V1(fileptr);
        case SELF_NODE:
            return readSelfNode_V1(fileptr);
        case SEQUENCE_NODE:
            return readSequenceNode_V1(fileptr);
        case STRING_NODE:
            return readStringNode_V1(fileptr);
        case UNARY_MESSAGE_NODE:
            return readUnaryMessageNode_V1(fileptr);
        case WHILE_TRUE_NODE:
            return readWhileTrueNode_V1(fileptr);
        case WRITE_INDEXED_NODE:
            return readWriteIndexedNode_V1(fileptr);
        case WRITE_INST_VAR_NODE:
            return readWriteInstVarNode_V1(fileptr);
        case WRITE_TEMP_NODE:
            return readWriteTempNode_V1(fileptr);
        case BLOCK_NODE:
            return readBlockNode_V1(fileptr);
        case METHOD_NODE:
            return readMethodNode_V1(fileptr);
    }

    fprintf(stderr, "Unknown type.\n");
    exit(-1);
}


