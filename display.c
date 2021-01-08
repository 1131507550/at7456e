#include "display.h"
/*Define Chinese characters*/
extern int charset_array[];
/*通道*/
char Channel[15][11]={"ROLL","PIT","HEAD","DE","MOD","AR","ALT","CP","TIME","DATA"," LAT","ALT","CP","MIL"};
u8 data_addre[10]={0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x76};
void mydis_init()
{
	writeAddrData(VM0, 0x42);
	delay_us(40);
	writeAddrData(DMAH, 0x00);             // address
  writeAddrData(DMAL, 68);    
  writeAddrData(OSDM, 0x00);
}


void clear()
{
	unsigned int i; 
  writeAddrData(DMAH, 0x00); // address 
  writeAddrData(DMAL, 0); 
  writeAddrData(DMM, 0x01); 
  for (i = 0; i < (16*30); i++) {
    ReadAddr(0); 
  } 
  ReadAddr(0xff); 
}

void displayChar(unsigned char row, unsigned char col, unsigned short addr){ 
  unsigned short k;
  unsigned char addrH, j; 

  k = row * 30 + col; 
  addrH = k >> 8; 
  writeAddrData(OSDBL,0X00);				//背景的电平
  writeAddrData(DMM, 0x40);					//地址控制位
  writeAddrData(DMAH, addrH | 0x2); //地址
  writeAddrData(DMAL, k); 					
  j = CHAR_LBC << 5; 						
  if ((addr >> 8) != 0)
    j |= 0x10; 
  writeAddrData(DMDI, j);						
  writeAddrData(DMAH, addrH); 			
  writeAddrData(DMAL, k); 
  writeAddrData(DMDI, addr);
  writeAddrData(VM0, 0x48); 
}












void storeChar(unsigned short addr,int temp[]){
  int buf[54] = {0};
  int dt[54] = {0};
  int i = 0;
  int n = 0;
  int *p;
  for(i = 0;i < 54;i++){
    if(i < 8){
      dt[i] = 0;
    }
    else
      dt[i] = temp[i-8];
  }
  for(i = 0;i < 18;i++){
    p = extend(dt[i*2]);
    buf[n++] = *p;
    buf[n++] = *(p+1);
    p = extend(dt[i*2+1]);
    buf[n++] = *p;
  } 
 writeAT7456E(addr,buf);
}








int unified(int src){
  int num = 0;
  int i = 0;
  for(i = 0;i<8;i++){
    num = (num << 1)|((src >> i)&0x01);
  }
  return num;
}






int *extend(int src){
  int i = 0;
	static int tar[2];
  src = unified(src);
  
  tar[0] = 0;
  tar[1] = 0;
  for(i = 0;i < 4;i++){
    if((src>>i) & 0x01){
      tar[0] = 0x02 |(tar[0]<< 2);
    }
    else
      tar[0] = 0x01|(tar[0]<< 2);
  }
  for(i = 4;i < 8;i++){
    if((src>>i) & 0x01){
      tar[1] = 0x02 | (tar[1] << 2);
    }
    else
      tar[1] = 0x01| (tar[1] << 2);
  }
  return tar;
}







u8 writeAddrData(unsigned char addr, unsigned char value)
{
	u8 a;
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	a=transfer(addr);
	transfer(value);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	return a;
}







u8 ReadAddr(u8 addr)
{
	u8 a;
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	a=transfer(addr);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	return a;
}







