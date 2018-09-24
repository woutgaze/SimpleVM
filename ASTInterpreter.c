#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ASTInterpreter.h"
#include "DevTools.h"
#include "Hashing.h"


ObjectPointer evaluate(ASTFrame *frame, Node *node);

ObjectPointer primIntAdd(ObjectPointer left, ObjectPointer right) {
    int leftValue = getInt(left);
    int rightValue = getInt(right);
    return registerInt(leftValue + rightValue);
}

ObjectPointer sendUnaryMessage(const ASTFrame *frame, ObjectPointer receiver, const char *selector) { return ast_perform(
            frame->objectmemory, receiver, selector, NULL); }

ObjectPointer evaluate_NaryMessageNode(ASTFrame *frame, NaryMessageNode *node) {
    ObjectPointer receiver = evaluate(frame, node->receiver);
    ObjectPointer *args = (ObjectPointer *) malloc(sizeof(ObjectPointer *) * node->arguments.size);
    for (int i = 0; i < node->arguments.size; i++) {
        args[i] = evaluate(frame, node->arguments.elements[i]);
    }
    ObjectPointer result = ast_perform(frame->objectmemory, receiver, node->selector, args);
    free(args);
    return result;
}

ObjectPointer evaluate_ReadArgNode(ASTFrame *frame, ReadArgNode *node) {
    return frame->arguments[node->index];
}

ObjectPointer evaluate_ConditionalNode(ASTFrame *frame, ConditionalNode *node) {
    ObjectPointer condition = evaluate(frame, node->condition);
    if (getBool(frame->objectmemory, condition)) {
        return evaluate(frame, node->trueBranch);
    } else {
        return evaluate(frame, node->falseBranch);
    }
}

ObjectPointer evaluate_PrimEqualsNode(ASTFrame *frame, PrimEqualsNode *node) {
    ObjectPointer leftValue = evaluate(frame, node->left);
    ObjectPointer rightValue = evaluate(frame, node->right);
    return getBoolValue(frame->objectmemory, leftValue == rightValue);
}

ObjectPointer evaluate_PrimSmallerThanNode(ASTFrame *frame, PrimIntSmallerThanNode *node) {
    int leftValue = getInt(evaluate(frame, node->left));
    int rightValue = getInt(evaluate(frame, node->right));
    return getBoolValue(frame->objectmemory, leftValue < rightValue);
}


ObjectPointer evaluate_WriteInstVarNode(ASTFrame *frame, WriteInstVarNode *node) {
    ObjectPointer value = evaluate(frame, node->value);
    setInstVar(frame->self, node->index, value);
    return value;
}

ObjectPointer evaluate_SequenceNode(ASTFrame *frame, SequenceNode *node) {
    ObjectPointer result = frame->objectmemory->nilValue;
    for (int i = 0; i < node->statements.size; i++) {
        result = evaluate(frame, node->statements.elements[i]);
    }
    return result;
}

ObjectPointer evaluate_PrimNotNode(ASTFrame *frame, PrimNotNode *node) {
    ObjectPointer value = evaluate(frame, node->value);
    return getBoolValue(frame->objectmemory, !getBool(frame->objectmemory, value));
}

ObjectPointer evaluate_WhileTrueNode(ASTFrame *frame, WhileTrueNode *node) {
    while (getBool(frame->objectmemory, evaluate(frame, node->condition))) {
        evaluate(frame, node->body);
    }
    return frame->objectmemory->nilValue;
}

ObjectPointer evaluate_ArrayConstructionNode(ASTFrame *frame, ArrayConstructionNode *node) {
    Class *arrayClass = frame->objectmemory->arrayClass;
    Object *obj = newObject(arrayClass, NULL, node->elements.size);
    for (int i = 0; i < node->elements.size; i++) {
        ObjectPointer result = evaluate(frame, node->elements.elements[i]);
        noCheckSetIndexed(obj, i, arrayClass->classNode->instSide->instVars.size, result);

    }
    return registerObject(frame->objectmemory, obj);
}

ObjectPointer evaluate_ReadIndexedNode(ASTFrame *frame, ReadIndexedNode *node) {
    return getIndexed(frame->self, node->index);
}

ObjectPointer evaluate_WriteIndexedNode(ASTFrame *frame, WriteIndexedNode *node) {
    ObjectPointer value = evaluate(frame, node->value);
    setIndexed(frame->self, node->index, value);
    return value;
}

ObjectPointer evaluate_PrimGetArraySizeNode(ASTFrame *frame, PrimGetArraySizeNode *node) {
    ObjectPointer value = evaluate(frame, node->value);
    size_t size = getIndexedSize(getObject(frame->objectmemory, value));
    return registerInt(size);
}

ObjectPointer evaluate_PrimArrayAtNode(ASTFrame *frame, PrimArrayAtNode *node) {
    Object *obj = getObject(frame->objectmemory, evaluate(frame, node->value));
    return getIndexed(obj, node->index);
}

ObjectPointer evaluate_StringNode(ASTFrame *frame, StringNode *node) {
    Class *stringClass = frame->objectmemory->stringClass;
    const char *src = node->value;
    size_t len = strlen(src);
    size_t hash = string_hash(src, len);

    // String class should never have instVars:
    BytesObject *obj = malloc(sizeof(BytesObject) + len);
    obj->class = stringClass;
    obj->size = len;
    memcpy(obj->bytes, src, len);

    ObjectPointer current = findObjectMatching(frame->objectmemory, obj, sizeof(BytesObject) + len, hash);
    if (current != 0) {
        free(obj);
        return current;
    }
    return registerObjectWithHash(frame->objectmemory, (Object *) obj, hash);
}

