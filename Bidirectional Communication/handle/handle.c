#include "handle.h"
#include "shtxx.h"
#include "LQ12864.h"
extern TxNetDataUnit  TNDU;

uint8_t UART2State = fail;
uint8_t UART3State = fail;
uint8_t UART4State = fail;
uint8_t UART5State = fail;

/***********************************************************
* �������ܣ�ϵͳʱ�ӳ�ʼ��
* ����������time ϵͳʱ��Ƶ��
* ����ֵ����
***********************************************************/
void SysTickInit(uint8_t time)
{
  delay_init(time);
}

/***********************************************************
* �������ܣ���ʱ����ʼ��
* ������������
* ����ֵ����
***********************************************************/
void TimInit(void)
{
  time2_init();
}

/***********************************************************
* �������ܣ����ڳ�ʼ��
* ������������
* ����ֵ����
***********************************************************/
void UsartInit(void)
{
  Initial_USART1(57600);
}

/***********************************************************
* �������ܣ�LED��ʼ��
* ������������
* ����ֵ����
***********************************************************/
void LedInit(void)
{
  leds_init();
}


/***********************************************************
* �������ܣ��豸��ʼ��
* ������������
* ����ֵ����
***********************************************************/
void EquipInit(void)
{
  SystemInit();                          //��ʼ��ϵͳ
  
  SysTickInit(72);                       //�δ�ʱ����ʼ��
  
  TimInit();                            //��ʱ����ʼ��

  UsartInit();                          //���ڳ�ʼ��
  
  WiFi_Init();                          //ESP8266��ʼ��
  
  UART_NVIC_Configuration();            //�����ж�����
  
  OLED_Init();
  SHTXX_Init();
  
  LedInit();                            //LED��ʼ��
  
  Usart_SendNumString(USART1,"D1=ON D1=OFF\r\n",14);

  delay_s(2);                           //�ȴ��豸����
}

/***********************************************************
* �������ܣ�����1��ز���
* ����������USART ���ڽ��յ�������
* ����ֵ����
***********************************************************/
void USART1_DataProcessing(UsartType* USART)
{
  if(USART->USART_DataFinishFlag)
  {
    USART->USART_DataFinishFlag &= ~0x01;
     
    memcpy(TNDU.TxIP,Target_IP,strlen((const char*)Target_IP));                //��ȡIP
    TNDU.TxLength = USART->USART_RxLength;                                      //��ȡ����
    memcpy(TNDU.TxData,USART->USART_RxEffecDataBuf,USART->USART_RxLength);       //��ȡ��������
    
    WifiTxDataFlag |= 0x01;                    //�������ݱ�־
  }
}

/***********************************************************
* �������ܣ�����1��������
* ������������
* ����ֵ����
***********************************************************/
void USART1_Receive(void)
{
  USART_Unit_ReceiveData(&USART_Unit[1]);
  
  USART1_DataProcessing(&USART_Unit[1]);
}

/***********************************************************
* �������ܣ�������ز���
* ������������
* ����ֵ����
***********************************************************/
void USART_Operation(void)
{
  USART1_Receive();
}

/***********************************************************
* �������ܣ���������
* ������������
* ����ֵ����
***********************************************************/
void Other_Operation(void)
{
  ScanKey_Operation();
}

/***********************************************************
* �������ܣ�����״̬���
* ������������
* ����ֵ����
* ע�������������쳣����øú������ú���������������ӵ�����״̬λ
      �����ظ��������״̬
***********************************************************/
void NetStateMonitor(void)
{
  if(NetModeAbnormal)
  {
    NetModeAbnormal &= ~0x01;                   //����쳣��־
    
    WifiModeSetDown &= ~0x01;                //��������ʼ����־
    CMD_CountNum = 0;                             //�������ӽ��������㣬ʹ����������ʼ
    WifiCCCNum = 0;                             //���緢�����ݽ��������㣬ʹ����������ʼ
    CMD_OtherCount &= ~0x01;                   //ƴ�Ӻ����ļ�������
    
    memset(&AT_CWJAP[9],0,sizeof(AT_CWJAP)-9);          //�����������
    memset(&AT_CIPSTA[10],0,sizeof(AT_CIPSTA)-10);              //�����������
    memset(&AT_CIPSTART[12],0,sizeof(AT_CIPSTART)-12);
    memset(&AT_CIPSEND[11],0,sizeof(AT_CIPSEND)-11);
    memset(&AT_CIPSERVER[13],0,sizeof(AT_CIPSERVER)-13);

  }
}
