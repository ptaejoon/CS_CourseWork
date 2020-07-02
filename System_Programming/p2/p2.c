#include "20151550.h"
/*
    I wrote every comments in English in case of encoding issues.
*/

const int assemble = 20;
const int type = 21;
const int symbol = 22;

//additional command number for new commands. 

typedef struct symt
{
    char label[20];
    int addr;
    struct symt *next;
}symbolTable;

//data structure for symbol command.

typedef struct modifier
{
    int loc;
    struct modifier *next;
}modRecord;

//data structure for mod section in pass2, obj file.

symbolTable* symtab=NULL;
//variable for symbol command.

const int A = 0;
const int X = 1;
const int L = 2;
const int B = 3;
const int S = 4;
const int T = 5;
const int F = 6;
const int PC = 8;
const int SW = 9;
//register number
int objOneLine[31];
const int blank = -1;
int executePass1();
int executePass2();
int printSymtab();
void addSymtab();
int objLen=0;
// objOneLine and objLen is used at pass2, saving hexadecimal values of one object code line.
// objOneLine works as a queue and objLen is used as index of rear.

opCode* isOpcode(char* str)
{
    int idx = hashFunc(str);
    opCode* temp = opHead[idx];
    while(temp->next != NULL)
    {
	temp = temp->next;
	if(strcmp(str,temp->op) == 0)
	{
	    return temp;
	}
    }
    if(strcmp(str,temp->op) == 0)
    {
	return temp;
    }
    return NULL;
}
/*
    isOpcode is function returning a opCode structure itself that matches it's op and string.
*/

int dictionaryOrder(char* newWord,char* dict)
{
    int count;
    int left = -1;
    int right = 1;
    for(count = 0; count < strlen(newWord); count++)
    {
	if(strlen(dict)<count)return right;
	else if(newWord[count] < dict[count])return left;
	else if(newWord[count] > dict[count])return right;
    }
    return right;
}
/*
   In symbol, you need to show symbol in dictionary order. 
   Therefore, while inserting to linked list, save nodes in dictionary order to print it easily.
   This function gets two string, which is new node's symbol string, and existing node's symbol string.
   it returns which one is prior.
*/

void addSymtab(symbolTable* curTable,char* label,int addr) // abcd order
{
    symbolTable* temp= curTable;
    symbolTable* head;
    symbolTable* newSymbol;
    
    while(temp->next != NULL)
    {
	head = temp;
	temp = temp->next;
	if(dictionaryOrder(label, temp->label) == -1)
	{
	    newSymbol = (symbolTable*)malloc(sizeof(symbolTable));
	    strcpy(newSymbol->label,label);
	    newSymbol->addr = addr;
	    newSymbol->next = NULL;
	    head->next = newSymbol;
	    newSymbol->next = temp;
	    return;
	}
    }
    newSymbol = (symbolTable*)malloc(sizeof(symbolTable));
    strcpy(newSymbol->label,label);
    newSymbol->addr = addr;
    newSymbol->next = NULL;
    temp->next = newSymbol;

}
/*
    In symbol, you need to show symbol in dictionary order. 
    Therefore, while inserting to linked list, save nodes in dictionary order to print it easily.
    This function gets new node, and compare it's order with existing one by using dictionaryOder function.
    When the order is correct, insert the new node to linked list.
*/

int findLabel(symbolTable* symtab,char* label)
{
    if(symtab == NULL)
    {
	return fail;
    }
    symbolTable* temp = symtab;
    while(temp->next != NULL)
    {
	if(strcmp(temp->label,label) == 0)
	{
	    return temp->addr;
	}
	temp = temp->next;
    }
    if(strcmp(temp->label,label) == 0)return temp->addr;
    return fail;
}
/*
    findLabel returns the location of a label. if the label does not exists, return fail.
*/

