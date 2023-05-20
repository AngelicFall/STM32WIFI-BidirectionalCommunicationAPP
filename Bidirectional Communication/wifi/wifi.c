#include "wifi.h"

/*��������*/
uint8_t WifiModeSetDown = fail;                 //WIFIģʽ������ɱ�־λ
uint8_t NetModeAbnormal = fail;                //����ģʽ�����쳣��־λ
uint8_t CMD_Count = 0;                          //����ȷ������ֻ��һ��
uint16_t CMD_ReSendTim = 0;                      //�ط�ʱ����
uint8_t CMD_ReSendCount = 0;                    //�ط��������
uint8_t CMD_ReturnOK = 0;                       //������OK����
uint8_t CMD_CountNum = 0;                       //����ȷ��ִ�е�����������
uint8_t CMD_ReturnReady = 0;                       //������ready����
uint8_t CMD_OtherCount = 0;                     //ƴ�Ӻ����ļ�����ִֻ��һ��
uint8_t NetMode = Net_Online;                           //����ģʽ ���߻�������  �ϵ�Ĭ��������ģʽ
uint8_t CIPSTART_Flag = 0;                      //CIPSTART�����־
uint8_t WifiClientModeFinishFlag = 0;           //WIFI�ͻ���ģʽ�������
uint8_t WifiReDataFlag = 0;                         //wifi���յ�����
uint8_t CMD_ReturnSign = 0;                       //������>����
uint8_t WifiSendDataNum = 0;                    //wifi������������
uint8_t CMD_ReturnSendOK = 0;                       //������send OK����
uint8_t WifiTxDataFlag = 0;                         //wifi��������
uint8_t WifiServerModeFinishFlag = 0;           //WIFI�����ģʽ�������
uint8_t WifiCCCNum = 0;                         //WIFIͨ������״̬�ı�֮���������
uint8_t WifiUartSignLoc = 0;                    //wifi���ڽ��յ������ݱ�ʶ����һ���ֽڵ�λ��
uint8_t WifiUartReLeng = 0;                     //wifi���ڽ��ܵ������ݳ���
uint8_t CMD_RSTFlag = 0;                        //wifi���͵�Ϊ��λ����   
uint8_t NoneEquipOnlineFlag = 0;                //�豸�������뱻���ӱ�־λ

uint8_t WifiReNetChannel[5] = {0};              //WIFI���յ����������ݵ�ͨ��
uint8_t WifiReNetDataLength[5] = {0};           //WIFI���յ����������ݵĳ���
uint8_t WifiReNetData[255] = {0};               //WIFI���յ�����������

uint8_t AT_AT[]         = {"AT\r\n"};                     //���Ӳ�������
uint8_t AT_ATE0[]       = {"ATE0\r\n"};                   //����Ҫ����
uint8_t AT_RST[]        = {"AT+RST\r\n"};                 //����ģ��
uint8_t AT_CWMODE[]     = {"AT+CWMODE=0\r\n"};           //����
uint8_t AT_CWJAP[50]    = {"AT+CWJAP="};                 //����AP
uint8_t AT_CWSAP[50]    = {"AT+CWSAP="};                 //����AP
uint8_t AT_CIPMUX[]     = {"AT+CIPMUX=0\r\n"};           //������������ 0������ 1������
uint8_t AT_CIPSERVER[25]  = {"AT+CIPSERVER="};          //�����˿ں� ��Ϊ�����
uint8_t AT_CIPSEND[20]  = {"AT+CIPSEND="};              //��������  ��󳤶�Ϊ2048
uint8_t AT_CIPSTA[30]   = {"AT+CIPSTA="};               //����STA��IP
uint8_t AT_CIPAP[20]    = {"AT+CIPAP="};                 //����AP��IP
uint8_t AT_CIPSTART[50] = {"AT+CIPSTART="};              //����TCP/UDP����
uint8_t AT_CIPSTATUS[]  = {"AT+CIPSTATUS\r\n"};          //��ȡ����״̬

uint8_t Exit_Passthrough[] = {"+++"};                   //�˳�͸��
uint8_t AT_RESTORE[] = {"AT+RESTORE\r\n"};              //�ָ���������


uint8_t Wifi_Name[15]   = {"wifi-001"};                 //WIFI����
uint8_t Wifi_Pwd[12]    = {"1234567890"};                //WIFI����
uint8_t Self_IP[15]     = {"192.168.43.142"};              //����IP
uint8_t Self_Port[5]    = {"8080"};                      //�������Ķ˿�
uint8_t MUX             = 0;                             //0 ������  1 ������
uint8_t Net[5]          = {"TCP"};                       //�������ӷ�ʽ
uint8_t Target_IP[15]   = {"192.168.43.1"};              //Ŀ��IP
uint8_t Target_Port[5]  = {"8086"};                      //Ŀ��˿�

NetConnectUnit NCU[MaxNum];
RxNetDataUnit  RNDU;
TxNetDataUnit  TNDU;

/***********************************************************
* �������ܣ�WIFI������ŵĳ�ʼ��
* ������������
* ����ֵ����
***********************************************************/
void Wifi_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* ��ʼ��GPIOʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_SetBits(GPIOA,GPIO_Pin_0);       //����Ϊ����ģʽ   �͵�ƽΪ��дģʽ
}

