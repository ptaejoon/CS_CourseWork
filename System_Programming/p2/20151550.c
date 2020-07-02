#include "20151550.h"
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
//const int assemble = 20;
//const int type = 21;
//const int symbol = 22;
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


char **sicMemory;

// Memory for sic machine.

int dumpAddress = 0;
historyLink* historyHead;
opCode* opHead[20];
// Variables used for history and opcode.

// The variable is used to save command 'dump'. 
// Dump without additional arguments need record of showed memory last time.

int checkComma(char* str)
{
    if(str[strlen(str)-1] == ',')return success;
    else return fail;
}
//opCode* opHead[20];
//historyLink* historyLink;
/*
checkComma is a function to find comma at last element of string.
This function is only called in stringToHex function when needed.
if the string don't have comman, return fail.
*/

int stringToHex(char* str, int needComma)
{
    int count;
    int stringLength = strlen(str);
    if(needComma == hasComma)
    {
	stringLength = strlen(str)-1;
	if(checkComma(str) == fail)return fail;
    }
    for(count = 0; count < stringLength; count++)
    {
	if(str[count] >= '0' && str[count] <= '9')continue;
	if(str[count] >= 'a' && str[count] <= 'f')continue;
	if(str[count] >= 'A' && str[count] <= 'F')continue;
	return fail;
    }
    int value = strtol(str,NULL,16);
    return value;
}
/*
   stringToHex is a function that convert string to hexadecimal number.
   However, if string does not suffice hexadecimal, return fail.
   But in case of comma, the function get the need of comma and if it is needed, check it through checkComma function.
   In normal case, it returns hexadecimal value.
*/

int checkFirstToken(char *firstTok)
{
    //if(firstTok == NULL)return fail;
    if (strcmp("h",firstTok)== 0)return help;
    if (strcmp("help",firstTok)==0)return help;
    if (strcmp("d",firstTok)== 0)return dir;
    if (strcmp("dir",firstTok)==0)return dir;
    if (strcmp("q",firstTok)== 0)return quit;
    if (strcmp("quit",firstTok)==0)return quit;
    if (strcmp("hi",firstTok)== 0)return history;
    if (strcmp("history",firstTok)==0)return history;
    if (strcmp("du",firstTok)== 0)return dump;
    if (strcmp("dump",firstTok)==0)return dump;
    if (strcmp("e",firstTok)== 0)return edit;
    if (strcmp("edit",firstTok)==0)return edit;
    if (strcmp("f",firstTok)== 0)return fill;
    if (strcmp("fill",firstTok)==0)return fill;
    if (strcmp("reset",firstTok)== 0)return reset;
    if (strcmp("opcode",firstTok)==0)return opcode;
    if (strcmp("opcodelist",firstTok)== 0)return opcodelist;
    if (strcmp("type",firstTok)==0)return type;
    if (strcmp("symbol",firstTok)==0)return symbol;
    if (strcmp("assemble",firstTok)==0)return assemble;
    return fail;
}
/*
    checkFirstToken check validation of first token.
    First token in this function means a first argument of command string.
    Ex.) In "dump 4, 37" command, the first token is dump.
    This function check whether first token is right command.
    If it is, return a code number of command.
    If not, return fail.
*/
int checkSecondToken(char *secTok,int firstTok)
{
    if (firstTok == dump)return firstTok;
    else if(firstTok == help || firstTok == dir || firstTok == quit || firstTok == history || firstTok ==reset||firstTok == opcodelist || firstTok == symbol)
    {
	if( secTok == NULL)return firstTok;
	else if(strlen(secTok) > 0)
	{
	    printf("This Command Does Not Contain Arguments\n");
	    return fail;
	}
	else return firstTok;
    }
    else
    {
	if(secTok == NULL || strlen(secTok) == 0)
	{
	    printf("This Command Needs More Arguments\n");
	    return fail;
	}
	else return firstTok;
    }
}
/*
    checkSecondToken check validation of second token.
    As explained above 'checkFirstToken' function, it check the second argument of string.
    Second string has speciality in that some command like help doesn't have second argument.
    Therefore, to commands don't have second argument, check if they really don't have second argument. if they have, return fail.
    To commands have second argument, if they actually don't have argument, return fail. 
    In right case, return command code.
*/