void display_string(u8 row, u8 col,const char *s)
{
		if(!strcmp(s,"德"))
		 displayChar(row,col,0x49);
		if(!strcmp(s,"润"))
		 displayChar(row,col,0x4a);
		if(!strcmp(s,"清"))
		 displayChar(row,col,0x4b);
		if(!strcmp(s,"华"))
		 displayChar(row,col,0x4c);
		if(!strcmp(s,"水"))
		 displayChar(row,col,0x4d);
		if(!strcmp(s,"下"))
		 displayChar(row,col,0x4e);
		if(!strcmp(s,"有"))
		 displayChar(row,col,0x4f);
		if(!strcmp(s,"限"))
		 displayChar(row,col,0x50);
		if(!strcmp(s,"公"))
		 displayChar(row,col,0x51);
		if(!strcmp(s,"司"))
		 displayChar(row,col,0x52);
		if(!strcmp(s,"年"))
		 displayChar(row,col,0x62);
		if(!strcmp(s,"月"))
		 displayChar(row,col,0x63);
		if(!strcmp(s,"日"))
		 displayChar(row,col,0x64);
		if(!strcmp(s,"时"))
		 displayChar(row,col,0x65);
		if(!strcmp(s,"分"))
		 displayChar(row,col,0x66);
		if(!strcmp(s,"秒"))
		 displayChar(row,col,0x67);
		if(!strcmp(s,"开"))
		 displayChar(row,col,0x68);
		if(!strcmp(s,"关"))
		 displayChar(row,col,0x69);
		if(!strcmp(s,"锁"))
		 displayChar(row,col,0x6b);
		if(!strcmp(s,"不"))
		 displayChar(row,col,0x6c);
}









void my_demo(u8 row, u8 col,char *s)
{
	int i=0;
	char toob[2];
	char *a=toob;
	while(*s != '\0')
	{
		for(i=0;i<2;i++)
		{
			*a ++ = *s++;
		}
			*a = '\0';
		display_string(row,col,toob);
		if(col<30)
		col++;
		else
		row++,col=1;
		a=toob;
   
	}
}


void mychar8bit_dis(Details *datails)
{
	int uint,other,mind,lest;
	u8 addr;
	char temp[20];
	
	displayString(datails->add_row, datails->add_col, datails->c_name);
	datails->add_row +=1;
	mind=(datails->data[0]);
	sprintf(datails->other ,"%d",mind );
	displayString(datails->add_row ,datails->add_col,datails->other);

	//printf("ssss\r\n");
}


void mychar32bit_dis(Details *datails)
{
	int uint,other,mind,lest;
	u8 addr;
	char temp[20];
	
	displayString(datails->add_row, datails->add_col, datails->c_name);
	datails->add_row +=1;
	mind=(((datails->data[0]*256*256*256)+(datails->data[1]*256*256)+(datails->data[2]*256)+datails->data[3]));
	uint=((((datails->data[0]*256*256*256)+(datails->data[1]*256*256)+(datails->data[2]*256)+datails->data[3])/100)%10);
	other=(((datails->data[0]*256*256*256)+(datails->data[1]*256*256)+(datails->data[2]*256)+datails->data[3])/100);
	lest=mind-other*100;
	sprintf(datails->other ,"%d",(other-uint)/10 );
	sprintf(datails->lest ,"%d",lest );
	datails->uint =data_addre[uint];
	if(*(datails->other)!='0')
	{
		displayString(datails->add_row ,datails->add_col,datails->other);
		Address(datails,strlen(datails->other));
	}	
		displayChar(datails->add_row ,datails->add_col,data_addre[uint]);
		Address(datails,1);
	  displayString(datails->add_row ,datails->add_col,datails->lest);
	  Address(datails,strlen(datails->lest));
	//printf("ssss\r\n");
}




void mychar_dis(Details *datails)
{
	int uint,other,mind,lest;
	u8 addr;
	char temp[20];
	
	displayString(datails->add_row, datails->add_col, datails->c_name);
	datails->add_row +=1;
	mind=(((datails->data[0]*256)+datails->data[1]));
	uint=((((datails->data[0]*256)+datails->data[1])/100)%10);
	other=(((datails->data[0]*256)+datails->data[1])/100);
	lest=mind-other*100;
	sprintf(datails->other ,"%d",(other-uint)/10 );
	sprintf(datails->lest ,"%d",lest );
	datails->uint =data_addre[uint];
	if(*(datails->other)!='0')
	{
		displayString(datails->add_row ,datails->add_col,datails->other);
		Address(datails,strlen(datails->other));
	}	
		displayChar(datails->add_row ,datails->add_col,data_addre[uint]);
		Address(datails,1);
	  displayString(datails->add_row ,datails->add_col,datails->lest);
	  Address(datails,strlen(datails->lest));
	//printf("ssss\r\n");
}