/***********************************************************
* �������ܣ�WIFI��صĳ�ʼ��
* ������������
* ����ֵ����
***********************************************************/
void WiFi_Init(void)
{
  Initial_USART3(115200);
  
  /*�˳�͸��ģʽ*/
  WifiSendCmd((uint8_t *)Exit_Passthrough,(uint8_t)strlen((const char*)Exit_Passthrough));
  delay_ms(1000);
  WifiSendCmd((uint8_t *)Exit_Passthrough,(uint8_t)strlen((const char*)Exit_Passthrough));
  delay_ms(1000);
  /*�ָ���������*/
  WifiSendCmd((uint8_t *)AT_RESTORE,(uint8_t)strlen((const char*)AT_RESTORE));
}

/***********************************************************
* �������ܣ����ڷ���
* ����������data ���͵�����   leng����
* ����ֵ����
***********************************************************/
void WifiSendCmd(uint8_t* data,uint8_t Leng)
{
  Usart_SendNumString(USART3,data,Leng);
  Usart_SendNumString(USART1,data,Leng);
}

/***********************************************************
* �������ܣ�����AT����ĺ���
* ����������CMD AT���� FinishFlag ���ͳɹ���־λ����  ReSendTime ��ʱʱ��  ReSendCount �ش����� DataType ��������(����ͨ�������������)
* ����ֵ��finish �ɹ�  fail ʧ��
***********************************************************/
uint8_t SendCMDProcess(uint8_t* CMD,uint8_t Length,uint8_t* FinishFlag,uint16_t ReSendTime,uint8_t ReSendCount,uint8_t DataType)
{
  if(!CMD_Count)
  {
    CMD_Count = success;
    WifiSendCmd(CMD,Length);
    CMD_ReSendTim = fail;                                           //��ԭ�ط�ʱ��
  }
  if(*FinishFlag)
  {
    CMD_Count = fail;
    *FinishFlag = fail;                                         //������յ�OK��־λ
    CMD_ReSendTim = fail;                                       //��ԭ�ط�ʱ��
    CMD_ReSendCount = fail;                                     //��ԭ�ط�����
    
    return success;
  }

  if(CMD_ReSendTim >= ReSendTime)                                 //�ط�
  {
    CMD_ReSendTim = fail;
    CMD_Count = fail;
    CMD_ReSendCount++;
    if(CMD_ReSendCount >= ReSendCount)                           //�ط������޷�Ӧ
    {
      CMD_ReSendCount = fail;
      if(DataType == CMD_Type)                            //��ͨ�����쳣�������磬���õ�AT���
      {
        NetModeAbnormal = success;                        //AT������쳣
      }
      else if(DataType == NetData_Type)                   //�������ݣ����������磬�������ݵ��ط���
      {
        CMD_Count = fail;
        *FinishFlag = fail;                               //������յ�OK��־λ
        return success;
      } 
    }
  }
  return fail;
}

/***********************************************************
* �������ܣ�AT��������
* ������������
* ����ֵ����
***********************************************************/
void CMD_ATTEST(uint8_t* CMD_Num)
{
  if(SendCMDProcess(AT_AT,strlen((const char*)AT_AT),&CMD_ReturnOK,200,3,CMD_Type))           //�ش�ʱ����С��λΪ5ms
    (*CMD_Num)++;
}

/***********************************************************
* �������ܣ��رջ�������
* ������������
* ����ֵ����
***********************************************************/
void CMD_ATEO(uint8_t* CMD_Num)
{
  if(SendCMDProcess(AT_ATE0,strlen((const char*)AT_ATE0),&CMD_ReturnOK,200,3,CMD_Type))         //�ش�ʱ����С��λΪ5ms
    (*CMD_Num)++;
}

/***********************************************************
* �������ܣ�����ģ��ģʽ
* ����������WifiState Mode ģ��ģʽ  
            1 STAģʽ  
            2 APģʽ  
            3 STA&APģʽ
* ����ֵ����
***********************************************************/
void CMD_CWMODE(WifiState Mode,uint8_t* CMD_Num)
{ 
  switch(Mode)
  {
    case ESP8266_STA:           AT_CWMODE[10] = '1';            break;
    case ESP8266_AP:            AT_CWMODE[10] = '2';            break;
    case ESP8266_STAandAP:      AT_CWMODE[10] = '3';            break;
    default:                                                    break;
  }
  if(SendCMDProcess(AT_CWMODE,strlen((const char*)AT_CWMODE),&CMD_ReturnOK,200,3,CMD_Type))       //�ش�ʱ����С��λΪ5ms
    (*CMD_Num)++;
}

/***********************************************************
* �������ܣ���λģ��
* ������������
* ����ֵ����
***********************************************************/
void CMD_RESET(uint8_t* CMD_Num)
{
  CMD_RSTFlag |= 0x01;
  if(SendCMDProcess(AT_RST,strlen((const char*)AT_RST),&CMD_ReturnReady,1000,3,CMD_Type))          //�ش�ʱ����С��λΪ5ms
  {
    (*CMD_Num)++;
    CMD_RSTFlag &= ~0x01;
  }
}

/***********************************************************
* �������ܣ��鿴�ַ�����ָ���ַ�������λ��
* ����������string �ַ��� sign ָ�����ַ�  num �ڼ���
* ����ֵ�������ַ���λ��
***********************************************************/
uint8_t LocationOfSign(uint8_t* string,uint8_t sign,uint8_t Num)
{
  uint8_t i = 0,Count = 0;
  while(string[i])
  {
    if(string[i] == sign)
    {
      Count++;
      if(Count == Num)
        break;
    } 
    i++;
    if(i >= 50)
      return fail;
  }
  return i;
}