int executeCommand(int command, char* Arg2,char* Arg3,char* Arg4,char* Arg5,historyLink* Hist,opCode* Op[])
{
    if(command == help)return executeHelp();
    else if(command == dir)return executeDir();
    else if(command == history)return executeHistory(Hist);
    else if(command == dump){
	int dumpArgNum = validDumpArg(Arg2,Arg3,Arg4);
	if(dumpArgNum == fail)return fail;
	return executeDump(Arg2,Arg3,dumpArgNum);
    }
    else if(command == edit){
	int editArgNum = validEditArg(Arg2,Arg3,Arg4);
	if(editArgNum == fail)return fail;
	return executeEdit(Arg2,Arg3);
    }
    else if(command == fill){
	int fillArgNum = validFillArg(Arg2,Arg3,Arg4,Arg5);
	if(fillArgNum == fail)return fail;
	return executeFill(Arg2,Arg3,Arg4);
    }
    else if(command == reset)return executeReset();
    else if(command == opcode)
    {
	if(validOpcodeArg(Arg2,Arg3) == fail)return fail;
	return executeOpcode(Arg2,Op);
    }
    else if(command == opcodelist)
    {
	return executeOpcodelist(Op);
    }
    else if(command == assemble)
    {
	if(validAssembleArg(Arg2,Arg3) == fail)return fail;
	return executeAssemble(Arg2);
    }
    else if(command == type)
    {
	if(validTypeArg(Arg2,Arg3) == fail)return fail;	
	return executeType(Arg2);
    }
    else if(command == symbol)return executeSymbol();
    else return fail;
}
/*
   executeCommand get code of command, and 4 additional argument on command string.
   If there's no argument, get NULL.
   Hist and Op are data for history and opcode.
   According to command code, it executes function.
   In some functions, validate arguments before actually execute command.
   If these process has error, return fail.
   Or else, return code number of command.
*/

void addHistory(historyLink* head,char *record)
{

    historyLink* temp = head;
    while(temp->next != NULL)
    {
        temp=temp->next;
    }
    historyLink *newLink = (historyLink*)malloc(sizeof(historyLink));
    temp->next = newLink;
    strcpy(temp->next->command,record);
    temp->next->next = NULL;
    
}
/*
   addHistory function is called when a command successfully executed.
   Succeded execution means an "execute...." like function return success.
   By using linked list structure, it saves the command sequentially.
*/

int executeHelp()
{
    printf("h[elp]\n");
    printf("d[ir]\n");
    printf("q[uit]\n");
    printf("hi[story]\n");
    printf("du[mp] [start, end]\n");
    printf("e[dit] address, value\n");
    printf("f[ill] start, end, value\n");
    printf("reset\n");
    printf("opcode mnemonic\n");
    printf("opcodelist\n");
    printf("assemble filename\n");
    printf("type filename\n");
    printf("symbol\n");
    return success;
}
/*
   executeHelp is a function that executes help command.
   It prints all possible commands.
   After print, return success.
*/

int executeDir()
{
    struct dirent **dirList;
    struct stat exeCheck;
    int fileCount;
    int count;
    fileCount = scandir(".", &dirList, NULL, alphasort);
    for(count = 0; count < fileCount; count++)
    {
	printf("%s",dirList[count]->d_name);
	if(stat(dirList[count]->d_name,&exeCheck)==0 && exeCheck.st_mode & S_IFDIR)
	{
	    printf("/");
	}
	else if(stat(dirList[count]->d_name,&exeCheck)==0 && exeCheck.st_mode & S_IXUSR)
	{
	    printf("*");
	}
	printf("    ");
    }
    printf("\n");
    for(count = 0; count < fileCount; count++)
    {
	free(dirList[count]);
    }
    free(dirList);

    return success;
}
/*
   executeDir is a function that executes dir command.
   By using dirent, sys/stat library, get lists of current directory.
   Next, check bits of file status.
   If the file is directory, print it with '/'
   If the file is executable, print it with '*'
   Else, print the file name.
   After print, return success.
*/

