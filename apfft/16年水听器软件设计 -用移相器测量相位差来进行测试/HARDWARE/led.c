#include "led.h"

/*
 * ��������Led_Init
 * ����  ��led��ʼ������
 * ����  ����
 * ���  ����
 */ 
void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//��ʼ���ɿ�����ĵ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ�������
	
}

