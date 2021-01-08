#ifndef _public_H
#define _public_H

#include "stdio.h"
#include "systick.h"
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"





#define FONT_IN_FLASH		(0x08030000)



typedef enum {FALSE = 0, TRUE = !FALSE} BOOL;

typedef struct
{
	unsigned char  fileName[128];
	unsigned char  fileDate[128];
	unsigned char  dots[54*512];
} FONT_INFOR;

extern FONT_INFOR fontInfo;

// ���ÿ��ض���
#define CF1					(1 << 0)
#define CF2					(1 << 1)
#define CF3					(1 << 2)
#define CF4					(1 << 3)
#define CF5					(1 << 4)
#define CF6					(1 << 5)
#define CF7					(1 << 6)
#define CF8					(1 << 7)


extern void init(void);
extern unsigned char fileName[];
extern unsigned char fileDate[];
extern unsigned char dots[];

extern unsigned char *sramFontAddr, *flashFontAddr;

extern void procCom(void);

//extern __weak void at7456_init(BOOL enDispHint);	// ��ʼ��AT7456�����û����
//extern __weak void display(void);					// ��λ�����Ļ��ʾ���������û����

extern void at7456_init(BOOL enDispHint);	// ��ʼ��AT7456�����û����
extern void display(void);					// ��λ�����Ļ��ʾ���������û����
uint8_t transfer(uint8_t data);


#endif
