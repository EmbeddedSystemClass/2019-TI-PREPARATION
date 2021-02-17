#include "DDS.h"
#include "math.h"
#include "arm_math.h"
#include "adc.h"
#define Fs 1000  //1M
 __IO uint16_t *u16DDS_DataOut = (__IO uint16_t*)0x38000000;


u16 DDSTable[Table_NUM];
u32 PhaseAccumulator = 0;
u32 PhaseDelta = 0;

void InitSinTable(void)
{
    u32 i;
    for(i = 0; i < Table_NUM; i++)     // 2�� ����ӳ�� Table_NUM�� �����������
    {
        float temp = (float)(2 * 3.141592653) / (float)Table_NUM * (float)i;
        DDSTable[i] = 32767.0 + 32767.0 * arm_sin_f32(temp);
    }
}


void DDS_init()
{
    hdma_bdma_generator0.XferCpltCallback         = DMA1_stream0_FULL_Handler;
    hdma_bdma_generator0.XferHalfCpltCallback     = DMA1_stream0_HALF_Handler;
    HAL_DMAEx_EnableMuxRequestGenerator(&hdma_bdma_generator0);
    HAL_DMA_Start_IT(&hdma_bdma_generator0, (uint32_t)u16DDS_DataOut, (uint32_t)&GPIOB->ODR, 2000);

    HAL_LPTIM_PWM_Start(&hlptim2, 50 - 1, 0);		//100M/2 ��Ƶ
    u32 Fx = 1000;					// 100k
    PhaseDelta = ((double)Fx / Fs) * 65536 * 65536;	// �Ŵ�32bit
}


void DMA1_stream0_HALF_Handler(DMA_HandleTypeDef* hdma)							//����
{
    //@arg DMA_IT_TC: Transfer complete interrupt mask.
    //@arg DMA_IT_HT: Half transfer complete interrupt mask.
    // ����
//		if(ADC_HALF)
//		{
    for(int i = 0; i < 25; i++)
    {
        if(decode[i] == 1)
        {
            for(int j = 0; j < 40; j++)
                u16DDS_DataOut[i * 40 + j] = 50000;
        }
        else
        {
            for(int j = 0; j < 40; j++)
                u16DDS_DataOut[i * 40 + j] = 0;
        }
        //			PhaseAccumulator += PhaseDelta;
    }
//		}
//			SCB_CleanInvalidateDCache();
//			SCB_CleanDCache_by_Addr((uint32_t *) &u16DDS_DataOut[0], 1024);

}

void DMA1_stream0_FULL_Handler(DMA_HandleTypeDef* hdma)
{
    // ȫ��

    for(int i = 25; i < 50; i++)
    {
        if(decode[i] == 1)
        {
            for(int j = 0; j < 40; j++)
                u16DDS_DataOut[i * 40 + j] = 50000;
        }
        else
        {
            for(int j = 0; j < 40; j++)
                u16DDS_DataOut[i * 40 + j] = 0;
        }
        //			PhaseAccumulator += PhaseDelta;
    }

}

