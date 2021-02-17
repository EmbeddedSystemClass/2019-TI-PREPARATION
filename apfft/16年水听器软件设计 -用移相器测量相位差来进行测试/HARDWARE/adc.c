#include "adc.h" 
//////////////////////////////////////////////////////////////////////////////////	 
/*
PA5 ͨ��5
PA5 ͨ��6
������
*/
////////////////////////////////////////////////////////////////////////////////// 		
u16 Adc1_Value[2];//ADC1��ֵ

/*
 * ��������fftji41024
 * ����  ������fft
 * ����  ������Ƶ��
 * ���  ����
 */														   
void  Adc_Init(void)
{    
  GPIO_InitTypeDef      GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOCʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��

 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;// 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����

	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_15Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1; //DMAģʽ1
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//��������ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 2;//2��ͨ�� 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
	ADC_Cmd(ADC1, ENABLE);//����ADת����	

  /* Enable DMA request after last transfer (multi-ADC mode) ******************/
  //ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
  ADC_DMACmd(ADC1, ENABLE);

  ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1, ADC_SampleTime_56Cycles     );//
  ADC_RegularChannelConfig(ADC1,ADC_Channel_6,2, ADC_SampleTime_56Cycles     );//
   /* ʹ��ADC DMA������ */
        
   ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
}	

/*
 * ��������Adc_DMA_Init
 * ����  ��adc��dma��ʼ��
 * ����  ����
 * ���  ����
 */
void Adc_DMA_Init(void)
{
//ADC1 DMA��ʼ��
	DMA_InitTypeDef DMA_InitStructure;
	RCC->AHB1ENR|=RCC_AHB1Periph_DMA2;
	DMA_InitStructure.DMA_BufferSize =2;
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;//ͨ��0��ӦADC1
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Adc1_Value;   //Ŀ������λ
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_BASE+0x4C;  //ADC1->DR��ַ
	DMA_InitStructure.DMA_PeripheralBurst =DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA2_Stream0,&DMA_InitStructure);//ADC1��Ӧ������0
	DMA_Cmd(DMA2_Stream0,ENABLE);//ADC1��Ӧ������0
	//ADC2 DMA��ʼ��	

// /* DMA2_Stream0 enable */
  DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TC);
  DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);

}




