#include <stdlib.h>
#include "ASTNodes.h"

Node *newInt(int val) {
    IntNode *node = (IntNode *) malloc(sizeof(IntNode));
    node->super.type = INT_NODE;
    node->value = val;
    return (Node *) node;
}

Node *newPrimAdd(Node *a, Node *b) {
    PrimAddNode *node = (PrimAddNode *) malloc(sizeof(PrimAddNode));
    node->super.type = PRIM_ADD_NODE;
    node->left = a;
    node->right = b;
    return (Node *) node;
}

Node *newReadInstVar(int index) {
    ReadInstVarNode *node = (ReadInstVarNode *) malloc(sizeof(ReadInstVarNode));
    node->super.type = READ_INST_VAR_NODE;
    node->index = index;
    return (Node *) node;
}

Node *newUnaryMessage(Node *receiver, const char *selector) {
    UnaryMessageNode *node = (UnaryMessageNode *) malloc(sizeof(UnaryMessageNode));
    node->super.type = UNARY_MESSAGE_NODE;
    node->receiver = receiver;
    node->selector = selector;
    return (Node *) node;
}

Node *newSelf() {
    SelfNode *node = (SelfNode *) malloc(sizeof(SelfNode));
    node->super.type = SELF_NODE;
    return (Node *) node;
}


