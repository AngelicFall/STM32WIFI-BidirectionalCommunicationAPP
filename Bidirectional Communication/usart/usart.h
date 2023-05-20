#ifndef _USART_H_
#define _USART_H_

#include "handle.h"

#define USART_MaxNum 6

#define UartRxMaxLength 255

typedef struct{
  uint8_t USART_Receiving;                      //�������ڽ������ݱ�־
  uint8_t USART_RxCount;                        //���ڽ������ݼ���
  uint8_t USART_ANum;                           //����ȷ�����ڽ��������Ƿ����
  uint8_t USART_DataFinishFlag;                 //���ڽ�����ɱ�ʾ
  uint8_t USART_RxLength;                       //���ڽ��ܵ����ݵĳ���
  uint8_t USART_RxBuf[UartRxMaxLength];         //���ڽ��ܵ�����Ч����
  uint8_t USART_RxEffecDataBuf[UartRxMaxLength];//�Ӵ���BUF����ȡ������Ч����
}UsartType;

void Initial_USART1(u32 baudrate);
void Initial_USART2(u32 baudrate);
void Initial_USART3(u32 baudrate);
void Initial_UART4(u32 baudrate);
void Initial_UART5(u32 baudrate);

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);
void Usart_SendNumString(USART_TypeDef * pUSARTx, uint8_t *str, uint8_t Num);
void UART_NVIC_Configuration(void);

void USART_Unit_ReceiveData(UsartType* USART);

extern uint8_t USART_TimCount;                                 //����1���ռ���
extern UsartType USART_Unit[USART_MaxNum];

#endif

