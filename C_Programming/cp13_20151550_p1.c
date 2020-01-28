#include<stdio.h>

int main()
{
	struct money{
		int fh;
		int oh;
		int ft;
		int ten;
	};
	struct money rt;
	printf("return money : ");
	int howmuch;

	scanf("%d",&howmuch);

	rt.fh = 0;
	rt.oh = 0;
	rt.ft = 0;
	rt.ten = 0;
	rt.fh = howmuch/500;
	rt.oh = (howmuch - rt.fh*500)/100;
	rt.ft = (howmuch - rt.fh*500 - rt.oh*100)/50;
	rt.ten = (howmuch - rt.fh*500 - rt.oh*100 - rt.ft*50)/10;

	printf(" 500 = %d\n",rt.fh);
	printf(" 100 = %d\n",rt.oh);
	printf(" 50 = %d\n",rt.ft);
	printf(" 10 = %d\n",rt.ten);
	return 0;
}
