///AT+NSOCL=0
#include "BC28.h"
#include "main.h"
#include "string.h"
#include "usart.h"
#include "stdarg.h"



 #define SERVERPORT 2346

/*************************************************************************�Ա�ķ��ͺ���***/

uint8_t * Send_check_cmd(uint8_t *str)
{
	
	char *strx=0;
	if(USART1_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
	} 
	return (uint8_t *)strx;
}

uint8_t Send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime)
{
	uint8_t res=0; 
	USART1_RX_STA=0;
	u1_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			HAL_Delay(10);
			if(USART1_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(Send_check_cmd(ack))
				{
					printf("ack:%s\r\n",(uint8_t*)ack);
					break;//�õ���Ч���� 
				}
					USART1_RX_STA=0;
			}
		}
		if(waittime==0)res=1; 
	}
	return res;
}

uint8_t Send_data(uint8_t *data,uint8_t *ack,uint16_t waittime)
{
	uint8_t res=0; 
	USART1_RX_STA=0;
	u1_printf("%s\r\n",data);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			HAL_Delay(10);
			if(USART1_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(Send_check_cmd(ack))break;//�õ���Ч���� 
				USART1_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}


/****************************************************************************BC28����ģ��****/


//int BC28_Init(void)
//{
//    int errcount = 0;
//    err = 0;    //�ж�ģ�鿨�Ƿ��������Ҫ
//    printf("start init bc28\r\n");
//    Uart2_SendStr("ATE1\r\n");
//    HAL_Delay(300);
//    printf(buf_uart2.buf);      //��ӡ�յ��Ĵ�����Ϣ
//    printf("get back bc28\r\n");
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
//    Clear_Buffer();	
//    while(strx==NULL)
//    {
//        printf("\r\n��Ƭ���������ӵ�ģ��...\r\n");
//        Clear_Buffer();	
//        Uart2_SendStr("AT\r\n");
//        HAL_Delay(300);
//        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
//    }
//    Uart2_SendStr("AT+CMEE=1\r\n"); //�������ֵ
//    HAL_Delay(300);
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
//    Clear_Buffer();	

//    Uart2_SendStr("AT+NBAND?\r\n"); //�������ֵ
//    HAL_Delay(300);
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
//    if(strx)
//    {
//        printf("======== BAND========= \r\n %s \r\n",buf_uart2.buf);
//        Clear_Buffer();
//        HAL_Delay(300);
//    }
//    
//    Uart2_SendStr("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
//    HAL_Delay(300);
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"ERROR");//ֻҪ�������� �����ͳɹ�
//    if(strx==NULL)
//    {
//        printf("�ҵĿ����� : %s \r\n",buf_uart2.buf);
//        Clear_Buffer();	
//        HAL_Delay(300);
//    }
//    else
//    {
//        err = 1;
//        printf("������ : %s \r\n",buf_uart2.buf);
//        Clear_Buffer();
//        HAL_Delay(300);
//    }

//    Uart2_SendStr("AT+CGATT=1\r\n");//�������磬PDP
//    HAL_Delay(300);
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//��OK
//    Clear_Buffer();	
//    if(strx)
//    {
//        Clear_Buffer();	
//        printf("init PDP OK\r\n");
//        HAL_Delay(300);
//    }
//    Uart2_SendStr("AT+CGATT?\r\n");//��ѯ����״̬
//    HAL_Delay(300);
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT:1");//��1 ��������ɹ� ��ȡ��IP��ַ��
//    Clear_Buffer();	
//    errcount = 0;
//    while(strx==NULL)
//    {
//        errcount++;
//        Clear_Buffer();	
//        Uart2_SendStr("AT+CGATT?\r\n");//��ȡ����״̬
//        HAL_Delay(300);
//        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT:1");//����1,����ע���ɹ�
//        if(errcount>100)     //��ֹ��ѭ��
//        {
//            err=1;
//            errcount = 0;
//            break;
//        }
//    }


//    Uart2_SendStr("AT+CSQ\r\n");//�鿴��ȡCSQֵ
//    HAL_Delay(300);
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CSQ");//����CSQ
//    if(strx)
//    {
//        printf("�ź�����:%s\r\n",buf_uart2.buf);
//        Clear_Buffer();
//        HAL_Delay(300);
//    }

//    Uart2_SendStr("AT+CEREG?\r\n");
//    HAL_Delay(300);
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG:0,1");//����ע��״̬
//    extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG:1,1");//����ע��״̬
//    Clear_Buffer();	
//    errcount = 0;
//    while(strx==NULL&&extstrx==NULL)//��������ֵ��û��
//    {
//        errcount++;
//        Clear_Buffer();
//        Uart2_SendStr("AT+CEREG?\r\n");//�ж���Ӫ��
//        HAL_Delay(300);
//        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG:0,1");//����ע��״̬
//        extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG:1,1");//����ע��״̬
//        if(errcount>100)     //��ֹ��ѭ��
//        {
//            err=1;
//            errcount = 0;
//            break;
//        }
//    }
//    return err;
//}