/***********************************************************
* �������ܣ�����ƴ�Ӻ���
* ����������*CMD  ����   
            *Data  ��Ҫ��ӵ�����
            Channel ͨ����
            ע ������WIFI��ʱ���ʹ��  ����WIFI��ʱ�����Ϊ0
            EncryptType ��������
            0  open
            2  WPA_PSK
            3  WPA2_PSK
            4  WPA_WPA2_PSK
* ����ֵ����
***********************************************************/
void CMDJoint(uint8_t* CMD,uint8_t* Data,uint8_t* Data2,uint8_t* Channel,uint8_t EncryptType)
{
  uint8_t enter[] = {"\r\n"};
  if(!CMD_OtherCount)
  {
    CMD_OtherCount |= 0x01;
    
    memset(&CMD[LocationOfSign(CMD,'=',1)+1],0,strlen((const char*)CMD)-LocationOfSign(CMD,'=',1)-1);
    
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)Data);
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)",");
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)Data2);
    strcat((char *)CMD,(const char *)"\"");
    if(memcmp(CMD,"AT+CWSAP",8) == 0)
    {
      strcat((char *)CMD,(const char *)",");
      strcat((char *)CMD,(const char *)Channel);
      strcat((char *)CMD,(const char *)",");
      strcat((char *)CMD,(const char *)&EncryptType);
    }
    strcat((char *)CMD,(const char *)&enter);
  }
}

/***********************************************************
* �������ܣ�����WIFI����
* ����������*name  WIFI����   
            *PSWord  WIFI����
* ����ֵ����
***********************************************************/
void CMD_CWJAP(uint8_t *name,uint8_t *PSWord,uint8_t* CMD_Num)
{
  CMDJoint(AT_CWJAP,name,PSWord,0,0);
  if(SendCMDProcess(AT_CWJAP,strlen((const char*)AT_CWJAP),&CMD_ReturnOK,1000,3,CMD_Type))          //�ش�ʱ����С��λΪ5ms
  {
    (*CMD_Num)++;
    
    CMD_OtherCount = fail;
    memset(&AT_CWJAP[9],0,sizeof(AT_CWJAP)-9);          //�����������
  }
}

/***********************************************************
* �������ܣ�IPƴ�Ӻ���
* ����������CMD ��Ҫ�ӵ�����  IP ��ӵ�IP
* ����ֵ����
***********************************************************/
void IPJoinCMD(uint8_t* CMD,uint8_t* IP)
{
  uint8_t enter[] = {"\r\n"};
  if(!CMD_OtherCount)
  {
    CMD_OtherCount |= 0x01;
    
    memset(&CMD[LocationOfSign(CMD,'=',1)+1],0,strlen((const char*)CMD)-LocationOfSign(CMD,'=',1)-1);
    
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)IP);
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)&enter);
  }
}

/***********************************************************
* �������ܣ�����IP
* ����������IP Ҫ���õ�IP��ַ  
* ����ֵ����
***********************************************************/
void CMD_CIPSTA(uint8_t* IP,uint8_t* CMD_Num)
{
  IPJoinCMD(AT_CIPSTA,IP);
  if(SendCMDProcess(AT_CIPSTA,strlen((const char*)AT_CIPSTA),&CMD_ReturnOK,200,3,CMD_Type))          //�ش�ʱ����С��λΪ5ms
  {
    CMD_OtherCount &= ~0x01;
    *CMD_Num = 0;
    WifiModeSetDown |= 0x01;
    memset(&AT_CIPSTA[10],0,sizeof(AT_CIPSTA)-10);              ////�����������
  }
}

/***********************************************************
* �������ܣ�����ΪSTAģʽ
* ������������
* ����ֵ����
***********************************************************/
void ModeSetSTA(void)
{
  switch(CMD_CountNum)
  {
    case 0:     CMD_ATTEST(&CMD_CountNum);                      break;                  //AT��������
    case 1:     CMD_ATEO(&CMD_CountNum);                        break;                  //�رջ�������
    case 2:     CMD_CWMODE(ESP8266_STA,&CMD_CountNum);          break;                  //����ģ��ģʽ STA/AP/STA+AP
    case 3:     CMD_RESET(&CMD_CountNum);                       break;                  //��λ����
    case 4:     CMD_ATEO(&CMD_CountNum);                        break;                  //�رջ�������
    case 5:     CMD_CWJAP(Wifi_Name,Wifi_Pwd,&CMD_CountNum);    break;                  //����WIFI��������
    case 6:     CMD_CIPSTA(Self_IP,&CMD_CountNum);              break;                  //��������IP����
    default:                                                    break;
  }
}

/***********************************************************
* �������ܣ�����ģ��ģʽ
* ����������WifiState Mode      ģ��ģʽ  
            ESP8266_STA         STAģʽ  
            ESP8266_AP          APģʽ       �ݲ�����     
            ESP8266_STAandAP    STA&APģʽ  �ݲ�����
* ����ֵ����
* ��ע�� ��ʱȥ���״̬����ʱʱ��Ϊ300ms
         ����ģ��ģʽΪSTA����AP ��ΪSTAģʽʱ ����WIFI����  
         ��ΪAPģʽʱ ����һ��WIFI����
***********************************************************/
void WIFIMode(WifiState Mode)
{
  if(!WifiModeSetDown)               //250ms ģʽ����δ���  ģ�����쳣
  {
    switch(Mode)
    {
      case ESP8266_STA:                                   
      {
        ModeSetSTA();
        break;
      }
      case ESP8266_AP:                                    
      {
        //ModeSetAP();
        break;
      }
      case ESP8266_STAandAP:                              
      {
      //  ModeSetSTAandAP();
        break;
      }
    }
  }
}