int matchRegisterNumber(char* reg)
{
    if(reg[0] == 'X')return X;
    if(reg[0] == 'A')return A;
    if(reg[0] == 'L')return L;
    if(reg[0] == 'B')return B;
    if(reg[0] == 'S')return S;
    if(reg[0] == 'T')return T;
    if(reg[0] == 'F')return F;
    if(reg[0] == 'P' && reg[1] == 'C') return PC;
    if(reg[0] == 'S' && reg[1] == 'W') return SW;
    return fail;
}
/*
   matchRegisterNumber simply compare string, matches string with it's register's number.
   If the register does not exist, return fail;
*/

void addModRecord(modRecord* md,int loc)
{
    modRecord* newNode = (modRecord*)malloc(sizeof(modRecord));
    newNode->loc = loc+1;
    newNode->next =NULL;
    modRecord* temp=md;
    while(temp->next !=NULL)
    {
	temp = temp->next;
    }
    temp->next = newNode;
}
/*
    ModRecord means a format 4 object code that needs modification of it's location.
    The md is a linked list that save modification target until object file finishes writing T row.
    This function is used for save a new ModRecord.
*/

void objWrite(FILE* objfp,int loc)
{
    int i;
    fprintf(objfp,"%02x",objLen);
    for(i = 0; i < objLen; i++)
    {
	if(objOneLine[i] == blank)continue;
	fprintf(objfp,"%02x",objOneLine[i]);
    }
    fprintf(objfp,"\n");
    fprintf(objfp,"T%06x",loc);
}
/*
    Write a T line of objCode.
    objWrite contains writing next row's location counter.
*/

void objWriteEnd(FILE* objfp)
{
    int i;
    fprintf(objfp,"%02x",objLen);
    for(i = 0; i < objLen; i++)
    {
	if(objOneLine[i] == blank)continue;
	fprintf(objfp,"%02x",objOneLine[i]);
    }
    fprintf(objfp,"\n");
}
/*
    Same function as objWrite.
    Only difference is objWriteEnd does not write next row's location counter.
*/

void writeMod(modRecord* md,FILE* fp)
{
    if(md == NULL)return ;
    modRecord* temp = md;
    while(temp->next != NULL)
    {
	temp = temp->next;
	fprintf(fp,"M%06x05\n",temp->loc);
    }
    return ;
}
/*
   writeMod is a function that write whole part of M lines.
*/

/*
    nixbpeCheck does check nixbpe part of instruction. Then it prints to file.
    However, concerned with obj file, since T and location, length of one line has to be written first, save object code to
    objOneLine first.
    If condition for writing fulfiiled, write it to obj file.
    The algorithm process is below.
    1. Check the format of command. if it's format 1 or 2, save to objOneline following format's saving method.
    2. if it's format 3 or 4, calculate and save nixbpe part following SIC/XE instruction.
    3. get object code of disp part.
    4. at each stage, write object code to lst file, and save the value of object code of obj file.
*/

