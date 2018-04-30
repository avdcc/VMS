
#include <signal.h>
#include <sys/param.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "semantic.h"
#include "structs/code.h"
#include "structs/opStack.h"
#include "structs/callStack.h"
#include "structs/heap.h"

#define eprintf(...) fprintf(stderr,__VA_ARGS__)

extern FILE* yyin;
FILE *dbout;
GHashTable* labels = NULL;
Code code;
OpStack opstack;
CallStack callstack;
Heap heap;
int codeSize = 10000;
int opSize   = 1000;
int callSize = 100;
int heapSize = 1000;
int debug = 0;
int parseInitial = 0;
pid_t pidGui=0;
char *debuggerFunctionNames[] = {
    "file",
    "run",
    "next",
    "quit",
    NULL
};


int yyparse();

void try(int erro) {
    switch (erro) {
        case  0 : return;
        case -1 : eprintf("error:\n");          break;
        case -2 : eprintf("error: index out of array\n");    break;
        case -3 : eprintf("error: fp == sp\n");              break;
        case -4 : eprintf("error: opning file\n");           break;
        case -5 : eprintf("error: invalid args\n");          break;
        case -6 : eprintf("error: invalid type write\n");          break;
        case -7 : eprintf("error: invalid type not\n");          break;
        case -8 : eprintf("error: invalid type equal\n");          break;
        case -9 : eprintf("error: invalid type padd second arg not int\n");          break;
        case -10: eprintf("error: invalid type padd first argument not adress\n");          break;
        case -11: eprintf("error: invalid type alocn\n");          break;
        case -12: eprintf("error: invalid type free\n");          break;
        case -13: eprintf("error: invalid type atox arg is not heap adress\n");          break;
        case -14: eprintf("error: invalid type itof\n");          break;
        case -15: eprintf("error: invalid type ftoi\n");          break;
        case -16: eprintf("error: invalid type stri\n");          break;
        case -17: eprintf("error: invalid type strf\n");          break;
        case -18: eprintf("error: invalid type loadn\n");          break;
        case -19: eprintf("error: invalid type load\n");          break;
        case -20: eprintf("error: invalid type dupn\n");          break;
        case -21: eprintf("error: invalid type popn\n");          break;
        case -22: eprintf("error: invalid type pon\n");          break;
        case -23: eprintf("error: invalid type storen\n");          break;
        case -24: eprintf("error: invalid type jz\n");          break;
        case -25: eprintf("error: invalid type call\n");          break;
    }
    _exit(erro);
}

void runInst(CodeElem ce){
    Value f = ce->first;
    Value s = ce->second;

    switch(ce->inst){

        case NOT    : semNot();     break;//v
        case EQUAL  : semEqual();   break;//v

      //-Operacoes Sobre Inteiros----------------------//
        case ADD    : semAdd();     break;//v
        case SUB    : semSub();     break;//v
        case MUL    : semMul();     break;//v
        case DIV    : semDiv();     break;//v
        case MOD    : semMod();     break;//v

        case INF    : semInf();     break;//v
        case INFEQ  : semInfeq();   break;//v
        case SUP    : semSup();     break;//v
        case SUPEQ  : semSupeq();   break;//v

      //-Operacoes Sobre Flutuantes--------------------//
        case FADD   : semFadd();  	break;//v
        case FSUB   : semFsub();  	break;//v
        case FMUL   : semFmul();  	break;//v
        case FDIV   : semFdiv();  	break;//v

        case FSIN   : semFsin();  	break;//v
        case FCOS   : semFcos();  	break;//v
        case FTAN   : semFcos();  	break;//v

        case FINF   : semFinf();  	break;//v
        case FINFEQ : semFinfeq();  break;//v
        case FSUP   : semFsup();  	break;//v
        case FSUPEQ : semFsupeq();  break;//v

      //-Operacoes Sobre Enderecos---------------------//
        case PADD   : semPadd();  	break;//v

      //-Operacoes Sobre Cadeias de Caracteres---------//
        case CONCAT : semConcat();  break;//v

      //-Operacoes Sobre Heap--------------------------//
        case ALLOC  : semAlloc(f.val.i);break;//v
        case ALLOCN : semAllocn();  break;//v
        case FREE   : semFree();  	break;//v

      //-Conversoes------------------------------------//
        case ATOI   : semAtoi();  	break;//v
        case ATOF   : semAtof();  	break;//v
        case ITOF   : semItof();  	break;//v
        case FTOI   : semFtoi();  	break;//v
        case STRI   : semStri();  	break;//v
        case STRF   : semStrf();  	break;//v

      //-Empilhar--------------------------------------//
        case PUSHI  : semPushi(f);break;//v
        case PUSHN  : semPushn(f.val.i);break;//v
        case PUSHF  : semPushf(f);break;//v
        case PUSHS  : semPushs(f.val.s);break;//v
        case PUSHG  : semPushg(f.val.i);break;//v
        case PUSHL  : semPushl(f.val.i);break;//v
        case PUSHSP : semPushsp();  break;//v
        case PUSHFP : semPushfp();  break;//v
        case PUSHGP : semPushgp();  break;//v
        case LOAD   : semLoad(f.val.i);  	break;//x
        case LOADN  : semLoadn();   break;//x
        case DUP    : semDup(f.val.i);  break;//v
        case DUPN   : semDupn();  	break;//v

      //-Retirar---------------------------------------//
        case POP    : semPop(f.val.i);  break;//v
        case POPN   : semPopn();  	break;//v

      //-Arquivar--------------------------------------//
        case STOREL : semStorel(f.val.i);   break;//x
        case STOREG : semStoreg(f.val.i);   break;//x
        case STORE  : semStore(f.val.i);    break;//x
        case STOREN : semStoren();          break;//x

      //-Diversos--------------------------------------//
        case CHECK  : semCheck(f.val.i, s.val.i);   break;//x
        case SWAP   : semSwap();  	                break;//v

      //-Input/Output----------------------------------//
        case WRITE  : semWrite(-1);     break;//x
        case WRITEI : semWritei();      break;//x
        case WRITES : semWrites();      break;//x
        case WRITEF : semWritef();      break;//x

        case READ   : semRead();  	    break;//x

      //-Registo PC------------------------------------//
        case JUMP   : semJump(f.val.s); break;//v
        case JZ     : semJz(f.val.s);   break;//v
        case PUSHA  : semPusha(f.val.s);break;//v

      //-Procedimentos---------------------------------//
        case CALL   : semCall();  	break;//v
        case ARETURN : semReturn();  break;//v

      //-Inicializacao e Fim---------------------------//
        case START  : semStart();   break;//v
        case STOP   :               break;//v
        case NOP    : semNop();   	break;//v
        case ERR    : semErr();   	break;//x
    }
    code.pc++;
}

