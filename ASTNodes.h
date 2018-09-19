// Generated by SimpleVM.GenerateASTNodes

#ifndef SIMPLEVM_ASTNODES_H
#define SIMPLEVM_ASTNODES_H

#include <stdint.h>
#include <stdbool.h>


#define INT_NODE 0
#define PRIM_ADD_NODE 1
#define READ_INST_VAR_NODE 2
#define UNARY_MESSAGE_NODE 3
#define NARY_MESSAGE_NODE 4
#define SELF_NODE 5
#define READ_ARG_NODE 6
#define RETURN_NODE 7
#define CONDITIONAL_NODE 8
#define WHILE_TRUE_NODE 9
#define BOOL_NODE 10
#define PRIM_EQUALS_NODE 11
#define PRIM_SMALLER_THAN_NODE 12
#define WRITE_INST_VAR_NODE 13
#define SEQUENCE_NODE 14
#define PRIM_NOT_NODE 15
#define ARRAY_CONSTRUCTION_NODE 16
#define READ_INDEXED_NODE 17
#define WRITE_INDEXED_NODE 18
#define PRIM_GET_ARRAY_SIZE_NODE 19
#define PRIM_ARRAY_AT_NODE 20
#define STRING_NODE 21


typedef struct {
	uint16_t type;
} Node;

typedef struct {
	Node ** elements;
	uint32_t size;
} NodeArray;

typedef struct {
	Node super;
	int value;
} IntNode;

typedef struct {
	Node super;
	Node * left;
	Node * right;
} PrimAddNode;

typedef struct {
	Node super;
	uint32_t index;
} ReadInstVarNode;

typedef struct {
	Node super;
	Node * receiver;
	const char * selector;
} UnaryMessageNode;

typedef struct {
	Node super;
	Node * receiver;
	const char * selector;
	NodeArray arguments;
} NaryMessageNode;

typedef struct {
	Node super;
} SelfNode;

typedef struct {
	Node super;
	uint32_t index;
} ReadArgNode;

typedef struct {
	Node super;
	Node * value;
} ReturnNode;

typedef struct {
	Node super;
	Node * condition;
	Node * trueBranch;
	Node * falseBranch;
} ConditionalNode;

typedef struct {
	Node super;
	Node * condition;
	Node * body;
} WhileTrueNode;

typedef struct {
	Node super;
	bool value;
} BoolNode;

typedef struct {
	Node super;
	Node * left;
	Node * right;
} PrimEqualsNode;

typedef struct {
	Node super;
	Node * left;
	Node * right;
} PrimSmallerThanNode;

typedef struct {
	Node super;
	uint32_t index;
	Node * value;
} WriteInstVarNode;

typedef struct {
	Node super;
	NodeArray statements;
} SequenceNode;

typedef struct {
	Node super;
	Node * value;
} PrimNotNode;

typedef struct {
	Node super;
	NodeArray elements;
} ArrayConstructionNode;

typedef struct {
	Node super;
	uint32_t index;
} ReadIndexedNode;

typedef struct {
	Node super;
	uint32_t index;
	Node * value;
} WriteIndexedNode;

typedef struct {
	Node super;
	Node * value;
} PrimGetArraySizeNode;

typedef struct {
	Node super;
	Node * value;
	uint32_t index;
} PrimArrayAtNode;

typedef struct {
	Node super;
	const char * value;
} StringNode;

Node *newInt(int value);

Node *newPrimAdd(Node * left, Node * right);

Node *newReadInstVar(uint32_t index);

Node *newUnaryMessage(Node * receiver, const char * selector);

Node *newNaryMessage(Node * receiver, const char * selector, Node ** arguments, uint32_t arguments_size);

Node *newSelf();

Node *newReadArg(uint32_t index);

Node *newReturn(Node * value);

Node *newConditional(Node * condition, Node * trueBranch, Node * falseBranch);

Node *newWhileTrue(Node * condition, Node * body);

Node *newBool(bool value);

Node *newPrimEquals(Node * left, Node * right);

Node *newPrimSmallerThan(Node * left, Node * right);

Node *newWriteInstVar(uint32_t index, Node * value);

Node *newSequence(Node ** statements, uint32_t statements_size);

Node *newPrimNot(Node * value);

Node *newArrayConstruction(Node ** elements, uint32_t elements_size);

Node *newReadIndexed(uint32_t index);

Node *newWriteIndexed(uint32_t index, Node * value);

Node *newPrimGetArraySize(Node * value);

Node *newPrimArrayAt(Node * value, uint32_t index);

Node *newString(const char * value);


#endif //SIMPLEVM_ASTNODES_H
