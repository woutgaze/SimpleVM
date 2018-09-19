// Generated by SimpleVM.GenerateASTNodes

#ifndef SIMPLEVM_ASTNODES_H
#define SIMPLEVM_ASTNODES_H

#include <stdint.h>
#include <stdbool.h>


static const char * NODE_LABELS[] = {"IntNode", "PrimAddNode", "ReadInstVarNode", "UnaryMessageNode", "NaryMessageNode", "SelfNode", "ReadArgNode", "ReturnNode", "ConditionalNode", "WhileTrueNode", "BoolNode", "PrimEqualsNode", "PrimSmallerThanNode", "WriteInstVarNode", "SequenceNode", "PrimNotNode", "ArrayConstructionNode", "ReadIndexedNode", "WriteIndexedNode", "PrimGetArraySizeNode", "PrimArrayAtNode", "StringNode", "ArgumentNode", "MethodNode", "BlockNode", "ReadTempNode", "WriteTempNode", "ClassSideNode", "ClassNode", "PrimStringConcatNode", "PrimStringInternNode", "UNDEFINED_31", "UNDEFINED_32", "UNDEFINED_33", "UNDEFINED_34", "UNDEFINED_35", "UNDEFINED_36", "UNDEFINED_37", "UNDEFINED_38", "UNDEFINED_39", "UNDEFINED_40", "UNDEFINED_41", "UNDEFINED_42", "UNDEFINED_43", "UNDEFINED_44", "UNDEFINED_45", "UNDEFINED_46", "UNDEFINED_47", "UNDEFINED_48", "UNDEFINED_49", "UNDEFINED_50", "UNDEFINED_51", "UNDEFINED_52", "UNDEFINED_53", "UNDEFINED_54", "UNDEFINED_55", "UNDEFINED_56", "UNDEFINED_57", "UNDEFINED_58", "UNDEFINED_59", "UNDEFINED_60", "UNDEFINED_61", "UNDEFINED_62", "UNDEFINED_63", "UNDEFINED_64", "UNDEFINED_65", "UNDEFINED_66", "UNDEFINED_67", "UNDEFINED_68", "UNDEFINED_69", "UNDEFINED_70", "UNDEFINED_71", "UNDEFINED_72", "UNDEFINED_73", "UNDEFINED_74", "UNDEFINED_75", "UNDEFINED_76", "UNDEFINED_77", "UNDEFINED_78", "UNDEFINED_79", "UNDEFINED_80", "UNDEFINED_81", "UNDEFINED_82", "UNDEFINED_83", "UNDEFINED_84", "UNDEFINED_85", "UNDEFINED_86", "UNDEFINED_87", "UNDEFINED_88", "UNDEFINED_89", "UNDEFINED_90", "UNDEFINED_91", "UNDEFINED_92", "UNDEFINED_93", "UNDEFINED_94", "UNDEFINED_95", "UNDEFINED_96", "UNDEFINED_97", "UNDEFINED_98", "UNDEFINED_99", "UNDEFINED_100", "UNDEFINED_101", "UNDEFINED_102", "UNDEFINED_103", "UNDEFINED_104", "UNDEFINED_105", "UNDEFINED_106", "UNDEFINED_107", "UNDEFINED_108", "UNDEFINED_109", "UNDEFINED_110", "UNDEFINED_111", "UNDEFINED_112", "UNDEFINED_113", "UNDEFINED_114", "UNDEFINED_115", "UNDEFINED_116", "UNDEFINED_117", "UNDEFINED_118", "UNDEFINED_119", "UNDEFINED_120", "UNDEFINED_121", "UNDEFINED_122", "UNDEFINED_123", "UNDEFINED_124", "UNDEFINED_125", "UNDEFINED_126", "UNDEFINED_127", "UNDEFINED_128", "UNDEFINED_129", "UNDEFINED_130", "UNDEFINED_131", "UNDEFINED_132", "UNDEFINED_133", "UNDEFINED_134", "UNDEFINED_135", "UNDEFINED_136", "UNDEFINED_137", "UNDEFINED_138", "UNDEFINED_139", "UNDEFINED_140", "UNDEFINED_141", "UNDEFINED_142", "UNDEFINED_143", "UNDEFINED_144", "UNDEFINED_145", "UNDEFINED_146", "UNDEFINED_147", "UNDEFINED_148", "UNDEFINED_149", "UNDEFINED_150", "UNDEFINED_151", "UNDEFINED_152", "UNDEFINED_153", "UNDEFINED_154", "UNDEFINED_155", "UNDEFINED_156", "UNDEFINED_157", "UNDEFINED_158", "UNDEFINED_159", "UNDEFINED_160", "UNDEFINED_161", "UNDEFINED_162", "UNDEFINED_163", "UNDEFINED_164", "UNDEFINED_165", "UNDEFINED_166", "UNDEFINED_167", "UNDEFINED_168", "UNDEFINED_169", "UNDEFINED_170", "UNDEFINED_171", "UNDEFINED_172", "UNDEFINED_173", "UNDEFINED_174", "UNDEFINED_175", "UNDEFINED_176", "UNDEFINED_177", "UNDEFINED_178", "UNDEFINED_179", "UNDEFINED_180", "UNDEFINED_181", "UNDEFINED_182", "UNDEFINED_183", "UNDEFINED_184", "UNDEFINED_185", "UNDEFINED_186", "UNDEFINED_187", "UNDEFINED_188", "UNDEFINED_189", "UNDEFINED_190", "UNDEFINED_191", "UNDEFINED_192", "UNDEFINED_193", "UNDEFINED_194", "UNDEFINED_195", "UNDEFINED_196", "UNDEFINED_197", "UNDEFINED_198", "UNDEFINED_199", "UNDEFINED_200", "UNDEFINED_201", "UNDEFINED_202", "UNDEFINED_203", "UNDEFINED_204", "UNDEFINED_205", "UNDEFINED_206", "UNDEFINED_207", "UNDEFINED_208", "UNDEFINED_209", "UNDEFINED_210", "UNDEFINED_211", "UNDEFINED_212", "UNDEFINED_213", "UNDEFINED_214", "UNDEFINED_215", "UNDEFINED_216", "UNDEFINED_217", "UNDEFINED_218", "UNDEFINED_219", "UNDEFINED_220", "UNDEFINED_221", "UNDEFINED_222", "UNDEFINED_223", "UNDEFINED_224", "UNDEFINED_225", "UNDEFINED_226", "UNDEFINED_227", "UNDEFINED_228", "UNDEFINED_229", "UNDEFINED_230", "UNDEFINED_231", "UNDEFINED_232", "UNDEFINED_233", "UNDEFINED_234", "UNDEFINED_235", "UNDEFINED_236", "UNDEFINED_237", "UNDEFINED_238", "UNDEFINED_239", "UNDEFINED_240", "UNDEFINED_241", "UNDEFINED_242", "UNDEFINED_243", "UNDEFINED_244", "UNDEFINED_245", "UNDEFINED_246", "UNDEFINED_247", "UNDEFINED_248", "UNDEFINED_249", "UNDEFINED_250", "UNDEFINED_251", "UNDEFINED_252", "UNDEFINED_253", "UNDEFINED_254"};