void freeStructs(){
    Code_free();
    OpStack_free();

    Code_init(codeSize);
    OpStack_init(opSize);
}

void runDebug(){
    CodeElem ce;
    char *input, *filename, path[MAXPATHLEN];
    int stop = 0, nInst=0, i=0;

    fprintf(dbout, "\n"); fflush(dbout);
    while(1){
        input = readline("(VMDB) ");
        if( !strncmp(input, "file ", 5) ){
            freeStructs();
            if(input[5] != '/'){ getwd(path); asprintf(&filename, "%s/%s", path, &input[5]); }
            else{ asprintf(&filename, "%s", &input[5]); }
            if((yyin = fopen(filename, "r"))<0) try(-1);
            free(filename);
            yyparse();
            fprintf(stdout, "\n"); fflush(stdout);

        }
        else if( !strncmp(input,"next ", 5) ){
            i=0;
            sscanf(input, "next %d", &nInst);
            while( i<nInst && !Code_get(&ce) && !stop ){
                if(ce->inst == STOP) stop = 1;
                runInst(ce);
                printCode(ce, '_', code.pc);
                i++;
            }
            fprintf(stdout, "\n"); fflush(stdout);
        }
        else if( !strncmp(input,"run", 3) ){
            while( !Code_get(&ce) && !stop ){
                if(ce->inst == STOP) stop = 1;
                runInst(ce);
                printCode(ce, '_', code.pc);
            }
            fprintf(stdout, "\n"); fflush(stdout);
        }
        else if( !strncmp(input,"quit", 4) ){
            _exit(0);
        }
        add_history(input);
        free(input);
    }
}

void runProgram(){
    CodeElem ce;
    int stop = 0;
    while( !Code_get(&ce) && !stop ){
        if(ce->inst == STOP) stop = 1;
        runInst(ce);
    }

}

void prepareDebug(){
    //fd = dup(1);
    dbout = fdopen(1, "w");
}

void execGui(){
    int wp[2], rp[2];

    if (pipe(wp)<0 || pipe(rp)<0) try(-4);
    if( (pidGui=fork()) ){//parent
        close(rp[1]);
        close(wp[0]);
        dup2(rp[0], 0);
        dup2(wp[1], 1);
    }
    else{//child
        close(rp[0]);
        close(wp[1]);
        dup2(rp[1], 1);
        dup2(wp[0], 0);
        execlp("./interface", "./interface", NULL);
    }
    prepareDebug();
}



void options(int argc, char** argv){// debug
    int i, j, k;
    if(argc < 2) try(-5);
    for(i=1; i<argc; i++){
        if(argv[i][0] == '-'){
            k=1;
            for(j=1; argv[i][j] != '\0'; j++){
                switch(argv[i][j]){
                    case 'c': codeSize = atoi(argv[i+k]); k++;  break;
                    case 'o': opSize   = atoi(argv[i+k]); k++;  break;
                    case 'C': callSize = atoi(argv[i+k]); k++;  break;
                    case 'h': heapSize = atoi(argv[i+k]); k++;  break;
                    case 'g': execGui();        debug = 1;      break;
                    case 'd': prepareDebug();   debug = 1;      break;
                }
            }
                i += k-1;
        }
        else{
            if((yyin = fopen(argv[i], "r"))<0) try(-1);
            if(debug) parseInitial=1;
        }
    }
}

void finish(){
    if(pidGui){
        close(0);
        close(1);
        kill(pidGui, SIGKILL);
    }
    _exit(-1);
}

void signals(){
    signal(SIGKILL, finish);
    signal(SIGQUIT, finish);
    signal(SIGSEGV, finish);
}

char* debuggerFunctionNames_generator(const char *text, int state){
    static int list_index, len;
    char *name;

    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    while ((name = debuggerFunctionNames[list_index++])) {
        if (strncmp(name, text, len) == 0) {
            return strdup(name);
        }
    }

    return NULL;
}

char** debuggerFunctionNames_completion(const char *text, int start, int end){
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, debuggerFunctionNames_generator);
}

int main(int argc, char** argv){
    rl_attempted_completion_function = debuggerFunctionNames_completion;
    dbout = fopen("/dev/null", "w");
    signals();
    options(argc, argv);
    Code_init(codeSize);
    OpStack_init(opSize);
    CallStack_init(callSize);
    Heap_init(heapSize);
    labels = g_hash_table_new(g_str_hash, g_str_equal);

    if(debug){
        if(parseInitial) yyparse();
        runDebug();
    }
    else{
        yyparse();
        runProgram();
    }


    return 0;
}
