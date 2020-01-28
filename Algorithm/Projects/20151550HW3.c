#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
/* 
I wrote every comment in English in case of Encoding Error 
bt is basic data structure of node of huffman code
data : value that huffman code compressed
count : to build huffman code tree, you need to compare the number of character in text
one : if the next character of compressed string is one, connect the subtree to one
zero : if the next character of compressed string is zero, connect the subtree to zero
by using one, zero you can get the value of huffman code of data
*/
typedef struct binaryTree* btpointer;
typedef struct binaryTree {
	int data;
	int count;
	btpointer one;
	btpointer zero;
}bt;

/*
swapTree is used for swapping two node's value
it is built for quick sort 
*/
void swapTree(bt* root, int a, int b)
{
	bt temp;
	temp = root[b];
	root[b] = root[a];
	root[a] = temp;

}
/*
sortBt and partition is Quick Sort algorithm for sorting trees. before connect nodes, they first be aligned by count since huffman code use greedy algorithm counting the number of times a character written in text file
*/
int partition(bt* root, int start, int end)
{
	int i, pivot;
	pivot = start;
	for (i = start; i < end; i++)
	{
		if (root[i].count < root[end].count) {
			swapTree(root,i,pivot);
			pivot++;
		}
	}
	swapTree(root,pivot,end);
	return pivot;
}
void sortBt(bt* root, int start, int end) {
	if (end - start > 0) {
		int pivot = partition(root, start, end);
		sortBt(root, start, pivot - 1);
		sortBt(root, pivot + 1, end);
	}
}

/*
createSortNode creates node if an ascii character has been written more than once. then it creates nodes as much as number of characters used. and then it calls sortBt to sort nodes by count increasing order after it finishes sorting, it returns the count of character used.
CounterTable is 256 size int array. i-th element of array saves a count of i-th ascii character in the text file
*/
int createSortNode(int CountTable[], bt* root) {
	int i;
	int asciinum = 0;
	int rootcount = 0;
	for (i = 0; i < 256; i++)
		if (CountTable[i] != 0)asciinum++;
	
	for (i = 0; i < 256; i++)
	{
		if (CountTable[i] != 0)
		{
			root[rootcount].data = i;
			root[rootcount].count = CountTable[i];
			root[rootcount].one = NULL;
			root[rootcount].zero = NULL;
			rootcount++;
		}
	}
	sortBt(root,0,rootcount-1);

	return rootcount;
}

/*
countAscFile get 256 size int array and file stream as parameter to traverse whole texts and save the count of each character at CountTable
*/
int countAscFile(int CountTable[], FILE* input)
{
	char fchar;
	int asciinum = 0;
	while (1)
	{
		fchar = fgetc(input);
		if (fchar == EOF)break;
		CountTable[fchar] += 1;
	}
	CountTable[255] += 1;
	for (int i = 0; i < 256; i++)
		if (CountTable[i] != 0)asciinum++;
	return asciinum;
}

/*
makeTree is function that creates pointer of bt and allocates memory to bt.
After that, two node, which is parameter of the function, is connected as child of new node. Counter value of new node is sum of child's count. By this, you can easily build tree structure.
The function returns the pointer of new node.
*/
btpointer makeTree(btpointer one, btpointer zero){
	bt* node;
	node = (btpointer)malloc(sizeof(bt));
	node->one = one;
	node->zero = zero;
	node->count = one->count + zero->count;
	return node;
}

/*
makeHuffTree get array of bt and the number of it.
By using this, makeHuffTree build Huffman-Coded tree. The array is already sorted by count in increasing order.
At first, it builds a tree with two smallest nodes. After that, at each step, compare the smallest count tree and two smallest node. By choosing two smallest value, it builds huffman tree.
The function returns top of the tree.
*/
btpointer makeHuffTree(bt root[],int treeNum)
{
	int targetTree = 0;
	btpointer P[255];
	int pStart = 0; 
	int pEnd = 0;
	while (targetTree < treeNum)
	{
		if (targetTree == 0)
		{
			P[0] = makeTree(&(root[targetTree]),&(root[targetTree+1]));
			targetTree += 2;
		}
		else 
		{
			if(targetTree+1 < treeNum){
				if (P[pStart]->count < root[targetTree].count)
				{
					pEnd++;
					P[pEnd] = makeTree(P[pStart], &(root[targetTree]));
					pStart++;
					targetTree += 1;
				}
				else
				{
					pEnd++;
					P[pEnd] = makeTree(&(root[targetTree]), &(root[targetTree + 1]));
					targetTree += 2;
				}
			}
			else
			{
				pEnd++;
				if (root[targetTree].count > P[pStart]->count) {
					int temp = pEnd;
					while (pStart < temp && root[targetTree].count < P[temp-1]->count)
					{
						P[temp] = P[temp - 1];
						temp--;
					}
					P[temp] = &(root[targetTree]);

				}
				else
				{
					P[pEnd] = makeTree(P[pStart], &(root[targetTree]));
					pStart++;
				}
				targetTree += 1;
				if (pStart != pEnd)
				{
					while (pStart != pEnd)
					{
						pEnd++;
						P[pEnd] = makeTree(P[pStart], P[pStart + 1]);
						pStart += 2;
					}
				}
				return P[pEnd];
			}
		}
	}
	if (pStart != pEnd)
	{
		while (pStart != pEnd)
		{
			pEnd++;
			P[pEnd] = makeTree(P[pStart], P[pStart + 1]);
			pStart += 2;
		}
	}
	return P[pEnd];
}
/*
buffer and buffcounter are variables used to compress bits and save as 8 bit character type.
buffcounter has 0 - 8 value. if buffcounter becomes 8, write the character in buffer to output file and initialize buffcounter as 0.
definedEOF is user defined EOF which substitute ascii character number -1. By this, even though .zz files get EOF, it doesn't finish reading text file but only finish when definedEOF is read.
*/
unsigned char buffer = 0;
int buffcounter=0;
char definedEOF[257];

