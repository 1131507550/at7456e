#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include "AT7456.H"
#include "sys.h"
#include "SPI.h"
#include "string.h"
#include "delay.h"

void mydis_init();
void clear();
void my74_display();
int *extend(int src);
int unified(int src);
void writeAT7456E(unsigned short addr, int *dt);
void displayString(unsigned char row, unsigned char col, const char *s);
void display_my();
u8 writeAddrData(unsigned char addr, unsigned char value);
void wite_char(int a);
void display_string(u8 row, u8 col,const char s[]);
void my_demo(u8 row, u8 col,char *s);
u8 ReadAddr(u8 addr);
void Dynamic_display(u8 data);
u8 compare(double a);
void displayChar(unsigned char row, unsigned char col, unsigned short addr);
typedef struct
{
  char ascii;
  short addr;
}AsciiAddr;



typedef struct
{
	char c_name[5];
	char data[8];
	u8 Id;
	u8 add_row;
	u8 add_col;
	char other[5];
	u8 uint;
	char lest[5];
} Details;

void mychar8bit_dis(Details *datails);//32位显示
void mychar_dis(Details *datails);//地址计算，显示
void Address(Details *datails,u8 i); //地址偏移计算
void mychar32bit_dis(Details *datails);//32位显示
extern const AsciiAddr tAsciiAddr[];
extern char Channel[15][11];
#endif