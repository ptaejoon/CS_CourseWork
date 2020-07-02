#ifndef _HEADER1_H_
#define _HEADER1_H_
#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
typedef struct hist{
    char command[500];
    struct hist *next;
}historyLink;
//Structure to save history.

typedef struct opc{
    char op[10];
    int format[2];
    int opCode;
    struct opc *next;
}opCode;
extern const int commandLen;
//Maximum Length of command.
extern const int help;
extern const int dir ;
extern const int quit ;
extern const int history ;
extern const int dump ;
extern const int edit ;
extern const int fill ;
extern const int reset ;
extern const int opcode ;
extern const int opcodelist ;
extern const int assemble ;
extern const int type ;
extern const int symbol ;
//Above constants are needed to distinguish functions, thereby making codes more readable.
extern const int progaddr;
extern const int loader;
extern const int run;
extern const int bp;

extern const int fail ;
extern const int success ;
//When functions succeed to make right result, returns success.
//In contrast, return fail when fail to make right result because of wrong command.

extern const int hasComma;
extern const int noComma;
// hasComma means a string includes comma at last.
// noComma means a string does not include comma.

extern const int memSize;
// memSize is total number of 16 bytes array.
// By defining memSize=65536, I can allocate 1MB memory.

extern char **sicMemory;
extern int dumpAddress;
extern opCode* opHead[20];
extern historyLink* historyHead;
// Defining History link, opcode, memory address
int executeHelp();
int executeDir();
int executeHistory();
int executeDump();
int executeEdit();
int executeFill();
int executeReset();
int executeOpcode();
int executeOpcodelist();
int validDumpArg();
int validEditArg();
int validOpcodeArg();
int validFillArg();
int hashFunc();
int checkComma();
int stringToHex();
int checkFirstToken();
int checkSecondToken();
int executeCommand();
void addHistory();
void dumpNoArg();
void dumpWithArgs();
int getOrder();
void freeHistory();
void sicMemoryAllocation();
void sicMemoryFree();
void opcodeInitialize();
void opcodeFree();
void buildOpTable();
//pre-defining functions. Will explain on c file.

//-------------------------project 2

int validAssembleArg();
int validTypeArg();
int executeAssemble();
int executeType();
int executeSymbol();


//-------------------------project 3

int validBpArg();
int validProgaddrArg();
int validLoaderArg();
int executeRun();
int executeLoader();
int executeProgaddr();
int executeBp();


#endif
