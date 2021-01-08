#include "SPI.h"



void SPI1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

	// SPI��IO������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7); // PA5.6.7����

	// SPI��nCS����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4); 					// PA4����


	/***************************************************************************/
	/************************* ����SPI�Ĳ��� ***********************************/
	/***************************************************************************/

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// ѡ��ȫ˫��SPIģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		// ����ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	// 8λSPI
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;			// ʱ�����յ͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		// �ڵ�һ��ʱ�Ӳɼ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			// Nssʹ���������
	//ѡ������Ԥ��ƵΪ8
//    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	// 72/8 = 18MHz --- �ܿɿ�����������ҵ�AT7456�������ɿ�
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;	// 72/8 = 9MHz --- ��ҵ�AT7456�����ɿ�
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	// �����λ��ʼ����
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Cmd(SPI1, ENABLE);
	SPI_Init(SPI1, &SPI_InitStructure);
}

uint8_t transfer(uint8_t data)
{
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	
	SPI_I2S_SendData(SPI1, data); //ͨ������SPIx����һ��byte  ����
		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	
 		    
}