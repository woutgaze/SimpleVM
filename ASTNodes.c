#include <stdlib.h>
#include <stdio.h>
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

typedef struct Compiler {
    uint8_t *bytecode;
    int ip;
} Compiler;

void compileNode(Compiler *compiler, Node *node);

uint8_t *compile(Node *node) {
    Compiler compiler;
    compiler.bytecode = calloc(1000, 1);
    compiler.ip = 0;
    compileNode(&compiler, node);
    return compiler.bytecode;
}

void emit(Compiler *compiler, uint8_t byte) {
    compiler->bytecode[compiler->ip++] = byte;
}

void compile_IntNode(Compiler *compiler, IntNode *node) {
    niy();
}

void compile_PrimAddNode(Compiler *compiler, PrimAddNode *node) {
    niy();
}

void compile_ReadInstVarNode(Compiler *compiler, ReadInstVarNode *node) {
    niy();
}

void compile_UnaryMessageNode(Compiler *compiler, UnaryMessageNode *node) {
    niy();
}

void compile_SelfNode(Compiler *compiler, SelfNode *node) {
    niy();
}

void compileNode(Compiler *compiler, Node *node) {
    switch (node->type) {
        case INT_NODE:
            return compile_IntNode(compiler, (IntNode *) node);
        case PRIM_ADD_NODE:
            return compile_PrimAddNode(compiler, (PrimAddNode *) node);
        case READ_INST_VAR_NODE:
            return compile_ReadInstVarNode(compiler, (ReadInstVarNode *) node);
        case UNARY_MESSAGE_NODE:
            return compile_UnaryMessageNode(compiler, (UnaryMessageNode *) node);
        case SELF_NODE:
            return compile_SelfNode(compiler, (SelfNode *) node);
    }
    fprintf(stderr, "Invalid type.\n");
    exit(-1);

}



