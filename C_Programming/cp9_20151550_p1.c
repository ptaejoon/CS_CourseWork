#include<stdio.h>

int string_length(char *st);
void main()
{
	char *t = "C프로그래밍";
	char s[] = "C is a high-level language";
	

	printf(" 문자열 s의 길이 : %d\n",string_length(s)); // 영어 문자 개수 26개를 함수를 통해 세었다.
	printf(" sizeof(s)	 : %d\n",(int)sizeof(s)); // 영어 문장을 받은 배열의 크기를 세었기 때문에 마지막 NULL 값도 받기 때문에 27개를 나타낸다.
	printf(" 문자열 t의 길이 : %d\n",string_length(t)); // 알파벳과 다르게 한글은 3byte를 받는다.
	printf(" sizeof(t)	 : %d\n",(int)sizeof(t)); // 64비트 운영체제에서 int형 포인터 변수가 8바이트를 할당받았다. 
}
int string_length(char *st)
{
	int count=0;
	while(st[count] != '\0')
	{
		count++;
	}
	
	return count;
}
