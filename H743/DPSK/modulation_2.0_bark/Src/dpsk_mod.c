#include "dpsk_mod.h"
#include "arm_math.h"


//#define N 10		// һ�����ڵ���
//#define N_phase 20  // һ��bit����
//#define N_bit 25	// bit��

uint16_t aSRC_0[N_phase] = {0};
uint16_t aSRC_Pi[N_phase] = {0};
uint16_t aSRC_Buffer[N_bit*N_phase] = {0};


float barker_7[7] = {1,1,1,-1,-1,1,-1};
float test[25] = {1,1,1,0,0,1,0,1,0,1,0,1,0,1,0};
float resul[32];
float max_val;
u32 max_index;
void Phase_init()	// һ��bit�������� N_phase/N 
{
	// Phase 0
	for(int i = 0; i < N_phase; i++)
	{
		aSRC_0[i] = 32767*0.8*(arm_sin_f32(i*2*(float)3.1415926545/(float)Num_T)) + (float)1*32767.; 
	}
	// Phase Pi
	for(int i = 0; i < N_phase; i++)
	{
		aSRC_Pi[i] = 32767*0.8*(-arm_sin_f32(i*2*(float)3.1415926545/(float)Num_T)) + (float)1*32767.; 
	}
	
//	 arm_correlate_f32((float32_t *)barker_7,7,(float32_t *)barker_7,7,resul);	
	 arm_correlate_f32(barker_7,7,test,25,resul);	
	 arm_max_f32	(resul,50,&max_val,&max_index );		
}

void SendOneFrameDPSK(uint32_t u32DataIn)
{
	int i,j,index = (1 << (N_bit));	// ��18bit + 7bit
	int barker = 0x72; // 1110010
	
	int ForePhase = 0;			// ��ʼ��λ

	if((u32DataIn & 1) == 1)// ȡβbit,�жϳ�ʼ��λ
		ForePhase = 180;
	else
		ForePhase = 0;
	
	u32DataIn = u32DataIn + (barker<<18);
	
	for( i = 0; i < N_bit; i++)	// DPSK ����
	{
		
			index = index >> 1;
		
			if(ForePhase == 0)			   // ǰһ����λΪ0
			{
					if( u32DataIn & index) // ����Ϊ1
					{
							for(j = 0; j < N_phase; j++)
							{
									aSRC_Buffer[N_phase*i + j] = aSRC_Pi[j];
							}
							ForePhase = 180;
					}
					else					// ����Ϊ0
					{
							for(j = 0; j < N_phase; j++)
							{
									aSRC_Buffer[N_phase*i + j] = aSRC_0[j];
							}
							ForePhase = 0;
					}
			}
			else		// ForePhase = 180
			{
					if( u32DataIn & index)
					{
							for(j = 0; j < N_phase; j++)
							{
									aSRC_Buffer[N_phase*i + j] = aSRC_0[j];
							}
							ForePhase = 0;
					}
					else
					{
							for(j = 0; j < N_phase; j++)
							{
									aSRC_Buffer[N_phase*i + j] = aSRC_Pi[j];
							}
							ForePhase = 180;
					}
			}
		
	}
//	for(int i = 0;i<N_bit*N_phase;i++)
//	{
//		printf("%d ",aSRC_Buffer[i]);
//	}
}