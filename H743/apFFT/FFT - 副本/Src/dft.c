#include <stdio.h>
#include <stdlib.h>
#include <math.h>		
#include "dft.h"
#define PI	3.1415926535	

#define N 1000   //�������г��ȱ���
float Input_Squence1[1000];	//�����ԭʼ�������� 
float Input_Squence2[1000];	//�����ԭʼ�������� 

float Amplitude1[1000];   //�洢��ֵ������
float Amplitude2[1000];   //�洢��ֵ������

float phi1[1000];
float phi2[1000];

typedef struct				//�����ṹ��,����ʵ�ָ���Ҷ���� 
{
	double real, imag;
}complex;
complex Result_Point[1000];

void DFT_Calculate_Point(int k,int *Input_Squence)
{
	int n = 0;
	complex Sum_Point;
	complex One_Point[N];
	Sum_Point.real = 0;
	Sum_Point.imag = 0;
	for (n = 0; n < N; n++)
	{
		One_Point[n].real = cos(2 * PI / N * k*n)*Input_Squence[n];  //����ŷ����ʽ�Ѹ�����ֳ�ʵ�����鲿 
		One_Point[n].imag = -sin(2 * PI / N * k*n)*Input_Squence[n];

		Sum_Point.real += One_Point[n].real;//��ÿһ�����ʵ�����
		Sum_Point.imag += One_Point[n].imag;
		//��ÿһ������鲿���
	}
	Result_Point[k].real = Sum_Point.real/N;
	Result_Point[k].imag = Sum_Point.imag/N;
}

void DFT_Calculate(int *Input_Squence,float *Amplitude,float *phi)
{
	int i = 0;
	for (i = 0; i < N; i++)
	{
		if(i == 1)
			DFT_Calculate_Point(i,Input_Squence);
		Amplitude[i] = 2*sqrt(Result_Point[i].real * Result_Point[i].real + Result_Point[i].imag * Result_Point[i].imag);  //�����ֵ,ֱ�����ó�2
		phi[i] = atan(Result_Point[i].imag / Result_Point[i].real);
		phi[i] = phi[i] / (2 * PI) * 360 ;
		if (Result_Point[i].imag > 0 & Result_Point[i].real < 0)
			phi[i] = phi[i] + 180;
		if (Result_Point[i].imag < 0 & Result_Point[i].real < 0)
			phi[i] = phi[i] - 180;
	}
}
