#include <mysql.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(void)
{
	wiringPiSetup();
	pinMode(22, OUTPUT);//blue-led
	pinMode(23, OUTPUT);//white-led
	pinMode(24, OUTPUT);//fan
	pinMode(25, OUTPUT);//pump
    
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	int state;
	int dev[5];//devices
	char *server = "localhost";
	char *user = "root";
	char *password = "12345"; 
	char *database = "embedded_sys";
	conn = mysql_init(NULL);
	while(1)
	{
		
		//Connect to database 
		mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) ;
		
		mysql_query(conn, "SELECT * FROM io_dev");
		res=mysql_store_result(conn);//retrieve all data from the onoff table.
		int num_fields = mysql_num_fields(res);//get the number of fields (columns) in the table
		int i=0;
		while ((row = mysql_fetch_row(res))) 
		{
			dev[i]=atoi(row[1]);
			printf("Dev[%d]:%d\n",i,dev[i]);
			i++;
		}
		
		digitalWrite(22,dev[0]);//fan
		digitalWrite(23,dev[1]);//pump
		digitalWrite(24,dev[2]);//blue
		digitalWrite(25,dev[3]);//white
		
		mysql_free_result(res);
		mysql_close(conn);
		return 0;
	}
}
