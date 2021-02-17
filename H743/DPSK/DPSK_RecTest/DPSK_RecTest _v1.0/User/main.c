/*
*********************************************************************************************************
*
*	ģ������ : ������ģ�顣
*	�ļ����� : main.c
*	��    �� : V2.0
*	˵    �� : ARM�ٷ�DSP����ֲ
*	�޸ļ�¼ : ����ʵ����Ҫ��Ϊ�����DSP�̵̳ĵ�8�¶�����
*              ʵ��Ŀ�ģ�
*                1. ѧϰBasicMathFunctions�е������в���������ֵ���ӷ�����˺ͳ˷�
*              ʵ�����ݣ�
*                1. ���°���K1, ���ڴ�ӡ����DSP_ABS��������
*                2. ���°���K2, ���ڴ�ӡ����DSP_Add��������
*                3. ���°���K3, ���ڴ�ӡ����DSP_DotProduct��������
*                4. ����ҡ�˵�UP�������ڴ�ӡ����DSP_Multiplication��������
*				 5. ����ʹ��
*                   SecureCRT��V5���������д�������鿴��ӡ��Ϣ��
*
*		�汾��   ����        ����     ˵��
*		V1.0    2014-08-25  Eric2013  �׷�
*		V2.0    2015-04-24  Eric2013  1. �����̼��⵽V1.5.0
*                                     2. ����BSP�弶֧�ְ�
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"			/* �ײ�Ӳ������ */
#include "arm_math.h"
#include "DualTimerMasterSlave_MD.h"
#include "DMA_TimerSPI_MD.h"
#include "DMA_TimerADC_MD.h"
#include "DMA_DAC.h"

#include "SEGGER_RTT.h"

/* ���������������̷������� */
#define EXAMPLE_NAME	"V5-908_BasicMathFunctions��ʹ�ã�һ��"
#define EXAMPLE_DATE	"2015-04-24"
#define DEMO_VER		"2.0"

/* �������ļ��ڵ��õĺ������� */
static void PrintfLogo(void);
static void DSP_ABS(void);
static void DSP_Add(void);
static void DSP_DotProduct(void);
static void DSP_Multiplication(void);


#define numStages  6                /* 2��IIR�˲��ĸ��� */
#define TEST_LENGTH_SAMPLES  32    /* �������� */

static float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES]; /* ������ */
static float32_t testOutput[TEST_LENGTH_SAMPLES];               /* �˲������� */
static float32_t IIRStateF32[4 * numStages];                    /* ״̬���棬��СnumTaps + blockSize - 1*/

volatile uint16_t u16Probe;
volatile int32_t u16Probe1;
volatile uint16_t u16Probe2;


///* ������˹��ͨ�˲���ϵ��2000Hz�����6000Hz fs = 200000Hz*/
//const float32_t IIRCoeffs32BP[5*numStages] = {
//	1.0f,  2.0f,  1.0f,  1.9721480977403503f,   -0.97670533367674994f,
//	1.0f,  2.0f,  1.0f,  1.9299028598494261f,   -0.93436247557338892f,
//	1.0f,  2.0f,  1.0f,  1.8967803358264643f,   -0.90116341208721196f,
//	1.0f,  2.0f,  1.0f,  1.8757675788836574f ,  -0.88010209890708602f,
//	1.0f,  1.0f,  0.0f,  0.93428921436017054f,  -0.f
//};



/* ������˹��ͨ�˲���ϵ��3000Hz�����8000Hz fs = 200000Hz*/
const float32_t IIRCoeffs32BP[5 * numStages] =
{
    1.0f,  2.0f,  1.0f,  1.9576933015564808f,   -0.96943717391709916f,
    1.0f,  2.0f,  1.0f,  1.9019198502274668f,   -0.91332914704808688f,
    1.0f,  2.0f,  1.0f,  1.8555703149971432f,   -0.86670156876306925f,
    1.0f,  2.0f,  1.0f,  1.821024845991349f,    -0.83194886730757556f,
    1.0f,  2.0f,  1.0f,  1.7997697117019655f,   -0.810566227047822f,
    1.0f,  1.0f,  0.0f,  0.89630084115608444f,  -0.f
};


