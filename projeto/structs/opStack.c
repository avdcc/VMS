#include <stdlib.h>
#include "opStack.h"

#include <stdio.h>


extern OpStack opstack;

extern void try(int);

int OpStack_pop(OperandElem* ret){
    if(opstack.sp == opstack.fp) return -1;
    opstack.sp -= 1;
    try(Array_remove(&(opstack.stack), opstack.sp, (void**)ret));
    printOpStack(*ret, '-');
    return 0;
}

int OpStack_top(OperandElem* ret){
    return Array_getPos(&(opstack.stack), opstack.sp-1, (void**)ret);
}

int OpStack_getPos(int index, OperandElem* ret){
    return Array_getPos(&(opstack.stack), index, (void**)ret);
}

int OpStack_addPos(int index, OperandElem oe){
    return Array_addPos(&(opstack.stack), index, oe);
}

void OpStack_push(OperandElem oe){
    Array_add(&(opstack.stack), oe);
    if(opstack.flagGlobal)  opstack.gp += 1;
    else                    opstack.sp += 1;
    printOpStack(oe, '+');
}


void OpStack_init(int size){
    Array_init(&(opstack.stack), size);
    opstack.sp = 0;
    opstack.fp = 0;
    opstack.gp = 0;
    opstack.flagGlobal = 1;
}


OperandElem newOperandElem(Value v){
    OperandElem oe = (OperandElem)malloc(sizeof(struct operandElem));
    oe->val = v;
    return oe;
}

void printOpStack(OperandElem oe, char signal){
    printf("OPSTACK\t\t(%d,%d,%d)\t\t%c%s\n", opstack.sp, opstack.fp, opstack.gp, signal,Value_toString(oe->val));
}