/***********************************************************
* �������ܣ��Ƿ���������
* ����������Num  �����ӱ�ʾ
            0  �ر�   
            1  ����
            
            TypeCount ������ ����˻�ͻ���
* ����ֵ����
***********************************************************/
void CMD_CIPMUX(uint8_t Num,uint8_t* CMD_Num)
{
  switch(Num)
  {
    case 0:     AT_CIPMUX[10] = '0';    break;
    case 1:     AT_CIPMUX[10] = '1';    break;
    default:                            break;
  }
  if(SendCMDProcess(AT_CIPMUX,strlen((const char*)AT_CIPMUX),&CMD_ReturnOK,200,3,CMD_Type))       //�ش�ʱ����С��λΪ5ms
    (*CMD_Num)++;
}

/***********************************************************
* �������ܣ���ʼƴ�Ӻ���
* ����������CMD     ����      
            Channel ͨ���� ��������Ч  0xff ������
            Net ���ӷ�ʽ tcp/udp
            IP  PORT�˿�
* ����ֵ����
***********************************************************/
void StartJoinCmd(uint8_t* CMD,uint8_t Channel,uint8_t* Net,uint8_t* IP,uint8_t* Port)
{
  uint8_t enter[] = {"\r\n"};
  if(!CMD_OtherCount)
  {
    CMD_OtherCount |= 0x01;
    
    memset(&CMD[LocationOfSign(CMD,'=',1)+1],0,strlen((const char*)CMD)-LocationOfSign(CMD,'=',1)-1);
    
    if(Channel != 0xff)
    {
      strcat((char *)CMD,(const char *)&Channel);
      strcat((char *)CMD,(const char *)",");
    }
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)Net);
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)",");
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)IP);
    strcat((char *)CMD,(const char *)"\"");
    strcat((char *)CMD,(const char *)",");
    strcat((char *)CMD,(const char *)Port);
    
    strcat((char *)CMD,(const char *)&enter);
  }
}

/***********************************************************
* �������ܣ���������ͨ��
* ����������
            channel ����ͨ�� 
            ע�����Ϊ������ ��д����ͨ����  ���Ϊ������ ��д0XFF  
            Net  ���ӷ�ʽ  TCP/UDP
            IP
            Ŀ��IP
            PORT
            Ŀ��˿ں�
* ����ֵ����
***********************************************************/
void CMD_CIPSTART(uint8_t channel,uint8_t* Net,uint8_t* IP,uint8_t* Port,uint8_t* CMD_Num)
{
  if(NetMode == Net_Offline)                                                  //���������ģʽ��������罨��SOCKET
  {
//    CIPSTART_Flag &= ~0x01;
    CMD_OtherCount = fail;
    (*CMD_Num)++;       
  }
  else                                                                          //����ģʽ
  {
    StartJoinCmd(AT_CIPSTART,channel,Net,IP,Port);
//    CIPSTART_Flag |= 0x01;
    if(SendCMDProcess(AT_CIPSTART,strlen((const char*)AT_CIPSTART),&CMD_ReturnOK,1000,3,CMD_Type))       //�ش�ʱ����С��λΪ5ms
    {
//      CIPSTART_Flag &= ~0x01;
      CMD_OtherCount = fail;
      (*CMD_Num)++;    
      memset(&AT_CIPSTART[12],0,sizeof(AT_CIPSTART)-12);
      
      if(channel == 0xff)
        WifiClientModeFinishFlag |= 0x01;
    }
  }
}

/***********************************************************
* �������ܣ��鿴�ַ�����ָ���ַ�����������
* ����������string �ַ��� sign ָ�����ַ�
* ����ֵ�������ֽ�����
***********************************************************/
uint8_t TheNumOfSign(uint8_t* string,uint8_t sign)
{
  uint8_t i = 0;
  uint8_t num = 0;
  while(string[i])
  {
    if(string[i] == sign)
    {
      num++;
    }
    i++;
    if(i >= 50)
      return fail;
  }
  return num;
}

/***********************************************************
* �������ܣ������ַ���ת��Ϊ�ַ���
* ����������string �ַ�������
* ����ֵ�������ַ�
***********************************************************/
uint8_t String2Num(uint8_t* String)
{
  uint8_t L = 0;
  L = strlen((const char *)String);
  switch(L)
  {
    case 1:     return String[0] - 0x30;
    case 2:     return (String[0] - 0x30)*10 + (String[1] - 0x30);
    case 3:     return (String[0] - 0x30)*100 + (String[1] - 0x30)*10 + (String[2] - 0x30);
    default:    return fail;
  }
}

