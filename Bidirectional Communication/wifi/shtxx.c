#include "shtxx.h"
#include "delay.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#define noACK 0
#define ACK   1

#define STATUS_REG_W 0x06   //000   0011    0
#define STATUS_REG_R 0x07   //000   0011    1
#define MEASURE_TEMP 0x03   //000   0001    1
#define MEASURE_HUMI 0x05   //000   0010    1

/***********************************************************
* ����: SHTXX_GPIOConfig()
* ����: SHTXX GPIO��ʼ��
* ����: ��
* ����: ��
* �޸�:
***********************************************************/
void SHTXX_GPIOConfig(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(SDA_GPIO_CLK | SCL_GPIO_CLK, ENABLE);
  //PB1 PB2
  GPIO_InitStructure.GPIO_Pin = SCL_GPIO_PIN | SDA_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStructure);
}

/***********************************************************
* ����: SDA_OUT()
* ����: SDA�������ó��������
* ����: ��
* ����: ��
* �޸�:
***********************************************************/
void SDA_OUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = SDA_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStructure);
}

/***********************************************************
* ����: SDA_IN()
* ����: SDA�������ó���������
* ����: ��
* ����: ��
* �޸�:
***********************************************************/
void SDA_IN(void )
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = SDA_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStructure);
}

/***********************************************************
* ����: Delay1us()
* ����: ��ʱ1us����
* ����: ���� ��Ҫ��ʱ��΢����
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
static void Delay1us(uint16_t t)
{
  uint8_t i, j;
  for (j = t; j > 0; j--)
    for (i = 10; i > 0; i--);
}

/***********************************************************
* ����: SHTXX_Init()
* ����: ��ʪ�ȴ�������ʼ������
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void SHTXX_Init(void)
{
  SHTXX_GPIOConfig();
}

/***********************************************************
* ����: ConnectionReset()
* ����: ��������
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void ConnectionReset(void)
{
  uint8_t i;
  SDA_OUT();
  Set_SHTXX_SDA();
  Delay1us(10);
  Clr_SHTXX_SCL();
  Delay1us(10);
  for(i = 0; i < 9; i++){
    Set_SHTXX_SCL();
    Delay1us(10);
    Clr_SHTXX_SCL();
    Delay1us(10);
  }
  TransStart();
}

/***********************************************************
* ����: WriteByte()
* ����: д��1�ֽ�����
* ����: ���� value ��Ҫд������������
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
static char WriteByte(unsigned char value)
{
  unsigned char i, Error = 0;
  SDA_OUT();
  for (i=0x80;i>0;i/=2)             //shift bit for masking
  { 
    if (i & value) 
      Set_SHTXX_SDA();          //masking value with i , write to SENSI-BUS
    else 
      Clr_SHTXX_SDA();
    Delay1us(10);
    Set_SHTXX_SCL();                          //clk for SENSI-BUS
    Delay1us(10);        //pulswith approx. 5 us  	
    Clr_SHTXX_SCL();
    Delay1us(10);
  }
  SDA_IN();
  Set_SHTXX_SDA();                           //release DATA-line
  Delay1us(10);
  Set_SHTXX_SCL();                            //clk #9 for ack 
  Delay1us(10);
  Error=READ_SDA();                       //check ack (DATA will be pulled down by SHT11)
  Clr_SHTXX_SCL();        
  return Error;                     //error=1 in case of no acknowledge
}


/***********************************************************
* ����: ReadByte()
* ����: ��ȡ1�ֽ�����
* ����: ���� ack ack=1��ʾ��Ҫ��MMA����ֹͣλ��Ϊ0���ʾ����
*       Ҫ��MMA����ֹͣλ
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
static char ReadByte(unsigned char ack)
{ 
  unsigned char i,val=0;
  
  Set_SHTXX_SDA();   //release DATA-line
  Delay1us(10);
  SDA_IN();
  for (i=0x80;i>0;i/=2)             //shift bit for masking
  { 
    Set_SHTXX_SCL();  //clk for SENSI-BUS
    Delay1us(10);
    if (READ_SDA()) val=(val | i);        //read bit  
    Clr_SHTXX_SCL();
    Delay1us(10);
  }
  SDA_OUT();
  WRITE_SDA((BitAction)!ack);                 //in case of "ack==1" pull down DATA-Line
  Delay1us(10);
  Set_SHTXX_SCL();   //clk #9 for ack
  Delay1us(10);
  Clr_SHTXX_SCL();
  Delay1us(10);
  Set_SHTXX_SDA();                           //release DATA-line
  Delay1us(10);
  return val;
}

/***********************************************************
* ����: TransStart()
* ����: ��ʼ��������
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
static void TransStart(void)
{
  SDA_OUT();
  Set_SHTXX_SDA();
  Delay1us(10);
  Clr_SHTXX_SCL();
  Delay1us(10);
  Set_SHTXX_SCL();
  Delay1us(10);
  Clr_SHTXX_SDA();
  Delay1us(10);
  Clr_SHTXX_SCL();
  Delay1us(10);
  Set_SHTXX_SCL();
  Delay1us(10);
  Set_SHTXX_SDA();
  Delay1us(10);
  Clr_SHTXX_SCL();
  Delay1us(10);
}

/***********************************************************
* ����: Measure()
* ����: �õ�������ֵ
* ����: ��� p_value ���������
��� p_checksum ���������У��ֵ
���� ģʽ ���¶�ģʽ��ʪ��ģʽ
* ����: �����ֵ 0Ϊ�޴���
* �޸�:
* ע��: 
***********************************************************/
uint8_t Measure(unsigned char *p_value, unsigned char *p_checksum, unsigned char mode)
{
  unsigned Error = 0, i = 0;
  
  ConnectionReset();
  switch(mode)
  {
  case TEMP: 
    Error = WriteByte(MEASURE_TEMP);break;
  case HUMI: 
    Error = WriteByte(MEASURE_HUMI);break;
  default: 
    break;
  }
  if(Error)  //д�����쳣
  {
    return 1;
  }
  
  SDA_IN(); 
  while(i++<400000 && READ_SDA());   //�����������߳�ʱ
  
  if(READ_SDA())  // ��ʱ
  {
    return 1;
  }
  *(p_value +1)  =ReadByte(ACK);    //����һ���ֽ� (MSB)
  *(p_value)=ReadByte(ACK);    //���ڶ����ֽ� (LSB)
  *p_checksum =ReadByte(noACK);  //��У��ֵ
  return 0;
}