void Address(Details *datails,u8 i)
{
		if(datails->add_col +i <30)
		{
			datails->add_col+=i;
		}
		else
		{
			datails->add_row  +=1;
			datails->add_col = (datails->add_col+i-30);
		}
}





void displayString(unsigned char row, unsigned char col, const char *s){
  unsigned short k;
  unsigned char addrH, j; 
  unsigned char c;
  unsigned short value; 
	int i = 0;
	int flag = 0;
  c = *s++; 
  k = row * 30 + col; 
  writeAddrData(OSDBL,0X00);
  while (c != 0){
    flag = 0;

    if(flag == 0){
      if ((c >= '0') && (c <='9'))
        value = ((c == '0')? 10 : c - '1' + 1);
      else if ((c >= 'A') && (c <= 'Z'))
        value = (c - 'A' + 11);
      else if ((c >= 'a') && (c <= 'z'))
        value = (c - 'a' + 37);
      else if(c == '.')
        value = (0x3f);
    }
    
    addrH = k >> 8; 
    writeAddrData(DMM, 0x40); 
    writeAddrData(DMAH, addrH | 0x2); 
    writeAddrData(DMAL, k); 
    j = CHAR_LBC << 5;
    if ((value >> 8) != 0)
      j |= 0x10; 
    writeAddrData(DMDI, j); 
    writeAddrData(DMAH, addrH); 
    writeAddrData(DMAL, k);
    if(value==0x3f)	
    {
			writeAddrData(DMDI, value);
		}	
		else{
			 writeAddrData(DMDI, value);
		}
   
    c = *s++;
    k = k+1;
  }
  writeAddrData(VM0, 0x48); 
}






void writeAT7456E(unsigned short addr, int *dt){ 
	
	unsigned char addrH, n;
  addrH = (addr >> 8); 

  writeAddrData(VM0, (0x00));
  delay_us(30);
  writeAddrData(CMAH, addr);

  writeAddrData(DMM, 0);

  for (n = 0; n < 54; n++) {
    char i = *dt;
    writeAddrData(CMAL, n | (addrH << 6));
    writeAddrData(CMDI, i); 
    dt++;
  } 
  writeAddrData(CMM, RAM_NVM);  
  delay_us(10);

  writeAddrData(VM0, 0x01<<3);
} 








void wite_char(int a)
{
	int i=0;
	
	for(i=0;i<a;i++)
	{
		writeAT7456E(i+1,&charset_array[(i*54)]);
		delay_ms(50);
	}
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}











void Dynamic_display(u8 data)
{
	int i=0;
	switch(data)
	{
		case 0x01: 
			  GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				displayString(11,0,Channel[data]);	
			break;
		case 0x02: 
			  GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				displayString(11,6,Channel[data]);	
			break;
		case 0x03: 
			  GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				displayString(11,11,Channel[data]);	
			break;
		case 0x04: 
			  GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				displayString(11,17,Channel[data]);	
			break;
		case 0x05: 
			  GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				displayString(11,20,Channel[data]);	
			break;
		case 0x06: 
			  GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				displayString(13,0,Channel[data]);	
			break;
		case 0x07: 
			  GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				displayString(13,6,Channel[data]);	
			break;
		case 0x08: 
			  GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				displayString(13,11,Channel[data]);	
			break;
		case 0x09: 
			  GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				displayString(13,17,Channel[data]);	
			break;
		case 0x0a: 
			  GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				displayString(13,20 ,Channel[data]);	
			break;
	}
}


VER7456 checkAT7456(void)
{
	unsigned char r1, r2;

	r1 = ReadAddr(VM0);
	r2 = (r1 & ~(1 << 1)) | 0x88; 		// VM0.1(Software Reset Bit) = 0，同时将VM0.3置位1
	writeAddrData(VM0, r2);	// 写VM0
	delay_us(30);
	r2 = ReadAddr(VM0) & 0x88;
	writeAddrData(VM0, r1);	// 恢复VM0
	delay_us(30);

	if (r2 == 0x88)
	{
		return NEW7456;					// 新版7456
	}
	else if (r2 == 0x08)
	{
		return OLD7456;					// 老版7456
	}
	else
	{
		return BAD7456;					// SPI接口异常
	}
}