/***********************************************************
* �������ܣ�WIFI���յ�������Ϣ
* ������������
* ����ֵ����
struct RxNetDataUnit  RNDU[MaxNum]; �������ݵĽṹ������
***********************************************************/
void WifiReceiveNetData(void)
{
  if(WifiReDataFlag)
  {
    WifiReDataFlag &= ~0x01;
    
    if(TheNumOfSign(WifiReNetData,',') > 1)
    {     
      memcpy(WifiReNetChannel,&WifiReNetData[LocationOfSign(WifiReNetData,',',1)+1],LocationOfSign(WifiReNetData,',',2) - LocationOfSign(WifiReNetData,',',1) - 1);
      RNDU.RxChannel = WifiReNetChannel[0];
      memcpy(WifiReNetDataLength,&WifiReNetData[LocationOfSign(WifiReNetData,',',2)+1],LocationOfSign(WifiReNetData,':',1) - LocationOfSign(WifiReNetData,',',2) - 1);
      RNDU.RxLength = String2Num(WifiReNetDataLength);
      memcpy(RNDU.RxData,&WifiReNetData[LocationOfSign(WifiReNetData,':',1)+1],RNDU.RxLength);
    }
    else 
    {
      RNDU.RxChannel = error;
      memcpy(WifiReNetDataLength,&WifiReNetData[LocationOfSign(WifiReNetData,',',1)+1],LocationOfSign(WifiReNetData,':',1) - LocationOfSign(WifiReNetData,',',1) - 1);
      RNDU.RxLength = String2Num(WifiReNetDataLength);
      memcpy(RNDU.RxData,&WifiReNetData[LocationOfSign(WifiReNetData,':',1)+1],RNDU.RxLength);
    }
    if (memcmp(RNDU.RxData,"D1=ON",strlen("D1=ON")) == 0) //D1=ON   ��D1��
    {
      D1_on();
    }
    if (memcmp(RNDU.RxData,"D1=OFF",strlen("D1=OFF")) == 0) //D1=OFF  ��D1��
    {
      D1_off();
    }
    
    memset(&RNDU.RxChannel,0,sizeof(RNDU));             //������ջ�����
  }
}

/***********************************************************
* �������ܣ��Ӷ�Ӧ�Ľṹ���в�ѯIP��Ӧ��ͨ����
* ����������IP IP��ַ Struct ��Ӧ�Ľṹ��
* ����ֵ��ͨ���� error ����
***********************************************************/
uint8_t CheckChannelFromIP(uint8_t* IP,NetConnectUnit* Struct)
{
  uint8_t i = 0;
  while(Struct[i].Channel)
  {
    if(memcmp(Struct[i].ConnectIP,IP,strlen((const char*)IP)) == 0)
      return Struct[i].Channel-0x30;
    i++;
  }
  return error;
}

/***********************************************************
* �������ܣ�����ƴ�Ӻ���
* ����������CMD     ���� 
            Leng    ����      

            Channel ͨ���� ��������Ч  0xff ������
* ����ֵ����
***********************************************************/
void SendJoinCMD(uint8_t* CMD,uint8_t Leng,uint8_t Channel)
{
  uint8_t LengArry[5] = {0};
  uint8_t One = 0;
  uint8_t Ten = 0;
  uint8_t Hun = 0;
  uint8_t enter[] = {"\r\n"};
  if(!CMD_OtherCount)
  {
    CMD_OtherCount |= 0x01;
    
    memset(&CMD[LocationOfSign(CMD,'=',1)+1],0,strlen((const char*)CMD)-LocationOfSign(CMD,'=',1)-1);
    
    Hun = Leng/100;
    Ten = Leng/10%10;
    One = Leng%10;
    if(Hun > 0)
    {
      LengArry[0] = Hun+0x30;
      LengArry[1] = Ten+0x30;
      LengArry[2] = One+0x30;
    }
    else if(Ten > 0)
    {
      LengArry[0] = Ten+0x30;
      LengArry[1] = One+0x30;
    }
    else if(One > 0)
    {
      LengArry[0] = One+0x30;
    }
    
    if(Channel != 0xff)
    { 
      Channel += 0x30;
      strcat((char *)CMD,(const char *)&Channel);
      strcat((char *)CMD,(const char *)",");
    }
    strcat((char *)CMD,(const char *)&LengArry);
    
    strcat((char *)CMD,(const char *)&enter);
  }
}

/***********************************************************
* �������ܣ���������
* ����������Channel ͨ���� ��������Ч  0xff ������
            Leng �������ݳ���
* ����ֵ����
***********************************************************/
void CMD_CIPSEND(uint8_t Channel,uint8_t Leng,uint8_t* CMD_Num)
{
  SendJoinCMD(AT_CIPSEND,Leng,Channel);

  if(SendCMDProcess(AT_CIPSEND,strlen((const char*)AT_CIPSEND),&CMD_ReturnSign,200,3,CMD_Type))       //�ش�ʱ����С��λΪ5ms
  {
    (*CMD_Num)++;
    memset(&AT_CIPSEND[11],0,sizeof(AT_CIPSEND)-11);
    CMD_OtherCount = fail;
  }
}

/***********************************************************
* �������ܣ���������
* ����������Data ���͵�����
* ����ֵ����
***********************************************************/
void DataSend(uint8_t *Data,uint8_t Length,uint8_t* CMD_Num)
{
  if(SendCMDProcess(Data,Length,&CMD_ReturnSendOK,200,3,NetData_Type))       //�ش�ʱ����С��λΪ5ms
  {
    *CMD_Num = 0;
    TNDU.TxDataFinish &= ~0x01;
    memset(TNDU.TxIP,0,sizeof(TNDU));
    WifiTxDataFlag &= ~0x01;
  }
}

/***********************************************************
* �������ܣ���������
* ����������Channel ͨ���� ��������Ч  0xff ������
            Data ���͵�����
* ����ֵ����
***********************************************************/
void WifiSend(uint8_t Channel,uint8_t *Data,uint8_t Length)
{
  switch(WifiSendDataNum)
  {
    case 0:     CMD_CIPSEND(Channel,Length,&WifiSendDataNum);   break;
    case 1:     DataSend(Data,Length,&WifiSendDataNum);         break;
    default:                                                    break;
  }
}