int executeHistory(historyLink* head)
{
    historyLink* temp=head;
    int histNum = 1;
    if(head->next == NULL)
    {
	return success;
    }
    else
    {
	temp = temp->next;
    }
    while(temp->next != NULL)
    {
	printf("%-3d ",histNum);
	printf("%s\n",temp->command);
	temp = temp->next;
	histNum++;
    }
    printf("%-3d ",histNum);
    printf("%s\n",temp->command);
    return success;
}
/*
   executeHistory is a function that executes history command.
   The function needs head as parameter, which is a linked list.
   Head contains a record of successfully executed command.
   Until the record exists, print it.
   After printing, return success.
*/

int validDumpArg(char* Arg2,char* Arg3,char* Arg4)
{
    if(Arg4 != NULL)
    {
	printf("Too many arguments\n");
	return fail;
    }
    if(Arg2 == NULL)return 0;
    int argNum2 = stringToHex(Arg2,noComma);
    if (argNum2 == fail && Arg3 == NULL){
	printf("Invalid Command Format.\n");
	return fail;
    }
    if(Arg3 == NULL)return 1;
    argNum2 = stringToHex(Arg2,hasComma);
    int argNum3 = stringToHex(Arg3,noComma);
    if(argNum3 == fail || argNum2 == fail){
	printf("Invalid Command Format.\n");
	return fail;
    }
    return 2;
}
/*
   validDumpArg is a function that checks validity of arguments for Dump command.
   Use stringToHex function to find error on string format and to get hexadecimal value of arguments.
   In normal case, it returns number of argument, since dump can have 0-2 additional aguments.
*/

void dumpNoArg()
{
    int count;
    int byte;
    for(count = dumpAddress; count < dumpAddress+10; count++)
    {
	if(count >= memSize)
	{
	    dumpAddress=0;
	    return;
	}
	printf("%04x0 ",count);
	for(byte=0; byte < 16; byte++)
	{
	    printf("%02x ",sicMemory[count][byte]);
	}
	printf(";");
	for(byte=0; byte < 16; byte++)
	{
	    if(sicMemory[count][byte] == 0)printf(".");
	    else printf("%c",sicMemory[count][byte]);
	}
	printf("\n");
    }
    dumpAddress = count;
}
/*
   dumpNoArg is a print function when no additional argument exists.
   It uses dumpAddress to save last address of mememory executed before.
*/
void dumpWithArgs(int h1, int h2)
{
    int count;
    int byte;
    int start = h1/16;
    int end;
    if(h2 == -1)
    {
	end=start+10;
	if(h1 % 16 != 0)end += 1;
	h2 = h1+159;
    }
    else
    {
	end = h2/16+1;
	//if(h2 % 16 != 0)end += 1;
    }
    for(count = start; count < end; count++)
    {
	if(count >= memSize)
	{
	    dumpAddress=0;
	    return;
	}
	printf("%04x0 ",count);
	for(byte=0; byte < 16; byte++)
	{
	    if(count*16+byte < h1)printf("   ");
	    else if(count == end-1){
		if(byte > h2%16)printf("   ");
		else printf("%02x ",sicMemory[count][byte]);
	    }
	    else printf("%02x ",sicMemory[count][byte]);
	}
	printf("; ");
	for(byte=0; byte < 16; byte++)
	{
	    if(count*16+byte < h1)printf(".");
	    else if(count == end-1){
		if(byte > h2%16)printf(".");
		else if(sicMemory[count][byte] == 0)printf(".");
		else printf("%c",sicMemory[count][byte]);
	    }
	    else if(sicMemory[count][byte] == 0)printf(".");
	    else printf("%c",sicMemory[count][byte]);
	}
	printf("\n");
    }
}
/*
   dumpWithArgs is a print function when there are additional arguments in command string.
   When h2 is -1, it means only a argument were added.
   If h2 exists, it prints from h1 to h2.
*/

int executeDump(char* Arg2,char* Arg3,int argNum)
{
    int hex1;
    int hex2;
    if(argNum == 0)
    {
	dumpNoArg();
    }
    else if(argNum == 1)
    {
	hex1 = stringToHex(Arg2,noComma);
	if(hex1 < 0 || hex1 >= memSize*16)
	{
	    printf("Memory Out Of Bound\n");
	    return fail;
	}
	dumpWithArgs(hex1,-1);
    }
    else if(argNum == 2)
    {
	hex1 = stringToHex(Arg2,hasComma);
	hex2 = stringToHex(Arg3,noComma);
	if(hex1 < 0 || hex1 >= memSize*16)
	{
	    printf("Memory Out Of Bound\n");
	    return fail;
	}
	if(hex2 < 0 || hex2 >= memSize*16)
	{
	    printf("Memory Out Of Bound\n");
	    return fail;
	}
	if(hex1 > hex2)
	{
	    printf("Start Argument Should Not Smaller Than End Argument\n");
	    return fail;
	}
	dumpWithArgs(hex1,hex2);
    }
    return success;
}
/*
   executeDump is a function that executes dump command.
   Dump command need printed output. Therefore, it uses dumpWithArgs and dumpNoArg to print.
   Before execute these printing functions, it changes string formed arguments to hexadecimal using stringToHex.
   If there's error during the process, return fail.
*/