int nixbpeCheck(opCode* op,char* tar,char* secTar,symbolTable* symtab,int base,int pc, int isform4,FILE* lstfp,FILE* objfp,modRecord* md)
{
    //fprintf(lstfp,"	    ");
    if(op->format[0] == 1)
    {
	char temp1 = op->opCode;
	fprintf(lstfp,"%02x",op->opCode);
	if(objLen < 30)
	{
	    objOneLine[objLen] = temp1;
	    objLen++;
	}
	else 
	{
	    objWrite(objfp,pc);
	    objOneLine[0] = temp1;
	    objLen += 1;
	}
	return success;
    }
    else if(op->format[0] == 2)
    {
	int temp2[2];
	temp2[0] = op->opCode;
	fprintf(lstfp,"%02x",op->opCode);
	int regnum1;
	int regnum2;
	if(tar[strlen(tar)-1] == ',')
	{
	    regnum1 = matchRegisterNumber(tar);
	    regnum2 = matchRegisterNumber(secTar);
	}
	else
	{
	    regnum1 = matchRegisterNumber(tar);
	    regnum2 = 0;
	}
	if(regnum1 == fail)
	{
	    printf("Error on Register.\n");
	    return fail;
	}
	fprintf(lstfp,"%x%x",regnum1,regnum2);
	temp2[1] = regnum1*16+regnum2;
	if(objLen + 2 < 30)
	{
	    objOneLine[objLen] = temp2[0];
	    objOneLine[objLen+1] = temp2[1];
	    objLen += 2;
	}
	else 
	{
	    objWrite(objfp,pc);
	    objOneLine[0] = temp2[0];
	    objOneLine[1] = temp2[1];
	    objLen = 2;
	}
	return success;
    }
    int codeSize=0;
    int objCode[4];
    //format 1 and 2
    if(tar == NULL)
    {
	fprintf(lstfp,"%02x",(op->opCode)+3);
	fprintf(lstfp,"%04x", 0);
	codeSize += 3;
	objCode[0] = op->opCode + 3;
	objCode[1] = 0;
	objCode[2] = 0;
	if(codeSize + objLen < 30)
	{
	    objOneLine[objLen] = objCode[0];
	    objOneLine[objLen+1] = objCode[1];
	    objOneLine[objLen+2] = objCode[2];
	    objLen += 3;
	}
	else
	{
	    objWrite(objfp,pc-3);
	    objOneLine[0] = objCode[0];
	    objOneLine[1] = objCode[1];
	    objOneLine[2] = objCode[2];
	    objLen += 3;
	}
	return success;
    }
    int xbpe = 0;
    if(isform4 == 1)
    {
	xbpe += 1;
	if(tar[0] != '#')
	{
	    addModRecord(md, pc - op->format[1]);
	}
    }
    if(tar[0] == '#')
    {
	fprintf(lstfp,"%02x",(op->opCode)+1);
	tar = tar+1;
	codeSize += 1;
	objCode[0] = op->opCode + 1;
    }
    else if(tar[0] == '@')
    {
	fprintf(lstfp,"%02x",(op->opCode)+2);
	tar = tar+1;
	codeSize += 1;
	objCode[0] = op->opCode+2;
    }
    else
    {
	fprintf(lstfp,"%02x",op->opCode + 3);
	codeSize += 1;
	objCode[0] = op->opCode + 3;
    }
    
    char* tok;
    char tarCopy[20];
    int loc;
    
    tok = strtok(tar," ");
    if(tok[strlen(tok)-1] == ',')
    {
	if(secTar == NULL)
	{
	    printf("Error : following value is missing \n");
	    return fail;
	}
	tok[strlen(tok)-1] = 0;
	xbpe += 8;
	loc = findLabel(symtab,tok);
	strcpy(tarCopy,tok);
    }
    else
    {
	loc = findLabel(symtab,tar);
	strcpy(tarCopy,tar);
    }
    if(loc == fail)
    {
	if(atoi(tarCopy) == 0 && strlen(tarCopy) != 1)
	{
	    printf("Error : Target is not Appropriate Value\n");
	    return fail;
	}
	else
	{
	    fprintf(lstfp,"%x",xbpe);
	    if(isform4 == 0)
	    {
		codeSize += 2;
		fprintf(lstfp,"%03x",atoi(tarCopy));
		objCode[1] = xbpe*16 + ((atoi(tarCopy)&0xF00)>>8);
		objCode[2] = atoi(tarCopy)&0x0FF;
	    }
	    else
	    {
		codeSize += 3;
		fprintf(lstfp,"%05x",atoi(tarCopy));
		objCode[1] = xbpe*16 + ((atoi(tarCopy)&0xF0000)>>16);
		objCode[2] = (atoi(tarCopy)&0xFF00)>>8;
		objCode[3] = atoi(tarCopy)&0x000FF;
	    }
	}
    }
    else
    {
	if(loc - pc < 0x1000 && pc - loc < 0x1000)
	{
	    xbpe += 2;
	    fprintf(lstfp,"%x",xbpe);
	    fprintf(lstfp,"%03x",(loc-pc)&0xFFF);
	    codeSize += 2;
	    objCode[1] = ((loc-pc)&0xF00)/0x100;
	    objCode[1] += xbpe*16;
	    objCode[2] =(loc-pc)&0x0FF;


	}
	else if(base - loc < 0x1000 && loc - base < 0x1000)
	{
	    xbpe += 4;
	    fprintf(lstfp,"%x",xbpe);
	    fprintf(lstfp,"%03x",(loc-base)&0xFFF);
	    codeSize += 2;
	    objCode[1] = ((loc-base)&0xf00)/0x100;
	    objCode[1] += xbpe*16;
	    objCode[2] = (loc-base)&0x00ff;

	}
	else
	{
	    fprintf(lstfp,"%x",xbpe);
	    fprintf(lstfp,"%05x",loc);
	    codeSize += 3;
	    objCode[1] = xbpe*16 +(((loc)&0xF0000)>>16);
	    objCode[2] = (loc&0x0FF00)>>8;
	    objCode[3] = loc&0x000FF;
	}


    }
    if(objLen + codeSize < 30)
    {
	objOneLine[objLen] = objCode[0];
	objOneLine[objLen+1] = objCode[1];
	objOneLine[objLen+2] = objCode[2];
	if(codeSize == 4)objOneLine[objLen+3] = objCode[3];
	objLen += codeSize;
    }
    else
    {
	if(isform4==1)objWrite(objfp,pc-4);
	else objWrite(objfp,pc-3);
	objOneLine[0] = objCode[0];
	objOneLine[1] = objCode[1];
	objOneLine[2] = objCode[2];
	if(codeSize == 4)objOneLine[objLen+3] = objCode[3];
	objLen = codeSize;
    }
    return success;

}
/*
    executePass2 works as an algorithm for SIC machine called PASS2.
    It calculates object code.
    It writes object code, list file, check if symbol does have error.
    Some part of code is written through nixbpecheck function.
    The algorithm process is below.
    1. get a line of asm file.
    2. set location counter.
    3. check whether the first token is symbol or opcode.
    4. if it's opcode but RESW,WORD,BYTE,RESB, add location counter as it's size and write it on objOneLine.
    5. if it's symbol, check next token as opcode. 
    6. if the opcode is not RESW,WORK,BYTE,RESB, execute nixbpecheck function to find object code
*/