///* ������˹��ͨ�˲���ϵ��1500Hz�����4000Hz fs = 200000Hz*/
//const float32_t IIRCoeffs32BP[5*numStages] = {
//	1.0f,  2.0f,  1.0f,  1.9816853781017116f,   -0.98462721113043628f,
//	1.0f,  2.0f,  1.0f,  1.9528802327644983f,   -0.95577930424848589f,
//	1.0f,  2.0f,  1.0f,  1.9283050807052526f,   -0.93116767011453105f,
//	1.0f,  2.0f,  1.0f,  1.9095989134728388f,   -0.91243373337782918f,
//	1.0f,  2.0f,  1.0f,  1.8979195251582135f,   -0.90073700688698333f,
//	1.0f,  1.0f,  0.0f,  0.94697556594603727f,  -0.f
//};

///* ������˹��ͨ�˲���ϵ��140Hz 400Hz*/
//const float32_t IIRCoeffs32BP[5*numStages] = {
//	1.0f,  0.0f,  -1.0f,  1.9683506315681893f,  -0.97336903639280747f,
//	1.0f,  0.0f,  -1.0f,  1.9995797282708723f,  -0.99958095298358873f,
//	1.0f,  0.0f,  -1.0f,  1.9987866632426345f,  -0.99878790750621571f,
//	1.0f,  0.0f,  -1.0f,  1.9211797322972277f,  -0.92599889645697431f,
//	1.0f,  0.0f,  -1.0f,  1.998141227601137f,   -0.99814250217182687f,
//	1.0f,  0.0f,  -1.0f,  1.8866656725653566f,  -0.89128409645992368f,
//	1.0f,  0.0f,  -1.0f,  1.9977680261710431f,  -0.99776932416556696f,
//	1.0f,  0.0f,  -1.0f,  1.86854243737184f,    -0.87303301539278078f
//};

/* ������˹��ͨ�˲���ϵ��0.5Hz 20Hz,������200Hz */
// Matlabԭʼϵ������Ϊb0 b1 b2 a0 a1 a2
// ע��ARM DSP�ⲻ��Ҫa0,��y[n]ǰ��ϵ��
//const float32_t IIRCoeffs32BP[5*numStages] = {
//	1.0f,  0.0f,  -1.0f,  1.3793859883903727f,  -0.78674855887445816f,
//	1.0f,  0.0f,  -1.0f,  1.9944688451877366f,  -0.99467076418443212f,
//	1.0f,  0.0f,  -1.0f,  1.9843878112562117f,  -0.98459288298735848f,
//	1.0f,  0.0f,  -1.0f,  1.157937295226525f,   -0.49193882857472243f,
//	1.0f,  0.0f,  -1.0f,  1.9761094915314679f,  -0.97632022303806754f,
//	1.0f,  0.0f,  -1.0f,  1.0354029521921215f,  -0.32354205520417195f,
//	1.0f,  0.0f,  -1.0f,  1.9712540696724012f,  -0.9714694232566099f,
//	1.0f,  0.0f,  -1.0f,  0.9811305854632264f,  -0.24676806198332699
//};


///* ������˹��ͨ�˲���ϵ��2000Hz�����6000Hz fs = 200000Hz*/
//volatile float32_t ScaleValue = 	0.0011393089840998985*
//																	0.0011149039309906953*
//																	0.0010957690651869237*
//																	0.0010836300058571109*
//																	0.032855392819914737;



/* ������˹��ͨ�˲���ϵ��3000Hz�����8000Hz fs = 200000Hz*/
volatile float32_t ScaleValue = 	0.002935968090154604 *
                                    0.002852324205155056 *
                                    0.0027828134414815352 *
                                    0.0027310053290566737 *
                                    0.0026991288364641079 *
                                    0.05184957942195776;


///* ������˹��ͨ�˲���ϵ��1500Hz�����4000Hz fs = 200000Hz*/
//volatile float32_t ScaleValue = 	0.00073545825718119705*
//																	0.00072476787099696246*
//																	0.00071564735231968929*
//																	0.00070870497624762603*
//																	0.00070437043219248195*
//																	0.026512217026981352;


