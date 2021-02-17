#include "timer.h"
#include "led.h"
#include "dac.h"
#include "ui.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
#include "math.h"
extern u16 sine[1024];
//extern u8 square_wave[1024];
unsigned long int counter = 0;
u16 m;
//#define frequence  10000000
//#define n (u32)(((1024*frequence)/107761000))
//u32 count_num = (u32)(((1024*frequence)/107761000))

extern double amp_div;
extern u32 count_num;
//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_Int_Init(u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ

	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM3	
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{		
		DAC_SetChannel1Data(DAC_Align_12b_R,(u16)((double)sine[counter>>22]/amp_div));
		
		switch(counter>>30)
		{
			case 0x00:
			{
				SQU1 = 1;
				SQU2 = 1;
				SQU3 = 0;
				SQU4 = 0;
				
			}break;
			case 0x01:
			{
				SQU1 = 1;
				SQU2 = 0;
				SQU3 = 0;
				SQU4 = 1;
				
			}break;
			case 0x10:
			{
				SQU1 = 0;
				SQU2 = 0;
				SQU3 = 1;
				SQU4 = 1;
				
			}break;
			case 0x11:
			{
				SQU1 = 0;
				SQU4 = 0;
				SQU2 = 1;
				SQU3 = 1;
				
			}break;
		}
		counter += count_num;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}
