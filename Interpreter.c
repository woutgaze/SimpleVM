#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Interpreter.h"
#include "Bytecodes.h"

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
void push(Frame *frame, ObjectPointer value) {
    niy();
}

void pushReceiverVariable(Frame *frame, unsigned char index) {
    push(frame, getInstVar(frame->self, index));
}

void pushTemporaryLocation(Frame *frame, unsigned char index) {
    niy();
}
void pushLiteralConstant(Frame *frame, unsigned char index) {
    niy();
}
void pushLiteralVariable(Frame *frame, unsigned char index) {
    niy();
}
void popAndStoreReceiverVariable(Frame *frame, unsigned char index) {
    niy();
}
void popAndStoreTemporaryLocation(Frame *frame, unsigned char index) {
    niy();
}
void pushReceiver(Frame *frame) {
    niy();
}
void pushTrue(Frame *frame) {
    niy();
}
void pushFalse(Frame *frame) {
    niy();
}
void pushNil(Frame *frame) {
    niy();
}
void pushMinusOne(Frame *frame) {
    niy();
}
void pushZero(Frame *frame) {
    niy();
}
void pushOne(Frame *frame) {
    niy();
}
void pushTwo(Frame *frame) {
    niy();
}
void returnReceiver(Frame *frame) {
    niy();
}
void returnTrue(Frame *frame) {
    niy();
}
void returnFalse(Frame *frame) {
    niy();
}
void returnNil(Frame *frame) {
    niy();
}
void returnStackTopFromMessage(Frame *frame) {
    niy();
}
void returnStackTopFromBlock(Frame *frame) {
    niy();
}
void extendedPush(Frame *frame) {
    niy();
}
void extendedStore(Frame *frame) {
    niy();
}
void extendedPopAndStore(Frame *frame) {
    niy();
}
void sendShort(Frame *frame) {
    niy();
}
void sendLong(Frame *frame) {
    niy();
}
void superSendShort(Frame *frame) {
    niy();
}
void superSendLong(Frame *frame) {
    niy();
}
void popStackTop(Frame *frame) {
    niy();
}
void duplicateStackTop(Frame *frame) {
    niy();
}
void pushActiveContext(Frame *frame) {
    niy();
}
void jump(Frame *frame, unsigned char index) {
    niy();
}
void popAndJumpOnFalse(Frame *frame, unsigned char index) {
    niy();
}
void longJump(Frame *frame, unsigned char index) {
    niy();
}
void longPopAndJumpOnTrue(Frame *frame, unsigned char index) {
    niy();
}
void longPopAndJumpOnFalse(Frame *frame, unsigned char index) {
    niy();
}
void sendArithmeticMessage(Frame *frame, unsigned char index) {
    niy();
}
void sendSpecialMessage(Frame *frame, unsigned char index) {
    niy();
}
void sendLiteralSelectorWithNoArguments(Frame *frame, unsigned char index) {
    niy();
}
void sendLiteralSelectorWith1Argument(Frame *frame, unsigned char index) {
    niy();
}
void sendLiteralSelectorWith2Arguments(Frame *frame, unsigned char index) {
    niy();
}
uint8_t next(Frame *frame) { return frame->method->bytecode[frame->ip++]; }


