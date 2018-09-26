//
// Created by Wouter on 24-9-2018.
//

#include <memory.h>
#include "BytecodeInterpreter.h"
#include "ObjectMemory.h"
#include "DevTools.h"
#include "Hashing.h"
#include "SizedString.h"


CompiledMethodNode *lookupSelector(Behavior *class, SizedString selector) {
    CompiledMethodNodeArray methods = class->side->methods;
    for (int i = 0; i < methods.size; i++) {
        CompiledMethodNode *method = methods.elements[i];
        if (sstrcmp(method->selector, selector) == 0) return method;
    }
    if (class->superClass) {
        return lookupSelector(class->superClass, selector);
    };
    panic_a("Selector not found", getNullTerminatedString(selector));
    exit(-1);
}

Frame *createFrame(ObjectPointer selfp, SizedString selector, ObjectPointer arguments[], Process *process) {
    ObjectMemory *om = process->objectmemory;
    Object *self;
    Behavior *selfClass;
    if (isSmallInteger(selfp)) {
        self = NULL;
        selfClass = om->smallintegerClass;
    } else {
        self = getObject(om, selfp);
        selfClass = self->class;
    }
    CompiledMethodNode *method = lookupSelector(selfClass, selector);

    Frame *frame = malloc(sizeof(Frame));
    frame->process = process;
    frame->sender = NULL;
    frame->self = self;
    frame->ip = 0;
    frame->sp = 0;
    frame->code = method->code;
    frame->selfp = selfp;
    frame->arguments = arguments;
    frame->temps = calloc(sizeof(ObjectPointer), method->code->temporariesSize);
    frame->stack = calloc(sizeof(ObjectPointer), method->code->maxStackDepth);
    return frame;
}

int codePop_int(Frame *frame) {
    char *elements = frame->code->bytecode.elements;
    int32_t *value = (int32_t *) &(elements[frame->ip]);
    frame->ip = frame->ip + sizeof(int32_t);
    return *value;
}

int codePop_index(Frame *frame) {
    char *elements = frame->code->bytecode.elements;
    uint16_t *value = (uint16_t *) &(elements[frame->ip]);
    frame->ip = frame->ip + sizeof(uint16_t);
    return *value;
}

SizedString codePop_string(Frame *frame) {
    size_t len = codePop_index(frame);
    SizedString str;
    str.elements = malloc(len);
    str.size = len;
    char *elems = &(frame->code->bytecode.elements[frame->ip]);
    frame->ip = frame->ip + len;
    memcpy(str.elements, elems, len);
    return str;
}

Node *codePop_node(Frame *frame) {
    niy();
}


void stackPushObject(Frame *frame, ObjectPointer value) {
    frame->stack[frame->sp++] = value;
}

ObjectPointer stackPop_object(Frame *frame) {
    return frame->stack[--frame->sp];
}

int stackPop_int(Frame *frame) {
    return getInt(stackPop_object(frame));
}

bool stackPop_bool(Frame *frame) {
    return getBool(frame->process->objectmemory, stackPop_object(frame));
}

int32_t executeInt(int32_t value) {
    return value;
}

void stackPushInt(Frame *frame, int32_t value) {
    stackPushObject(frame, registerInt(value));
}

void stackPushBool(Frame *frame, bool value) {
    stackPushObject(frame, getBoolValue(frame->process->objectmemory, value));
}

char codePop_char(Frame *frame) {
    return frame->code->bytecode.elements[frame->ip++];
}

int32_t executePrimIntAdd(int a, int b) {
    return a + b;
}

ObjectPointer executeReadInstVar(Frame *frame, int index) {
    return getInstVar(frame->self, index);
}

void executeMessage(Frame *frame, SizedString selector, ObjectPointer receiver, ObjectPointer *arguments) {
    Frame *newFrame = createFrame(receiver, selector, arguments, frame->process);
    newFrame->sender = frame;
    frame->process->currentFrame = newFrame;
}

void executeUnaryMessage(Frame *frame, SizedString selector, ObjectPointer receiver) {
    executeMessage(frame, selector, receiver, NULL);
}

