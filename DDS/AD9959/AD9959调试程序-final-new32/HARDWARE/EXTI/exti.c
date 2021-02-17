#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "timer.h"
#include "stm32f4xx.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

u8 tim2_flag = 0;
u32 tim2_cnt_num;
u32 freq_get;
void EXTI2_IRQHandler(void)
{
	if(tim2_flag == 0)
	{
		TIM_Cmd(TIM2,ENABLE);
		tim2_flag = 1;	
	}
	else
	{
		
		
		tim2_cnt_num = TIM2->CNT;
		TIM_Cmd(TIM2,DISABLE);
		TIM2->CNT = 0X00;
		freq_get = (84000000)/tim2_cnt_num;
		tim2_flag = 0;
	}
}
	   
//�ⲿ�жϳ�ʼ������
//��ʼ��PE2~4,PA0Ϊ�ж�����.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PB1 ���ӵ��ж���1
	
  /* ����EXTI_Line1 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE1
  EXTI_Init(&EXTI_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ⲿ�ж�3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����  
}