/*
compressWrite gets str and output as parameter.
str is array of character which represents bit string of certain character.
therefore, by using buffer and buffcounter, write a character to output file per 8 bit.
*/
void compressWrite(char *str, FILE *output)
{
	int count = 0;
	for (count = 0; count < strlen(str); count++)
	{
		if (str[count] == '0')
		{//0
			buffer = buffer << 1;
			buffcounter++;
			if (buffcounter == 8)
			{
				fprintf(output, "%c", buffer);
				buffcounter = 0;
				buffer = 0;
			
			}
		}
		else
		{//1
			buffer = buffer << 1;
			buffer += 1;
			buffcounter++;
			if (buffcounter == 8)
			{
				fprintf(output, "%c", buffer);
				buffcounter = 0;
				buffer = 0;
			
			}
		}
	}
}
/*
at the end of the code, you need to write user defined EOF, which is definedEOF, to let decompressed file end.
but, most of the time, writing to output file will done while some bits are left on the buffer. Therefore, if there are bits left at buffer, you need to write a one character more.
*/
void dealLastByte(FILE* output)
{
	int count;
	for (count = 0; count < strlen(definedEOF); count++)
	{
		if (definedEOF[count] == '0')
		{
			buffer = buffer << 1;
			buffcounter++;
			if (buffcounter == 8)
			{
				fprintf(output, "%c", buffer);
				buffcounter = 0;
				buffer = 0;
			}
		}
		else
		{
			buffer = buffer << 1;
			buffer += 1;
			buffcounter++;
			if (buffcounter == 8)
			{
				fprintf(output, "%c", buffer);
				buffcounter = 0;
				buffer = 0;
			}
		}
	}
	if (buffcounter != 0)
	{
		while (1)
		{
			buffer = buffer << 1;
			buffcounter++;
			if (buffcounter == 8)
			{
				fprintf(output, "%c", buffer);
				break;
			}
		}
	}
		
}

/*
writeTable writes data of huffman coded tree.
it is written as header of text file. however, I deal those data purely as integer and string.
By this, small sized input file can have compressed file larger than input file.
By traversing all tree as recursive way, write every data to the head of the output file.
*/
void writeTable(FILE* output, btpointer huff, char *str)
{
	char string[256];
	strcpy(string, str);
	if (huff->one == NULL && huff->zero == NULL)
	{
		fprintf(output,"%d %s\n", huff->data,string);
		if (huff->data == 255)strcpy(definedEOF, string);
		return;
	}
	if (huff->one != NULL)
	{
		strcat(string, "1");
		writeTable(output,huff->one, string);
	}
	strcpy(string, str);
	if (huff->zero != NULL)
	{
		strcat(string, "0");
		writeTable(output,huff->zero, string);
	}
}
/* 
findHuffman is build to call compressWrite at the last stage of recursive call.
by traversing huffman coded tree from top, the function can get full bit string of a data. At this moment, the function calls compressWrite to write the bit string to output file.
str start as "" which is blank string. but at last stage, it will become a bit string of data.
fchar is character that the function seeks seeks for to write into output file.
*/
void findHuffman(btpointer huff, char fchar, char* str, FILE *output)
{
	char string[256];
	strcpy(string, str);
	if (fchar == huff->data)
	{
		compressWrite(str, output);
		return;
	}
	if (huff->one != NULL)
	{
		strcat(string, "1");
		findHuffman(huff->one, fchar,string,output);
	}
	strcpy(string, str);
	if (huff->zero != NULL)
	{
		strcat(string, "0");
		findHuffman(huff->zero, fchar,string,output);
	}
}
/*
writeFile gets all input data and calls findHuffman function to write into output text file.
*/
void writeFile(btpointer huff, FILE* input, FILE* output)
{
	char fchar;
	while (1)
	{
		fchar = fgetc(input);
		if (fchar == EOF)break;
		findHuffman(huff, fchar, "",output);
		
	}
}

/*
Till now, All functions was for compressing input data.
----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------
Functions down below are for decompressing file.
However, user defined data structure and global variable for buffer are still used.
*/

