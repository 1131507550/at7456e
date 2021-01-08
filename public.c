#include "public.h"
#include "rtc.h"
#include "beep.h"
#include "stdio.h"
FONT_INFOR fontInfo;

/*******************************************************************************
* �� �� ��			: time_init
* ��������			: ��ʱ��2, 1ms
					: ��ʱ��4, ��PB8�ϲ���10KHz�����ź�
* ��    ��			: ��
* ��    ��			: ��
*******************************************************************************/

int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}

  void uart_init(){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = 115200;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}
	



/****************************************************************************
* Function Name  : SPI1_Config
* Description    : ��ʼ��SPI1
* Input          : None
* Return         : None
****************************************************************************/
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
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4); 					// PA4����
	GPIO_SetBits(GPIOA, GPIO_Pin_1); 					// PA4����

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
void init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//SPI1_Config();
	uart_init();			// ����1��ʼ��
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