int executePass2(FILE* fp, symbolTable* tempSymtab,int base,int codeLength,int startExist,FILE* lstfp,FILE* objfp,modRecord* md)
{
    char asmLine[50];
    char firstLine[50];
    char programName[20];
    char *lineTok;
    char *secArg;
    int startAddr = 0x0000;
    int LOCCTR = 0x0000;
    int lineNum = 1;
    int iter;
    int reserveFlag = 0;
    opCode* opTemp;
    if(fp == NULL)
    {
	printf("File Does Not Exists!\n");
	return fail;
    }
    fgets(asmLine,sizeof(asmLine),fp);
    asmLine[strlen(asmLine)-1] = 0;
    strcpy(firstLine,asmLine);
    lineTok = strtok(asmLine," ");
    strcpy(programName,lineTok);
    lineTok = strtok(NULL," ");
    if(lineTok != NULL && strcmp(lineTok,"START") == 0)
    {
	lineTok = strtok(NULL," ");
	LOCCTR= strtol(lineTok,NULL,16);
	startAddr = LOCCTR;
    }
    fprintf(lstfp,"%-3d    %04x    %s\n",lineNum*5,LOCCTR,firstLine);
    fprintf(objfp,"H%-6s  %06x%06x\n",programName,startAddr,codeLength);
    lineNum++;
    fprintf(objfp,"T%06x",LOCCTR);
    while(1)
    {
	fgets(asmLine,sizeof(asmLine)-1,fp);
	asmLine[strlen(asmLine)-1]=0;
	if(asmLine[0] == '.')
	{
	    fprintf(lstfp,"%-3d %s\n",lineNum*5,asmLine);
	    lineNum++;
	    continue;
	}
	fprintf(lstfp,"%-3d    %04x    %-40s",lineNum*5,LOCCTR,asmLine);
	lineTok = strtok(asmLine," ");
	if(strcmp(lineTok,"END")==0)
	{
	    if(objLen != 0)objWriteEnd(objfp);
	    fprintf(lstfp,"\n");
	    break;
	}
	else if((opTemp=isOpcode(lineTok))!= NULL)
	{
	    if(reserveFlag == 1)
	    {
		fprintf(objfp,"T%06x",LOCCTR);
		reserveFlag = 0;
	    }
	    lineTok = strtok(NULL," ");
	    secArg = strtok(NULL," ");
	    LOCCTR+=opTemp->format[0];
	    if(nixbpeCheck(opTemp,lineTok,secArg,tempSymtab,base,LOCCTR,0,lstfp,objfp,md) == fail)
	    {
		printf("Failure On Line %d \n",lineNum*5);
		return fail;
	    }
	}
	else if((opTemp =isOpcode(lineTok+1))!= NULL&& lineTok[0] == '+')
	{ 
	    if(reserveFlag == 1)
	    {
		fprintf(objfp,"T%06x",LOCCTR);
		reserveFlag = 0;
	    }
	    lineTok = strtok(NULL," ");
	    secArg = strtok(NULL," ");
	    LOCCTR+=opTemp->format[1];
	    if(nixbpeCheck(opTemp,lineTok,secArg,tempSymtab,base,LOCCTR,1,lstfp,objfp,md) == fail)
	    {
		printf("Failure On Line %d \n",lineNum*5);
		return fail;
	    }
	}
	else if(strcmp(lineTok,"BASE")==0)
	{
	    fprintf(lstfp,"\n");
	    lineNum++;
	    continue;
	}
	else
	{
	    lineTok = strtok(NULL," ");
	    if((opTemp =isOpcode(lineTok))!= NULL)
	    {
		if(reserveFlag == 1)
		{
		    fprintf(objfp,"T%06x",LOCCTR);
		    reserveFlag = 0;
		}
		lineTok = strtok(NULL," ");
		secArg = strtok(NULL," ");
		LOCCTR += opTemp->format[0];
		if(nixbpeCheck(opTemp,lineTok,secArg,tempSymtab,base,LOCCTR,0,lstfp,objfp,md)==fail)
		{
		    printf("Failure On Line %d \n",lineNum);
		    return fail;
		}
		
	    }
	    else if((opTemp = isOpcode(lineTok+1)) != NULL&&lineTok[0] == '+')
	    {
		if(reserveFlag == 1)
		{
		    fprintf(objfp,"T%06x",LOCCTR);
		    reserveFlag = 0;
		}
		lineTok = strtok(NULL," ");
		secArg = strtok(NULL," ");
		LOCCTR+= opTemp->format[1];
		if(nixbpeCheck(opTemp,lineTok,secArg,tempSymtab,base,LOCCTR,1,lstfp,objfp,md)==fail)
		{
		    printf("Failure On Line %d \n",lineNum*5);
		    return fail;
		}
	    }
	    else if(strcmp(lineTok,"BASE")==0)
	    {
		lineNum++;
		continue;
	    }
	    else if(strcmp(lineTok,"WORD")==0)
	    {
		if(reserveFlag == 1)
		{
		    fprintf(objfp,"T%06x",LOCCTR);
		    reserveFlag = 0;
		}	
		lineTok = strtok(NULL," ");
		fprintf(lstfp,"%06x",atoi(lineTok));
		LOCCTR += 3;
		if(objLen+3 < 30){
		    objOneLine[objLen] = atoi(lineTok) & 0xf00;
		    objOneLine[objLen+1] = atoi(lineTok) & 0x0f0;
		    objOneLine[objLen+2] = atoi(lineTok) & 0x00f;
		}
		else
		{
		    objWrite(objfp,LOCCTR);
		    objLen = 3;
		    objOneLine[0] = atoi(lineTok) & 0xf00;
		    objOneLine[1] = atoi(lineTok) & 0x0f0;
		    objOneLine[2] = atoi(lineTok) & 0x00f;
		}
		//mod
	    }
	    else if(strcmp(lineTok,"BYTE")==0)
	    {
		if(reserveFlag == 1)
		{
		    fprintf(objfp,"T%06x",LOCCTR);
		    reserveFlag = 0;
		}
		lineTok = strtok(NULL," ");
		lineTok = strtok(lineTok,"'");
		if(lineTok[0] == 'C')
		{
		    lineTok = strtok(NULL,"'");
		    for(iter = 0; iter < strlen(lineTok);iter++)
		    {
			fprintf(lstfp,"%02x",lineTok[iter]);
		    }
		    LOCCTR += strlen(lineTok);
		    if(objLen + strlen(lineTok) < 30){
			int count;
			for(count = 0; count < strlen(lineTok); count++)
			{
			    objOneLine[count+objLen]=lineTok[count];
			}
			objLen += strlen(lineTok);
		    }
		    else
		    {
			printf("I am problem");
			objWrite(objfp,LOCCTR);
			int count;
			for(count = 0; count < strlen(lineTok); count++)
			{
			    objOneLine[count] = lineTok[count];
			}
			objLen = strlen(lineTok)+1;
		    }
		}
		else
		{
		    lineTok = strtok(NULL,"'");
		    fprintf(lstfp,"%s", lineTok);//두개씩 끊어서 나와야 하는 문제
		    LOCCTR += (int)(strlen(lineTok)/2);
		    if(objLen+strlen(lineTok) < 30){
			int count;
			objOneLine[objLen] = strtol(lineTok,NULL,16);
			for(count = objLen+1; count < objLen+strlen(lineTok)/2; count++)objOneLine[count] = blank;
			objLen += strlen(lineTok)/2;

		    }
		    else
		    {
			objWrite(objfp,LOCCTR);
			int count;
			objOneLine[objLen] = strtol(lineTok,NULL,16);
			for(count = objLen+1; count < objLen+strlen(lineTok)/2; count++)objOneLine[count] = blank;
			objLen = strlen(lineTok)/2+1;

		    }
		}
		//mod
	    }
	    else if(strcmp(lineTok,"RESW")==0)
	    {
		lineTok = strtok(NULL," ");
		LOCCTR += atoi(lineTok)*3;
		if(objLen != 0)objWriteEnd(objfp);
		objLen = 0;
		reserveFlag = 1;
	    }
	    else if(strcmp(lineTok,"RESB")==0)
	    {
		lineTok = strtok(NULL," ");
		LOCCTR += atoi(lineTok);
		if(objLen != 0)objWriteEnd(objfp);
		objLen = 0;
		reserveFlag = 1;
	    }
	    else
	    {
		printf("Undefined Instruction On Line : %d \n",lineNum);
		return fail;
	    }

	}
	fprintf(lstfp,"\n");
	lineNum += 1;
	//test
	//printf(" %04x %s\n",LOCCTR,asmLine);
    }
    writeMod(md,objfp);
    fprintf(objfp,"E%06x\n",startAddr);
    return success;
}

