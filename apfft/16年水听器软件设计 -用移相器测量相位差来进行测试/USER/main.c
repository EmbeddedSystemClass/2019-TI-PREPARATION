#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "my_fft.h"
#include "led.h"
#include "AD7367_SPI.h"
#include "AD7367.h"
#include "adc.h"
#include "hanning_window.h"



extern u8 sample_flag;
float fre;//Ƶ��
extern float sin1_fft_inputbuf1[4096];
extern float sin2_fft_inputbuf1[4096];//���������Ծ��
extern float conv[2047];//�����
#define fl 40000
#define a2r 0.0174532
extern u16 Adc1_Value[2];//ADC1��ֵ
u8 sample_flag1 = 1;
u8 sample_flag2 = 0;
int sample_num = 1;
//10k,28.8����λ��

int main(void)

{
    int i;
    delay_init(168);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
    Led_Init();
    delay_init(168);  //��ʼ����ʱ����
    Adc_Init();
    Adc_DMA_Init();
    ADC_SoftwareStartConv(ADC1);/* �������ADC1ת�� */
    Led_Init();
    uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
    TIM3_Int_Init(24, 83); //
    while(1)
    {
        if(sample_flag)
        {
            sample_flag = 0; //��ʱ����λ��־λ
//���в�����һ�β���2048���㣬ʵ���õ���ֻ��1024����,����2048��ֻ��Ϊ���ܹ��úò���
            if(sample_flag1)
            {
                sin1_fft_inputbuf1[sample_num * 2] = ((float)Adc1_Value[0] / 4096.0) * 3.3;
                sin2_fft_inputbuf1[sample_num * 2] = ((float)Adc1_Value[1] / 4096.0) * 3.3;
                sample_num++;
                if(sample_num >= 2048) //����ı��˱�־λ��Ӧ������ʱ��Ӧʱ���ٲ�������Ȼ�൱������������ͬ��ֵ��ɺܴ����
                {
                    sample_num = 1;
                    sample_flag1 = 0;
                    sample_flag2 = 1;
                }
            }
//����ȫ��λfft�Ĵ�������affft�õ�Ƶ�ʺ���λ
            if(sample_flag2)
            {
                for(i = 1; i < 2048; i++) //1��2N-1��,0��2n-2ѭ��2n-1��
                {
                    sin1_fft_inputbuf1[i * 2] = sin1_fft_inputbuf1[i * 2] * conv[i - 1]; //ÿһ����
                    sin2_fft_inputbuf1[i * 2] = sin2_fft_inputbuf1[i * 2] * conv[i - 1]; //ÿһ����
                }
                for(i = 0; i < 1024; i++) //1��N-1��,ѭ��n-1��
                {
                    sin1_fft_inputbuf1[i * 2] += sin1_fft_inputbuf1[(i + 1024) * 2]; //��ô�����fftֵ��ֻ����ʵ��
                    sin2_fft_inputbuf1[i * 2] += sin2_fft_inputbuf1[(i + 1024) * 2]; //��ô�����fftֵ��ֻ����ʵ��
                }

                fftji41024(fl);
                for(i = 0; i < 4096; i++)
                {
                    sin1_fft_inputbuf1[i] = 0; //�������㣬�м��мǣ���Ȼ�ᵼ�¼������
                    sin2_fft_inputbuf1[i] = 0;
				}
                sample_flag1 = 1;
                sample_flag2 = 0;
            }
        }
    }
}
