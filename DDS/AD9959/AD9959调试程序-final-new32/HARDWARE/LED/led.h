#ifndef __LED_H
#define __LED_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED�˿ڶ���
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1
#define SQU1 PGout(8)
#define SQU2 PGout(7)
#define SQU3 PAout(9)
#define SQU4 PAout(10)

void LED_Init(void);//��ʼ��		 				    
#endif
