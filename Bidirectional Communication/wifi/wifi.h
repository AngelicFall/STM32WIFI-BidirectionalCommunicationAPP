#ifndef _WIFI_H_
#define _WIFI_H_

#include "handle.h"

#define error 0xfe

#define MaxNum 5

#define Net_Online      0x01
#define Net_Offline     0x02

#define CMD_Type        1               //��ͨ����
#define NetData_Type    2               //������������
#define ATTest_Type     3               //AT��������
#define Spec_Type       4               //���������������󷵻�ֵЯ������Ҫ�жϵĲ���
#define Choice_type     5               //ѡ��������������������ʧ�ܣ�������ض���λ�ÿ�ʼ����ִ��

typedef enum{
  ESP8266_NULL = 0,
  ESP8266_STA,
  ESP8266_AP,                   //�ݲ�����
  ESP8266_STAandAP              //�ݲ�����
}WifiState;

typedef enum
{
  C_Null = 0,
  C_Client,
  C_Server
}WifiChildState;

typedef struct
{
  uint8_t Channel;              //ͨ����
  uint8_t ConnectState;         //����״̬
  uint8_t ConnectIP[20];           //IP
}NetConnectUnit;

typedef struct            //���յ������ݵ�Ԫ
{
  uint8_t RxChannel;
  uint8_t RxLength;
  uint8_t RxData[255];
}RxNetDataUnit;

typedef struct             //���͵����ݵ�Ԫ
{
  uint8_t TxIP[20];
  uint8_t TxLength;
  uint8_t TxData[255];
  uint8_t TxDataFinish;         //��һ֡���ݰ�����Ҫ��ȡ�����Ч����ʱ��Ҫ ���� һ֡���ݰ�����Ҫ��ȡ10���¶���Ϣ
}TxNetDataUnit;

void WiFi_Init(void);
void Wifi_Operation(void);
void ServerClientRxTxData(void);
void WifiSendCmd(uint8_t* data,uint8_t Leng);

extern int WifiConfigFlag;//WiFi�������

extern TxNetDataUnit  TNDU;
extern uint16_t CMD_ReSendTim;
extern uint8_t NetModeAbnormal;
extern uint8_t Target_IP[15];
extern uint8_t WifiTxDataFlag;
extern uint8_t WifiModeSetDown;
extern uint8_t CMD_CountNum;
extern uint8_t WifiCCCNum;
extern uint8_t CMD_OtherCount;
extern uint8_t AT_CWJAP[50];
extern uint8_t AT_CIPSTA[30];
extern uint8_t AT_CIPSTART[50];
extern uint8_t AT_CIPSEND[20];
extern uint8_t AT_CIPSERVER[25];

void WifiSendDataFromIP(uint8_t* IP,uint8_t* Data,uint8_t Length);

#endif