/*
createDecTreeNode gets parent and ZorO, which is 'Zero or One'. 
If ZorO is one, create new node and connect it to one of parent.
if ZorO is zero, create new node and connect it to zero of parent.
*/
void createDecTreeNode(btpointer parent, int ZorO)
{
	btpointer newnode;
	newnode = (btpointer)malloc(sizeof(bt));
	newnode->one = NULL;
	newnode->zero = NULL;
	if (ZorO == 1)
	{
		parent->one = newnode;
	}
	else
	{
		parent->zero = newnode;
	}
}
/*
decodeHuffTree gets information of asciiCharacter as input. For example, table[49('a')] has bit string of character 'a'
The function allocate memory to root node first.
Then, to all ascii character, create and connect tree nodes following the bit string.
Finally it returns head node of huffman coded tree.
*/
btpointer decodeHuffTree(char table[][258])
{
	btpointer root = (btpointer)malloc(sizeof(bt));
	root->one = NULL;
	root->zero = NULL;
	btpointer traverse;
	for (int i = 0; i < 256; i++)
	{
		traverse = root;
		if (table[i][0] == '0' || table[i][0] == '1')
		{
			for (int j = 0;  table[i][j] == '0' || table[i][j] == '1'; j++)
			{
				if (table[i][j] == '1')
				{
					if (traverse->one == NULL)
					{
						createDecTreeNode(traverse,1);
						traverse = traverse->one;
					}
					else
					{
						traverse = traverse->one;
					}
				}
				else
				{
					if (traverse->zero == NULL)
					{
						createDecTreeNode(traverse, 0);
						traverse = traverse->zero;
					}
					else
					{
						traverse = traverse->zero;
					}
				}
			
			}
			traverse->data = i;
		}
	}
	return root;
}
/*
writeDecomFile do opposite as compressWrite do.
It gets character from input file (compressed file) until it print user defined character.
At each binary value, function traverses huffman tree finding data.
If a node don't have child, it means the node is leaf that has character data. So print if to output file
If traversing node lead to EOF, finish the function.
*/
void writeDecomFile(btpointer head, FILE* input, FILE* output)
{
	
	unsigned char ch;
	unsigned char buffer;
	char buffercount;
	btpointer traverse = head;
	while (1)
	{
		fscanf(input, "%c", &ch);
		buffer = ch;
		buffercount = 0;
		while (buffercount < 8)
		{
			if(128 <= buffer)
			{

				buffer = buffer * 2;
				buffercount++;
				traverse = traverse->one;
				if (traverse->one == NULL && traverse->zero == NULL)
				{
					if (traverse->data == 255)return;
					fprintf(output, "%c",(traverse->data));
					traverse = head;
					
				}
			}
			else
			{
				buffer = buffer * 2;
				buffercount++;
				traverse = traverse->zero;
				if (traverse->one == NULL && traverse->zero == NULL)
				{
					if (traverse->data == 255)return;
					fprintf(output, "%c", (char)(traverse->data));
					traverse = head;
				}
			}
		}
	}
}
int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Execution Error\n");
		printf("./mp2_20180001 -c(-d) input.txt is required\n");
		return 1;
	}
	//make Exception about command.
	FILE *input;
	FILE *output;
	input = fopen(argv[2],"rb"); //read file as binary file to prevent fscanf or fgetc from stop when meet -1
	if (input == NULL)
	{
		printf("Input File Error!\n");
		return 2;
	}//make Exception about missing input file
	char outputFile[200];
	if (strcmp(argv[1], "-c") == 0) { //compressing
		strcpy(outputFile, argv[2]);
		strcat(outputFile, ".zz");
		bt *root = NULL;
		output = fopen(outputFile, "wb");
		int asciiCountTable[256] = { 0, };
		int treeNum;
		int asciinum = countAscFile(asciiCountTable, input); // count the  number of each character in file
		root = (bt*)malloc(sizeof(bt) * asciinum);
		btpointer huffTree = NULL;
		treeNum = createSortNode(asciiCountTable, root); // create nodes for huffman coded tree
		huffTree = makeHuffTree(root, treeNum); // create huffman coded tree
		fprintf(output, "%d\n", treeNum); // write the number of ascii character used as header
		writeTable(output, huffTree,""); // start write encoded file
		fclose(input);
		input = fopen(argv[2], "r");
		writeFile(huffTree, input, output);
		dealLastByte(output);
		fclose(output);
	}
	else if (strcmp(argv[1], "-d") == 0){ //decompressing
		//decompress
		strcpy(outputFile, argv[2]);
		strcat(outputFile, ".yy");
		output = fopen(outputFile, "w");
		int asciiNum;
		fscanf(input, "%d\n", &asciiNum);
		char asciiTable[256][258] = { 0, };
		char temp[258];
		int num;
		for (int i = 0; i < asciiNum; i++)
		{
			fscanf(input,"%d %s\n",&num,temp);
			strcpy(asciiTable[num], temp);
		}// asciiTable Creation
		btpointer huffTree;
		huffTree = decodeHuffTree(asciiTable); // create huffman coded tree
		writeDecomFile(huffTree, input, output); // start write decompressed file
		fclose(output);
	}
	else {
		printf("Command Error\n");
		printf("-c is for compression, -d is for decompression\n");
	}//make Exception about wrong command
	fclose(input);
	return 0;
}
