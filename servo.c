#include <mysql.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <softPwm.h>
#include <string.h>

int main(void)
{
	wiringPiSetup();
    softPwmCreate (7, 0, 100) ;
    
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	int state;
	int dev;//devices
	char *server = "localhost";
	char *user = "root";
	char *password = "12345"; 
	char *database = "embedded_sys";
	while(1)
	{
		
		conn = mysql_init(NULL);
		//Connect to database 
		mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) ;
		
		mysql_query(conn, "SELECT * FROM servo");
		res=mysql_store_result(conn);//retrieve all data from the onoff table.
		row = mysql_fetch_row(res);
		dev=atoi(row[1]);
	
		//roof
		if(dev==1)
		{
			softPwmWrite(7,15);
			delay(70);
		}
		if(dev==0)
		{
			softPwmWrite(7,30);
			delay(30);
		}
		mysql_free_result(res);
		mysql_close(conn);
	}
}