/*
    executePass1 works as an algorithm for SIC machine called PASS1.
    It counts location of command, save symbol, save length of code at last, and set base if it exists.
    Algorithm process is below.
    1. set location counter.
    2. get line of asm file and count address location according to command.
    3. if it's symbol, check it were duplicated.
    4. if it's symbol and new, save it to temporal symbol table.
*/
int executePass1(FILE* fp,symbolTable* tempSymtab,int *base,int *codeLength,int *startExist)
{
    char asmLine[50];
    char programName[20];
    char *lineTok;
    int startAddr = 0x0000;
    int LOCCTR = 0x0000;
    int lineNum = 1;
    char baseLabel[10];
    opCode* opTemp;
    if(fp == NULL)
    {
	printf("File Does Not Exists!\n");
	return fail;
    }
    fgets(asmLine,sizeof(asmLine),fp);
    asmLine[strlen(asmLine)-1] = 0;
    lineTok = strtok(asmLine," ");
    strcpy(programName,lineTok);
    lineTok = strtok(NULL," ");
    if(lineTok != NULL && strcmp(lineTok,"START") == 0)
    {
	lineTok = strtok(NULL," ");
	LOCCTR= strtol(lineTok,NULL,16);
	startAddr = LOCCTR;
	lineTok += 1;
	*startExist = 1;
    }
    while(1)
    {
	fgets(asmLine,sizeof(asmLine)-1,fp);
	asmLine[strlen(asmLine)-1]=0;
	lineTok = strtok(asmLine," ");
	if(lineTok[0] == '.')
	{
	    lineNum += 1;
	    continue;
	}
	if(strcmp(lineTok,"END")==0)
	{
	    break;
	}
	else if((opTemp=isOpcode(lineTok))!= NULL)
	{
	    if(strcmp(opTemp->op,"LDB") == 0)
	    {
		lineTok = strtok(NULL," ");
		if(lineTok[0] == '#')strcpy(baseLabel,lineTok+1);
		else strcpy(baseLabel, lineTok);
	    }
	    LOCCTR+=opTemp->format[0];
	}
	else if((opTemp =isOpcode(lineTok+1))!= NULL&& lineTok[0] == '+')
	{
	    if(strcmp(opTemp->op,"LDB") == 0)
	    {
		lineTok = strtok(NULL," ");
		if(lineTok[0] == '#')strcpy(baseLabel,lineTok+1);
		else strcpy(baseLabel, lineTok);
	    }
	    LOCCTR+=opTemp->format[1];
	}
	else if(strcmp(lineTok,"BASE")==0)
	{
	    lineTok = strtok(NULL," ");
	    strcpy(baseLabel,lineTok);
	}
	else
	{
	    if(findLabel(tempSymtab,lineTok) != fail)
	    {
		printf("Error On Line %d. Label Declared More Than Once.\n",lineNum*5);
		return fail;
	    }
	    addSymtab(tempSymtab,lineTok,LOCCTR);
	    lineTok = strtok(NULL," ");
	    if((opTemp =isOpcode(lineTok))!= NULL)
	    {
		if(strcmp(opTemp->op,"LDB") == 0)
		{
		    lineTok = strtok(NULL," ");
		    if(lineTok[0] == '#')strcpy(baseLabel,lineTok+1);
		    else strcpy(baseLabel, lineTok);
		}
		LOCCTR+=opTemp->format[0];
	    }
	    else if((opTemp = isOpcode(lineTok+1)) != NULL&&lineTok[0] == '+')
	    {
		if(strcmp(opTemp->op,"LDB") == 0)
		{
		    lineTok = strtok(NULL," ");
		    if(lineTok[0] == '#')strcpy(baseLabel,lineTok+1);
		    else strcpy(baseLabel, lineTok);
		}
		LOCCTR+= opTemp->format[1];
	    }
	    else if(strcmp(lineTok,"BASE")==0)continue;
	    else if(strcmp(lineTok,"WORD")==0)LOCCTR += 3;
	    else if(strcmp(lineTok,"BYTE")==0)
	    {
		lineTok = strtok(NULL," ");
		lineTok = strtok(lineTok,"'");
		if(lineTok[0] == 'C')
		{
		    lineTok = strtok(NULL,"'");
		    LOCCTR += strlen(lineTok);
		}
		else
		{
		    lineTok = strtok(NULL,"'");
		    LOCCTR += (int)(strlen(lineTok)/2);
		}
	    }
	    else if(strcmp(lineTok,"RESW")==0)
	    {
		lineTok = strtok(NULL," ");
		LOCCTR += atoi(lineTok)*3;
	    }
	    else if(strcmp(lineTok,"RESB")==0)
	    {
		lineTok = strtok(NULL," ");
		LOCCTR += atoi(lineTok);
	    }
	    else
	    {
		printf("Undefined Instruction On Line : %d \n",lineNum*5);
		return fail;
	    }

	}

	lineNum += 1;
    }
    *codeLength = LOCCTR-startAddr;
    *base = findLabel(tempSymtab,baseLabel);
    return success;
}

