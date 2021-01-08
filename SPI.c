#include "SPI.h"



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
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4); 					// PA4上拉


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