#include "DDS.h"
#include "math.h"
#include "arm_math.h"
#include "dpsk_mod.h"
#define Fs 5000000  //5M
//__attribute__((section(".ARM.__at(0x38000000"))) __IO uint16_t u16DDS_DataOut[16384] = {0, 1, 2, 3};
 __IO uint16_t *u16DDS_DataOut = (__IO uint16_t*)0x38000000;
extern u8 flag;
u16 DDSTable[Table_NUM];
u32 PhaseAccumulator = 0;
u32 PhaseDelta = 0;
u8 DAC_HALF = 0;
u8 DAC_FULL = 0;
void InitSinTable(void)
{
    u32 i;
    for(i = 0; i < Table_NUM; i++)     // 2�� ����ӳ�� Table_NUM�� �����������
    {
        float temp = (float)(2 * 3.141592653) / (float)Table_NUM * (float)i;
        DDSTable[i] = 32767.0 + 32767.0 * arm_sin_f32(temp)*0.8;
    }
}


void DDS_init()
{
    hdma_bdma_generator0.XferCpltCallback         = DMA1_stream0_FULL_Handler;
    hdma_bdma_generator0.XferHalfCpltCallback        = DMA1_stream0_HALF_Handler;
    HAL_DMAEx_EnableMuxRequestGenerator(&hdma_bdma_generator0);
    HAL_DMA_Start_IT(&hdma_bdma_generator0, (uint32_t)u16DDS_DataOut, (uint32_t)&GPIOB->ODR, N_bit*N_phase*2);
    u32 Fx = 5000;					// 50k
	
    HAL_LPTIM_PWM_Start(&hlptim2, 50 - 1, 0);		//100M ��Ƶ  2M������
//    PhaseDelta = ((double)Fx / Fs) * 65536 * 65536;	// �Ŵ�32bit
}


void DMA1_stream0_HALF_Handler(DMA_HandleTypeDef* hdma)							//����
{

	DAC_HALF = 1;
            if(flag == 1)
            {
                for(int i = 0; i < N_bit*N_phase; i++)
                {
                    u16DDS_DataOut[i] = (uint16_t)aSRC_Buffer[i];
                }
                flag = 0;
            }
            else
			{
                for(int i = 0; i < N_bit*N_phase; i++)
                {
                    u16DDS_DataOut[i] = (uint16_t)aSRC_Buffer_0[i];
                }
			}
}

void DMA1_stream0_FULL_Handler(DMA_HandleTypeDef* hdma)
{
    // ȫ��

	DAC_FULL = 1;
            if(flag == 1)
            {
                for(int i = N_bit*N_phase; i < 2*N_bit*N_phase; i++)
                {
                    u16DDS_DataOut[i] = (uint16_t)aSRC_Buffer[i-N_bit*N_phase];
                }
                flag = 0;
            }
            else
			{
                for(int i = N_bit*N_phase; i < 2*N_bit*N_phase; i++)
                {
                    u16DDS_DataOut[i] = (uint16_t)aSRC_Buffer_0[i-N_bit*N_phase];
                }
			}
}

