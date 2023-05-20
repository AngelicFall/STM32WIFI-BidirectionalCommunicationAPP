#include "key.h"

/***********************************************************
* �������ܣ���������
* ������������
* ����ֵ����
***********************************************************/
uint8_t Key1_KeyPushedFlag = fail;
uint8_t KeyPushedCount = fail;

/***********************************************************
* �������ܣ��������Ŷ���
* ������������
* ����ֵ����
***********************************************************/
uint8_t _Select_Key1(void){     
  return GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4);}           //����1    

/***********************************************************
* �������ܣ�������ʼ��
* ������������
* ����ֵ����
***********************************************************/
void Key_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* ��ʼ��GPIOʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}


/***********************************************************
* �������ܣ�����ɨ��
* ����������WhichKey ɨ�谴������   KeyPushedFlag ���±�־
* ����ֵ����
* ע��KeyPushedFlag ���±�־  ֻ�Ǳ�ʾ���������£�����һ���ϸ��
      ����������Ҫ��ⰴ��̧��󣬲���Ϊһ�������İ�������
***********************************************************/
uint8_t ScanKey(_Key WhichKey,uint8_t* KeyPushedFlag)
{
  if(!WhichKey() || (*KeyPushedFlag))
  {
    KeyPushedCount++;                                   //�μ����ڶ�ʱ��4���ж������  ����Ϊ1S
    if(KeyPushedCount > 1)                          //һ��ʱ����ӳ� ����
    {
      KeyPushedCount = 0;
      *KeyPushedFlag = success; 
    }
    if(WhichKey() && (*KeyPushedFlag))
    {
      KeyPushedCount = 0;
      *KeyPushedFlag = fail;
      return success;
    }
  }
  return fail;
}

/***********************************************************
* �������ܣ�����ɨ��
* ������������
* ����ֵ����
* ע��ֻ�е����ŵ�״̬�ı�󣬲Ż�ȥ�ж��л�ģʽ
      �����ڲ�ͣ�ļ�����ŵ�״̬��
***********************************************************/
void ScanKey_Operation(void)
{ 
  if(ScanKey(_Select_Key1,&Key1_KeyPushedFlag))
  {
    D2_toggle();
  }
}
