#ifndef __ADC_H
#define __ADC_H	
#include "stm32f4xx.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ADC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
 							   
extern float current;//ADC1��ֵ
void Adc_Init(void); 	//ADCͨ����ʼ��
void Adc_DMA_Init(void);
void current_caculate(void);//�������,�˷ŷŴ���50��

#endif 





