#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "code.h"
#include "../interface.h"

extern Code code;
extern int try(int);

char* Code_toString(CodeElem ce){
    char* ret = (char*)malloc(MAX_LINE*sizeof(char));
    snprintf(ret, MAX_LINE, "%s %s %s", Inst_toString(ce->inst), Value_toString(ce->first), Value_toString(ce->second));
    return ret;
}

void Code_add(CodeElem ce){
    printCode(ce, '+', code.array.len);
    Array_add(&code.array, ce);
}

int Code_get(CodeElem* ret){
    return Array_getPos(&code.array, code.pc++, (void**)ret);
}

void Code_init(int size){
    Array_init(&code.array, size);
    code.pc = 0;
}

CodeElem newCodeElem(Einst inst, Value v1, Value v2){
    CodeElem ce = (CodeElem)malloc(sizeof(struct codeElem));

    ce->inst = inst;
    ce->first = v1;
    ce->second = v2;

    return ce;
}


void printCode(CodeElem ce, char signal, int index){
    /*char str[MAX_LINE];
    int len;
    len = snprintf(str, MAX_LINE, "CODE %c %d %s %d\n", signal,
                    index, 
                    Code_toString(ce),
                    code.pc
                   );
    if(len>MAX_LINE) try(1);
    write(1,str,len);
    */
    fprintf(stdout, "CODE %c %d %s %d\n", signal, index, Code_toString(ce), code.pc);
    fflush(stdout);
}