//volatile float32_t ScaleValue = 	0.034907330466672125*
//																	0.034907330466672125*
//																	0.034479329045770668*
//																	0.034479329045770668*
//																	0.034162181085070645*
//																	0.034162181085070645*
//																	0.03399415063903985*
//																	0.03399415063903985;

//volatile float32_t ScaleValue = 	0.31282462151819557f*
//																	0.31282462151819557f*
//																	0.28546239546776436f*
//																	0.28546239546776436f*
//																	0.2687742215913746f*
//																	0.2687742215913746f*
//																	0.26088103084165865f*
//																	0.26088103084165865f;

/*
*********************************************************************************************************
*	�� �� ��: arm_iir_f32_bp
*	����˵��: ���ú���arm_iir_f32_hpʵ�ִ�ͨ�˲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void arm_iir_f32_bp(void)
{
    static uint8_t u8Flag = 0;
    uint32_t i;
    static arm_biquad_casd_df1_inst_f32 S;
    volatile float32_t f32Tmp;

    if(u8Flag == 0)
    {
        u8Flag = 1;
        /* ��ʼ�� */
        arm_biquad_cascade_df1_init_f32(&S, numStages, (float32_t*)&IIRCoeffs32BP[0], (float32_t*)&IIRStateF32[0]);
    }

    /* IIR�˲� */
    arm_biquad_cascade_df1_f32(&S, testInput_f32_50Hz_200Hz, testOutput, TEST_LENGTH_SAMPLES);

    /*����ϵ�� */
//	ScaleValue = 	0.034907330466672125*
//								0.034907330466672125*
//								0.034479329045770668*
//								0.034479329045770668*
//								0.034162181085070645*
//								0.034162181085070645*
//								0.03399415063903985*
//								0.03399415063903985;

//	ScaleValue = 	0.31282462151819557f*
//								0.31282462151819557f*
//								0.28546239546776436f*
//								0.28546239546776436f*
//								0.2687742215913746f*
//								0.2687742215913746f*
//								0.26088103084165865f*
//								0.26088103084165865;
//	__disable_irq();
    /* ��ӡ�˲����� */
//	for(i=0; i<TEST_LENGTH_SAMPLES; i++)
//	{
////		printf("%f\r\n", testOutput[i]*ScaleValue);
//			f32Tmp = testOutput[i]*ScaleValue + 0.0f;
//			u16Probe1 = f32Tmp;
////			printf("%d\r\n", u16Probe1);
////			bsp_DelayUS(10);
//	}
//	__enable_irq();
}



/*
*********************************************************************************************************
*	�� �� ��: arm_iir_f32_bp
*	����˵��: ���ú���arm_iir_f32_hpʵ�ִ�ͨ�˲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void arm_iir_f32_Lp(float32_t fInputData[], float32_t fOutputData[], uint32_t u32Len)
{
    static uint8_t u8Flag = 0;
    uint32_t i;
    static arm_biquad_casd_df1_inst_f32 S;
    volatile float32_t f32Tmp;

    if(u8Flag == 0)
    {
        u8Flag = 1;
        /* ��ʼ�� */
        arm_biquad_cascade_df1_init_f32(&S, numStages, (float32_t*)&IIRCoeffs32BP[0], (float32_t*)&IIRStateF32[0]);
    }

    /* IIR�˲� */
    arm_biquad_cascade_df1_f32(&S, fInputData, fOutputData, u32Len);

}

void Timer2_ADC_T2CC__Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef         TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    // �����Stm32f103
    // ��ʱ��2 PWMƵ��Ϊ72000000/72/10=100Khz
    // ��ʱ��1 PWMƵ��Ϊ72000000/72/(10*ADCNUM*2)
    // �����Stm32f407��APB1���Ƶ��168Mhz/4=42Mhz
    // CMSISĬ������
    /* PCLK2 = HCLK / 2*/
    /* PCLK1 = HCLK / 4*/
    // ���APB1��Ƶ����42M�����ڷ�Ƶϵ����4������1����
    // ��ʱ��2 ʹ��APB1�����PWMƵ��ΪHCLK/4 * 2 /TIM_Prescaler/10 = 100Khz
    // ��ʱ��1 ʹ��APB2��TIMCLK = 2 * HCLK / 2 = 168000000
    // PWMƵ��Ϊ168000000/TIM_Prescaler/(10*ADCNUM*2) = 100Khz/(ADCNUM*2)

    //��ʼ����ʱ�� TIM2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


    /* TIM2 configuration ------------------------------------------------------*/
    /* Time Base configuration */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 10 - 1;          // 200khz
    TIM_TimeBaseStructure.TIM_Prescaler = 84 / 2 - 1;   // 2000khz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /* TIM2 channel2 configuration in PWM mode */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 10 / 2 ;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);


