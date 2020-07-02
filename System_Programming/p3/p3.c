#include "20151550.h"
/*
   All comments are written in english for encoding problem
*/
const int progaddr = 23;
const int loader = 24;
const int bp = 25;
const int run = 26;
/*
    set command number
*/
typedef struct breakpoint{
    int addr;
    struct breakpoint* next;

}bps;
/*
   data structure for break point
*/
bps* breakPoint = NULL;

typedef struct et{
    int addr;
    char name[7];
    int length;
    struct et* next;
}estab;
// data structure for ESTAB;

estab* esTable = NULL;

int executeRun();
int executeBp();
int executeLoader();
int executeProgaddr();
int validBpArg();
int validLoaderArg();
int validProgaddrArg();

int reg[10];
int endAddr;
int startAddr = 0x00;
int executeAddr = -1;
int validProgaddrArg(char* Arg2, char* Arg3)
{
    if(Arg3 != NULL)
    {
	printf("Too Many Arguments!\n");
	return fail;
    }
    if(stringToHex(Arg2,noComma) == fail)
    {
	printf("Address Must Be Hexadecimal\n");
	return fail;
    }
    return success;
}
/* Check Argument from commandline is valid to progaddr command*/


int validBpArg(char* Arg2,char* Arg3)
{
    if(Arg3 != NULL)
    {
	printf("Too Many Arguments!\n");
	return fail;
    }
    if(Arg2 == NULL)return 0;
    if(strcmp(Arg2,"clear") == 0)return 1;
    int addr = stringToHex(Arg2,noComma);
    if(addr == fail || addr < 0 || addr > 0xFFFFF)
    {
	printf("Argument is not valid\n");
	return fail;
    }
    return 1;
}
// Check Argument from commandline if it is valid to bp command



int validLoaderArg(char* Arg2, char* Arg3, char* Arg4, char* Arg5)
{
    if(Arg5 != NULL)
    {
	printf("Only Three Files Can Be Linked At Once\n");
	return fail;
    }
    if(Arg2 == NULL)
    {
	printf("Loader needs object file\n");
	return fail;
    }
    if(Arg3 == NULL)return 1;
    if(Arg4 == NULL)return 2;
    return 3;
}
// check argument from commandline if it is valid to loader command

void addBreakPoint(int addr)
{
    if(breakPoint == NULL)
    {
	breakPoint = (bps*)malloc(sizeof(bps));
	breakPoint->next = NULL;
    }
    bps* temp = breakPoint;
    bps* newNode = (bps*)malloc(sizeof(bps));
    newNode->next =NULL;
    newNode->addr = addr;
    bps* tempHead;
    while(temp->next != NULL)
    {
	tempHead = temp;
	temp = temp->next;
	if(temp->addr > newNode->addr)
	{
	    newNode->next = tempHead->next;
	    tempHead->next = newNode;
	    return;
	}
    }
    temp->next = newNode;
}
//add new break point 
void freeBreakPoint()
{
    if(breakPoint == NULL)return; 
    bps* temp = breakPoint;
    while(temp->next != NULL)
    {
	temp = temp->next;
	free(breakPoint);
	breakPoint = temp;
    }
    free(breakPoint);
    breakPoint = NULL;
}
//freeBreakPoint is called when bp clear is provided
int executeBp(char* Arg2, int argNum)
{
    if(argNum == 0)
    {
	if(breakPoint == NULL)
	{
	    printf("There's No Break Point Yet\n");
	}
	else
	{
	    printf("        breakpoint\n");
	    printf("        ----------\n");
	    bps* temp = breakPoint->next;
	    while(temp->next != NULL)
	    {
		printf("        %x\n",temp->addr);
		temp = temp->next;
	    }
	    printf("        %x\n",temp->addr);
	}
    }
    else
    {
	if(strcmp(Arg2,"clear") == 0)
	{
	    if(breakPoint != NULL)freeBreakPoint();
	    printf("	    [ok] clear all breakpoints\n");
	}
	else
	{
	    int addr = stringToHex(Arg2,noComma);
	    if(addr == fail || addr < 0 || addr > 0xFFFFF)
	    {
		printf("Argument is not valid\n");
		return fail;
	    }
	    printf("	    [ok] create breakpoint %4x\n",addr);
	    addBreakPoint(addr);
	}
    }
    return success;
}
/*
    This function is a process to deal with bp command.
    following argument of command line, it add, delete, and show break point
*/
int addESTAB(int addr, char* name, int length)
{
    if(esTable == NULL)
    {
	esTable = (estab*)malloc(sizeof(estab));
	esTable->next =NULL;
    }
    estab* temp = esTable;
    estab* newNode = (estab*)malloc(sizeof(estab));
    newNode->next = NULL;
    memcpy(newNode->name,name,7);
    newNode->name[6] = 0;
    newNode->addr = addr;
    newNode->length = length;
    while(temp->next != NULL)
    {
	temp=temp->next;
	if(strcmp(temp->name,name) == 0)
	{
	    printf("ES Name Duplicated\n");
	}
    }
    temp->next = newNode;
    return success; 
}
/*
   function for add ESTAB. It is essential for loader process 
 */