ObjectPointer evaluate_WriteTempNode(ASTFrame *frame, WriteTempNode *node) {
    ObjectPointer value = evaluate(frame, node->value);
    frame->temps[node->index] = value;
    return value;
}

ObjectPointer evaluate_ReadTempNode(ASTFrame *frame, ReadTempNode *node) {
    return frame->temps[node->index];
}

ObjectPointer evaluate_ReturnNode(ASTFrame *frame, ReturnNode *node) {
    ObjectPointer value = evaluate(frame, node->value);
    return value;
}

ObjectPointer evaluate_PrimStringConcatNode(ASTFrame *frame, PrimStringConcatNode *node) {
    Object *leftObject = getObject(frame->objectmemory, evaluate(frame, node->left));
    Object *rightObject = getObject(frame->objectmemory, evaluate(frame, node->right));
    if ((leftObject->class->classNode->indexedType != BYTE_INDEXED) ||
        (rightObject->class->classNode->indexedType != BYTE_INDEXED)) {
        panic("Argument is not bytes.");
    }
    BytesObject *left = (BytesObject *) leftObject;
    BytesObject *right = (BytesObject *) rightObject;
    BytesObject *result = malloc(sizeof(BytesObject) + left->size + right->size);
    result->size = left->size + right->size;
    result->class = left->class;

    char *srcLeft = ((char *) left) + sizeof(BytesObject);
    char *srcRight = ((char *) right) + sizeof(BytesObject);

    char *dest = result->bytes;
    memcpy(dest, srcLeft, left->size);
    char *dest2 = dest + left->size;
    memcpy(dest2, srcRight, right->size);
    uint32_t hash = string_hash(result->bytes, result->size);
    return registerObjectWithHash(frame->objectmemory, (Object *) result, hash);
}

ObjectPointer evaluate_PrimStringInternNode(ASTFrame *frame, PrimStringInternNode *node) {
    BytesObject *value = (BytesObject *) getObject(frame->objectmemory, evaluate(frame, node->value));
    uint32_t hash = string_hash(value->bytes, value->size);
    // The "interned" object is always the first object found.
    return findObjectMatching(frame->objectmemory, value, sizeof(BytesObject) + value->size, hash);
}


ObjectPointer evaluate(ASTFrame *frame, Node *node) {
    switch (node->type) {
        case INT_NODE: {
            return registerInt(((IntNode *) node)->value);
        }
        case PRIM_INT_ADD_NODE: {
            return primIntAdd(evaluate(frame, ((PrimIntAddNode *) node)->left), evaluate(frame, ((PrimIntAddNode *) node)->right));
        }
        case READ_INST_VAR_NODE: {
            return getInstVar(frame->self, ((ReadInstVarNode *) node)->index);
        }
        case UNARY_MESSAGE_NODE: {
            return sendUnaryMessage(frame, evaluate(frame, ((UnaryMessageNode *) node)->receiver), ((UnaryMessageNode *) node)->selector);
        }
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
        case TRUE_NODE:
            return frame->objectmemory->trueValue;
        case FALSE_NODE:
            return frame->objectmemory->falseValue;
        case PRIM_EQUALS_NODE:
            return evaluate_PrimEqualsNode(frame, (PrimEqualsNode *) node);
        case PRIM_INT_SMALLER_THAN_NODE:
            return evaluate_PrimSmallerThanNode(frame, (PrimIntSmallerThanNode *) node);
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
        case PRIM_STRING_CONCAT_NODE:
            return evaluate_PrimStringConcatNode(frame, (PrimStringConcatNode *) node);
        case PRIM_STRING_INTERN_NODE:
            return evaluate_PrimStringInternNode(frame, (struct PrimStringInternNode *) node);
        case NIL_NODE:
            return frame->objectmemory->nilValue;
    }
    const char *typeLabel = NODE_LABELS[node->type];
    panic_a("Invalid type", typeLabel);
    exit(-1);
}

MethodNode *lookupASTSelector(Class *class, SizedString *selector) {
    MethodNodeArray methods = class->classNode->instSide->methods;
    for (int i = 0; i < methods.size; i++) {
        MethodNode *method = methods.elements[i];
        if (sstrcmp(method->selector, selector) == 0) return method;
    }
    if (class->superClass) {
        return lookupASTSelector(class->superClass, selector);
    };
    panic_a("Selector not found", selector);
    exit(-1);
}

ObjectPointer ast_perform(ObjectMemory *om, ObjectPointer selfp, char *selector, ObjectPointer *arguments) {
    return ast_perform_sym(om, selfp, getSizedString(selector), arguments);
}

ObjectPointer ast_perform_sym(ObjectMemory *om, ObjectPointer selfp, SizedString *selector, ObjectPointer *arguments) {
    Object *self = getObject(om, selfp);
    MethodNode *method = lookupASTSelector(self->class, selector);

    ASTFrame frame;
    frame.self = self;
    frame.selfp = selfp;
    frame.objectmemory = om;
    frame.arguments = arguments;
    frame.temps = calloc(sizeof(ObjectPointer), method->block->temporaries.size);
    ObjectPointer result = evaluate(&frame, (Node *) method->block->body);
    return result;
}
