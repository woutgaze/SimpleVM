//
// Created by Wouter on 17-9-2018.
//

#ifndef SIMPLEVM_ASTNODES_H
#define SIMPLEVM_ASTNODES_H

#include <stdint.h>

#define INT_NODE 0
#define PRIM_ADD_NODE 1
#define READ_INST_VAR_NODE 2
#define UNARY_MESSAGE_NODE 3
#define SELF_NODE 4

typedef struct {
    int type;
} Node;

typedef struct {
    Node super;
    int value;
} IntNode;

typedef struct {
    Node super;
    Node *left;
    Node *right;
} PrimAddNode;

typedef struct {
    Node super;
    int index;
} ReadInstVarNode;

typedef struct {
    Node super;
    Node *receiver;
    const char *selector;
} UnaryMessageNode;

typedef struct {
    Node super;
} SelfNode;

Node *newInt(int val);

Node *newPrimAdd(Node *a, Node *b);

Node *newReadInstVar(int index);

Node *newUnaryMessage(Node *receiver, const char *selector);

Node *newSelf();

uint8_t* compile(Node *node);

#endif //SIMPLEVM_ASTNODES_H