void freeESTAB()
{
    estab* temp = esTable;
    while(temp->next != NULL)
    {
	esTable = temp;
	temp = temp->next;
	free(esTable);
    }
    free(temp);
    esTable = NULL;
	
}
/*
   if loader command called more than one time, previous ESTAB must be deallocated.
*/

void printESTAB(int csAddr)
{
    estab* temp=esTable;
    printf("control symbol address length\n");
    printf("section name\n");
    printf("------------------------------\n");
    
    while(temp->next != NULL)
    {
	temp = temp->next;
	if(temp->length != -1)
	{
	    printf("%6s         %04x   %04x\n",temp->name,temp->addr,temp->length);
	}
	else
	{
	    printf("       %6s  %04x\n",temp->name,temp->addr);
	}
    }
    printf("------------------------------\n");
    printf("         total length %04x\n",csAddr-startAddr);
    
}
/*
    by visiting all nodes of estab structure, print all ESTAB elements.
*/

estab* findESTAB(char* name)
{
    estab* temp=esTable;
    while(temp->next !=NULL)
    {
	temp = temp->next;
	if(strcmp(temp->name,name) == 0)return temp;
    }
    return temp;
}
/*
    return estab that program wants to find by symbol's name.
*/

int loaderPass1(FILE* file,int csAddr)
{
    char name[7];
    char buffer[61];
    char hex[7];
    char command;
    int count;
    int addr;
    int start,length;
    
    fscanf(file,"%c",&command);
    fscanf(file,"%6c",name);
    fscanf(file,"%6x",&start);
    fscanf(file,"%6x\n",&length);
    addESTAB(csAddr,name,length);
    while(1)
    {
	fscanf(file,"%c",&command);
	if(command == 'E')break;
	else if(command == 'D')
	{
	    fscanf(file,"%[^\n]s",buffer);
	    for(count = 0; count < strlen(buffer)/12; count++)
	    {
		memcpy(name,buffer+count*12,6);
		memcpy(hex,buffer+count*12+6,6);
		addr = csAddr + strtol(hex,NULL,16);
		addESTAB(addr,name,-1);
	    }
	}
	else fscanf(file,"%s\n",buffer);
    }
    csAddr += length;
    return csAddr;
}
/*
    loader needs two pass to finish loading process.
    loaderPass1 works as pass 1 of loader.
    It reads file, make symbol table and save it to estab structure.
    plus, it calculate address length.
   */

