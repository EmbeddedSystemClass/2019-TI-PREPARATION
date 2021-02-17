#include "AD7367_SPI.h"
u16 ADC_date[4];//4���ֽ�DMA���䣬����Ƶ�����ж�������

//ת��ʱ�����1.25��s,��ת�������źŵ���ʼת����СΪ40ns��ʱ����2��s
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//ʹ��SPI1ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_3; //AD7367ת���źź�Ƭѡ�ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��ָ��IO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                             //���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                           //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                                       //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                             //����
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
	
	AD7367_CNVST(1);
	AD7367_CS(1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);//SCK
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);//MISO

	SPI_Cmd(SPI1,DISABLE);  
	
	SPI_I2S_DeInit(SPI1);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//��λSPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//ֹͣ��λSPI1
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_RxOnly ;  //���ó�ֻ����ģʽ�����SCK
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI��ģʽ�²Żᷢ������
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//���ݲ����ڵ�1��ʱ���ؼ��½���
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16��72M/8=9M��
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//���ݴ����MSBλ��ʼ
	
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ�������������п��ܳ�����
	
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	SPI_Cmd(SPI1,DISABLE);//ʹ��SPI1

}

/*
normal mode��ȡAD��ֵ
 */ 
int sign1;
int sign2;
u32 date;//ֱ�Ӵ洢�ĸ��ֽڵ�����
void AD7367_Read_Reg()
{
	u8 i=1;
	uint8_t reg_val;
	AD7367_SCK_HIGN;//����Ϊ��
	AD7367_CNVST(0);//�൱�ڸ������źţ��½��ظ������ź�,�������ֿ�ʼ
  while(i--);//��ʱ��Լ80ns�������źű����㹻�̣���֤��������ģʽ
	AD7367_CNVST(1);//
  delay_us(2);//ת��ʱ�������1.25��s,���Դ��Ϊ0.5��s
	AD7367_CS(0);//�����˳���̬�����̾����MSB
  SPI1_Read4byte();//
	AD7367_CS(1); //�ر�Ƭѡ����ȡ���ĸ��ֽڣ��ָ�����̬��
	
	//�����ص����ݽ��д���
	date=ADC_date[0]<<24|ADC_date[1]<<16|ADC_date[2]<<8|ADC_date[3];
	date=date>>4;
	sign1=date&0x1fff;//��һλ�Ƿ���λ��1Ϊ����Ϊ�����ǲ����ʾ��
	if(date&0x2000)//˵���ǲ���
		  sign1=-1*((~sign1)+1);

	sign2=(date>>14)&0x1fff;
	if((date>>14)&0x2000)
		   sign2=-1*((~sign2)+1);
	date=0;
	//ת�����и� t quiet����С��50ns
}

/*
  ���ڶ�ȡ4���ֽڵ����ݴ�AD7367��
 */ 
u8 SPI1_Read4byte()
{	
	u8 i;
	u32 retry=0;
	SPI_Cmd(SPI1,ENABLE);//ʹ��SPI1
   for(i=0;i<4;i++)
  {	
			delay_us(1);//ת��ʱ�������1.25��s,���Դ��Ϊ0.5��s
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		  retry++;
			if(retry>200000)return 0;
		}
		ADC_date[i]=SPI1->DR;//����̫��Ӧ�û����ݻᱻ���ǣ�ÿ�ζ�ȡRXNE���ᱻ���㣬��������ת��
	}			
  SPI_Cmd(SPI1,DISABLE);//ʹ��SPI1	
}
/*********************ģ��SPI**************************/


void AD7367_swapByte()
{  
    u8 i = 0;
	 
    for(i=0;i<28;i++)
    {   
			  delay_us(1);
			  AD7367_SCK_HIGN;//�������SPIӦ���ڸߵ�ƽ�ڼ������
        if(GPIO_ReadInputDataBit(GPIO_SPI,AD7367_MISO))
        {
            date|=1;//�ȴ�������MSB��MSB�Ƿ���λ
					  delay_us(1);
        }
		    	date = date<<1;
           AD7367_SCK_LOW;//�½��ع���ֻ��7ns��Ӳ����ȡӦ���������ʱ��
    }
}