/***********************************************************
* �������ܣ���������
* ����������IP IP��ַ
            Data ���͵�����
* ����ֵ����
***********************************************************/
void WifiSendDataFromIP(uint8_t* IP,uint8_t* Data,uint8_t Length)
{
  uint8_t result = 0;
  if(!WifiClientModeFinishFlag)                  //�ǿͻ���ģʽ
    result = CheckChannelFromIP(IP,NCU);
  else
    result = 0xff;
  
  if(result != error)                           //Ŀ��IP�� IP����
    WifiSend(result,Data,Length);
  else                                          //Ŀ��IP���� IP����
  {
    TNDU.TxDataFinish &= ~0x01;
    memset(TNDU.TxIP,0,sizeof(TNDU));
    WifiTxDataFlag &= ~0x01;
  }
}

/***********************************************************
* �������ܣ�WIFI����������Ϣ
* ������������
* ����ֵ����
struct TxNetDataUnit  TNDU;  �������ݵĽṹ������
***********************************************************/
void WifiSendNetData(void)
{
  if(WifiTxDataFlag)
  {
    WifiSendDataFromIP(TNDU.TxIP,TNDU.TxData,TNDU.TxLength);  
  }
}

/***********************************************************
* �������ܣ��������ݺͽ������ݵĲ���
* ������������
* ����ֵ����
***********************************************************/
void RxTxDataOperation(void)
{
  WifiReceiveNetData();                         //wifi����������ȡ
  WifiSendNetData();                            //WIFI������������
}

/***********************************************************
* �������ܣ��ͻ��˵Ĳ���
* ������������
* ����ֵ����
      ע����Ҫ����ʵ�ֶ����ӣ���������IP������TCP��
***********************************************************/
void ClientRxTxData(void)
{
  RxTxDataOperation();
}

/***********************************************************
* �������ܣ�����Ϊ�ͻ���
* ������������
* ����ֵ����
      ע����Ҫ����ʵ�ֶ����ӣ���������IP������TCP��
***********************************************************/
void ClientOperation(void)
{
  switch(CMD_CountNum)
  {
    case 0:     CMD_CIPMUX(0,&CMD_CountNum);                                    break;          //��������ģʽΪ�����ӻ������
    case 1:     CMD_CIPSTART(0xff,Net,Target_IP,Target_Port,&CMD_CountNum);     break;          //������·��ͨ��
    case 2:     WifiSendCmd("AT+CIPMODE=1\r\n",strlen("AT+CIPMODE=1\r\n"));delay_ms(300);
                WifiSendCmd("AT+CIPSEND\r\n",strlen("AT+CIPSEND\r\n"));delay_ms(300);
                WifiConfigFlag = 1;break;//ServerClientRxTxData();                                         break;          //�շ�����
    default:                                                                    break;
  }
}

/***********************************************************
* �������ܣ��˿�ƴ�Ӻ���
* ����������CMD     ���� 
            Port    �˿ں�      

            Num     ����״̬
            0       �ر�
            1       ����
* ����ֵ����
***********************************************************/
void PortJoinCMD(uint8_t* CMD,uint8_t* Port,uint8_t Num)
{
  uint8_t enter[] = {"\r\n"};
  if(!CMD_OtherCount)
  {
    CMD_OtherCount |= 0x01;
    
    memset(&CMD[LocationOfSign(CMD,'=',1)+1],0,strlen((const char*)CMD)-LocationOfSign(CMD,'=',1)-1);
    
    switch(Num)
    {
      case 0:   strcat((char *)CMD,(const char *)"0");  break;
      case 1:   strcat((char *)CMD,(const char *)"1");  break;
      default:                                            break;
    }
    strcat((char *)CMD,(const char *)",");
    strcat((char *)CMD,(const char *)Port);
    strcat((char *)CMD,(const char *)&enter);
  }
}

/***********************************************************
* �������ܣ��Ƿ�������� �˿�
* ����������Num 
            0  �ر�   
            1  ����
            Port �˿ں�
* ����ֵ����
***********************************************************/
void CMD_CIPSERVER(uint8_t Num,uint8_t *Port,uint8_t* CMD_Num)
{
  PortJoinCMD(AT_CIPSERVER,Port,Num);
  if(SendCMDProcess(AT_CIPSERVER,strlen((const char*)AT_CIPSERVER),&CMD_ReturnOK,200,3,CMD_Type))       //�ش�ʱ����С��λΪ5ms
  {
    CMD_OtherCount = fail;
    (*CMD_Num)++; 
    if(NetMode == Net_Offline)                                                  //���������ģʽ��������罨��SOCKET
      (*CMD_Num)++;    
    WifiServerModeFinishFlag |= 0x01;
    memset(&AT_CIPSERVER[13],0,sizeof(AT_CIPSERVER)-13);
  } 
}

/***********************************************************
* �������ܣ���ȡ����״̬
* ������������
* ����ֵ����
***********************************************************/
void CMD_CIPSTATUS(uint8_t* CMD_Num)
{
  if(SendCMDProcess(AT_CIPSTATUS,strlen((const char*)AT_CIPSTATUS),&CMD_ReturnOK,200,3,CMD_Type))           //�ش�ʱ����С��λΪ5ms
    (*CMD_Num)++;
}