int validEditArg(char* Arg2,char* Arg3,char* Arg4)
{
    if(Arg4 != NULL){
	printf("Too many argument\n");
	return fail;
    }
    if(Arg2 == NULL){
	printf("Invalid Command Needs Address, Value.\n");
	return fail;
    }
    int argNum2 = stringToHex(Arg2,hasComma);
    if (argNum2 == fail)
    {
	printf("Invalid Command Format.\n");
	return fail;
    }
    if(Arg3 == NULL)
    {
	printf("Invalid Command Needs Address, Value.\n");
	return fail;
    }
    int argNum3 = stringToHex(Arg3,noComma);
    if(argNum3 == fail)
    {
	printf("Invalid Command Format.\n");
	return fail;
    }
    return success;
}
/*
   validEditArg is a function that validates arguments for edit command.
   Edit function needs 2 additional arguments. If the command string does not contain proper command, return fail.

*/

int executeEdit(char* Arg2, char* Arg3)
{
    int address = stringToHex(Arg2,hasComma);
    int value = stringToHex(Arg3,noComma);
    if(address < 0 || address >= memSize*16)
    {
	printf("Memory Out Of Bound.\n");
	return fail;
    }
    if(value < 0 || value >= 256)
    {
	printf("Value Out Of Bound.\n");
	return fail;
    }
    int line = address/16;
    int col = address%16;
    sicMemory[line][col] = value;
    return success;
}
/*
   executeEdit is a function that executes edit function.
   Edit changes value of allocated sic machine's memory.
   Since sic machine's memories are allocated through global memory, the function can directly access memory.
   If selected memory address is out of bound, return fail.
*/


int validFillArg(char* Arg2,char* Arg3,char* Arg4,char* Arg5)
{
    if(Arg5 != NULL)
    {
	printf("Too many argument\n");
	return fail;
    }
    if(Arg2 == NULL)
    {
	printf("Fill Needs 3 Arguments\n");
	return fail;
    }
    int argNum2 = stringToHex(Arg2,hasComma);
    if (argNum2 == fail)
    {
	printf("Invalid Command Format\n");
	return fail;
    }
    if(Arg3 == NULL)
    {
	printf("Fill Needs 3 Arguments\n");
	return fail;
    }
    int argNum3 = stringToHex(Arg3,hasComma);
    if(argNum3 == fail)
    {
	printf("Invalid Command Format\n");
	return fail;
    }
    
    if(Arg4 == NULL)
    {
	printf("Fill Needs 3 Arguments\n");
	return fail;
    }
    int argNum4 = stringToHex(Arg4,noComma);
    if(argNum4 == fail)
    {
	printf("Invalid Command Format.\n");
	return fail;
    }
    
    return success;
}
/*
   validFillArg is a function that checks validity of arguments for fill function.
   Fill function must have 3 additional arguments.
   Only when 3 arguments are exist and they are valid returns success.
*/

int executeFill(char* Arg2,char* Arg3, char* Arg4)
{
    int start = stringToHex(Arg2,hasComma);
    int end = stringToHex(Arg3,hasComma);
    int value = stringToHex(Arg4,noComma);
    if(start > end)
    {
	printf("Start Argument Should Not Bigger Than End Argument\n");
	return fail;
    }
    if(start < 0 || start >= memSize*16)
    {
	printf("Access Memory Out Of Range\n");
	return fail;
    }
    if(end < 0 || end >= memSize*16)
    {
	printf("Access Memory Out Of Range\n");
	return fail;
    }
    if(value < 0|| value >= 256)
    {
	printf("Value Out Of Bound\n");
	return fail;
    }
    int count;
    int line,col;
    for(count = start; count <= end; count++)
    {
	line = count/16;
	col = count%16;
	sicMemory[line][col] = value;
    }
    return success;

}
/*
   executeFill is a function that executes fill command.
   Fill command change memory value to Value argument from start to end address.
   If arguments are invalid, return fail and doesn't change memory and return fail.
   If valid, change the memory value of sicMemory and return success.
*/

