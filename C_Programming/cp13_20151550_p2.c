#include<stdio.h>
#include<string.h>

enum getdate{mon = 1, tues, weds, thurs, fri, sat, sun = 0};
enum getdate getDate(int y, int m, int d);

char* getDay(enum getdate currDate);

int main()
{
	enum Date{mon = 0, tues, weds, thurs, fri, sat, sun};
	struct Calendar{
		int year,month,day;
		enum Date date;
		char* dateString;
	};
	struct Calendar cd;

	int result;

	printf("Year : ");
	scanf("%d",&cd.year);
	printf("Month : ");
	scanf("%d",&cd.month);
	printf("Dat : ");
	scanf("%d",&cd.day);

	cd.date = getDate(cd.year,cd.month,cd.day);

	cd.dateString = getDay(cd.date);

	printf("dataString : %s\n",cd.dateString);
}
enum getdate getDate(int y, int m, int d)
{
	
	return (y + y/4 - y/100 + y/400 + (13 * m + 8) / 5 + d)%7;
	
}

char* getDay(enum getdate currDate)
{
	switch(currDate){

		case mon:
		return "Monday";
		case tues:
		return "Tuesday";
		case weds:
		return "Wednesday";
		case thurs:
		return "Thursday";
		case fri:
		return "Friday";
		case sat:
		return "Saturday";
		case sun:
		return "Sunday";
	}
	
}