/***********************************************************
* �������ܣ�����˿ͻ��˵��շ�����
* ������������
* ����ֵ����
***********************************************************/
void ServerClientRxTxData(void)
{
  switch(WifiCCCNum)
  {
    case 0:     CMD_CIPSTATUS(&WifiCCCNum);     break;          //��ȡ��ǰģ�������Ӻͱ����ӵ���Ϣ ����IP�Ͷ˿�
    case 1:     RxTxDataOperation();            break;          //�շ�����
    default:                                    break;
  }
}

/***********************************************************
* �������ܣ�����Ϊ�����
* ������������
* ����ֵ����
      ע����Ҫ����ʵ�ֶ����ӣ���������IP������TCP��
***********************************************************/
void ServerOperation(void)
{
  switch(CMD_CountNum)
  {
    case 0:     CMD_CIPMUX(1,&CMD_CountNum);                                    break;          //������·����ģʽΪ�����ӻ������
    case 1:     CMD_CIPSERVER(1,Self_Port,&CMD_CountNum);                       break;          //��������˿�
    case 2:     CMD_CIPSTART(0X31,Net,Target_IP,Target_Port,&CMD_CountNum);     break;          //��������ͨ��
    case 3:     ServerClientRxTxData();                                         break;          //�շ�����
    default:                                                                    break;
  }
  
}

/***********************************************************
* �������ܣ�����ģ����ģʽ
* ����������WifiChildState ģ����ģʽ  
            C_Client �ͻ���ģʽ  
            C_Server �����ģʽ  
* ����ֵ����
* ��ע�� ��ʱȥ���״̬����ʱʱ��Ϊ300ms
         ��Ϊ�ͻ���ʱ�����������ӣ�����TCP����
         ��Ϊ�����ʱ�����������ӣ���������˿�
***********************************************************/
void WIFIChildMode(WifiChildState C_Mode)
{
  if(WifiModeSetDown)                     //250ms
  {
    switch(C_Mode)
    {
      case C_Client:    ClientOperation();      break;
      case C_Server:    ServerOperation();      break;
      default:                                  break;
    }
  }
}

/***********************************************************
* �������ܣ��ԱȺ���
* ����������Res ���Աȵ��ַ���  Des �Ա��ַ���
* ����ֵ��fail ��ƥ��  finish ƥ�� 
***********************************************************/
uint8_t ContrastData(uint8_t *Res,uint8_t *Des)
{
  uint8_t i=0;
  for (i=0;i<sizeof(USART_Unit[3].USART_RxEffecDataBuf); i++)
  {
    if (memcmp(&Res[i],Des,strlen((const char *)Des)) == 0) 
    {
      WifiUartSignLoc = i + strlen((const char *)Des);
      WifiUartReLeng = USART_Unit[3].USART_RxLength - WifiUartSignLoc;
      memset(&Res[i], 0, strlen((const char *)Des));              //ֻ���Աȵ����������
      return success;
    }
  }           
  return fail;
}

/***********************************************************
* �������ܣ��������״̬��
* ������������
* ����ֵ����
struct NetConnectUnit NCU[5];
***********************************************************/
void RefreshConnectState(NetConnectUnit* List)
{
  uint8_t i = 0;
  while(List[i].Channel)
  {
    memset(&List[i].Channel,0,sizeof(List[i]));
    i++;
  }
}

/***********************************************************
* �������ܣ��鿴���ַ�����ָ���Ķ��ַ�������������
* ����������string ���ַ��� sign ָ���Ķ��ַ���
* ����ֵ����������
***********************************************************/
uint8_t TheNumOfShortString(uint8_t* string,uint8_t* ShortString)
{
  uint8_t i = 0;
  uint8_t Count = 0;
  for(i = 0;i < strlen((const char*)string);i++)
  {
    if(memcmp(&string[i],ShortString,strlen((const char *)ShortString)) == 0)
    {
      Count++;
    }
  }
  return Count;
}

/***********************************************************
* �������ܣ�������ȡ
* ����������Loc��ʼλ��  EndLoc ��ֹλ��  String Դ�ַ���  Data �õ����ַ���
* ����ֵ����
***********************************************************/
void GetString(uint8_t Loc,uint8_t EndLoc,uint8_t* String,uint8_t* Data)
{
  memcpy(Data,&String[Loc],EndLoc);
}

/***********************************************************
* �������ܣ��鿴���ַ�����ָ�����ַ������ڵ�λ��
* ����������string ���ַ��� shortString ָ���Ķ��ַ���  num �ڼ���
* ����ֵ�����ַ�����������һ���ַ���λ��
***********************************************************/
uint8_t LocationOfShortString(uint8_t* string,uint8_t* ShortString,uint8_t Num)
{
  uint8_t i = 0,Count = 0,Loc = 0;;
  for(i = 0;i < strlen((const char*)string);i++)
  {
    if(memcmp(&string[i],ShortString,strlen((const char *)ShortString)) == 0)
    {
      Count++;
      if(Count == Num)
        Loc = i+strlen((const char *)ShortString);
    }
  }
  return Loc;
}

/***********************************************************
* �������ܣ����ݸ�ֵ���ض��Ľṹ��
* ����������Struct Ŀ��ṹ��  Data ��Ҫ����ȡ������
* ����ֵ����
***********************************************************/
void SturctGetData(NetConnectUnit* Struct,uint8_t* Data)
{
  uint8_t Buf[5] = {0};
  uint8_t IPBuf[20] = {0};
  memcpy(Buf,Data,LocationOfSign(Data,',',1));
  Struct[0].Channel = Buf[0];                     //ͨ����
//  memcpy(Buf,&Data[LocationOfSign(Data,',',4)],strlen((const char *)Data));
//  Struct[0].ConnectState = String2Num(Buf);                //����״̬
  Struct[0].ConnectState |= 0x01;
  memcpy(IPBuf,&Data[LocationOfSign(Data,'"',3)+1],LocationOfSign(Data,'"',4) - LocationOfSign(Data,'"',3) - 1);
  memcpy(Struct[0].ConnectIP,IPBuf,strlen((const char*)IPBuf));                             //IP
}

