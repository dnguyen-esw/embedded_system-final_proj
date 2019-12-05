#include "ssd1306_i2c.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <mysql.h>
#include <stdlib.h>

int chucngan,ngan,tram,chuc_lux,donvi_lux;
int chuc,donvi,phaychuc,phaydonvi;
void tachso(float temp)
{
	 chucngan=(int)temp/10000%10+48;
	 ngan=(int)temp/1000%10+48;
	 tram=(int)temp/100%10+48;
	 chuc_lux=(int)temp/10%10+48;
	 donvi_lux=(int)temp%10+48;
	 
	 chuc=temp/10+48;
	 donvi=(int)temp%10+48;
	 phaychuc=((temp-(int)temp)*100)/10+48;
	 phaydonvi=(int)((temp-(int)temp)*100)%10+48;
}
int main(void) 
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char *server = "localhost";
	char *user = "root";
	char *password = "12345"; 
	char *database = "embedded_sys";
	
	float temperature,humidity,light;
	ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
	ssd1306_clearDisplay();
	
	while(1)
	{
		ssd1306_clearDisplay();
		ssd1306_drawFastHLine(0, 15, 130, 0);
		ssd1306_setTextSize(1);
		ssd1306_drawString("Embedded System                           ");
		ssd1306_drawChar(0,8,49,1,1);
		ssd1306_drawChar(5,8,54,1,1);
		ssd1306_drawChar(10,8,49,1,1);
		ssd1306_drawChar(15,8,52,1,1);
		ssd1306_drawChar(20,8,54,1,1);
		ssd1306_drawChar(26,8,67,1,1);
		ssd1306_drawChar(32,8,76,1,1);
		ssd1306_drawChar(38,8,50,1,1);
		
		conn = mysql_init(NULL);
		mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) ;
		
		mysql_query(conn, "SELECT Temperature FROM sensor WHERE ID=(SELECT MAX(ID) FROM sensor)");
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);
		temperature=atof(row[0]);
		
		mysql_query(conn, "SELECT Humidity FROM sensor WHERE ID=(SELECT MAX(ID) FROM sensor)");
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);
		humidity=atof(row[0]);
		
		mysql_query(conn, "SELECT Light FROM sensor WHERE ID=(SELECT MAX(ID) FROM sensor)");
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);
		light=atof(row[0]);
		tachso(temperature);
		
		printf("\nNhiet do: %.2f\nDo am: %.2f\nDo sang: %.2f\n",temperature,humidity,light);
		ssd1306_setTextSize(2);
		ssd1306_drawString("Temp:");
		ssd1306_drawChar(58,16,chuc,1,2);
		ssd1306_drawChar(70,16,donvi,1,2);
		ssd1306_drawChar(80,23,46,1,1);
		ssd1306_drawChar(86,16,phaychuc,1,2);
		ssd1306_drawChar(98,16,phaydonvi,1,2);
		ssd1306_drawChar(105,10,248,1,2);
		ssd1306_drawChar(115,16,67,1,2);
		
		tachso(humidity);
		ssd1306_drawString("     Humid:");
		ssd1306_drawChar(70,32,chuc,1,2);
		ssd1306_drawChar(82,32,donvi,1,2);
		ssd1306_drawChar(94,32,37,1,2);
		
		ssd1306_drawString("    Lux:");
		tachso(light);
		if(chucngan!=48)
			ssd1306_drawChar(43,48,chucngan,1,2);
		else
			ssd1306_drawChar(43,48,32,1,2);
			
		if(chucngan==48&&ngan==48)
			ssd1306_drawChar(55,48,32,1,2);
		else
			ssd1306_drawChar(55,48,ngan,1,2);
			
		if(chucngan==48&&ngan==48&&tram==48)
			ssd1306_drawChar(67,48,32,1,2);
		else
			ssd1306_drawChar(67,48,tram,1,2);
			
		if(chucngan==48&&ngan==48&&tram==48&&chuc_lux==48)
			ssd1306_drawChar(79,48,32,1,2);
		else
			ssd1306_drawChar(79,48,chuc_lux,1,2);
		ssd1306_drawChar(91,48,donvi_lux,1,2);
		
		
		//ssd1306_startscrollright(0x00, 0x08);

		ssd1306_display();
	
		mysql_free_result(res);
		mysql_close(conn);
	}
		
}
