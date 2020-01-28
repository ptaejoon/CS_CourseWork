#include<stdio.h>
#include<stdlib.h>
#include<time.h>
typedef struct maze{
	int what;
	int up;
	int down;
	int left;
	int right;
}maze;
int main()
{
	FILE *fp;
	fp = fopen("maze.maz","w");
	int width,height;
	int count,count2;
	int count3,count4;
	int check;
	srand(time(NULL));
	maze **M;
	printf("Width : ");
	scanf("%d",&width);
	printf("Height : ");
	scanf("%d",&height);
	int newnum=width;
	int howmany;
	int manycounter = 0;

	M = (maze **)malloc(sizeof(maze *)*height);
	for(count = 0; count < height; count++)
	{
		M[count] = (maze *)malloc(sizeof(maze)*width);
	}
	//할당 파트


	for(count =0; count < width; count++)
	{
		for(count2 = 0; count2 < height; count2++)
		{
			M[count2][count].up = 1;
			M[count2][count].down = 1;
			M[count2][count].right= 1;
			M[count2][count].left = 1;
		}
	}
	// 전부 벽세우기


	for(count = 0; count < width; count++)
	{
		M[0][count].what = count;
	}
	// 맨 위 행렬은 번호주기


	for(count = 0; count < width-1; count++)
	{
		check = rand()%2;
		if(check == 1)
		{
			M[0][count+1].what = M[0][count].what;
			M[0][count+1].left = 0;
			M[0][count].right = 0;
		}
	}
	//랜덤으로 합치기 (맨 위줄)

	for(count = 1; count < height; count++)
	{
		
		for(count2 = 0; count2 < width; count2++)
		{
			count3=0;
			while(M[count-1][count2+count3].what == M[count-1][count2+1+count3].what)
			{
				count3++;
			} // 합쳐진 만큼 계산
			for(count4 = 0; count4 < count3; count4++)
			{
				check = rand()%count3;
				if(count4 != 0)
				{
					if(M[count][count2+count4-1].down == 0)
					{
						M[count][count2 + count4].down = 1;
						continue;
					}
				}
				if(check == 0)
				{
					M[count-1][count2+count4].down = 0;
					M[count][count2+count4].up = 0;
					M[count][count2+count4].what = M[count-1][count2+count4].what;
				}
			} // 그중에 내려갈 것 고르기

			for(count4 = 0; count4 <= count3; count4++)
			{
				if(M[count-1][count2+count4].down == 0)
				{
					break;
				}
				M[count-1][count2+count4].down = 0;
				M[count][count2+count4].up = 0;
				M[count][count2+count4].what = M[count-1][count2+count4].what;
			}
			//0개인 애들 하나 뽑아다가 줘버리기

			for(count4 = 0; count4 < width; count4++)
			{
				if(M[count][count4].what >= newnum || M[count][count4].what < 0)
				{
					M[count][count4].what = newnum;
					newnum++;
				}
			}
			for(count4 = count3-1; count4 >= 0; count4--)
			{
				if(M[count-1][count2+count4].down == 0)
				{
						
					check = rand()%2;
					if(check == 0){
						if(M[count-1][count2+count4+1].down == 1 && M[count][count2+count4+1].what >width)
						{
						M[count][count2+count4+1].what = M[count][count2+count4].what;
						M[count][count2+count4+1].left = 0;
						M[count][count2+count4].right = 0;
						}
						else if(M[count-1][count2+count4-1].down == 1 && M[count][count2+count4-1].what> width)
						{
							M[count][count2+count4-1].what = M[count][count2+count4].what;
							M[count][count2+count4].left = 0;
							M[count][count2+count4-1].right = 0;
						}
					}
					else if(check == 1){
						if(M[count-1][count2+count4+1].down == 0)
						{
							M[count][count2+count4].down = 1;
							M[count][count2+count4].right = 0;
							M[count][count2+count4+1].left = 0;
							M[count][count2+count4].what = M[count][count2+count4+1].what;
						}
						else if(M[count-1][count2+count4-1].down == 0)
						{
						M[count][count2+count4].what = M[count][count2+count4-1].what;
						M[count][count2+count4].left = 0;
						M[count][count2+count4-1].right = 0;
						}
					}
					
				}
			}
			count2 += count3;
		}		
	}
	for(count = 0; count < width-1; count++)
	{
		if(M[height-1][count].what != M[height-1][count+1].what)
		{
			M[height-1][count].right = 0;
			M[height-1][count+1].left = 0;
		}
	}

	for (count = 2; count < height-1; count++)
	{

		for (count2 = 0; count2 < width; count2++)
		{
			check = rand() % 3;
			if (check != 0)
			{
				if (M[count - 1][count2 + 1].down == 0 && M[count - 1][count2].down == 0 && M[count][count2].down == 0 && M[count][count2+1].down == 0)
				{
					M[count][count2].down = 1;
					M[count + 1][count2 ].up = 1;
					M[count][count2 ].right = 0;
					M[count][count2+1 ].left = 0;
					M[count][count2 ].what = M[count][count2+ 1].what;
				}
			}
		}
		
	}
	for (count = 0; count < height-1; count++)
	{
		for (count2 = 0; count2 < width-1; count2++)
		{
			if (M[count][count2 + 1].left == 0 && M[count][count2 + 1].down == 0 && M[count][count2].down == 0 && M[count + 1][count2 + 1].left == 0)
			{
				M[count][count2+1].left = 1;
			}
		}
	}

	// 왼쪽기둥 초기화
	for(count = 0; count < height ; count++)
	{
		M[count][0].left = 1;
	}

	if (width > height)
	{
		howmany = height/2;
	}
	else
	{
		howmany = width/2;
	}
	for (count = 0; count < width-1; count++)
	{
		if (manycounter == howmany)
		{
			break;
		}
		if (M[0][count + 1].left == 1)
		{
			M[0][count + 1].left = 0;
			manycounter++;
		}
		
	}
	for (count = 0; count < height - 1; count++)
	{
		if (manycounter == howmany)
		{
			break;
		}
		if (M[count][0].down == 1)
		{
			M[count][0].down == 0;
			manycounter++;
		}

	}
	for (count = 0; count < height - 1; count++)
	{
		if (manycounter == howmany)
		{
			break;
		}
		if (M[count][width-1].down == 1)
		{
			M[count][width-1].down == 0;
			manycounter++;
		}

	}
	for (count = 1; count < height - 1; count++)
	{
		for (count2 = 1; count2 < width - 1; count2++)
		{

		}
	}
	for(count = 0; count < width; count++)
	{
		fprintf(fp,"+-");
	}
	fprintf(fp,"+\n");
	for(count = 0; count < height; count++)
	{
		for(count2 = 0; count2 < width; count2++)
		{
			if(M[count][count2].left == 0)
			{
				fprintf(fp," ");
			}
			if(M[count][count2].left == 1)
			{
				fprintf(fp,"|");
			}
			fprintf(fp," ");
			//printf("%d",M[count][count2].what);
			

		}
		fprintf(fp,"|\n");
		fprintf(fp,"+");
		for(count2 = 0; count2<width; count2++)
		{
			if(M[count][count2].down == 0)
			{
				fprintf(fp," ");
			}
			if(M[count][count2].down == 1)
			{
				fprintf(fp,"-");
			}
			fprintf(fp,"+");
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}