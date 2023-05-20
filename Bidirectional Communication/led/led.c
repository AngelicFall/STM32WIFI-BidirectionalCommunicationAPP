/***********************************************************
***********************************************************/
#include "led.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



/***********************************************************
* ����: leds_init()
* ����: ��ʼ��led��GPIOʱ�ӣ�����ʼ��Ϊ���ģʽ
* ����: ��
* ����: ��
* �޸�:
* ע��: 1) ��ʹ��stm32��ledǰ�������ȵ��������ʼ������
***********************************************************/
void leds_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* ��ʼ��GPIOʱ�� */
  RCC_APB2PeriphClockCmd(D1_GPIO_ACC | D2_GPIO_ACC, ENABLE);

  /* ����LED������Ϊ������� */
  GPIO_InitStructure.GPIO_Pin = D1_GPIO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(D1_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = D2_GPIO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(D2_GPIO_PORT, &GPIO_InitStructure);
  
  //D1��D2Ĭ�Ϲر�
  D1_off();
  D2_off(); 
}

/***********************************************************
* ����: D1_off()
* ����: Ϩ��D1
* ����: ��
* ����: ��
* �޸�:
* ע��:
***********************************************************/
void D1_off(void)
{
  GPIO_SetBits(D1_GPIO_PORT, D1_GPIO_PIN);
}

/***********************************************************
* ����: D2_off()
* ����: Ϩ��D2
* ����: ��
* ����: ��
* �޸�:
* ע��:
***********************************************************/
void D2_off(void)
{
  GPIO_SetBits(D2_GPIO_PORT, D2_GPIO_PIN);
}

/***********************************************************
* ����: D1_on()
* ����: ����D1
* ����: ��
* ����: ��
* �޸�:
* ע��:
***********************************************************/
void D1_on(void)
{
  GPIO_ResetBits(D1_GPIO_PORT, D1_GPIO_PIN);
}

/***********************************************************
* ����: D2_on()
* ����: ����D2
* ����: ��
* ����: ��
* �޸�:
* ע��:
***********************************************************/
void D2_on(void)
{
  GPIO_ResetBits(D2_GPIO_PORT, D2_GPIO_PIN);
}

/***********************************************************
* ����: D1_toggle()
* ����: ��תD1
* ����: ��
* ����: ��
* �޸�:
* ע��:
***********************************************************/
void D1_toggle(void)
{
  GPIO_WriteBit(D1_GPIO_PORT, D1_GPIO_PIN, (BitAction)!GPIO_ReadOutputDataBit(D1_GPIO_PORT, D1_GPIO_PIN));
}

/***********************************************************
* ����: D2_toggle()
* ����: ��תD2
* ����: ��
* ����: ��
* �޸�:
* ע��:
***********************************************************/
void D2_toggle(void)
{
  GPIO_WriteBit(D2_GPIO_PORT, D2_GPIO_PIN, (BitAction)!GPIO_ReadOutputDataBit(D2_GPIO_PORT, D2_GPIO_PIN));
}