/***********************************************************
* ����: SHTXX_Cal()
* ����: ת���õ�����ʪ�ȵ�ֵΪ��׼��λ��
* ����: ���� temp �õ����¶ȵ�ֵ
���� p_checksum �õ���ʪ�ȵ�ֵ
��� f_temp ת������¶ȵ�ֵ
��� f_humi ת������¶ȵ�ֵ
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void SHTXX_Cal(uint16_t *temp, uint16_t *humi, float *f_temp, float *f_humi )
{
  
  const float C1=-4.0;              // for 12 Bit
  const float C2=+0.0405;           // for 12 Bit
  const float C3=-0.0000028;        // for 12 Bit
  const float T1=+0.01;             // for 14 Bit @ 5V
  const float T2=+0.00008;           // for 14 Bit @ 5V	
  
  float rh;             // rh:      Humidity [Ticks] 12 Bit 
  float t;           // t:       Temperature [Ticks] 14 Bit
  float rh_lin;                     // rh_lin:  Humidity linear
  float rh_true;                    // rh_true: Temperature compensated humidity
  float t_C;                        // t_C   :  Temperature  
  
  rh = (float)(*humi);
  t = (float)(*temp);
  
  t_C=t*0.01 - 40;                  //calc. temperature from ticks 
  rh_lin=C3*rh*rh + C2*rh + C1;     //calc. humidity from ticks to [%RH]
  rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;   //calc. temperature compensated humidity [%RH]
  if(rh_true>100)rh_true=100;       //cut if the value is outside of
  if(rh_true<0.1)rh_true=0.1;       //the physical possible range
  
  *f_temp=t_C;               //return temperature 
  *f_humi=rh_true;              //return humidity[%RH]
  
}