void executeBinaryMessage(Frame *frame, SizedString selector, ObjectPointer receiver, ObjectPointer arg1) {
    ObjectPointer *arguments = malloc(sizeof(ObjectPointer));
    arguments[0] = arg1;
    executeMessage(frame, selector, receiver, arguments);
}

void executeTernaryMessage(Frame *frame, SizedString selector, ObjectPointer receiver, ObjectPointer arg1,
                           ObjectPointer arg2) {
    ObjectPointer *arguments = malloc(sizeof(ObjectPointer) * 2);
    arguments[0] = arg1;
    arguments[1] = arg2;
    executeMessage(frame, selector, receiver, arguments);
}

void executeNaryMessage(Frame *pFrame, SizedString selector, ObjectPointer receiver, int numArgs) {
    niy();
}

ObjectPointer executeSelf(Frame *frame) {
    return frame->selfp;
}

ObjectPointer executeReadArg(Frame *frame, int index) {
    return frame->arguments[index];
}

void executeReturn(Frame *frame, ObjectPointer returnValue) {
    Process *process = frame->process;
    process->currentFrame = frame->sender;
    if (process->currentFrame == NULL) {
        process->returnValue = returnValue;
        return;
    }
    stackPushObject(process->currentFrame, returnValue);
}

bool executeFalse() {
    return false;
}

bool executePrimEquals(ObjectPointer a, ObjectPointer b) {
    return a == b;
}

bool executePrimIntSmallerThan(int a, int b) {
    return a < b;
}

ObjectPointer executeWriteInstVar(Frame *frame, int index, ObjectPointer value) {
    setInstVar(frame->self, index, value);
    return value;
}

bool executePrimNot(bool value) {
    return !value;
}

ObjectPointer executeArrayConstruction(Frame *frame, int index) {
    niy();
}

ObjectPointer executeReadIndexed(Frame *pFrame, int index) {
    niy();
}

ObjectPointer executeWriteIndexed(Frame *pFrame, int index, ObjectPointer object) {
    niy();
}

int32_t executePrimGetArraySize(Frame *frame, ObjectPointer value) {
    size_t size = getIndexedSize(getObject(frame->process->objectmemory, value));
    return size;
}

ObjectPointer executePrimArrayAt(int index, ObjectPointer object) {
    niy();
}

ObjectPointer executeString(Frame *frame, SizedString string) {
    Behavior *stringClass = frame->process->objectmemory->stringClass;
    size_t len = string.size;
    size_t hash = string_hash(string.elements, len);

    // String class should never have instVars:
    BytesObject *obj = malloc(sizeof(BytesObject) + len);
    obj->class = stringClass;
    obj->size = len;
    memcpy(obj->bytes, string.elements, len);

    free(string.elements);

    ObjectPointer current = findObjectMatching(frame->process->objectmemory, obj, sizeof(BytesObject) + len, hash);
    if (current != 0) {
        free(obj);
        return current;
    }
    return registerObjectWithHash(frame->process->objectmemory, (Object *) obj, hash);
}

ObjectPointer executeReadTemp(Frame *frame, int index) {
    return frame->temps[index];
}

ObjectPointer executeWriteTemp(Frame *frame, int index, ObjectPointer value) {
    frame->temps[index] = value;
    return value;
}

ObjectPointer executePrimStringConcat(Frame *pFrame, ObjectPointer object, ObjectPointer pop_object) {
    niy();
}

ObjectPointer executePrimStringIntern(Frame *pFrame, ObjectPointer object) {
    niy();
}

ObjectPointer executeNil() {
    return 0;
}

int32_t executePrimIntDiv(int anInt, int pop_int) {
    niy();
}

int32_t executePrimIntMul(int anInt, int pop_int) {
    niy();
}

int32_t executePrimIntRem(int anInt, int pop_int) {
    niy();
}

int32_t executePrimIntSub(int anInt, int pop_int) {
    niy();
}

bool executeTrue() {
    return true;
}

void executePop(ObjectPointer object) {
    //NOOP
}