//	/* TIM2 counter enable */
    TIM_CtrlPWMOutputs(TIM2, ENABLE);

    // ʹ�ܴӶ�ʱ��Timer2
    TIM_Cmd(TIM2, ENABLE);

}

extern __IO uint16_t uhADCxConvertedValue[];
extern __IO uint8_t u8ADC_Halffull;
__IO float fSpecN4_real;
__IO float fSpecN4_imaginal;
__IO float fWorkingBuf[160];
__IO float fPower;
__IO float fCosineKtable[160];

//__IO uint32_t u32WorkingBuf[8192];
__IO float_t u16WorkingBuf[8192];
__IO uint16_t u16DecodeOutBuf[8192];
__IO uint32_t u32Tmp = 0;
extern __IO uint32_t u32ADC;

__IO float_t fLpFilterBuf[4000];

void SendOneFrameDPSK(uint32_t u32DataIn)
{
    int i, j, index = (1 << 24), u32ForePhse = 0;

    for(j = 0; j < 160; j++)
    {
        aSRC_Buffer[j] = aSRC_0[j];
    }

    u32ForePhse = 0;

    for(i = 1; i < 25; i++)
    {
        index = index >> 1;

        if(u32ForePhse == 0)
        {
            if(u32DataIn & index)
            {
                for(j = 0; j < 160; j++)
                {
                    aSRC_Buffer[160 * i + j] = aSRC_Pi[j];
                }
                u32ForePhse = 180;
            }
            else
            {
                for(j = 0; j < 160; j++)
                {
                    aSRC_Buffer[160 * i + j] = aSRC_0[j];
                }
                u32ForePhse = 0;
            }
        }
        else		// u32ForePhse = 180
        {
            if(u32DataIn & index)
            {
                for(j = 0; j < 160; j++)
                {
                    aSRC_Buffer[160 * i + j] = aSRC_0[j];
                }
                u32ForePhse = 0;
            }
            else
            {
                for(j = 0; j < 160; j++)
                {
                    aSRC_Buffer[160 * i + j] = aSRC_Pi[j];
                }
                u32ForePhse = 180;
            }
        }
    }
}

typedef struct
{
    uint8_t u8FlagHalfFull;
    uint8_t u8FlagSync;
    uint16_t u16LevelZeroCounter;
    uint16_t u16LevelOneCounter;
    uint16_t u16PosOfRiseEdge;
    uint32_t u32BitNumGot;
    uint8_t u8Code[8];
} tsBitSyncStruct;
tsBitSyncStruct sBitSyncStruct;

 uint8_t u8BitPool[256];
