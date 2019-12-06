#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
void display(long int number)
{
	int count=0;
	long n=number;
	while(n!=0)
	{
		count++;
		n/=10;
	}

	unsigned char buffer[2];
	buffer[0] = 0x0B;//scan limit, gioi han so led sang
	buffer[1] =5;
	wiringPiSPIDataRW(0, buffer, 2);
	if(count==4||count==5) count=6;//nếu thời gian là 00:xx:xx hoặc 0x:xx:xx thì led thứ 6 sẽ ko sáng do là số 0 vô nghĩa=>>để led sáng count =6
	for(int i=1;i<=count;i++)
	{
		buffer[0]=i;
		if(i==3||i==5)
			buffer[1]=number%10|0x80;//0x80:display dot, hien thi dấu chấm tại led thứ 3 & led thứ 5
		else
			buffer[1]=number%10;
		number/=10;
		wiringPiSPIDataRW(0, buffer, 2);
		delay(3);
	}
	
	delay(3);
	
}

int main()
{
	
	int rt;
	wiringPiSetup();
	rt=wiringPiI2CSetup(0x68);
	
	unsigned char buffer[100];
	unsigned char mssv[8] ={1,6,1,4,6,0,9,1};
	// initialize SPI1 interface, speed 500 kHz
	wiringPiSPISetup(0, 1000000);

	// send and read 1 byte
	
	buffer[0]=0x09;//decode
	buffer[1]=0xff;//B decode
	wiringPiSPIDataRW(0, buffer, 2);
	delay(3);
	
	buffer[0] = 0x0A;//inten
	buffer[1] =0x0F;//
	wiringPiSPIDataRW(0, buffer, 2);
	delay(3);
	
	buffer[0] = 0x0B;//scan
	buffer[1] =0x07;//
	wiringPiSPIDataRW(0, buffer, 2);
	delay(3);
	
	buffer[0] = 0x0C;//shutdown register
	buffer[1] =0x01;//normal operation
	wiringPiSPIDataRW(0, buffer, 2);
	delay(3);
	
	buffer[0]=0x0F;//display test
	buffer[1]=0x01;
	wiringPiSPIDataRW(0, buffer, 2);
	delay(3);
	
	buffer[0]=0x0F;//display data
	buffer[1]=0x00;
	wiringPiSPIDataRW(0, buffer, 2);
	delay(3);
	
	while(1)
	{
		int year, month, date, day, h, m ,s;
		
		//year=wiringPiI2CReadReg8(rt,0x06);
		//month=wiringPiI2CReadReg8(rt,0x05);
		//date=wiringPiI2CReadReg8(rt,0x04);
		//day=wiringPiI2CReadReg8(rt,0x03);
		h=wiringPiI2CReadReg8(rt,0x02);
		m=wiringPiI2CReadReg8(rt,0x01);
		s=wiringPiI2CReadReg8(rt,0x00);
		
		//year = (year >> 4 ) * 10 + (year & 0x0F) ;
		//month = (month >> 4 ) * 10 + (month & 0x0F);
		//date = (date >>4) * 10 + (date & 0x0F);
		h = ( h >> 4 ) * 10 + (h & 0x0F);
		m = ( m >> 4 ) * 10 + (m & 0x0F);
		s = ( s >> 4 ) * 10 + (s & 0x0F);
		display(h*10000+m*100+s);
		delay(1000);
		printf("\nThoi gian: %d:%d:%d\n",h,m,s);
	}

	return 0;
}
