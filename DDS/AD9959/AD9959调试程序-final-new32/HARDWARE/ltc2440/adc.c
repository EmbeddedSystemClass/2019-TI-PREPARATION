#include "adc.h" 
#include "stdio.h"
#include "lcd.h"
#include "ui.h"
/**
  ******************************************************************************
  * @file    adc.c
  * @author  han
  * @version V1
  * @date    15.4.20
  * @brief  ltc2440�����Ͷ�������
  ******************************************************************************
  * @attention
  *
  * �������ݡ����������ٶ���С��2M��
  *
  ******************************************************************************
  */ 
 extern int UI_ID;

//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void ADC1_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

  //GPIOB13��ʼ������
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13;//SCK,��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_11;//CS,��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_14;//MISO��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15;//BUSY��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����

  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	//GPIOB11 INIT
	
	
	GPIO_SetBits(GPIOB,GPIO_Pin_11|GPIO_Pin_13);//GPIOF9,F10���øߣ�����

}

double GET_ADC(void)
{
	u32 data = 0;
	double vol=0;
	int i = 0;
	static u8 Show_Val[20] = {0};
	if(BUSY == 0)
	{
		CS = 0;
			__asm{
        NOP
    }
			SCK = 0;
			__asm{
        NOP
    }
		for(;i<32;i++)
		{
			
					SCK = 1;
			 __asm{
        NOP
					}
				
			
			data |= (MISO<<(31-i));
			__asm{
        NOP
    }

			
			SCK = 0;
			__asm{
        NOP
    }
		}		
	
	CS = 1;
			__asm{
        NOP
    }
			if((UI_ID != FREQ)&&(UI_ID != REF))
			{
				if(data & 0x20000000)
				{
					vol = (((double)(data&(0x1fffffff)))/268435456.0)*2.5000000;
					if(vol>2.5)
						__asm{
        NOP
    }
					sprintf((char*)Show_Val," %.8lfV",vol);
					LCD_ShowString(88+32,117-32,200,200,32,Show_Val);
				}
				else
				{
					vol = (((double)(~data&(0x1fffffff)))/268435456.0)*2.5000000;
					sprintf((char*)Show_Val,"-%.8lfV",vol);
					LCD_ShowString(88+32,117-32,200,200,32,Show_Val);
				}
			}
}
	return vol;
}