int executeReset()
{
    int count;
    int count2;
    for(count = 0; count < memSize; count++)
    {
	for(count2 = 0; count2 < 16; count2++)
	   sicMemory[count][count2] = 0;
    }
    return success;
}
/*
    executeReset is a function that initalize all memory value to zero.
*/

int validOpcodeArg(char* Arg2, char* Arg3)
{
    if(Arg3 != NULL)
    {
	printf("Too many Argument\n");
	return fail;
    }
    if(Arg2 == NULL)
    {
	printf("Opcode Command Needs 1 Argument \n");
	return fail;
    }
    return success;
}
/*
   validOpcodeArg is a function that checks validity of arguments for opcode command.
   However, except cases for missing argument or redundant arguments, hash table can solve other exceptions.
   Therefore, this function only checks above cases.

*/
int executeOpcode(char* Arg2,opCode* Op[])
{
    int idx = hashFunc(Arg2);
    opCode* temp = Op[idx];
    while(temp->next != NULL)
    {
	temp = temp->next;
	if(strcmp(Arg2,temp->op) == 0)
	{
	    printf("opcode is %x\n",temp->opCode);
	    return success;
	}
    }
    if(strcmp(Arg2,temp->op) == 0)
    {
	printf("opcode is %x\n",temp->opCode);
	return success;
    }
    printf("There's No Opcode Matches.\n");
    return fail;
}
/*
   executeOpcode is a function that executes opcode command.
   it uses Op as parameter, which is 20 size hash table saving opcode commands.
   By using hashFunc, it can find matched index number of hash table.
   Explanation of hashFunc will be covered below.
   If there's no matched opcode, print no matched result and return fail.
   Otherwise, return success and print matched result.
*/

int executeOpcodelist(opCode* Op[])
{
    int count;
    opCode* temp;
    for(count = 0; count < 20; count++)
    {
	temp = Op[count];
	if(temp->next == NULL)continue;
	printf("%d : ",count);
	temp = temp->next;
	while(temp->next != NULL)
	{
	    printf("[%s,%x] ->",temp->op,temp->opCode);
	    temp = temp->next;
	}
	printf("[%s,%x]\n",temp->op,temp->opCode);
    }
    return success;

}
/*
   executeOpcodeList is a function that executes opcodelist command.
   opcodelist commands do only print the list of opcode.
   When this program start, the opcode automatically saved to linked list formed hash table Op.
   Therefore, this function do only, for 20 size hash table, print the opcode.
*/

int getOrder(historyLink* histHead, opCode* opHead[]){
    char command[500];
    char commandCopy[500];
    char *splitedCommand;
    char *secArg;
    char *thirdArg;
    char *fourthArg;
    char *fifthArg;
    int commandCode;
    int ArgCode;
    fgets(command,sizeof(command)-1,stdin);
    command[strlen(command)-1] = 0;
    strcpy(commandCopy,command);
    if(strlen(command) >= commandLen)
    {
	printf("Command Is Too Long\n");
	return fail;
    }
    
    splitedCommand = strtok(command," ");
    if(splitedCommand == NULL)return fail;
    commandCode = checkFirstToken(splitedCommand);
    if(commandCode == fail)
    {
	printf("Cannot Understand The Command. Please Check Help Command.\n");
	return fail;
    }

    secArg = strtok(NULL," ");
    ArgCode = checkSecondToken(secArg,commandCode);
    if(ArgCode == fail)
    {
	return fail;
    }
    thirdArg = strtok(NULL," ");
    fourthArg = strtok(NULL, " ");
    fifthArg = strtok(NULL, " ");
    if(executeCommand(commandCode,secArg,thirdArg,fourthArg,fifthArg,histHead,opHead) == success)
    {
	addHistory(histHead,commandCopy);
    }
    
    return commandCode;
}

