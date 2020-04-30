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
const int commandLen = 50;
//Maximum Length of command.

const int help = 10;
const int dir = 11;
const int quit = 12;
const int history = 13;
const int dump = 14;
const int edit = 15;
const int fill = 16;
const int reset = 17;
const int opcode = 18;
const int opcodelist = 19;
//Above constants are needed to distinguish functions, thereby making codes more readable.

const int fail = -1;
const int success = 1;
//When functions succeed to make right result, returns success.
//In contrast, return fail when fail to make right result because of wrong command.

const int hasComma = -2;
const int noComma = -3;
// hasComma means a string includes comma at last.
// noComma means a string does not include comma.

const int memSize = 65536;
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
