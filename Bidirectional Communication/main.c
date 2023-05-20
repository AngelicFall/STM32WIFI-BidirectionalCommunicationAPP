#include "handle.h"
#include "shtxx.h"
#include "LQ12864.h"
#include "wifi.h"
/***********************************************************
* ����: main()
* ����: ������
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
int WifiConfigFlag = 0;
void main(void)
{
  //int count = 300;
   uint8_t Error, checksum;
  uint16_t humi_val, temp_val;
  char temp[16] = {0},humi[16] = {0};
  float f_humi, f_temp;
  EquipInit();
  //Wifi_Operation();                           //WIFI��ز���
  for(;;)
  {
    if(!WifiConfigFlag)
    {
      Wifi_Operation();                           //WIFI��ز���
    
      USART_Operation();                          //������ز���
    
      NetStateMonitor();                          //�����쳣��� 
    }
    else
    {
     Error = 0;
    Error += Measure((unsigned char*) &temp_val,&checksum,TEMP);  //�����¶�
    Error += Measure((unsigned char*) &humi_val,&checksum,HUMI);  //����ʪ��
    
    if (Error == 0) 
    {
      //ͨ����ʽ����ȡ����ʪ�ȵ�ֵת���ɿ�ʶ���ֵ
      SHTXX_Cal(&temp_val, &humi_val, &f_temp, &f_humi);
      sprintf(temp, "Temp = %.1fC", f_temp);
      sprintf(humi, "Humi = %.1f%%", f_humi);
      
      OLED_Clear_Line(OLED_Line_3);
      OLED_Clear_Line(OLED_Line_4);
      OLED_Display(OLED_Line_3, temp);   //OLED����ӡ��ʪ����Ϣ
      OLED_Display(OLED_Line_4, humi);
      sprintf(TNDU.TxData,"%s,%s\n",temp,humi);
      WifiSendCmd(TNDU.TxData,strlen(TNDU.TxData));
     // printf("Temp = %.1f�� , Humi = %.1f%%\r\n", f_temp, f_humi);  //���ڴ�ӡ��ʪ����Ϣ
    }
    delay_ms(2000);
    }
  }
  
}