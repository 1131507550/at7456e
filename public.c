#include "public.h"
#include "rtc.h"
#include "beep.h"
#include "stdio.h"
FONT_INFOR fontInfo;

/*******************************************************************************
* 函 数 名			: time_init
* 函数功能			: 定时器2, 1ms
					: 定时器4, 在PB8上产生10KHz方波信号
* 输    入			: 无
* 输    出			: 无
*******************************************************************************/

int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}

  void uart_init(){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = 115200;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}
	



/****************************************************************************
* Function Name  : SPI1_Config
* Description    : 初始化SPI1
* Input          : None
* Return         : None
****************************************************************************/
void SPI1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

	// SPI的IO口设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7); // PA5.6.7上拉

	// SPI的nCS设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4); 					// PA4上拉
	GPIO_SetBits(GPIOA, GPIO_Pin_1); 					// PA4上拉

	/***************************************************************************/
	/************************* 设置SPI的参数 ***********************************/
	/***************************************************************************/

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// 选择全双工SPI模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		// 主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	// 8位SPI
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;			// 时钟悬空低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		// 在第一个时钟采集数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			// Nss使用软件控制
	//选择波特率预分频为8
//    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	// 72/8 = 18MHz --- 能可靠工作，但外挂的AT7456工作不可靠
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;	// 72/8 = 9MHz --- 外挂的AT7456工作可靠
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	// 从最高位开始传输
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Cmd(SPI1, ENABLE);
	SPI_Init(SPI1, &SPI_InitStructure);
}

uint8_t transfer(uint8_t data)
{
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SPI1, data); //通过外设SPIx发送一个byte  数据
		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
 		    
}
void init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//SPI1_Config();
	uart_init();			// 串口1初始化
}

void USART1_IRQHandler(void)
{
	u8 Res;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
		{
		Res =USART_ReceiveData(USART1);	
		USART_SendData(USART1,Res);
		   		 
     } 
}