/*
   getOrder is main function of command line.
   It is executed in while loop acting as command line.
   This function gets a string from user and execute command.
   Before executing command, it first cut string with space.
   And then, it checks validity of the command through above functions.
   For each command, it return commandCode to main function.
   If main function get commandCode as quit, it finishes the program.

*/

void freeHistory(historyLink *head)
{
    historyLink *temp=head;
    while(temp->next != NULL)
    {
	temp=temp->next;
	free(head);
	head = temp;
    }
    free(temp);

}
/*
   freeHistory function frees memory of linked list with historyLink structure.
   It is executed after quit command are entered, thereby while loop is terminated.
*/
void sicMemoryAllocation(){
    sicMemory = (char **)malloc(sizeof(char*)*memSize);
    int count;
    for(count = 0; count < memSize; count++)
    {
	sicMemory[count] = (char *)malloc(sizeof(char)*16);
    }
}
/*
   At the very start of program, sicMemoryAllocation is called to allocate 1MB memory for sic/xe machine.
*/

void sicMemoryFree()
{
    int count;
    for(count = 0; count < memSize; count++)
    {
	free(sicMemory[count]);
    }
    free(sicMemory);
}
/*
   sicMemoryFree function frees memory of sic/xe machine's allocated memory.
   It is executed after quit command are entered, thereby while loop is terminated.
*/

void opcodeInitialize(opCode* head[])
{
    int count;
    for(count = 0; count < 20; count++)
    {
	head[count] = (opCode*)malloc(sizeof(opCode));
	head[count]->next = NULL;
	head[count]->opCode = -1;
	head[count]->format[0] = 0;
	head[count]->format[1] = 0;
    }
}
/*
   opcodeInitialize is a function that initialize heads of hash table.
   It is executed at the very start of program.
*/

void opcodeFree(opCode* head[])
{
    opCode* temp;
    opCode* rear;
    int count;
    for(count = 0; count < 20; count++)
    {
	temp = head[count];
	rear = temp;
	while(temp->next != NULL)
	{
	    temp = temp->next;
	    free(rear);
	    rear = temp;
	}
    }
}
/*
   opcodeFree function frees memory of hash table for opcode.
   It is executed only after quit command are entered.
*/

int hashFunc(char *command)
{
    int count;
    int idx=0;
    for(count = 0; count < strlen(command);count++)
    {
	idx +=(int) command[count];
    }
    return idx%20;
}
/*
    hashFunc is an algorithm to save opcode through an efficient way in aspect of memory, time.
    When an opcode is entered, add all ascii value of each character.
    Then, return remainder of the value with 20.
    By using this function, it can find the index of hash table only with the string of opcode.
*/
void buildOpTable(opCode* head[])
{
    FILE* opTxt;
    char command[10];
    char format[10];
    int code;
    int hashIdx;
    opCode* temp;
    opCode* allocator;
    opTxt = fopen("opcode.txt","r");
    if(opTxt == NULL)printf("opcode.txt does not exist. You cannot execute opcode_concerned command.");
    while(fscanf(opTxt,"%x  %s	%s\n",&code,command,format) != EOF)
    {
	hashIdx = hashFunc(command);
	
	temp = (opCode*)malloc(sizeof(opCode));
	allocator = head[hashIdx];
	while(allocator->next != NULL)allocator = allocator->next;
	allocator->next = temp;
	allocator = allocator->next;
	allocator->next = NULL;
	allocator->opCode = code;
	strcpy(allocator->op,command);
	if(strlen(format) == 1)allocator->format[0] = atoi(format);
	else{
	    allocator->format[0] = 3;
	    allocator->format[1] = 4;
	}
    }
}
/*
    buildOpTable function get head pointers of hash tables.
    It opens opcode.txt and read data of opcode.
    Using hashFunc, it saves data to 20 sized hash table.
    it is called at the very start of program.
*/

int main(){
    int commandStatus;
    //historyLink* historyHead;
    //opCode* opHead[20];
    opcodeInitialize(opHead);
    buildOpTable(opHead);
    historyHead = malloc(sizeof(historyLink));
    sicMemoryAllocation();
    while(1){
	printf("sicsim> ");
	commandStatus = getOrder(historyHead,opHead);
	if (commandStatus == quit)break;
    }
    opcodeFree(opHead);
    sicMemoryFree();
    freeHistory(historyHead);
    return 0;
}
/*
   main function do only initialization, start of while loop, and deallocation of memory
*/