ObjectPointer executePushBlock(Frame *pFrame, Node *pNode) {
    niy();
}

ObjectPointer executeReadOuterArg(Frame *pFrame, int index, int pop_index) {
    niy();
}

ObjectPointer executeReadOuterTemp(Frame *pFrame, int index, int pop_index) {
    niy();
}

ObjectPointer executeWriteOuterTemp(Frame *pFrame, int index, int pop_index, ObjectPointer object) {
    niy();
}

ObjectPointer executeGlobalRead(Frame *pFrame, SizedString sizedString) {
    niy();
}

void executeJumpFalse(Frame *frame, int index, bool test) {
    if (!test) {
        frame->ip += index;
    }
}

void executeJump(Frame *frame, int index) {
    frame->ip += index;
}

bool executePrimIntSmallerOrEqual(int a, int b) {
    return a <= b;
}

ObjectPointer executeProcess(Process *process) {
    while (true) {
        Frame *frame = process->currentFrame;
        if (frame == NULL) {
            return process->returnValue;
        }
        uint32_t bytecodeSize = frame->code->bytecode.size;
        if (frame->ip >= bytecodeSize) {
            panic("IP past end");
            break;
        }
        char type = codePop_char(frame);
        switch (type) {
            case INT_NODE: {
                stackPushInt(frame, executeInt(codePop_int(frame)));
                break;
            }
            case PRIM_INT_ADD_NODE: {
                stackPushInt(frame, executePrimIntAdd(stackPop_int(frame), stackPop_int(frame)));
                break;
            }
            case READ_INST_VAR_NODE: {
                stackPushObject(frame, executeReadInstVar(frame, codePop_index(frame)));
                break;
            }
            case UNARY_MESSAGE_NODE: {
                executeUnaryMessage(frame, codePop_string(frame), stackPop_object(frame));
                break;
            }
            case NARY_MESSAGE_NODE: {
                executeNaryMessage(frame, codePop_string(frame), stackPop_object(frame), codePop_index(frame));
                break;
            }
            case SELF_NODE: {
                stackPushObject(frame, executeSelf(frame));
                break;
            }
            case READ_ARG_NODE: {
                stackPushObject(frame, executeReadArg(frame, codePop_index(frame)));
                break;
            }
            case RETURN_NODE: {
                executeReturn(frame, stackPop_object(frame));
                break;
            }
            case FALSE_NODE: {
                stackPushBool(frame, executeFalse());
                break;
            }
            case PRIM_EQUALS_NODE: {
                stackPushBool(frame, executePrimEquals(stackPop_object(frame), stackPop_object(frame)));
                break;
            }
            case PRIM_INT_SMALLER_THAN_NODE: {
                stackPushBool(frame, executePrimIntSmallerThan(stackPop_int(frame), stackPop_int(frame)));
                break;
            }
            case WRITE_INST_VAR_NODE: {
                stackPushObject(frame, executeWriteInstVar(frame, codePop_index(frame), stackPop_object(frame)));
                break;
            }
            case PRIM_NOT_NODE: {
                stackPushBool(frame, executePrimNot(stackPop_bool(frame)));
                break;
            }
            case ARRAY_CONSTRUCTION_NODE: {
                stackPushObject(frame, executeArrayConstruction(frame, codePop_index(frame)));
                break;
            }
            case READ_INDEXED_NODE: {
                stackPushObject(frame, executeReadIndexed(frame, codePop_index(frame)));
                break;
            }
            case WRITE_INDEXED_NODE: {
                stackPushObject(frame, executeWriteIndexed(frame, codePop_index(frame), stackPop_object(frame)));
                break;
            }
            case PRIM_GET_ARRAY_SIZE_NODE: {
                stackPushInt(frame, executePrimGetArraySize(frame, stackPop_object(frame)));
                break;
            }
            case PRIM_ARRAY_AT_NODE: {
                stackPushObject(frame, executePrimArrayAt(codePop_index(frame), stackPop_object(frame)));
                break;
            }
            case STRING_NODE: {
                stackPushObject(frame, executeString(frame, codePop_string(frame)));
                break;
            }
            case READ_TEMP_NODE: {
                stackPushObject(frame, executeReadTemp(frame, codePop_index(frame)));
                break;
            }
            case WRITE_TEMP_NODE: {
                stackPushObject(frame, executeWriteTemp(frame, codePop_index(frame), stackPop_object(frame)));
                break;
            }
            case PRIM_STRING_CONCAT_NODE: {
                stackPushObject(frame, executePrimStringConcat(frame, stackPop_object(frame), stackPop_object(frame)));
                break;
            }
            case PRIM_STRING_INTERN_NODE: {
                stackPushObject(frame, executePrimStringIntern(frame, stackPop_object(frame)));
                break;
            }
            case NIL_NODE: {
                stackPushObject(frame, executeNil());
                break;
            }
            case PRIM_INT_DIV_NODE: {
                stackPushInt(frame, executePrimIntDiv(stackPop_int(frame), stackPop_int(frame)));
                break;
            }
            case PRIM_INT_MUL_NODE: {
                stackPushInt(frame, executePrimIntMul(stackPop_int(frame), stackPop_int(frame)));
                break;
            }
            case PRIM_INT_REM_NODE: {
                stackPushInt(frame, executePrimIntRem(stackPop_int(frame), stackPop_int(frame)));
                break;
            }
            case PRIM_INT_SUB_NODE: {
                stackPushInt(frame, executePrimIntSub(stackPop_int(frame), stackPop_int(frame)));
                break;
            }
            case TRUE_NODE: {
                stackPushBool(frame, executeTrue());
                break;
            }
            case POP_NODE: {
                executePop(stackPop_object(frame));
                break;
            }
            case PUSH_BLOCK_NODE: {
                stackPushObject(frame, executePushBlock(frame, codePop_node(frame)));
                break;
            }
            case READ_OUTER_ARG_NODE: {
                stackPushObject(frame, executeReadOuterArg(frame, codePop_index(frame), codePop_index(frame)));
                break;
            }
            case BINARY_MESSAGE_NODE: {
                executeBinaryMessage(frame, codePop_string(frame), stackPop_object(frame), stackPop_object(frame));
                break;
            }
            case READ_OUTER_TEMP_NODE: {
                stackPushObject(frame, executeReadOuterTemp(frame, codePop_index(frame), codePop_index(frame)));
                break;
            }
            case TERNARY_MESSAGE_NODE: {
                executeTernaryMessage(frame, codePop_string(frame), stackPop_object(frame), stackPop_object(frame),
                                      stackPop_object(frame));
                break;
            }
            case WRITE_OUTER_TEMP_NODE: {
                stackPushObject(frame, executeWriteOuterTemp(frame, codePop_index(frame), codePop_index(frame),
                                                             stackPop_object(frame)));
                break;
            }
            case GLOBAL_READ_NODE: {
                stackPushObject(frame, executeGlobalRead(frame, codePop_string(frame)));
                break;
            }
            case JUMP_FALSE_NODE: {
                executeJumpFalse(frame, codePop_index(frame), stackPop_bool(frame));
                break;
            }
            case JUMP_NODE: {
                executeJump(frame, codePop_int(frame));
                break;
            }
            case PRIM_INT_SMALLER_OR_EQUAL_NODE: {
                stackPushBool(frame, executePrimIntSmallerOrEqual(stackPop_int(frame), stackPop_int(frame)));
                break;
            }
            default: {
                const char *typeLabel = NODE_LABELS[type];
                panic_a("Invalid type", typeLabel);
            }
        }
    }
}

ObjectPointer perform(ObjectMemory *om, ObjectPointer selfp, SizedString selector, ObjectPointer arguments[]) {
    Process *process = (Process *) malloc(sizeof(Process));
    process->objectmemory = om;
    process->currentFrame = NULL;
    process->returnValue = 0;

    Frame *frame = createFrame(selfp, selector, arguments, process);
    process->currentFrame = frame;

    return executeProcess(process);
}