#define		BIT_LEVEL_THRESHHOLD		3
void	BitSync(uint16_t u16InputData[], uint32_t u32Level, uint32_t u32FlagHalforFull,
                tsBitSyncStruct* psBitSyncStruct)
{
    uint32_t i, j;
    uint32_t u32ScanStart, u32ScanEnd;
    uint32_t u32LevelZeroCounter = 0, u32LevelOneCounter = 0, u32LevelState = 1;
    uint32_t u32BitOneCounter = 0, u32BitZeroCounter = 0;
    uint32_t SyncState = 0;
    uint32_t u32Code = 0;

    u32LevelZeroCounter += psBitSyncStruct->u16LevelZeroCounter;
    u32LevelOneCounter += psBitSyncStruct->u16LevelOneCounter;

    // ����
    if(u32FlagHalforFull == 1)
    {
        u32ScanStart = 0;
        u32ScanEnd = 4000;
    }
    else if(u32FlagHalforFull == 2)
    {
        u32ScanStart = 4000;
        u32ScanEnd = 8000;
    }
    else
    {
        return;
    }

    {

        // ����ͬ�������أ���ʹ֮ǰ�Ѿ�ͬ��Ҳ���Լ�������
        for(i = u32ScanStart; i < u32ScanEnd; i++)								// һ��4000����
        {
            switch(u32LevelState)												// ����״̬ �ߺ͵�
            {
            case 1:
                if(u16InputData[i] < u32Level)									// ��ֵ���ݶ�2000
                {
                    u32LevelZeroCounter++;										// ���ƽ����������ֵ���ݶ�3
                    if(u32LevelZeroCounter > BIT_LEVEL_THRESHHOLD)
                    {
                        // �½���												// �����������ƽ�ж�Ϊ�½���
                        u32LevelState = 0;					
                        u32BitOneCounter = (u32LevelOneCounter + 80) / 160;		// һ�ĸ���ֱ����1��ƽ�ļ������������

                        if(psBitSyncStruct->u8FlagSync == 1)
                        {
                            // ����Ѿ�ͬ��
                            // �ɽ���λ�о�
                            // ����Ŀǰ����·Э�飬���������u32BitOneCounter������8
                            uint32_t k;
                            for(k = 0; k < u32BitOneCounter; k++)
                            {
                                if((psBitSyncStruct->u32BitNumGot + k) > 100)
                                {
                                    psBitSyncStruct->u32BitNumGot = 0;
                                }
                                u8BitPool[psBitSyncStruct->u32BitNumGot + k] = 1;
                            }
                            psBitSyncStruct->u32BitNumGot += u32BitOneCounter;

                            if(psBitSyncStruct->u32BitNumGot >= (16 + 1))
                            {
                                uint32_t m = 0, u32Data = 0;
                                // ת�Ʊ��سص�Code
                                for(k = 0; k < psBitSyncStruct->u32BitNumGot - 1; k++)
                                {
                                    u32Data = u32Data << 1;
                                    if(u8BitPool[k] == 0)
                                    {
                                        u32Data |= 0x01;
                                    }
                                    m++;
                                    if(m >= 8)
                                    {
                                        m = 0;
                                        psBitSyncStruct->u8Code[k / 8] = u32Data;
                                        u32Data = 0;
                                    }
                                }// End for
                                psBitSyncStruct->u32BitNumGot = 0;
                            }
                        }			// ���û��ͬ��������

                        u32LevelOneCounter = 0;

                    }
                }
                else
                {
                    u32LevelZeroCounter = 0;
                    u32LevelOneCounter++;

                }
                break;
            case 0:
                if(u16InputData[i] >= u32Level)
                {
                    u32LevelOneCounter++;
                    if(u32LevelOneCounter > BIT_LEVEL_THRESHHOLD)
                    {
                        // �����أ���Ҫ���л���
                        u32LevelState = 1;
                        u32BitZeroCounter = (u32LevelZeroCounter + 80) / 160;
                        u32LevelZeroCounter = 0;
                        if(u32BitZeroCounter == 8)
                        {
                            u32BitZeroCounter = 0;

                            // ͬ��
                            SyncState = 1;
                            psBitSyncStruct->u8FlagSync = 1;
                            psBitSyncStruct->u16PosOfRiseEdge = i;
//										for(j = 0; j < 8; j++ )
//										{
//											u32Code = u32Code << 1;
//											if(u16InputData[i + j*160 + 80] > u32Level)
//											{
//												u32Code = u32Code | 0x01;
//											}
//											else
//											{
//											}
//										}
//										u32Code = ~u32Code;

                            // ȡ�߽������ݺ���ر�����0
                            psBitSyncStruct->u32BitNumGot = 0;
//										memset((int8_t *)psBitSyncStruct->u8Code,0,sizeof(psBitSyncStruct->u8Code));
                        }
                        else
                        {
                            uint32_t k;
                            // ��ǰ��ͬ��������
                            SyncState = 0;
                            // ����Ѿ�ͬ�����ɽ���λ�о�
                            if(psBitSyncStruct->u8FlagSync == 1)
                            {
                                for(k = 0; k < u32BitZeroCounter; k++)
                                {
                                    if((psBitSyncStruct->u32BitNumGot + k) > 100)
                                    {
                                        psBitSyncStruct->u32BitNumGot = 0;
                                    }
                                    u8BitPool[psBitSyncStruct->u32BitNumGot + k] = 0;
                                }
                                psBitSyncStruct->u32BitNumGot += u32BitZeroCounter;

                                if(psBitSyncStruct->u32BitNumGot >= (16 + 1))
                                {
                                    uint32_t m = 0, u32Data = 0;
                                    // ת�Ʊ��سص�Code
                                    for(k = 0; k < psBitSyncStruct->u32BitNumGot - 1; k++)
                                    {
                                        u32Data = u32Data << 1;
                                        if(u8BitPool[k] == 0)
                                        {
                                            u32Data |= 0x01;
                                        }
                                        m++;
                                        if(m >= 8)
                                        {
                                            m = 0;
                                            psBitSyncStruct->u8Code[k / 8] = u32Data;
                                            u32Data = 0;
                                        }
                                    }// End for
                                    psBitSyncStruct->u32BitNumGot = 0;
                                }// End if(psBitSyncStruct->u32BitNumGot >= (16+1))
                            }// End if( psBitSyncStruct->u16FlagSync == 1)
                        }// End else
                    }// End if(u32LevelOneCounter > BIT_LEVEL_THRESHHOLD)
                }// End if(u16InputData[i] >= u32Level)
                else
                {
                    u32LevelOneCounter = 0;
                    u32LevelZeroCounter++;

                }
                break;
            default:
                break;
            }
        }// End for( i = 0; i < 4000; i++)
    }//

    if(u32LevelZeroCounter > 4000)
    {
        u32LevelZeroCounter = 4000;
        psBitSyncStruct->u32BitNumGot = 0;
        memset((int8_t*)psBitSyncStruct->u8Code, 0, sizeof(psBitSyncStruct->u8Code));
    }
    if(u32LevelOneCounter > 4000)
    {
        u32LevelOneCounter = 4000;
        psBitSyncStruct->u32BitNumGot = 0;
        memset((int8_t*)psBitSyncStruct->u8Code, 0, sizeof(psBitSyncStruct->u8Code));
    }
    psBitSyncStruct->u16LevelZeroCounter = u32LevelZeroCounter;
    psBitSyncStruct->u16LevelOneCounter = u32LevelOneCounter;

    return;
}



