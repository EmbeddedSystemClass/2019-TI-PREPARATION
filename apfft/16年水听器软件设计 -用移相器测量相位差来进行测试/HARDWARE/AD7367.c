/*
��normal mode��ȡ��·�źŵ�����
*/
#include "AD7367.h"
/*
	��ʼ��,power_up��һ��ʱ�䣬��ʼ��IO�ں����������
*/
void AD7367_Init()
{
	 AD7367_CNVST(1);//���ָߵ�ƽ����70��s��powerupʱ��
	 delay_ms(1);
}