//void BC28_PDPACT(void)//�������Ϊ���ӷ�������׼��
//{
//    int errcount = 0;
//    Uart2_SendStr("AT+CGDCONT=1,\042IP\042,\042HUAWEI.COM\042\r\n");//����APN
//    HAL_Delay(300);
//    Uart2_SendStr("AT+CGATT=1\r\n");//�����
//    HAL_Delay(300);
//    Uart2_SendStr("AT+CGATT?\r\n");//�����
//    HAL_Delay(300);
//    strx=strstr((const char*)buf_uart2.buf,(const char*)" +CGATT:1");//ע����������Ϣ
//    Clear_Buffer();	
//    while(strx==NULL)
//    {
//        errcount++;
//        Clear_Buffer();
//        Uart2_SendStr("AT+CGATT?\r\n");//�����
//        HAL_Delay(300);
//        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT:1");//һ��Ҫ�ն�����
//        if(errcount>100)     //��ֹ��ѭ��
//        {
//            errcount = 0;
//            break;
//        }
//    }
//    Uart2_SendStr("AT+CSCON?\r\n");//�ж�����״̬������1���ǳɹ�
//    HAL_Delay(300);
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CSCON:0,1");//ע����������Ϣ
//    extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CSCON:0,0");//ע����������Ϣ
//    Clear_Buffer();	
//    errcount = 0;
//    while(strx==NULL&&extstrx==NULL)    //���Ӳ��ܷ�������
//    {
//        errcount++;
//        Clear_Buffer();
//        Uart2_SendStr("AT+CSCON?\r\n");//
//        HAL_Delay(300);
//        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CSCON:0,1");//
//        extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CSCON:0,0");//
//        if(errcount>100)     //��ֹ��ѭ��
//        {
//            errcount = 0;
//            break;
//        }
//    }

//}

//void BC28_ConTCP(void)
//{
//    int errcount = 0;
//    Uart2_SendStr("AT+NSOCL=1\r\n");//�ر�socekt����
//    Uart2_SendStr("AT+NSOCL=2\r\n");//�ر�socekt����
//    Uart2_SendStr("AT+NSOCL=3\r\n");//�ر�socekt����
//    HAL_Delay(300);
//    Clear_Buffer();	
//    Uart2_SendStr("AT+NSOCR=STREAM,6,0,1\r\n");//����һ��TCP Socket
//    HAL_Delay(300);
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
//    while(strx==NULL)
//    {
//        errcount++;
//        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
//        if(errcount>10)     //��ֹ��ѭ��
//        {
//            errcount = 0;
//            break;
//        }

//    }
//    Clear_Buffer();

//    memset(atstr,0,BUFLEN);
//    //AT+NSOCO=1,123.57.41.13,1001
//    sprintf(atstr,"AT+NSOCO=1,%s,%d\r\n",SERVERIP,SERVERPORT);
//    Uart2_SendStr(atstr);//����0 socketIP�Ͷ˿ں������Ӧ���ݳ����Լ�����
//    HAL_Delay(300);
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
//    errcount = 0;
//    while(strx==NULL)
//    {
//        errcount++;
//        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
//        if(errcount>10)     //��ֹ��ѭ��
//        {
//            errcount = 0;
//            break;
//        }
//    }
//    Clear_Buffer();	
//}

//void BC28_Senddata(uint8_t *len,uint8_t *data)
//{
//    int errcount=0;
//    memset(atstr,0,BUFLEN);
//    //AT+NSOSD=1,4,31323334
//    sprintf(atstr,"AT+NSOSD=1,%s,%s\r\n",len,data);
//    Uart2_SendStr(atstr);//����0 socketIP�Ͷ˿ں������Ӧ���ݳ����Լ�����
//    HAL_Delay(300);
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
//    while(strx==NULL)
//    {
//        errcount++;
//        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
//        if(errcount>100)     //��ֹ��ѭ��
//        {
//            errcount = 0;
//            break;
//        }
//    }
//    Clear_Buffer();	
//}

///*
//+NSONMI:0,4
//AT+NSORF=0,4
//0,47.104.238.90,1001,4,31323334,0

//OK*/
//void BC28_RECData(void)
//{
//    char i;
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"+NSONMI:");//����+NSONMI:��+NSONMI:1,5
//    if(strx)
//    {
//        BC28_Status.Socketnum=strx[8];//���
//        for(i=0;;i++)
//        {
//            if(strx[i+10]==0x0D)
//                break;
//            BC28_Status.reclen[i]=strx[i+10];//����
//        }

//        memset(atstr,0,BUFLEN);
//        sprintf(atstr,"AT+NSORF=%c,%s\r\n",BC28_Status.Socketnum,BC28_Status.reclen);//�����Լ����
//        Uart2_SendStr(atstr);
//        HAL_Delay(300);
//        Clear_Buffer();

//    }
//    Uart2_SendStr("AT+NSORF=1,100\r\n");
//}