/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
int main(void)
{
    uint8_t ucKeyCode;		/* �������� */
    static uint32_t si = 0, su32Pointer = 0;
    uint32_t u32Sum = 0, i,  u32Index = 0, u32Len = 0;

//	u16DecodeOutBuf[8000] = 4000;
//	u16DecodeOutBuf[8001] = 4000;
//	u16DecodeOutBuf[8002] = (u16DecodeOutBuf[8000] * u16DecodeOutBuf[8001]) >> 12;

    bsp_Init();		/* Ӳ����ʼ�� */
//	PrintfLogo();	/* ��ӡ������Ϣ������1 */

    bsp_StartAutoTimer(0, 500);	/* ����1��500ms���Զ���װ�Ķ�ʱ�� */

//	DualTimer_Init();
    Timer2_ADC_T2CC__Init();

    ADC_Config(8000);
//	ADC_Config(256);

//	SEGGER_RTT_Init();
//	SEGGER_RTT_printf(0,"Hello!\r\n",8);

    printf("Hello!\r\n");

//	SPI1_Init();
//  DMA2_Config();
//  TIM1_Init(240);

//	SPI2_Init();
//  DMA1_Config();
//  TIM2_Init(240);

    // Phase 0
    for(i = 0; i < 160; i++)
    {
//		aSRC_0[i] = 2000*(arm_sin_f32(i*2*(float)3.1415926545/(float)40.) + (float)1.);
        aSRC_0[i] = 2000 * (arm_sin_f32(i * 2 * (float)3.1415926545 / (float)10.) + (float)1.);
    }
    // Phase Pi
    for(i = 0; i < 160; i++)
    {
//		aSRC_Pi[i] = 2000*(-arm_sin_f32(i*2*(float)3.1415926545/(float)40.) + (float)1.);
        aSRC_Pi[i] = 2000 * (-arm_sin_f32(i * 2 * (float)3.1415926545 / (float)10.) + (float)1.);
    }

//	for( i = 0; i < 160; i++)
//	{
//		aSRC_Buffer[i] = aSRC_0[i];
//	}
//	for( i = 0; i < 160; i++)
//	{
//		aSRC_Buffer[160 + i] = aSRC_Pi[i];
//	}

    SendOneFrameDPSK(0x00ff7456);

    DAC_Ch1_SineWaveConfig(aSRC_Buffer, 4000);
//	DAC_Ch1_SineWaveConfig(aSRC_Buffer, 320);
    DAC_Ch2_SineWaveConfig((uint16_t*)u16DecodeOutBuf, 8000);

    fSpecN4_real = 0;
    fSpecN4_imaginal = 0;
    /* ����������ѭ���� */
    while(1)
    {
        if(u8ADC_Halffull == 1)
        {
            // ����
            u32Tmp = 4000 - 160;

            // For test
//				for( i = 0; i < 4000; i++ )
//				{
//						 u16WorkingBuf[i] = uhADCxConvertedValue[i];
//				}
            //

            // ��ADC��ȡ4000����������25bit������
            for(i = 0; i < 160; i++)
            {
                u16WorkingBuf[i] = ((int32_t)(uhADCxConvertedValue[i] - 2047) * (int32_t)(u16WorkingBuf[8000 + i] - 2047)) / 4096 + 2047;
            }
            for(i = 0; i < 4000 - 160; i++)
            {
                u16WorkingBuf[160 + i] = ((int32_t)(uhADCxConvertedValue[160 + i] - 2047) * (int32_t)(uhADCxConvertedValue[i] - 2047)) / 4096 + 2047;
            }

        }
        else if(u8ADC_Halffull == 2)
        {
            // ȫ��
            u32Tmp = 8000;

            // For test
//				for( i = 4000; i < 8000; i++ )
//				{
//						 u16WorkingBuf[i] = uhADCxConvertedValue[i];
//				}
            //

            for(i = 4000 - 160; i < 8000 - 160; i++)
            {
                u16WorkingBuf[160 + i] = ((int32_t)(uhADCxConvertedValue[160 + i] - 2047) * (int32_t)(uhADCxConvertedValue[i] - 2047)) / 4096 + 2047;
            }
            for(i = 0; i < 160; i++)
            {
                u16WorkingBuf[8000 + i] = uhADCxConvertedValue[8000 - 160 + i];
            }
        }


        if(u8DAC2_Halffull == 1)
        {
            // ����

            arm_iir_f32_Lp((float_t*)u16WorkingBuf, (float_t*)fLpFilterBuf, 4000);

            for(i = 0; i < 4000; i++)
            {

//						u16DecodeOutBuf[i] = u16WorkingBuf[i];
                u16DecodeOutBuf[i] = (uint16_t)(fLpFilterBuf[i] * ScaleValue);
            }

            BitSync((uint16_t*)u16DecodeOutBuf, 2000, 1, &sBitSyncStruct);

        }
        else if(u8DAC2_Halffull == 2)
        {
            // ȫ��

            arm_iir_f32_Lp((float_t*)(&u16WorkingBuf[4000]), (float_t*)fLpFilterBuf, 4000);
            for(i = 4000; i < 8000; i++)
            {
//						 u16DecodeOutBuf[i] = u16WorkingBuf[i];
                u16DecodeOutBuf[i] = (uint16_t)(fLpFilterBuf[i - 4000] * ScaleValue);
            }

            BitSync((uint16_t*)u16DecodeOutBuf, 2000, 2, &sBitSyncStruct);

//				for( i = 4000 - 160; i < 8000 - 160; i++ )
//				{
//						 u16DecodeOutBuf[i] = ((uint32_t)u16WorkingBuf[i + 160] * (uint32_t)u16WorkingBuf[i]) >> 12;
//				}
//				for( i = 0; i < 160; i++ )
//				{
//						 u16DecodeOutBuf[8000 + i] = u16WorkingBuf[8000 - 160 + i];
//				}
        }
    }
}

