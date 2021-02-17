#include "DDS.h"
#include "math.h"
#include "arm_math.h"

#define Fs 5000000  //10M
__IO uint16_t* u16DDS_DataOut = (__IO uint16_t*)0x38000000;


u16 DDSTable[Table_NUM];
u32 PhaseAccumulator=0;
u32 PhaseDelta=0;

u32 PhaseAccumulator_n=0;
u32 PhaseDelta_n=0;

void InitSinTable(void)
{
		u32 i;
		for( i = 0; i < Table_NUM; i++ )   // 2�� ����ӳ�� Table_NUM�� �����������
		{
				 float temp=(float)(2*3.141592653)/(float)Table_NUM*(float)i;
				 DDSTable[i] = 32767.0 + 32767.0*arm_sin_f32(temp);	
		}
}

 
void DDS_init()
{
	hdma_bdma_generator0.XferCpltCallback         = DMA1_stream0_FULL_Handler;
    hdma_bdma_generator0.XferHalfCpltCallback     = DMA1_stream0_HALF_Handler;
	HAL_DMAEx_EnableMuxRequestGenerator (&hdma_bdma_generator0);
    HAL_DMA_Start_IT(&hdma_bdma_generator0, (uint32_t)u16DDS_DataOut, (uint32_t)&GPIOB->ODR, 1024);

	HAL_LPTIM_PWM_Start(&hlptim2, 20-1, 0);		//100M ��Ƶ
	u32 Fx=5000;					// 100k
	PhaseDelta = ((double)Fx / Fs) *65536*65536;	// �Ŵ�32bit
	u32 Fn=10000;					// 100k
	PhaseDelta_n = ((double)Fn / Fs) *65536*65536;	// �Ŵ�32bit
	
}


void DMA1_stream0_HALF_Handler(DMA_HandleTypeDef *hdma)							//����
{
	//@arg DMA_IT_TC: Transfer complete interrupt mask.
    //@arg DMA_IT_HT: Half transfer complete interrupt mask.
		// ����
	
		for( int i = 0; i < 512; i++ )
		{
			u16DDS_DataOut[i] = 0.7*(uint16_t)DDSTable[(PhaseAccumulator >> 22)&(0x3FF)]+0.3*(uint16_t)DDSTable[(PhaseAccumulator_n >> 22)&(0x3FF)];
			PhaseAccumulator += PhaseDelta;
			PhaseAccumulator_n += PhaseDelta_n;

 		}
//		SCB_CleanInvalidateDCache();
}	

void DMA1_stream0_FULL_Handler(DMA_HandleTypeDef *hdma)
{
			// ȫ��
 		 
		for( int i = 512; i < 1024; i++ )
		{
			u16DDS_DataOut[i] = 0.7*(uint16_t)DDSTable[(PhaseAccumulator >> 22)&(0x3FF)]+0.3*(uint16_t)DDSTable[(PhaseAccumulator_n >> 22)&(0x3FF)];
			PhaseAccumulator += PhaseDelta;
			PhaseAccumulator_n += PhaseDelta_n;

 
		}
//		SCB_CleanInvalidateDCache();

}

