#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Interpreter.h"
#include "DevTools.h"


ObjectPointer evaluate(Frame *frame, Node *node);

ObjectPointer evaluate_IntNode(Frame *frame, IntNode *node) {
    return registerInt(node->value);
}

ObjectPointer evaluate_PrimAddNode(Frame *frame, PrimAddNode *node) {
    int leftValue = getInt(evaluate(frame, node->left));
    int rightValue = getInt(evaluate(frame, node->right));
    return registerInt(leftValue + rightValue);
}

ObjectPointer evaluate_ReadInstVarNode(Frame *frame, ReadInstVarNode *node) {
    return getInstVar(frame->self, node->index);
}

ObjectPointer evaluate_UnaryMessageNode(Frame *frame, UnaryMessageNode *node) {
    ObjectPointer receiver = evaluate(frame, node->receiver);
    return perform(frame->objectmemory, receiver, node->selector, NULL);
}

ObjectPointer evaluate_NaryMessageNode(Frame *frame, NaryMessageNode *node) {
    ObjectPointer receiver = evaluate(frame, node->receiver);
    ObjectPointer *args = (ObjectPointer *) malloc(sizeof(ObjectPointer *) * node->arguments.size);
    for (int i = 0; i < node->arguments.size; i++) {
        args[i] = evaluate(frame, node->arguments.elements[i]);
    }
    ObjectPointer result = perform(frame->objectmemory, receiver, node->selector, args);
    free(args);
    return result;
}

ObjectPointer evaluate_ReadArgNode(Frame *frame, ReadArgNode *node) {
    return frame->arguments[node->index];
}

ObjectPointer evaluate_ConditionalNode(Frame *frame, ConditionalNode *node) {
    ObjectPointer condition = evaluate(frame, node->condition);
    if (getBool(frame->objectmemory, condition)) {
        return evaluate(frame, node->trueBranch);
    } else {
        return evaluate(frame, node->falseBranch);
    }
}

ObjectPointer evaluate_BoolNode(Frame *frame, BoolNode *node) {
    return node->value ? frame->objectmemory->trueValue : frame->objectmemory->falseValue;
}

ObjectPointer evaluate_PrimEqualsNode(Frame *frame, PrimEqualsNode *node) {
    ObjectPointer leftValue = evaluate(frame, node->left);
    ObjectPointer rightValue = evaluate(frame, node->right);
    return leftValue == rightValue ? frame->objectmemory->trueValue : frame->objectmemory->falseValue;
}

ObjectPointer evaluate_PrimSmallerThanNode(Frame *frame, PrimSmallerThanNode *node) {
    int leftValue = getInt(evaluate(frame, node->left));
    int rightValue = getInt(evaluate(frame, node->right));
    return leftValue < rightValue ? frame->objectmemory->trueValue : frame->objectmemory->falseValue;
}



ObjectPointer evaluate_WriteInstVarNode(Frame *frame, WriteInstVarNode *node) {
    ObjectPointer value = evaluate(frame, node->value);
    setInstVar(frame->self, node->index, value);
    return value;
}

ObjectPointer evaluate_SequenceNode(Frame *frame, SequenceNode *node) {
    ObjectPointer result = frame->objectmemory->nilValue;
    for (int i = 0; i < node->statements.size; i++) {
        result = evaluate(frame, node->statements.elements[i]);
    }
    return result;
}

ObjectPointer evaluate_PrimNotNode(Frame *frame, PrimNotNode *node) {
    ObjectPointer value = evaluate(frame, node->value);
    return !getBool(frame->objectmemory, value) ? frame->objectmemory->trueValue : frame->objectmemory->falseValue;
}

ObjectPointer evaluate_WhileTrueNode(Frame *frame, WhileTrueNode *node) {
    while(getBool(frame->objectmemory, evaluate(frame,node->condition))) {
        evaluate(frame,node->body);
    }
    return frame->objectmemory->nilValue;
}

ObjectPointer evaluate_ArrayConstructionNode(Frame *frame, ArrayConstructionNode *node) {
    Class * arrayClass = frame->objectmemory->arrayClass;
    Object * obj = newObject(arrayClass, NULL, node->elements.size);
    for (int i = 0; i < node->elements.size; i++) {
        ObjectPointer result = evaluate(frame, node->elements.elements[i]);
        noCheckSetIndexed(obj, i,  arrayClass->classNode->instSide->instVars.size, result);

    }
    return registerObject(frame->objectmemory, obj);
}

ObjectPointer evaluate_ReadIndexedNode(Frame *frame, ReadIndexedNode *node) {
    return getIndexed(frame->self, node->index);
}

ObjectPointer evaluate_WriteIndexedNode(Frame *frame, WriteIndexedNode *node) {
    ObjectPointer value = evaluate(frame, node->value);
    setIndexed(frame->self, node->index, value);
    return value;
}

ObjectPointer evaluate_PrimGetArraySizeNode(Frame *frame, PrimGetArraySizeNode *node) {
    ObjectPointer value = evaluate(frame, node->value);
    size_t size = getIndexedSize(getObject(frame->objectmemory, value));
    return registerInt(size);
}

ObjectPointer evaluate_PrimArrayAtNode(Frame *frame, PrimArrayAtNode *node) {
    Object * obj = getObject(frame->objectmemory, evaluate(frame, node->value));
    return getIndexed(obj, node->index);
}

