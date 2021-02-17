#include "my_fft.h"

#define FFT_LENGTH		1024 		//FFT���ȣ�Ĭ����1024��FFT
//�������ϲ�ȥ�������ӵ���
float sin1_fft_inputbuf1 [FFT_LENGTH * 4];	//FFT����������ʵ��
float sin1_fft_outputbuf[FFT_LENGTH];	//FFT�������

float sin2_fft_inputbuf1 [FFT_LENGTH * 4];	//FFT����������ʵ��
float sin2_fft_outputbuf[FFT_LENGTH];	//FFT�������

float phase1;//�����ź�һ����λ��Ϣ
float phase2;//�����źŶ�����λ��Ϣ
float fre1;
float fre2;
float result;//������λ����

/*
 * ��������fftji41024
 * ����  ������fft
 * ����  ������Ƶ��
 * ���  ����
 */
void fftji41024(float fs)
{
    float max;
    int max_num;
    u16 i;
    arm_cfft_radix4_instance_f32 scfft;
    arm_cfft_radix4_init_f32(&scfft, FFT_LENGTH, 0, 1); //��ʼ��scfft�ṹ�壬�趨FFT��ز���

    arm_cfft_radix4_f32(&scfft, sin1_fft_inputbuf1);	//FFT���㣨��4��
    arm_cmplx_mag_f32(sin1_fft_inputbuf1, sin1_fft_outputbuf, FFT_LENGTH);	//��������������ģ�÷�ֵ
    max = sin1_fft_outputbuf[5]; //��ֱ������Ҫע�⣬���Բ���һ��ʼ��ʼ�ͽ��з���
    for(i = 6; i < FFT_LENGTH / 2; i++) //ֻҪ��鵽һ���ok����Ϊ�ǶԳƵ�
    {
        if(sin1_fft_outputbuf[i] > max)
        {
            max = sin1_fft_outputbuf[i];
            max_num = i; //�����ֵ���ĵ�
        }
    }
    fre1 = ((float)(max_num) / 1024) * fs; //��0�����Ӧֱ��������max_num=0��ʱ��Ϊֱ��������fsΪ����Ƶ��
    phase1 = atan2(sin1_fft_inputbuf1[max_num * 2 + 1], sin1_fft_inputbuf1[max_num * 2]) * 57.3; //�鲿����ʵ���������Ǻ���


    arm_cfft_radix4_init_f32(&scfft, FFT_LENGTH, 0, 1); //��ʼ��scfft�ṹ�壬�趨FFT��ز���
    arm_cfft_radix4_f32(&scfft, sin2_fft_inputbuf1);	//FFT���㣨��4��
    arm_cmplx_mag_f32(sin2_fft_inputbuf1, sin2_fft_outputbuf, FFT_LENGTH);	//��������������ģ�÷�ֵ
    max = sin2_fft_outputbuf[5]; //��ֱ������Ҫע�⣬���Բ���һ��ʼ��ʼ�ͽ��з���
    for(i = 6; i < FFT_LENGTH / 2; i++) //ֻҪ��鵽һ���ok����Ϊ�ǶԳƵ�
    {
        if(sin2_fft_outputbuf[i] > max)
        {
            max = sin2_fft_outputbuf[i];
            max_num = i; //�����ֵ���ĵ�
        }
    }

    fre2 = ((float)(max_num) / 1024) * fs; //��0�����Ӧֱ��������max_num=0��ʱ��Ϊֱ��������fsΪ����Ƶ��
    phase2 = atan2(sin2_fft_inputbuf1[max_num * 2 + 1], sin2_fft_inputbuf1[max_num * 2]) * 57.3; //�鲿����ʵ���������Ǻ���
    result = phase1 - phase2 + 5.0 * fs / 40000.0; //����ͨ��ɨ����ʱ��Ϊ�˸��źŵĽ������㹻��ʱ�䣬�������Щ��ʱ�������ʱӦ�ø�Ƶ���й�ϵ�����ȣ�10k�Ҿ͸���5.3
    if(result >= 360)
        result -= 360;
    if(result <= -360)
        result += 360;
}