void loaderPass2(FILE* file)
{
    char name[7];
    char buffer[200];
    char hex[3];
    char membuff[4];
    membuff[3] = 0;
    hex[2]= 0;
    char command;
    int count;
    int addr;
    int start,length;
    int Taddr,Tlen;
    int line,col,value;
    int Raddr[256];
    int Rnum;
    char pm; //plus or minus;
    fscanf(file,"%c",&command);
    fscanf(file,"%6c",name);
    name[6] = 0;
    fscanf(file,"%6x",&start);
    fscanf(file,"%6x\n",&length);
    estab* temp = findESTAB(name);
    addr = temp->addr;
    Raddr[1] = addr;
    while(1)
    {
	fscanf(file,"%c",&command);
	if(command == '\n')continue;
	if(command == 'E')
	{
	    fscanf(file,"%6x",&executeAddr);
	    break;
	}
	else if(command == 'D'){
	    fscanf(file,"%[^\n]s",buffer);
	}
	else if(command == 'R')
	{
	    fscanf(file,"%[^\n]s",buffer);
	    for(count = 0; count <= strlen(buffer)/8; count++)
	    {
		memcpy(hex,buffer+count*8,2);
		memcpy(name,buffer+count*8+2,6);
		if(strlen(name) < 6)memset(name+strlen(name),' ',6-strlen(name));
		name[6] = 0;
		temp = findESTAB(name);
		Raddr[strtol(hex,NULL,16)] = temp->addr;
	    }


	}
	else if(command == 'T')
	{
	    fscanf(file,"%6x",&Taddr);
	    fscanf(file,"%2x",&Tlen);
	    for(count = 0; count < Tlen; count++)
	    {
		fscanf(file,"%2x",&value);
		line = (Taddr+addr+count)/16;
		col = (Taddr+addr+count)%16;
		sicMemory[line][col] = value;
	    }
	}
	else if(command == 'M')
	{
	    fscanf(file,"%6x",&Taddr);
	    fscanf(file,"%2x",&Tlen);
	    fscanf(file,"%c",&pm);
	    fscanf(file,"%2x\n",&Rnum);
	    line = (Taddr+addr)/16;
	    col = (Taddr+addr)%16;
	    if(Tlen == 6)value = Raddr[Rnum]&0xFFFFFF;
	    if(Tlen == 5)value = Raddr[Rnum]&0x0FFFFF;
	    memcpy(membuff,sicMemory[line]+col,1);
	    line = (Taddr+addr+1)/16;
	    col = (Taddr+addr+1)%16;
	    memcpy(membuff+1,sicMemory[line]+col,1);
	    line = (Taddr+addr+2)/16;
	    col = (Taddr+addr+2)%16;
	    memcpy(membuff+2,sicMemory[line]+col,1);
	    int memval;
	    memval = (membuff[0]&0xff)<<16 | (membuff[1]&0xff) << 8 | (membuff[2]&0xff);
	    if(pm == '+')memval = memval + value;
	    else memval = memval - value;
	    sicMemory[line][col] = memval & 0xFF;
	    memval = memval >> 8;
	    line = (Taddr+addr+1)/16;
	    col = (Taddr+addr+1)%16;
	    sicMemory[line][col] = memval & 0xFF;
	    memval = memval >> 8;
	    line = (Taddr+addr)/16;
	    col = (Taddr+addr)%16;
	    if(Tlen == 6)sicMemory[line][col] = memval & 0xFF;
	    else sicMemory[line][col] += memval & 0x0F;
	}
	else fscanf(file,"%s\n",buffer);
    }
}
/*
    This function works as pass2 of loading process.
    Reading each file, at each row, it saves object file to SIC/XE Memory space.
    For modification Record, program adds address value of symbol to designated memory.
*/


int executeLoader(char* file1,char* file2, char* file3,int filenum)
{
    FILE* fp1;
    FILE* fp2;
    FILE* fp3;
    if(esTable != NULL)
    {
	freeESTAB();
    }
    int csAddr = startAddr;
    switch(filenum)
    {
	case 1:
	    fp1 = fopen(file1,"r");
	    if(fp1 == NULL)
	    {
		printf("File Does Not Exist!\n");
		return fail;
	    }
	    csAddr = loaderPass1(fp1,csAddr);
	    fclose(fp1);
	    printESTAB(csAddr);

	    fp1 = fopen(file1,"r");
	    loaderPass2(fp1);
	    fclose(fp1);
	    break;
	case 2:
	    fp1 = fopen(file1,"r");
	    fp2 = fopen(file2,"r");
	    if(fp1 == NULL || fp2 == NULL)
	    {
		printf("File Does Not Exist!\n");
		return fail;
	    }
	    csAddr = loaderPass1(fp1,csAddr);
	    csAddr = loaderPass1(fp2,csAddr);
	    fclose(fp1);
	    fclose(fp2);
	    printESTAB(csAddr);
	    
	    fp1 = fopen(file1,"r");
	    fp2 = fopen(file2,"r");
	    loaderPass2(fp1);
	    loaderPass2(fp2);
	    fclose(fp1);
	    fclose(fp2);
	    break;
	default:
	    fp1 = fopen(file1,"r");
	    fp2 = fopen(file2,"r");
	    fp3 = fopen(file3,"r");
	    if(fp1 == NULL || fp2 == NULL || fp3 == NULL)
	    {
		printf("File Does Not Exist!\n");
		return fail;
	    }
	    csAddr =loaderPass1(fp1,csAddr);
	    csAddr=loaderPass1(fp2,csAddr);
	    csAddr=loaderPass1(fp3,csAddr);
	    printESTAB(csAddr);
	    fclose(fp1);
	    fclose(fp2);
	    fclose(fp3);

	    fp1 = fopen(file1,"r");
	    fp2 = fopen(file2,"r");
	    fp3 = fopen(file3,"r");
	    loaderPass2(fp1);
	    loaderPass2(fp2);
	    loaderPass2(fp3);
	    fclose(fp1);
	    fclose(fp2);
	    fclose(fp3);
	    break;
    }
    endAddr = csAddr;
    memset(reg,0,9);
    reg[2] = endAddr-startAddr; 
    return success;
}
/*
    Function for processing whole loading process.
    it opens at most three file, call loaderPass1 to process pass 1, and print ESTAB information.
    And then, call loaderPass2 to insert obj file's value to SIC/XE machine's memory.
    Finally, initialize registers for run operation.
*/

