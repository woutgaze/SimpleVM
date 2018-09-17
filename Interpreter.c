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
    return perform(frame->objectmemory, receiver, node->selector);
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
        case SELF_NODE:
            return frame->selfp;
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

ObjectPointer evaluateNode(ObjectMemory *om, ObjectPointer selfp, Node *node) {
    Object *self = getObject(om, selfp);
    Frame frame;
    frame.self = self;
    frame.selfp = selfp;
    frame.objectmemory = om;
    return evaluate(&frame, node);
}

ObjectPointer perform(ObjectMemory *om, ObjectPointer selfp, const char *selector) {
    Object *self = getObject(om, selfp);
    Method *method = lookupSelector(self->class, selector);
    Frame frame;
    frame.self = self;
    frame.selfp = selfp;
    frame.objectmemory = om;
    return evaluate(&frame, method->node);
}