int validAssembleArg(char* Arg2, char* Arg3)
{
    if(Arg3 != NULL)
    {
	printf("Too Many Arguments\n");
	return fail;
    }
    if(Arg2 == NULL)
    {
	printf("File Name Must Be Provided\n");
	return fail;
    }
    return success;
}
int validTypeArg(char* Arg2, char* Arg3)
{
    if(Arg3 != NULL)
    {
	printf("Too Many Arguments\n");
	return fail;
    }
    if(Arg2 == NULL)
    {
	printf("File Name Must Be Provided\n");
	return fail;
    }
    return success;
}
/* 
   These functions are for validation as mentioned at 20151550.c 
   Same functions as other valid functions, but for different commands.
 */

void freeModRecord(modRecord* mr)
{
    modRecord* temp=mr;
    while(temp->next !=NULL)
    {
	temp = temp->next;
	free(mr);
	mr = temp;
    }
    free(mr);
    return ;
}
void freeSymtab(symbolTable* sym)
{
    symbolTable* temp = sym;
    while(temp->next != NULL)
    {
	sym = temp;
	temp = temp->next;
	free(sym);
    }
    free(temp);
}
/* These are functions for memory deallocation. */


int executeAssemble(char* filename)
{
    FILE* fp;
    fp = fopen(filename,"r");
    char* fileTok;
    char lstFile[50];
    char objFile[50];
    char filenameCopy[50];
    int successFlag;
    int startExist = 0;
    if(fp == NULL)
    {
	printf("File Does Not Exists!\n");
	return fail;
    }
    strcpy(filenameCopy,filename);
    fileTok = strtok(filenameCopy,".");
    fileTok = strtok(NULL,".");
    if(fileTok == NULL)
    {
	printf("File Name Does Not Fit\n");
	return fail;
    }
    if(strcmp(fileTok,"asm") != 0)
    {
	printf("Only asm Format File Can Be Assembled\n");
	return fail;
    }
    int base;
    int codeLength;
    symbolTable* tempSymtab = (symbolTable*)malloc(sizeof(symbolTable));
    successFlag = executePass1(fp,tempSymtab,&base,&codeLength,&startExist);
    fclose(fp);
    if(successFlag == fail)return fail;
    fp = fopen(filename,"r");
    fileTok = strtok(filename,".");
    strcpy(lstFile,fileTok);
    strcpy(objFile,fileTok);
    strcat(lstFile,".lst");
    strcat(objFile,".obj");
    FILE* lstfp;
    FILE* objfp;
    modRecord* mr = (modRecord*)malloc(sizeof(modRecord));
    mr->next = NULL;
    lstfp = fopen(lstFile,"w");
    objfp = fopen(objFile,"w");
    successFlag = executePass2(fp,tempSymtab,base,codeLength,startExist,lstfp,objfp,mr);
    fclose(fp);
    fclose(lstfp);
    fclose(objfp);
    if(successFlag == fail)
    {
	remove(lstFile);
	return fail;
    }
    freeModRecord(mr);
    if(symtab != NULL)
    {
	freeSymtab(symtab);
    }
    symtab = tempSymtab;
    return success;
}
/*
    Assemble command consists of 4 task.
    1. basic allocations.
    2. executePass1
    3. executePass2
    4. deallocations and allocate temporal symbol table to global symbol table.
    This function does 1,4 tasks and call executePass1 and 2 to do 2,3 tasks.
*/


int executeType(char* filename)
{
    FILE *filePointer;
    filePointer = fopen(filename,"r");
    char fileChar;
    if(filePointer == NULL)
    {
	printf("File Does Not Exist!\n");
	return fail;
    }
    else
    {
	while(EOF != fscanf(filePointer,"%c",&fileChar)){
	    printf("%c",fileChar);
	}
	fclose(filePointer);
	return success;	
    }
}
/*
    This function is for executing type command.
    It gets name of file. If it exists, print whole text of file.
*/

int executeSymbol()
{
    if(symtab == NULL)
    {
	printf("Nothing Has Used Assembler Command Yet.\n");
	return fail;
    }
    else
    {
	printSymtab();
	return success;
    }
}
/*
    if symbol table is empty, return error message and fail.
    if symbol talbe does exists, call printSymtab function.
*/
int printSymtab()
{
    symbolTable* temp = symtab;
    while(temp->next != NULL)
    {
	temp = temp->next;
	printf("        %-8s %04x\n",temp->label,temp->addr);
    }
    return success;
}
/*
    This function prints all symbol in symbol table.
*/