ObjectPointer evaluate_StringNode(Frame *frame, StringNode *node) {
    Class * stringClass = frame->objectmemory->stringClass;
    size_t len = strlen(node->value);
    Object * obj = newObject(stringClass, NULL, len);
    const char *src = (node->value);
    size_t offset = sizeof(Object) +
                    (sizeof(ObjectPointer) * stringClass->classNode->instSide->instVars.size) + sizeof(size_t);
    char * dest = ((char *) obj) + offset;
    memcpy(dest, src, len);
    return registerObject(frame->objectmemory, obj);
}

ObjectPointer evaluate_WriteTempNode(Frame *frame, WriteTempNode *node) {
    ObjectPointer value = evaluate(frame, node->value);
    frame->temps[node->index] = value;
    return value;
}

ObjectPointer evaluate_ReadTempNode(Frame *frame, ReadTempNode *node) {
    return    frame->temps[node->index];
}

ObjectPointer evaluate_ReturnNode(Frame *frame, ReturnNode *node) {
    ObjectPointer value = evaluate(frame, node->value);
    return value;
}

ObjectPointer evaluate(Frame *frame, Node *node) {
    switch (node->type) {
        case INT_NODE:
            return evaluate_IntNode(frame, (IntNode *) node);
        case PRIM_ADD_NODE:
            return evaluate_PrimAddNode(frame, (PrimAddNode *) node);
        case READ_INST_VAR_NODE:
            return evaluate_ReadInstVarNode(frame, (ReadInstVarNode *) node);
        case UNARY_MESSAGE_NODE:
            return evaluate_UnaryMessageNode(frame, (UnaryMessageNode *) node);
        case NARY_MESSAGE_NODE:
            return evaluate_NaryMessageNode(frame, (NaryMessageNode *) node);
        case SELF_NODE:
            return frame->selfp;
        case READ_ARG_NODE:
            return evaluate_ReadArgNode(frame, (ReadArgNode *) node);
        case CONDITIONAL_NODE:
            return evaluate_ConditionalNode(frame, (ConditionalNode *) node);
        case WHILE_TRUE_NODE:
            return evaluate_WhileTrueNode(frame, (WhileTrueNode *) node);
        case BOOL_NODE:
            return evaluate_BoolNode(frame, (BoolNode *) node);
        case PRIM_EQUALS_NODE:
            return evaluate_PrimEqualsNode(frame, (PrimEqualsNode *) node);
        case PRIM_SMALLER_THAN_NODE:
            return evaluate_PrimSmallerThanNode(frame, (PrimSmallerThanNode *) node);
        case WRITE_INST_VAR_NODE:
            return evaluate_WriteInstVarNode(frame, (WriteInstVarNode *) node);
        case SEQUENCE_NODE:
            return evaluate_SequenceNode(frame, (SequenceNode *) node);
        case PRIM_NOT_NODE:
            return evaluate_PrimNotNode(frame, (PrimNotNode *) node);
        case ARRAY_CONSTRUCTION_NODE:
            return evaluate_ArrayConstructionNode(frame, (ArrayConstructionNode *) node);
        case READ_INDEXED_NODE:
            return evaluate_ReadIndexedNode(frame, (ReadIndexedNode *) node);
        case WRITE_INDEXED_NODE:
            return evaluate_WriteIndexedNode(frame, (WriteIndexedNode *) node);
        case PRIM_GET_ARRAY_SIZE_NODE:
            return evaluate_PrimGetArraySizeNode(frame, (PrimGetArraySizeNode *) node);
        case PRIM_ARRAY_AT_NODE:
            return evaluate_PrimArrayAtNode(frame, (PrimArrayAtNode *) node);
        case STRING_NODE:
            return evaluate_StringNode(frame, (StringNode *) node);
        case WRITE_TEMP_NODE:
            return evaluate_WriteTempNode(frame, (WriteTempNode *) node);
        case READ_TEMP_NODE:
            return evaluate_ReadTempNode(frame, (ReadTempNode *) node);
        case RETURN_NODE:
            return evaluate_ReturnNode(frame, (ReturnNode *) node);
    }
    const char *typeLabel = NODE_LABELS[node->type];
    fprintf(stderr, "Invalid type: %S.\n", typeLabel);
    exit(-1);
}

MethodNode *lookupSelector(Class *class, const char *selector) {
    MethodNodeArray methods = class->classNode->instSide->methods;
    for (int i = 0; i < methods.size; i++) {
        MethodNode *method = methods.elements[i];
        if (strcmp(method->selector, selector) == 0) return method;
    }
    if (class->superClass) return lookupSelector(class->superClass, selector);
    fprintf(stderr, "Selector not found: #%s\n", selector);
    exit(-1);
}

ObjectPointer perform(ObjectMemory *om, ObjectPointer selfp, const char *selector, ObjectPointer *arguments) {
    Object *self = getObject(om, selfp);
    MethodNode *method = lookupSelector(self->class, selector);

    Frame frame;
    frame.self = self;
    frame.selfp = selfp;
    frame.objectmemory = om;
    frame.arguments = arguments;
    frame.temps = calloc(sizeof(ObjectPointer ), method->block->temporaries.size);
    ObjectPointer result = evaluate(&frame, method->block->body);
    return result;
}