void interpretNext(Frame *frame) {
    uint8_t byte = next(frame);
    switch (byte) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            // Push Receiver Variable #iiii
            pushReceiverVariable(frame, (byte & 0b00001111));
            return;
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
            // Push Temporary Location #iiii
            pushTemporaryLocation(frame, (byte & 0b00001111));
            return;
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56:
        case 57:
        case 58:
        case 59:
        case 60:
        case 61:
        case 62:
        case 63:
            // Push Literal Constant #iiiii
            pushLiteralConstant(frame, (byte & 0b00011111));
            return;
        case 64:
        case 65:
        case 66:
        case 67:
        case 68:
        case 69:
        case 70:
        case 71:
        case 72:
        case 73:
        case 74:
        case 75:
        case 76:
        case 77:
        case 78:
        case 79:
        case 80:
        case 81:
        case 82:
        case 83:
        case 84:
        case 85:
        case 86:
        case 87:
        case 88:
        case 89:
        case 90:
        case 91:
        case 92:
        case 93:
        case 94:
        case 95:
            // Push Literal Variable #iiiii
            pushLiteralVariable(frame, (byte & 0b00011111));
            return;
        case 96:
        case 97:
        case 98:
        case 99:
        case 100:
        case 101:
        case 102:
        case 103:
            // Pop and Store Receiver Variable #iii
            popAndStoreReceiverVariable(frame, (byte & 0b00000111));
            return;
        case 104:
        case 105:
        case 106:
        case 107:
        case 108:
        case 109:
        case 110:
        case 111:
            // Pop and Store Temporary Location #iii
            popAndStoreTemporaryLocation(frame, (byte & 0b00000111));
            return;
        case 112:
            pushReceiver(frame);
            return;
        case 113:
            pushTrue(frame);
            return;
        case 114:
            pushFalse(frame);
            return;
        case 115:
            pushNil(frame);
            return;
        case 116:
            pushMinusOne(frame);
            return;
        case 117:
            pushZero(frame);
            return;
        case 118:
            pushOne(frame);
            return;
        case 119:
            pushTwo(frame);
            return;
        case 120:
            returnReceiver(frame);
            return;
        case 121:
            returnTrue(frame);
            return;
        case 122:
            returnFalse(frame);
            return;
        case 123:
            returnNil(frame);
            return;
        case 124:
            returnStackTopFromMessage(frame);
            return;
        case 125:
            returnStackTopFromBlock(frame);
            return;
        case 128:
            extendedPush(frame);
            return;
        case 129:
            extendedStore(frame);
            return;
        case 130:
            extendedPopAndStore(frame);
            return;
        case 131:
            sendShort(frame);
            return;
        case 132:
            sendLong(frame);
            return;
        case 133:
            superSendShort(frame);
            return;
        case 134:
            superSendLong(frame);
            return;
        case 135:
            popStackTop(frame);
            return;
        case 136:
            duplicateStackTop(frame);
            return;
        case 137:
            pushActiveContext(frame);
            return;
        case 144:
        case 145:
        case 146:
        case 147:
        case 148:
        case 149:
        case 150:
        case 151:
            // Jump #iii + 1 (i.e., 1 through 8)
            jump(frame, (byte & 0b00000111) + 1);
            return;
        case 152:
        case 153:
        case 154:
        case 155:
        case 156:
        case 157:
        case 158:
        case 159:
            // Pop and Jump on False #iii +1 (i.e., 1 through 8)
            popAndJumpOnFalse(frame, (byte & 0b00000111) + 1);
            return;
        case 160:
        case 161:
        case 162:
        case 163:
        case 164:
        case 165:
        case 166:
        case 167:
            // Jump(iii - 4) *256+jjjjjjjj
            longJump(frame, (byte & 0b00000111) - 4);
            return;
        case 168:
        case 169:
        case 170:
        case 171:
            // Pop and Jump On True ii *256+jjjjjjjj
            longPopAndJumpOnTrue(frame, (byte & 0b00000011));
            return;
        case 172:
        case 173:
        case 174:
        case 175:
            // Pop and Jump On False ii *256+jjjjjjjj
            longPopAndJumpOnFalse(frame, (byte & 0b00000011));
            return;
        case 176:
        case 177:
        case 178:
        case 179:
        case 180:
        case 181:
        case 182:
        case 183:
        case 184:
        case 185:
        case 186:
        case 187:
        case 188:
        case 189:
        case 190:
        case 191:
            // Send Arithmetic Message #iiii
            sendArithmeticMessage(frame, (byte & 0b00001111));
            return;
        case 192:
        case 193:
        case 194:
        case 195:
        case 196:
        case 197:
        case 198:
        case 199:
        case 200:
        case 201:
        case 202:
        case 203:
        case 204:
        case 205:
        case 206:
        case 207:
            // Send Special Message #iiii
            sendSpecialMessage(frame, (byte & 0b00001111));
            return;
        case 208:
        case 209:
        case 210:
        case 211:
        case 212:
        case 213:
        case 214:
        case 215:
        case 216:
        case 217:
        case 218:
        case 219:
        case 220:
        case 221:
        case 222:
        case 223:
            // Send Literal Selector #iiii With No Arguments
            sendLiteralSelectorWithNoArguments(frame, (byte & 0b00001111));
            return;
        case 224:
        case 225:
        case 226:
        case 227:
        case 228:
        case 229:
        case 230:
        case 231:
        case 232:
        case 233:
        case 234:
        case 235:
        case 236:
        case 237:
        case 238:
        case 239:
            // Send Literal Selector #iiii With 1 Argument
            sendLiteralSelectorWith1Argument(frame, (byte & 0b00001111));
            return;
        case 240:
        case 241:
        case 242:
        case 243:
        case 244:
        case 245:
        case 246:
        case 247:
        case 248:
        case 249:
        case 250:
        case 251:
        case 252:
        case 253:
        case 254:
        case 255:
            // Send Literal Selector #iiii With 2 Arguments
            sendLiteralSelectorWith2Arguments(frame, (byte & 0b00001111));
            return;
    }
    fprintf(stderr, "Invalid type.\n");
    exit(-1);
}


ObjectPointer evaluate(Frame *frame) {
    while(!frame->hasReturned) {
        interpretNext(frame);
    }
    return frame->returnValue;
}

Method *lookupSelector(Class *class, const char *selector) {
    for (size_t i = 0; i < class->methodsSize; i++) {
        Method *method = class->methods[i];
        if (strcmp(method->name, selector) == 0) return method;
    }
    if (class->superClass) return lookupSelector(class->superClass, selector);
    fprintf(stderr, "Selector not found: #%s\n", selector);
    exit(-1);
}

ObjectPointer perform(ObjectMemory *om, ObjectPointer selfp, const char *selector) {
    Object *self = getObject(om, selfp);
    Method *method = lookupSelector(self->class, selector);

    Frame frame;
    frame.self = self;
    frame.selfp = selfp;
    frame.objectmemory = om;
    frame.method = method;
    frame.hasReturned = false;
    frame.returnValue = NULL;
    return evaluate(&frame);
}

