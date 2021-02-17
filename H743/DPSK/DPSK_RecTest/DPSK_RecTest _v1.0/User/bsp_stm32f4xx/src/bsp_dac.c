/*
*********************************************************************************************************
*
*	ģ������ : DAC����ģ��
*	�ļ����� : bsp_dac.c
*	��    �� : V1.0
*	˵    �� : ʵ��DAC������ݲ�
*	�޸ļ�¼ :
*		�汾��    ����        ����     ˵��
*		V1.0    2013-12-30   armfly   ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"


#define DAC_DHR12R2_ADDRESS    0x40007414
#define DAC_DHR8R1_ADDRESS     0x40007410

/* ���β��ο��� */
const uint8_t aEscalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitDAC
*	����˵��: DAC��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitDAC(void)
{   
    TIM6_Config(); 
    DAC_GPIOConfig(); 
    DAC_Ch1_EscalatorConfig();
}

/*
*********************************************************************************************************
*	�� �� ��: TIM6_Config
*	����˵��: ���ö�ʱ��6�����ڴ���DAC��ÿTIM6CLK/256����һ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM6_Config(void)
{
     TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
      
    
    /* ʹ��TIM6ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    
    /* --------------------------------------------------------
    system_stm32f4xx.c �ļ��� void SetSysClock(void) ������ʱ�ӵ��������£�

    HCLK = SYSCLK / 1     (AHB1Periph)
    PCLK2 = HCLK / 2      (APB2Periph)
    PCLK1 = HCLK / 4      (APB1Periph)

    ��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
    ��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIMxCLK = PCLK2 x 2 = SystemCoreClock;

    APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
    APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11
          
    TIM6 ���������� = TIM6CLK / ��TIM_Period + 1��/��TIM_Prescaler + 1��= TIM6CLK/256
    ----------------------------------------------------------- */
    
    /* ����TIM6 */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    TIM_TimeBaseStructure.TIM_Period = 0xFF;          
    TIM_TimeBaseStructure.TIM_Prescaler = 0;       
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    /* TIM6 TRGO ѡ�� */
    TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

    /* ʹ��TIM6 */
    TIM_Cmd(TIM6, ENABLE);     
}

/*
*********************************************************************************************************
*	�� �� ��: DAC_GPIOConfig
*	����˵��: DAC��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DAC_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /*  ʹ������ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* DACͨ��1����������ã�PA.4�� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: DAC_Ch1_EscalatorConfig
*	����˵��: DACͨ��1������ݲ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DAC_Ch1_EscalatorConfig(void)
{
    DAC_InitTypeDef  DAC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    /* DAC ����ʱ��ʹ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    
    /* DMA1 ʱ��ʹ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
     
    /* DAC ͨ��1���� */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    
    /* DMA1_Stream5 ͨ��7���� **************************************/  
    DMA_DeInit(DMA1_Stream5);
    DMA_InitStructure.DMA_Channel = DMA_Channel_7;  
    DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8R1_ADDRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&aEscalator8bit;
    DMA_InitStructure.DMA_BufferSize = 6;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream5, &DMA_InitStructure);    

    /* ʹ�� DMA1_Stream5 */
    DMA_Cmd(DMA1_Stream5, ENABLE);

    /* ʹ�� DAC ͨ��1 */
    DAC_Cmd(DAC_Channel_1, ENABLE);

    /* ʹ��DACͨ��1��DMA���� */
    DAC_DMACmd(DAC_Channel_1, ENABLE); 
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/