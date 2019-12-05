#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <mysql.h>
#include <stdlib.h>

float temp, humid, lux;
int h,m,s;
int si,rt,ls;//device
uint16_t read_light_ls(int add)
{
	uint16_t value=wiringPiI2CReadReg16(ls,add);
	delay(130);
	lux =(float)value/1.2;
	return lux;
}
int16_t read_temp_si(int add)
{
	uint16_t tempcode = wiringPiI2CReadReg16(si,add);
	uint16_t high_byte, low_byte, nd;
	high_byte = tempcode & 0xFF;
	low_byte = (tempcode>>8) & 0xFF;
	nd = (high_byte<<8) | low_byte;
 	temp=(float)nd*175.72/65536.0-46.85;
 	delay(15);
 	return temp;
}

int16_t read_humid_si(int add)
{
	uint16_t humidcode = wiringPiI2CReadReg16(si,add);
	uint16_t high_byte, low_byte, da;
	high_byte = humidcode & 0xFC;
	low_byte = (humidcode>>8) & 0xFF;
	da = (high_byte<<8) | low_byte;
 	humid=(float)da*125/65536.0-6.0;
 	delay(15);
 	return humid;
}
void read_time()
{
	h=wiringPiI2CReadReg8(rt,0x02);
	m=wiringPiI2CReadReg8(rt,0x01);
	s=wiringPiI2CReadReg8(rt,0x00);
		
	h = ( h >> 4 ) * 10 + (h & 0x0F);
	m = ( m >> 4 ) * 10 + (m & 0x0F);
	s = ( s >> 4 ) * 10 + (s & 0x0F);
}
int main(void)
{
	wiringPiSetup();
	si=wiringPiI2CSetup(0x40);
	rt=wiringPiI2CSetup(0x68);
	ls=wiringPiI2CSetup(0x23);
	
	wiringPiI2CWriteReg8(si,0xE6,0x00);
	
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char *server = "localhost";
	char *user = "root";
	char *password = "12345"; 
	char *database = "embedded_sys";
	conn = mysql_init(NULL);
	while(1)
	{
		read_temp_si(0xE3);
		read_humid_si(0xE5);
		read_light_ls(0x10);
		read_time();
		
		//Connect to database 
		mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) ;
		mysql_query(conn, "SELECT * FROM sensor");
		res = mysql_store_result(conn);
		
		printf ("Number of rows: %lu\n", (unsigned long) mysql_num_rows(res));
		unsigned long num_row=mysql_num_rows(res);
			
			//limit 5 values on table
		if(num_row<50)
		{
				char cmd[200];
				sprintf(cmd,"INSERT INTO sensor(Temperature,Humidity,Light,Time) VALUES(%.2f,%.2f,%.0f,'%d:%d:%d')",temp,humid,lux,h,m,s); 
				mysql_query(conn, cmd);
		}
			
		else if(num_row>=50)
		{
			char cmd[200];
			sprintf(cmd,"DELETE FROM sensor ORDER BY id LIMIT 1"); //sap xep tang dan va xoa di 1 gia tri
			mysql_query(conn, cmd);
			sprintf(cmd,"INSERT INTO sensor(Temperature,Humidity,Light,Time) VALUES(%.2f,%.2f,%.0f,'%d:%d:%d')",temp,humid,lux,h,m,s); 
			mysql_query(conn, cmd);
		}
			
		delay(840);
	}
	
}