#define ARGUMENT_NODE 22
#define ARRAY_CONSTRUCTION_NODE 16
#define BOOL_NODE 10
#define CONDITIONAL_NODE 8
#define INT_NODE 0
#define NARY_MESSAGE_NODE 4
#define PRIM_ADD_NODE 1
#define PRIM_ARRAY_AT_NODE 20
#define PRIM_EQUALS_NODE 11
#define PRIM_GET_ARRAY_SIZE_NODE 19
#define PRIM_NOT_NODE 15
#define PRIM_SMALLER_THAN_NODE 12
#define PRIM_STRING_CONCAT_NODE 29
#define PRIM_STRING_INTERN_NODE 30
#define READ_ARG_NODE 6
#define READ_INDEXED_NODE 17
#define READ_INST_VAR_NODE 2
#define READ_TEMP_NODE 25
#define RETURN_NODE 7
#define SELF_NODE 5
#define SEQUENCE_NODE 14
#define STRING_NODE 21
#define UNARY_MESSAGE_NODE 3
#define WHILE_TRUE_NODE 9
#define WRITE_INDEXED_NODE 18
#define WRITE_INST_VAR_NODE 13
#define WRITE_TEMP_NODE 26
#define BLOCK_NODE 24
#define METHOD_NODE 23
#define CLASS_SIDE_NODE 27
#define CLASS_NODE 28


typedef struct Node {
	uint16_t type;
} Node;

typedef struct ArgumentNode ArgumentNode;

typedef struct ArrayConstructionNode ArrayConstructionNode;

typedef struct BoolNode BoolNode;

typedef struct ConditionalNode ConditionalNode;

typedef struct IntNode IntNode;

typedef struct NaryMessageNode NaryMessageNode;

typedef struct PrimAddNode PrimAddNode;

typedef struct PrimArrayAtNode PrimArrayAtNode;

typedef struct PrimEqualsNode PrimEqualsNode;

typedef struct PrimGetArraySizeNode PrimGetArraySizeNode;

typedef struct PrimNotNode PrimNotNode;

typedef struct PrimSmallerThanNode PrimSmallerThanNode;

typedef struct PrimStringConcatNode PrimStringConcatNode;

typedef struct PrimStringInternNode PrimStringInternNode;

typedef struct ReadArgNode ReadArgNode;

typedef struct ReadIndexedNode ReadIndexedNode;

typedef struct ReadInstVarNode ReadInstVarNode;

typedef struct ReadTempNode ReadTempNode;

typedef struct ReturnNode ReturnNode;

typedef struct SelfNode SelfNode;

typedef struct SequenceNode SequenceNode;

typedef struct StringNode StringNode;

typedef struct UnaryMessageNode UnaryMessageNode;

typedef struct WhileTrueNode WhileTrueNode;

typedef struct WriteIndexedNode WriteIndexedNode;

typedef struct WriteInstVarNode WriteInstVarNode;

typedef struct WriteTempNode WriteTempNode;

typedef struct BlockNode BlockNode;