/***********************************************************
* �������ܣ�����WIFI����״̬��
* ����������List ����״̬������
* ����ֵ����
***********************************************************/
void UpdateConnectState(uint8_t* List)
{
  uint8_t i = 0,Num = 0,j = 0,k = 0;
  uint8_t DataBuf[300] = {0};
  RefreshConnectState(NCU);
  Num = TheNumOfShortString(List,"+CIPSTATUS:");
  for(i = 0;i < Num;i++)
  {
    GetString(LocationOfShortString(List,"+CIPSTATUS:",i+1),LocationOfShortString(List,"\r\n",i+1)-3,List,DataBuf);
    while(NCU[j].Channel)               //����Ƿ����ظ���IP
    {
      if(memcmp(NCU[j].ConnectIP,&DataBuf[LocationOfSign(DataBuf,'"',3)+1],LocationOfSign(DataBuf,'"',4) - LocationOfSign(DataBuf,'"',3) - 1) == 0)//IP�ظ�
      {
        k++;
        SturctGetData(&NCU[j],DataBuf); //����ԭIP��Ϣ
      }
      j++;
    }
    j = 0;
    if(!k)              //û���ظ���IP
      SturctGetData(&NCU[i],DataBuf);
  }
}

/***********************************************************
* �������ܣ�WIFI���ڽ��ܵ����ݺ�����ݴ���
* ������������
* ����ֵ����
***********************************************************/
void Usart_DataProcessing(UsartType* USART)
{
  uint8_t WifiListMesg[255] = {0};                       //WIFI����״̬��
  
  if(USART->USART_DataFinishFlag)
  {
    USART->USART_DataFinishFlag &= ~0x01;
    if(ContrastData(USART->USART_RxEffecDataBuf,"\r\n>"))
    {
      CMD_ReturnSign = success;
    }
    if(ContrastData(USART->USART_RxEffecDataBuf,"SEND OK\r\n"))
    {
      CMD_ReturnSendOK = success;
    }
    if(ContrastData(USART->USART_RxEffecDataBuf,"+IPD"))//�յ�������λWifiReDataFlag��־λ
    {
      memcpy(WifiReNetData,&USART_Unit[3].USART_RxEffecDataBuf[WifiUartSignLoc],WifiUartReLeng);
      WifiReDataFlag = success;
    }
    if(ContrastData(USART->USART_RxEffecDataBuf,"ready\r\n")||ContrastData(USART->USART_RxEffecDataBuf,"invalid\r\n"))           //��λ
    {
      CMD_ReturnReady = success;
      WifiModeSetDown &= ~0xff;
    }
    
    if((WifiServerModeFinishFlag || WifiClientModeFinishFlag))
    {
      if(ContrastData(USART->USART_RxEffecDataBuf,"CONNECT\r\n") || ContrastData(USART->USART_RxEffecDataBuf,"CLOSED\r\n") || ContrastData(USART->USART_RxEffecDataBuf,"CONNECT FAIL\r\n"))     //���豸����
      {
        WifiCCCNum = fail;      //���ȼ������㣬���Ͳ�ѯ��������״̬����
      }
      if(ContrastData(USART->USART_RxEffecDataBuf,"STATUS:3\r\n"))                                 //��ѯ��������״̬�ķ���ֵ  3��ʾ�����ӻ�����    4��ǰ������
      {
        NoneEquipOnlineFlag &= ~0x01;
        memcpy(WifiListMesg,&USART->USART_RxEffecDataBuf[WifiUartSignLoc],WifiUartReLeng);//��ȡ�豸��Ϣ  ����IP ͨ����
        UpdateConnectState(WifiListMesg); 
      }
      else if(ContrastData(USART->USART_RxEffecDataBuf,"STATUS:4\r\n"))                         //��ѯ��������״̬�ķ���ֵ  3��ʾ�����ӻ�����    4��ǰ������
      {
        CMD_CountNum = 1;               //�����ǰû�����Ӻͱ������豸��������ת�������˿ڵ�λ��
        NoneEquipOnlineFlag |= 0x01;
        CMD_Count &= ~0x01;
      }
    }
    
    if(ContrastData(USART->USART_RxEffecDataBuf,"\r\nOK\r\n"))
    {
      if(!CMD_RSTFlag)                                  //�Ǹ�λ����
        CMD_ReturnOK = success;
    }
  }
}

/***********************************************************
* �������ܣ�WIFI���ڽ��ܺʹ������ݺ���
* ������������
* ����ֵ����
***********************************************************/
void Esp8266_Receive(void)
{
  USART_Unit_ReceiveData(&USART_Unit[3]);                         //WIFI����
  
  Usart_DataProcessing(&USART_Unit[3]);                      //WIFI���ݴ���
}

/***********************************************************
* �������ܣ�WIFI����ز���
* ������������
* ����ֵ����
***********************************************************/
void Wifi_Operation(void)
{
  Esp8266_Receive();
  
  WIFIMode(ESP8266_STA);
    
  WIFIChildMode(C_Client);
}