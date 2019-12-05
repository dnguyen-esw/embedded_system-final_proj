#include <mysql.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(void)
{
	wiringPiSetup();
	pinMode(26, OUTPUT);//buzzer
    
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	int state;
	char *server = "localhost";
	char *user = "root";
	char *password = "12345"; 
	char *database = "embedded_sys";
	float temperature,humidity,light;
	float real_temp,real_humid,real_light;
	while(1)
	{
		//Connect to database 
		conn = mysql_init(NULL);
		mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) ;
		
		mysql_query(conn, "SELECT temp FROM alarm");
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);
		temperature=atof(row[0]);
		printf("\n%.2f\n",temperature);
		
		mysql_query(conn, "SELECT humid FROM alarm ");
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);
		humidity=atof(row[0]);
		printf("%.2f\n",humidity);
		
		mysql_query(conn, "SELECT light FROM alarm ");
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);
		light=atof(row[0]);
		printf("%.0f\n",light);
		
		mysql_query(conn, "SELECT Temperature FROM sensor WHERE ID=(SELECT MAX(ID) FROM sensor)");
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);
		real_temp=atof(row[0]);
		printf("\n%.2f\n",real_temp);
		
		mysql_query(conn, "SELECT Humidity FROM sensor WHERE ID=(SELECT MAX(ID) FROM sensor)");
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);
		real_humid=atof(row[0]);
		printf("%.2f\n",real_humid);
		
		mysql_query(conn, "SELECT Light FROM sensor WHERE ID=(SELECT MAX(ID) FROM sensor)");
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);
		real_light=atof(row[0]);
		printf("%.2f\n",real_light);
		
		if((real_temp>temperature)||(real_humid>humidity)||(real_light>light))
		{
			digitalWrite(26,1);
		}
		else
		{
			digitalWrite(26,0);
		}
		mysql_free_result(res);
		mysql_close(conn);
	}
}