int executeProgaddr(char* Arg2)
{
    int chAddr = stringToHex(Arg2,noComma);
    if(chAddr < 0 || chAddr >= 0x10000)
    {
	printf("Address Out Out Range\n");
	return fail;
    }
    startAddr = chAddr;
    return success;
}
/*
    set startAddr.
    startAddr works as PROGADDR in this program.
*/


void printReg()
{
    printf(" A : %06x  ",reg[0]);
    printf(" X : %06x\n",reg[1]);
    printf(" L : %06x ",reg[2]);
    printf(" PC : %06x\n",reg[8]);
    printf(" B : %06x  ",reg[3]);
    printf(" S : %06x\n",reg[4]);
    printf(" T : %06x\n",reg[5]);

}
/*
    simply print register values
*/


int check34(int addr)
{
    int line = addr/16;
    int col = addr%16;
    char buffer = sicMemory[line][col];
    if(buffer & 0x10)return 4;
    else return 3;
}
/*
   return whether command is format 3 or format 4
*/

int checkAddressing(int addr)
{
    int line = addr/16;
    int col = addr %16;
    char buffer = sicMemory[line][col];
    if((buffer & 0x03) == 1)return 0; // immediate
    else if((buffer & 0x03) == 2)return 1; // indirect
    else return 2; //simple
}
/*
    return addressing state. whether it is immedate, indirect or simple addressing.
*/

int checkX(int addr)
{

    int line = addr/16;
    int col = addr % 16;
    char buffer = sicMemory[line][col];
    if((buffer & 0x80) != 0)return 1;
    else return 0;
}
/*
    check if indexed addressing mode is used.
   */
int checkbp(int addr)
{
    int line = addr/16;
    int col = addr % 16;
    char buffer = sicMemory[line][col];
    if((buffer & 0x40) != 0)return 1; //b
    else return 0; //p
}
/*
    check if relative addressing use PC or Base
*/
int getvalue3(int addr)
{
    int line = addr/16;
    int col = addr%16;
    int r1 = 0x0f & sicMemory[line][col];
    r1 = r1<<8;
    line = (addr+1)/16;
    col = (addr+1)%16;
    r1 += 0xff & sicMemory[line][col];
    return r1;
}
/*
    if command was format 3, then it returns disp for format 3 command
   */

int getvalue4(int addr)
{
    int line = addr/16;
    int col = addr%16;
    int r1 = 0x0f & sicMemory[line][col];
    r1 = r1<<8;
    line = (addr+1)/16;
    col = (addr+1)%16;
    r1 += 0xff & sicMemory[line][col];
    r1 = r1 <<8;
    line = (addr +2)/16;
    col = (addr+2)%16;
    r1 += 0xff & sicMemory[line][col];
    return r1;
}
/*
   if command was format 4, then it returns address for format 4 command
*/

