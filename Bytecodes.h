//
// Created by Wouter on 9/17/2018.
//

#ifndef SIMPLEVM_BYTECODES_H
#define SIMPLEVM_BYTECODES_H
/*
 * From: http://www.mirandabanda.org/bluebook/bluebook_chapter28.html
 *

The Smalltalk-80 Bytecodes
        Range
Bits
        Function
0-15	0000iiii	Push Receiver Variable #iiii
16-31	0001iiii	Push Temporary Location #iiii
32-63	001iiiii	Push Literal Constant #iiiii
64-95	010iiiii	Push Literal Variable #iiiii
96-103	01100iii	Pop and Store Receiver Variable #iii
104-111	01101iii	Pop and Store Temporary Location #iii
112-119	01110iii	Push (receiver, true, false, nil, -1, 0, 1, 2) [iii]
120-123	011110ii	Return (receiver, true, false, nil) [ii] From Message
124-125	0111110i	Return Stack Top From (Message, Block) [i]
126-127	0111111i	unused
128	10000000 jjkkkkkk	Push (Receiver Variable, Temporary Location, Literal Constant, Literal Variable) [jj] #kkkkkk
129	10000001 jjkkkkkk	Store (Receiver Variable, Temporary Location, Illegal, Literal Variable) [jj] #kkkkkk
130	10000010 jjkkkkkk	Pop and Store (Receiver Variable, Temporary Location, Illegal, Literal Variable) [jj] #kkkkkk
131	10000011 jjjkkkkk	Send Literal Selector #kkkkk With jjj Arguments
132	10000100 jjjjjjjj kkkkkkkk	Send Literal Selector #kkkkkkkk With jjjjjjjj Arguments
133	10000101 jjjkkkkk	Send Literal Selector #kkkkk To Superclass With jjj Arguments
134	10000110 jjjjjjjj kkkkkkkk	Send Literal Selector #kkkkkkkk To Superclass With jjjjjjjj Arguments
135	10000111	Pop Stack Top
136	10001000	Duplicate Stack Top
137	10001001	Push Active Context
138-143		unused
144-151	10010iii	Jump iii + 1 (i.e., 1 through 8)
152-159	10011iii	Pop and Jump 0n False iii +1 (i.e., 1 through 8)
160-167	10100iii jjjjjjjj	Jump(iii - 4) *256+jjjjjjjj
168-171	101010ii jjjjjjjj	Pop and Jump On True ii *256+jjjjjjjj
172-175	101011ii jjjjjjjj	Pop and Jump On False ii *256+jjjjjjjj
176-191	1011iiii	Send Arithmetic Message #iiii
192-207	1100iiii	Send Special Message #iiii
208-223	1101iiii	Send Literal Selector #iiii With No Arguments
224-239	1110iiii	Send Literal Selector #iiii With 1 Argument
240-255	1111iiii	Send Literal Selector #iiii With 2 Arguments
*/

#define PUSH_RECEIVER_VARIABLE_START 0
#define PUSH_RECEIVER_VARIABLE_END 15
#define PUSH_TEMPORARY_LOCATION_START 16
#define PUSH_TEMPORARY_LOCATION_END 31
#define PUSH_LITERAL_CONSTANT_START 32
#define PUSH_LITERAL_CONSTANT_END 63
#define PUSH_LITERAL_VARIABLE_START 64
#define PUSH_LITERAL_VARIABLE_END 95
#define POP_AND_STORE_RECEIVER_VARIABLE_START 96
#define POP_AND_STORE_RECEIVER_VARIABLE_END 103
#define POP_AND_STORE_TEMPORARY_LOCATION_START 104
#define POP_AND_STORE_TEMPORARY_LOCATION_END 111
#define PUSH_RECEIVER 112
#define PUSH_TRUE 113
#define PUSH_FALSE 114
#define PUSH_NIL 115
#define PUSH_MINUSONE 116
#define PUSH_ZERO 117
#define PUSH_ONE 118
#define PUSH_TWO 119
#define RETURN_RECEIVER 120
#define RETURN_TRUE 121
#define RETURN_FALSE 122
#define RETURN_NIL 123
#define RETURN_STACK_TOP_FROM_MESSAGE 124
#define RETURN_STACK_TOP_FROM_BLOCK 125
#define EXTENDED_PUSH 128
#define EXTENDED_STORE 129
#define EXTENDED_POP_AND_STORE 130
#define SEND 131
#define SEND_LONG 132
#define SUPER_SEND 133
#define SUPER_SEND_LONG 134
#define POP_STACK_TOP 135
#define DUPLICATE_STACK_TOP 136
#define PUSH_ACTIVE_CONTEXT 137
#define JUMP_START 144
#define JUMP_END 151
#define POP_AND_JUMP_ON_FALSE_START 152
#define POP_AND_JUMP_ON_FALSE_END 159
#define LONG_JUMP_START 160
#define LONG_JUMP_END 167
#define LONG_POP_AND_JUMP_ON_TRUE_START 168
#define LONG_POP_AND_JUMP_ON_TRUE_END 171
#define LONG_POP_AND_JUMP_ON_FALSE_START 172
#define LONG_POP_AND_JUMP_ON_FALSE_END 175
#define SEND_ARITHMETIC_MESSAGE_START 176
#define SEND_ARITHMETIC_MESSAGE_END 191
#define SEND_SPECIAL_MESSAGE_START 192
#define SEND_SPECIAL_MESSAGE_END 207
#define SEND_LITERAL_SELECTOR_NO_ARG_START 208
#define SEND_LITERAL_SELECTOR_NO_ARG_END 223
#define SEND_LITERAL_SELECTOR_ONE_ARG_START 224
#define SEND_LITERAL_SELECTOR_ONE_ARG_END 239
#define SEND_LITERAL_SELECTOR_TWO_ARG_START 240
#define SEND_LITERAL_SELECTOR_TWO_ARG_END 255



#endif //SIMPLEVM_BYTECODES_H
