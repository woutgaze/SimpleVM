#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Interpreter.h"


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
        case BOOL_NODE:
            return evaluate_BoolNode(frame, (BoolNode *) node);
        case PRIM_EQUALS_NODE:
            return evaluate_PrimEqualsNode(frame, (PrimEqualsNode *) node);
    }
    fprintf(stderr, "Invalid type.\n");
    exit(-1);
}

Method *lookupSelector(Class *class, const char *selector) {
    for (int i = 0; i < class->methodsSize; i++) {
        Method *method = class->methods[i];
        if (strcmp(method->name, selector) == 0) return method;
    }
    if (class->superClass) return lookupSelector(class->superClass, selector);
    fprintf(stderr, "Selector not found: #%s\n", selector);
    exit(-1);
}

ObjectPointer perform(ObjectMemory *om, ObjectPointer selfp, const char *selector, ObjectPointer *arguments) {
    Object *self = getObject(om, selfp);
    Method *method = lookupSelector(self->class, selector);

    Frame frame;
    frame.self = self;
    frame.selfp = selfp;
    frame.objectmemory = om;
    frame.arguments = arguments;
    ObjectPointer result = evaluate(&frame, method->node);
    return result;
}