int searchBP(int addr)
{
    bps* temp = breakPoint;
    while(temp->next != NULL)
    {
	temp = temp->next;
	if(temp->addr == addr)
	{
	    return 1;
	}
    }
    return 0;
}
/*
    find Break Point to check current PC is break point.
*/


/*
    executeRun is operation for run copy.obj file.
    by switch function, it deals with every command in copy.obj file
    since this function is quite long, some comments also be written in function.
*/
int executeRun()
{
    if(executeAddr == -1)
    {
	printf("You Need To Load Program First\n");
	return fail;
    }
    int addr = startAddr;
    if(reg[6] != 0)addr = reg[6]; // reg[6] saves previous break point.
    int line = addr/16;
    int col = addr%16;
    int c34;
    char command;
    char buffer[3];
    int r1,r2;
    while(1)
    {
	//printReg();
	reg[8] = addr;
	if(breakPoint!=NULL)
	{
	    if(searchBP(addr) == 1 && reg[6] != addr){ 
		printReg();
		printf("	Stop at checkpoint [%x]\n",addr);
		reg[6] = addr; // save new break point
		return success;
	    }
	    //find break point and finish running. since program does not work as top-down order, you need to check every break point.
	}
	command = sicMemory[line][col] & 0xFC; // set command line
	if(addr >= endAddr)break;
	switch((unsigned char)command)
	{
	    case 0x14: // STL
		addr+= 1;
		if(checkbp(addr) == 0)
		{
		    r1 = getvalue3(addr) + addr + 2;
		    line = r1/16;
		    col = r1%16;
		    sicMemory[line][col] = (reg[2] & 0xFF0000)>>16;
		    line = (r1+1)/16;
		    col = (r1+1)%16;
		    sicMemory[line][col] = (reg[2] & 0x00FF00)>>8;
		    line = (r1+2)/16;
		    col = (r1+2)%16;
		    sicMemory[line][col] = (reg[2] & 0x0000FF);
		}
		else
		{
		    r1 = getvalue3(addr) + reg[3];
		    line = r1/16;
		    col = r1%16;
		    sicMemory[line][col] = (reg[2] & 0xFF0000)>>16;
		    line = (r1+1)/16;
		    col = (r1+1)%16;
		    sicMemory[line][col] = (reg[2] & 0x00FF00)>>8;
		    line = (r1+2)/16;
		    col = (r1+2)%16;
		    sicMemory[line][col] = (reg[2] & 0x0000FF);

		}
		addr +=1;
		break;
	    case 0x0c: // STA
		addr += 1;
		r1 = getvalue3(addr)+addr+2;
		line = r1/16;
		col = r1%16;
		sicMemory[line][col] = (reg[0] & 0xFF0000)>>16;
		line = (r1+1)/16;
	        col = (r1+1)%16;
		sicMemory[line][col] = (reg[0] & 0x00FF00)>>8;
		line = (r1+2)/16;
		col = (r1+2)%16;
		sicMemory[line][col] = (reg[0] & 0x0000FF);
		addr += 1;
		break;
	    case 0x10: // STX
		addr += 1;
		r1 = getvalue3(addr) + reg[3];
		line = r1/16;
		col = r1%16;
		sicMemory[line][col] = (reg[1] & 0xFF0000)>>16;
		line = (r1+1)/16;
	        col = (r1+1)%16;
		sicMemory[line][col] = (reg[1] & 0x00FF00)>>8;
		line = (r1+2)/16;
		col = (r1+2)%16;
		sicMemory[line][col] = (reg[1] & 0x0000FF);
		addr += 1;
		break;
	    case 0x54: // STCH
		printf("STCH\n");
		addr += 3;
		break;
	    case 0x30: //JEQ
		if(reg[9] != 0)
		{
		    addr += 2;
		    break;
		}
		addr += 1;
		if(check34(addr) == 4)
		{
		    printf("JEQ Error1\n");
		}
		else
		{
		    r1 = getvalue3(addr);
		    addr += 1;
		}
		if(checkAddressing(addr-1) == 0)
		{
		    printf("JEQ error2\n");
		}
		else if(checkAddressing(addr-1) == 1)
		{
		    printf("jeq error3\n");
		}
		else
		{
		    if(checkbp(addr) == 0) //p
		    {
			addr = r1 + addr;
		    }
		    else
		    {
			addr = reg[3] + r1;
		    }
		}
		break;
	    case 0x38: //JLT
		if(reg[9] != -1)
		{
		    addr +=2;
		    break;
		}
		addr += 1;
		r1 = getvalue3(addr);
		if(checkAddressing(addr-1) == 0)
		{
		    printf("error JLT\n");
		}
		else if(checkAddressing(addr-1) == 1)
		{
		    //indirect
		    printf("errro JLT2\n");
		}
		else
		{
		    if(checkbp(addr) == 0) //p
		    {
			
			if((r1 & 0x0800) != 0)r1 = r1 | 0xFFFFF000;
		

			addr = r1+addr+2;
			line = addr/16;
			col = addr%16;
			continue;
		    }
		    else
		    {
			addr = reg[3] + addr;
		    }
		}
		break;
	    case 0x3C: //J
		addr += 1;
		if(check34(addr) == 4)
		{
		    r1 = getvalue4(addr);
		    addr += 2;
		}
		else
		{
		    r1 = getvalue3(addr);
		}
		if(checkAddressing(addr-1) == 0)
		{
		    addr = r1;
		}
		else if(checkAddressing(addr-1) == 1)
		{
		    //indirect
		    addr += 2;
		    r2 = r1+addr;
		    line = r2/16;
		    col = r2%16;
		    addr = sicMemory[line][col] & 0xff;
		    line = (r2+1)/16;
		    col = (r2+1)%16;
		    addr = (addr << 8)| sicMemory[line][col];
		    line = (r2+2)/16;
		    col = (r2+2)%16;
		    addr = (addr << 8) | sicMemory[line][col];
		    continue;
		}
		else
		{
		    if(checkbp(addr) == 0) //p
		    {
			addr = r1 + addr;
		    }
		    else
		    {
			addr = reg[3] + addr;
		    }
		}
		break;
	    case 0x48: //JSUB
		addr += 1;
		r1 = getvalue4(addr);
		addr += 3;
		reg[2] = addr;
		addr = r1;
		line = addr/16;
		col = addr%16;
		continue;
		break;
	    case 0x00: //LDA
		addr += 1;
		if(check34(addr) == 4)
		{
		    printf("error1 LDA\n");
		}
		else
		{
		    r1 = getvalue3(addr);
		
		}
		if(checkAddressing(addr-1) == 0)
		{
		    reg[0] = r1;
		    addr += check34(addr)-2;
		}
		else if(checkAddressing(addr-1) == 1)
		{ //indirect
		    printf("pass");
		}
		else
		{
		    if(checkbp(addr) == 0) //p
		    {
			r2 = r1+addr+2;
			line = (r2)/16;
			col = (r2)%16;
			reg[0] = sicMemory[line][col] & 0xFF;
			line = (r2+1)/16;
			col = (r2+1)%16;
			reg[0] = reg[0]<<8 | (sicMemory[line][col]&0xFF);
			line = (r2+2) / 16;
			col = (r2+2) % 16;
			reg[0] = reg[0]<<8 | (sicMemory[line][col]&0xFF);
			addr += 1;
		    }
		    else
		    {
			printf("error LDA2\n");
			addr += 1;	
			reg[0] = getvalue3(r1+reg[3]);
		    }

		}
		break;
	    case 0x68: //LDB
		addr += 1;
		if(check34(addr) == 4)
		{
		    printf("LDB Error\n");
		    break;
		}
		else
		{
		    r1 = getvalue3(addr);
		    addr += 1;
		}
		if(checkAddressing(command) == 0)
		{
		    printf("LDB Error1\n");
		}
		else if(checkAddressing(command) == 1)
		{
		    //ind
		    printf("ldb error 2pass");
		}
		else
		{
		    if(checkbp(addr) == 0) //p
		    {
			reg[3] = r1+addr+1;
		    }
		}
		break;
	    case 0x50: //LDCH
		addr += 1;
		if(check34(addr) == 4)
		{
		    r1 = getvalue4(addr);
		    addr += 2;
		}
		else
		{
		    r1 = getvalue3(addr);
		}
		int tempx = reg[1]; // used for indexed addressing
		if(checkAddressing(command) == 0)
		{
		    reg[0] = r1;
		    addr += check34(addr)-2;
		}
		else if(checkAddressing(command) == 1)
		{
		    //indirect
		    addr = getvalue3(r1);
		}
		else
		{
		    if(checkX(addr) != 1)
		    {
			tempx = 0;
		    }
		    if(checkbp(addr) == 0) //p
		    {
			reg[0] = reg[0] +  getvalue3(r1+addr+tempx+1);
		    }
		    else
		    {
			line = (r1+reg[3]+tempx)/16;
			col = (r1+reg[3]+tempx)%16;
			reg[0] = (reg[0]&0xffff00)|(sicMemory[line][col]&0xff );
			addr += 1;
		    }

		}
		break;
	    case 0x74: //LDT

		addr += 1;
		if(check34(addr) == 4)
		{
		    r1 = getvalue4(addr);
		    reg[5] = r1;
		    addr += 2;
		    break;
		}
		else
		{
		    r1 = getvalue3(addr);
		}
    
		if(checkAddressing(command) == 0)
		{
		    reg[5] = r1;
		}
		else if(checkAddressing(command) == 1)
		{
		    //indirect
		    reg[5] = r1;
		    addr += 1;
		}
		else
		{
		    if(checkbp(addr) == 0) //p
		    {
			printf("Error LDT2\n");
		    }
		    else
		    {
			addr += 1;
			r2 = r1+reg[3];
	    
			line = (r2) / 16;
			col = (r2) %16;
		    
			reg[5] = sicMemory[line][col] & 0xFF;
		    
			line = (r2+1) / 16;
			col = (r2+1) % 16;
			reg[5] = reg[5]<<8 | (sicMemory[line][col]&0xFF);
		
			line = (r2+2) / 16;
			col = (r2+2) % 16;
			reg[5] = reg[5]<<8 | (sicMemory[line][col]&0xFF);
			
		    }

		}
		break;
	    case 0x28: //COMP
		//---------------------------------------------
		addr += 1;
		r1 = getvalue3(addr);
		if(reg[0] > reg[r1])reg[9] = -1;
		else if(reg[0] == reg[r1])reg[9] = 0;
		else reg[9] = 1;
		addr += 1;
		break;
	    case 0xB4: //CLEAR
		addr += 1;
		line = addr/16;
		col = addr%16;
		buffer[0] = sicMemory[line][col];
		r1 = buffer[0] & 0xf0;
		reg[r1] = 0;
		break;
	    case 0xA0: //COMPR
		addr += 1;
		line = addr/16;
		col = addr%16;
		buffer[0] = sicMemory[line][col];
		r1 = buffer[0] & 0xf0;
		r2 = buffer[0] & 0x0f;
		if(reg[r1] > reg[r2])reg[9] = -1;
		else if(reg[r1] == reg[r2])reg[9] = 0;
		else reg[9] = 1;
		break;
	    case 0xE0: //TD
		addr += 1;
		if(check34(addr) == 4)addr += 2;
		else addr += 1;
		reg[9] = 1;
		break;
	    case 0xB8: //TIXR
		reg[1] += 1;
		addr += 1;
		line = addr/16;
		col = addr%16;
		buffer[0] = sicMemory[line][col];
		r1 = (buffer[0] & 0xf0)>>4;
		if(reg[1] < reg[r1])reg[9] = -1;
		else if(reg[r1] == reg[r2])reg[9] = 0;
		else reg[9] = 1;
		break;
	    case 0x4C: //RSUB
		addr += 1;
		c34 = check34(addr);
		if(c34 == 4)addr +=2;
		else addr += 1;
		reg[8] = reg[2];
		addr = reg[2];
		line = addr/16;
		col = addr%16;
		continue;
		break;
	    case 0xD8: //RD
		addr += 1;
		c34 = check34(addr);
		if(c34 == 4)addr += 2;
		else addr += 1;
		reg[9] = 0;
		break;
	    case 0xDC: //WD
		addr += 2;
		break;
	    default:
		printf("Another opcode is used %2x",command);
	}

	addr += 1; // addr works as pointer. each iteration, addr is added as much as size of command.
	line = addr/16;
	col = addr%16;
    }
    printReg();
    reg[6] = 0; // if program finished, set previous breakpoint to zero
    printf("	    End Program\n");
    return success;
}
