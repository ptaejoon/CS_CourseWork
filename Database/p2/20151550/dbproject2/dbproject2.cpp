#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "mysql.h"
#include <string.h>

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "sogangsp";
const char* db = "delivery";

int main(void) {

	MYSQL* connection = NULL; 
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	int select_q;
	int sub = 0;
	int truck_correct = 0;
	char user_input[50];
	char query[300];
	const char* c_db = "create database delivery;";
	const char* use_db = "use delivery;";
	const char* drop_db = "drop database delivery;";
	const char* query1 = "select distinct(name) from delivery_record where name != 'recipient' and arrival_time is NULL;";
	const char* query11 = "select name,customer_id from customer where customer_id = (select distinct(p.customer_id) from product as p inner join delivery_record as d on p.product_id=d.product_id where d.name=";
	const char* query112 = " and d.arrival_time is NULL);";
	const char* query12 = "select distinct(p.recipient_name) from product as p inner join delivery_record as d on p.product_id=d.product_id where d.name= ";
	const char* query122 = " and d.arrival_time is NULL;";
	const char* query13 = "select * from delivery_record where name=  ";
	const char* query132 = " and arrival_time < (select distinct(start_time) from delivery_record where name=";
	const char* query133 = " and arrival_time is NULL) order by start_time desc limit 1;";
	const char* query2 = "select c.name,c.customer_id,count(p.customer_id) from customer as c join product as p on c.customer_id = p.customer_id where year(p.order_date)= ";
	const char* query22 = " group by p.customer_id order by count(p.customer_id) desc limit 1";
	const char* query3 = "select name, customer_id from customer where customer_id = (select customer_id from product as p inner join charge_info as c on p.package_type = c.package_type and p.timeliness = c.timeliness and p.weight = c.weight where year(order_date)=";
	const char* query32 = " group by customer_id order by sum(charge) desc limit 1)";
	const char* query4 = "select p.product_id from product as p inner join delivery_record as d on p.product_id = d.product_id where d.name = 'recipient' and d.start_time > p.eta";
	const char* query51 = "select product_id,product_name,charge,pay_type,address,recipient_name,timeliness,package_type,weight,order_date,caution from product natural join charge_info where customer_id = ";
	const char* query52 = " and year(order_date)= ";
	const char* query53 = " and month(order_date)= ";
	const char* query54 = "select customer_id,name,address from customer where customer_id = ";
	//Query Declaration
	//in while loop, queries with same number will be concatenated with user input
	int state = 0;
	int write = 0;
	char create_q[1001];
	char truck[20];
	char customer[10];
	char date[10];
	char year[5];
	char month[3];
	float amount;
	FILE* create_f,*insert_f,*bill;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw,NULL, 3306, (const char*)NULL, 0);
	//DB connection
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}
	else
	{ // when connection succeed
		create_f = fopen("table_creation_one_line.sql", "r"); 
		state = mysql_query(connection, c_db);
		//"create delivery database"
		state = mysql_query(connection, use_db);
		//"use delivery database"
		while (fgets(create_q,1000,create_f) != NULL)
		{
			state = mysql_query(connection, create_q);
		}
		//open create table sql file and execute
		fclose(create_f);
		insert_f = fopen("table_insertion.sql", "r");
		while (fgets(create_q, 1000, insert_f) != NULL)
		{
			state = mysql_query(connection, create_q);
		}
		//open insert table sql file and execute

		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}
		printf("***********************************************\n");
		printf("** Sogang Package Delivery Management System **\n");
		printf("***********************************************\n");
		//start of program! let's go!
		while (1) {
			
			printf("------- SELECT QUERY TYPES -------\n\n");
			printf("        1. TYPE I\n");
			printf("        2. TYPE II\n");
			printf("        3. TYPE III\n");
			printf("        4. TYPE IV\n");
			printf("        5. TYPE V\n");
			printf("        0. QUIT\n");
			printf("Any Input not in 0-5 will be ignored.\n");
			printf("Which type of query? : ");
			scanf("%s", &user_input);
			select_q = atoi(user_input);
			if (user_input[0] != '0' && select_q == 0)select_q = 6;
			//use switch for types
			switch (select_q)
			{
				case 0:
					printf("HAVE A GOOD DAY :) \n");
					mysql_query(connection, drop_db);
					mysql_close(connection);
					//finish program
					return 0;
				case 1:
					sub = 1;
					truck_correct = 0;
					while (1)
					{
						//type 1
						if (truck_correct == 0)
						{
							memset(truck, 0, 20);
							printf("---- TYPE I ----\n");
							printf("Input the number of truck : ");
							scanf("%s", &user_input);
							select_q = atoi(user_input);
							if (select_q == 0)break;
							state = mysql_query(connection, query1);
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								memset(truck, 0, 20);
								strcpy(truck, "truck ");
								strcat(truck, user_input);
								if (strcmp(truck, sql_row[0]) == 0)
								{
									truck_correct = 1;
								}
							}//find truck if it had an accident before
							mysql_free_result(sql_result);
							
							if(truck_correct == 0)
							{
								printf("%s is not destroyed\n",truck);
								continue;
							}
						}
						printf("----- Subtypes in TYPE I -----\n");
						printf("    1. TYPE I - 1.\n");
						printf("    2. TYPE I - 2.\n");
						printf("    3. TYPE I - 3.\n");
						printf("    0. QUIT.\n");
						printf("Which type of query? ");
						scanf("%s", &user_input);
						select_q = atoi(user_input);
						if (user_input[0] != '0' && select_q == 0)select_q = 6;
						switch (select_q)// use switch for subtypes
						{
							case 0:
								sub = 0;
								break;
							case 1:
								printf("---- TYPE I-1 ----\n\n");
								printf("** Find all customers who had a package on the truck at the time of the crash.**\n");
								memcpy(query, query11, strlen(query11) + 1);
								strcat(query, "'");
								strcat(query, truck);
								strcat(query, "'");
								strcat(query, query112);
								state = mysql_query(connection, query);
								if (state == 0)
								{
									sql_result = mysql_store_result(connection);
									while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
									{
										printf("Customer name : %s id : %s \n", sql_row[0] , sql_row[1]);
									}
									mysql_free_result(sql_result);
								}
								else
								{
									printf("Must be Error on Query.\n");
								}
								break;
							case 2:
								printf("---- TYPE I-2 ----\n\n");
								printf("** Find all recipients who had a package on that truck at the time of the crash.**\n");
								memcpy(query, query12, strlen(query12) + 1);
								strcat(query, "'");
								strcat(query, truck);
								strcat(query, "'");
								strcat(query, query122);
								printf("%s\n", query);
								state = mysql_query(connection, query);
								if (state == 0)
								{
									sql_result = mysql_store_result(connection);
									while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
									{
										printf("Recipient : %s \n", sql_row[0]);
									}
									mysql_free_result(sql_result);
								}
								else
								{
									printf("Must be Error on Query.\n");
								}
								break;
							case 3:
								
								printf("---- TYPE I-3 ----\n\n");
								printf("** Find the last successful delivery by that truck prior to the crash.**\n");
								memcpy(query, query13, strlen(query13) + 1);
								strcat(query, "'");
								strcat(query, truck);
								strcat(query, "'");
								strcat(query, query132);
								strcat(query, "'");
								strcat(query, truck);
								strcat(query, "'");
								strcat(query, query133);

								state = mysql_query(connection, query);
								if (state == 0)
								{
									sql_result = mysql_store_result(connection);
									printf("Last Delivery Record :\n");
									while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
									{
										printf("Product id : %s Name : %s Arrival time : %s Start time : %s \n", sql_row[0],sql_row[1],sql_row[2],sql_row[3]);
									}
									mysql_free_result(sql_result);
								}
								else
								{
									printf("Must be Error on Query.\n");
								}
								break;
							default:
								printf("Wrong Input Error.Please insert type number 0 - 3\n");
						}
						if (sub == 0)break;
					}
					break;
				case 2:
					//type 2
					while (1)
					{
						printf("---- TYPE II ----\n\n");
						printf("** Find the customer who has shipped the most packages in certain year**\n");
						printf(" Which Year? : ");
						scanf("%s", &user_input);
						select_q = atoi(user_input);
						if (strcmp(user_input, "0") == 0)break;
						if (user_input[0] != '0' && select_q == 0)
						{
							printf("Input Error. Insert year only with integer!\n");
							continue;
						}
						memcpy(query, query2, strlen(query2)+1);
						strcat(query, user_input);
						strcat(query, query22);
						state = mysql_query(connection, query);
						if (state == 0)
						{
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("Customer name : %s id : %s total order : %s \n", sql_row[0], sql_row[1],sql_row[2]);
							}
							mysql_free_result(sql_result);
						}
						else
						{
							printf("Must be Error on Query.\n");
						}
					}
					break;
				case 3: // type 3
					while (1)
					{
						printf("---- TYPE III ----\n\n");
						printf("** Find the customer who has spent the most money on shipping in the past certain year**\n");
						printf(" Which Year? : ");
						scanf("%s", &user_input);
						select_q = atoi(user_input);
						if (strcmp(user_input, "0") == 0)break;
						if (user_input[0] != '0' && select_q == 0)
						{
							printf("Input Error. Insert year only with integer!\n");
							continue;
						}
						memcpy(query, query3, strlen(query3) + 1);
						strcat(query, user_input);
						strcat(query, query32);
						state = mysql_query(connection, query);
						if (state == 0)
						{
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								printf("Customer Name : %s ID : %s \n", sql_row[0], sql_row[1]);
							}
							mysql_free_result(sql_result);
						}
						else
						{
							printf("Must be Error on Query. Please insert year between 2018-2020\n");
						}
					}
					break;
				case 4: //type 4
					printf("---- TYPE IV ----\n\n");
					printf("** Find those packages that were not delivered within the promised time**\n");
					if (user_input[0] != '0' && select_q == 0)
					{
						printf("Input Error. Insert year only with integer!\n");
						continue;
					}
					memcpy(query, query4, strlen(query4) + 1);
					state = mysql_query(connection, query);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						printf("Late Delivered Product's ID : ");
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s ", sql_row[0]);
						}
						printf("\n");
						mysql_free_result(sql_result);
					}
					break;
				case 5:// type 5

					while (1)
					{
						//customer , date
						amount = 0;
						write = 0;
						printf("---- TYPE V ----\n\n");
						printf("** Generate the bill for each customer for the past certain month.**\n");
						printf(" Customer Id : ");
						scanf("%s", &user_input);
						if (strcmp(user_input, "0") == 0)break;
						memset(customer, 0, 10);
						memset(date, 0, 10);
						strcpy(customer, user_input);
						printf("Which month(YYYY-MM)? ");
						scanf("%s", &user_input);
						if (strcmp(user_input, "0") == 0)break;
						strcpy(date, user_input);

						printf("Generating Bill..\n");
						memcpy(query, query51, strlen(query51) + 1);
						strcat(query, customer);
						strcat(query, query52);
						strcpy(year, strtok(date, "-"));
						strcat(query, year);
						strcat(query, query53);
						strcpy(month, strtok(NULL, "-"));
						strcat(query, month);
						state = mysql_query(connection, query);
						if (state == 0)
						{
							sql_result = mysql_store_result(connection);
							while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
							{
								write = 1;
								// check if it had record
								amount += strtod(sql_row[2], NULL);
							}
							mysql_free_result(sql_result);
							if (write == 0)
							{
								//if record does not exists, print it fails to make bill!
								printf("There was no matching information on database\n");
							}
							else
							{
								//make bill!
								bill = fopen("bill.txt", "w");
								fprintf(bill,"Customer ID    Name    Address    Amount\n");
								memcpy(query, query54, strlen(query54) + 1);
								strcat(query, customer);
								
								mysql_query(connection, query);
								sql_result = mysql_store_result(connection);
								sql_row = mysql_fetch_row(sql_result);
								fprintf(bill, "%s        %s        %s       $ %.4f\n", sql_row[0], sql_row[1], sql_row[2], amount);
								fprintf(bill, "-----------------------------------------\n");
								fprintf(bill, "Itemized Billing List\n");
								fprintf(bill, "====================================================================================================\n");
								fprintf(bill, "| product_id | product_name | charge | pay_type | address | recipient_name | timeliness | package_type | weight | order_date | caution |\n");
								fprintf(bill, "====================================================================================================\n");
								memcpy(query, query51, strlen(query51) + 1);
								strcat(query, customer);
								strcat(query, query52);
								strcat(query, year);
								strcat(query, query53);
								strcat(query, month);
								state = mysql_query(connection, query);
								sql_result = mysql_store_result(connection);
								while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
								{
									fprintf(bill,"| %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | \n", sql_row[0], sql_row[1],sql_row[2],sql_row[3],sql_row[4],sql_row[5],sql_row[6],sql_row[7],sql_row[8], sql_row[9], sql_row[10]);
								}
								fprintf(bill, "----------------------------------------------------------------------------------------------------\n");
								printf("Generating Done!\n");
								fclose(bill);
								
							}
						}
						else
						{
							printf("There's no matching information on database\n");
						}
					}
					break;
				default:
					printf("Wrong Input Error. Please insert type number 0 - 5\n");
			}
		}
		
	}
	return 0;
}