typedef struct MethodNode MethodNode;

typedef struct ClassSideNode ClassSideNode;

typedef struct ClassNode ClassNode;

typedef struct ArgumentNodeArray {
	ArgumentNode ** elements;
	uint32_t size;
} ArgumentNodeArray;

typedef struct MethodNodeArray {
	MethodNode ** elements;
	uint32_t size;
} MethodNodeArray;

typedef struct NodeArray {
	Node ** elements;
	uint32_t size;
} NodeArray;

struct ArgumentNode {
	Node super;
	const char * name;
};

struct ArrayConstructionNode {
	Node super;
	NodeArray elements;
};

struct BoolNode {
	Node super;
	bool value;
};

struct ConditionalNode {
	Node super;
	Node * condition;
	Node * trueBranch;
	Node * falseBranch;
};

struct IntNode {
	Node super;
	int value;
};

struct NaryMessageNode {
	Node super;
	Node * receiver;
	const char * selector;
	NodeArray arguments;
};

struct PrimAddNode {
	Node super;
	Node * left;
	Node * right;
};

struct PrimArrayAtNode {
	Node super;
	Node * value;
	uint32_t index;
};

struct PrimEqualsNode {
	Node super;
	Node * left;
	Node * right;
};

struct PrimGetArraySizeNode {
	Node super;
	Node * value;
};

struct PrimNotNode {
	Node super;
	Node * value;
};

struct PrimSmallerThanNode {
	Node super;
	Node * left;
	Node * right;
};

struct PrimStringConcatNode {
	Node super;
	Node * left;
	Node * right;
};

struct PrimStringInternNode {
	Node super;
	Node * value;
};

struct ReadArgNode {
	Node super;
	uint32_t index;
};

struct ReadIndexedNode {
	Node super;
	uint32_t index;
};

struct ReadInstVarNode {
	Node super;
	uint32_t index;
};

struct ReadTempNode {
	Node super;
	uint32_t index;
};

struct ReturnNode {
	Node super;
	Node * value;
};

struct SelfNode {
	Node super;
};

struct SequenceNode {
	Node super;
	NodeArray statements;
};

struct StringNode {
	Node super;
	const char * value;
};

struct UnaryMessageNode {
	Node super;
	Node * receiver;
	const char * selector;
};

struct WhileTrueNode {
	Node super;
	Node * condition;
	Node * body;
};

struct WriteIndexedNode {
	Node super;
	uint32_t index;
	Node * value;
};

struct WriteInstVarNode {
	Node super;
	uint32_t index;
	Node * value;
};

struct WriteTempNode {
	Node super;
	uint32_t index;
	Node * value;
};

struct BlockNode {
	Node super;
	ArgumentNodeArray arguments;
	ArgumentNodeArray temporaries;
	Node * body;
};

struct MethodNode {
	Node super;
	const char * selector;
	BlockNode * block;
};

struct ClassSideNode {
	Node super;
	ArgumentNodeArray instVars;
	MethodNodeArray methods;
};

struct ClassNode {
	Node super;
	const char * name;
	const char * superName;
	uint32_t indexedType;
	ClassSideNode * instSide;
	ClassSideNode * classSide;
};

Node *newArgument(const char * name);

Node *newArrayConstruction(Node ** elements, uint32_t elements_size);

Node *newBool(bool value);

Node *newConditional(Node * condition, Node * trueBranch, Node * falseBranch);

Node *newInt(int value);

Node *newNaryMessage(Node * receiver, const char * selector, Node ** arguments, uint32_t arguments_size);

Node *newPrimAdd(Node * left, Node * right);

Node *newPrimArrayAt(Node * value, uint32_t index);

Node *newPrimEquals(Node * left, Node * right);

Node *newPrimGetArraySize(Node * value);

Node *newPrimNot(Node * value);

Node *newPrimSmallerThan(Node * left, Node * right);

Node *newPrimStringConcat(Node * left, Node * right);

Node *newPrimStringIntern(Node * value);

Node *newReadArg(uint32_t index);

Node *newReadIndexed(uint32_t index);

Node *newReadInstVar(uint32_t index);

Node *newReadTemp(uint32_t index);

Node *newReturn(Node * value);

Node *newSelf();

Node *newSequence(Node ** statements, uint32_t statements_size);

Node *newString(const char * value);

Node *newUnaryMessage(Node * receiver, const char * selector);

Node *newWhileTrue(Node * condition, Node * body);

Node *newWriteIndexed(uint32_t index, Node * value);

Node *newWriteInstVar(uint32_t index, Node * value);

Node *newWriteTemp(uint32_t index, Node * value);

Node *newBlock(ArgumentNode ** arguments, uint32_t arguments_size, ArgumentNode ** temporaries, uint32_t temporaries_size, Node * body);

Node *newMethod(const char * selector, BlockNode * block);

Node *newClassSide(ArgumentNode ** instVars, uint32_t instVars_size, MethodNode ** methods, uint32_t methods_size);

Node *newClass(const char * name, const char * superName, uint32_t indexedType, ClassSideNode * instSide, ClassSideNode * classSide);


#endif //SIMPLEVM_ASTNODES_H
