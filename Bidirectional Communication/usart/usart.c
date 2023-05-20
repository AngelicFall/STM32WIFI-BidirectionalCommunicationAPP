#include "usart.h"

/*��������*/
uint8_t SpecialEffecDataFlag = 0;
/*��������*/

//ע��IARҪ���� _DLIB_FILE_DESCRIPTOR �궨��
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


UsartType USART_Unit[USART_MaxNum];

/***********************************************************
* ����: Initial_USART1()
* ����: ����1��ʼ�����򣬽����ڳ�ʼ��Ϊ
        - ������ = baudrate
        - �ֳ� = 8 λ
        - ��ֹͣλ
        - ����żУ��λ
        - ��ֹӲ��������
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void Initial_USART1(u32 baudrate)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

  /* ���ô���1 Tx��PA9��Ϊ���츴��ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ���ô���1 Rx��PA10��Ϊ��������ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ���ô���1 �ĸ��ֲ��� */
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
  /* ����1��ʼ���� */
  USART_Cmd(USART1, ENABLE);
  
  USART_ClearFlag(USART1,USART_FLAG_TC);
}

/***********************************************************
* ����: Initial_USART2()
* ����: ����2��ʼ�����򣬽����ڳ�ʼ��Ϊ
        - ������ = baudrate
        - �ֳ� = 8 λ
        - ��ֹͣλ
        - ����żУ��λ
        - ��ֹӲ��������
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void Initial_USART2(u32 baudrate)
{  
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /* ���ô���2 Tx��PA2��Ϊ���츴��ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ���ô���2 Rx��PA3��Ϊ��������ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ���ô���2 �ĸ��ֲ��� */
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART2, &USART_InitStructure);
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
  /* ����1��ʼ���� */
  USART_Cmd(USART2, ENABLE);
  
  USART_ClearFlag(USART2,USART_FLAG_TC);
}

/***********************************************************
* ����: Initial_USART3()
* ����: ����3��ʼ�����򣬽����ڳ�ʼ��Ϊ
        - ������ = baudrate
        - �ֳ� = 8 λ
        - ��ֹͣλ
        - ����żУ��λ
        - ��ֹӲ��������
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void Initial_USART3(u32 baudrate)
{  
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* ���ô���3 Tx��PB10��Ϊ���츴��ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* ���ô���3 Rx��PB11��Ϊ��������ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* ���ô���3 �ĸ��ֲ��� */
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART3, &USART_InitStructure);
  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
  /* ����3��ʼ���� */
  USART_Cmd(USART3, ENABLE);
  
  USART_ClearFlag(USART3,USART_FLAG_TC);
}

/***********************************************************
* ����: Initial_UART4()
* ����: ����4��ʼ�����򣬽����ڳ�ʼ��Ϊ
        - ������ = baudrate
        - �ֳ� = 8 λ
        - ��ֹͣλ
        - ����żУ��λ
        - ��ֹӲ��������
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void Initial_UART4(u32 baudrate)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOF, ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); //����USART4ʱ��

  /* ���ô���4 Tx��PC10��Ϊ���츴��ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* ���ô���4 Rx��PC11��Ϊ��������ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* ���ô���4 �ĸ��ֲ��� */
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(UART4, &USART_InitStructure);
  //ʹ��USART4�����ж�
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
  //ʹ��USART4
  USART_Cmd(UART4, ENABLE); 
  USART_ClearFlag(UART4,USART_FLAG_TC);
}

/***********************************************************
* ����: Initial_UART5()
* ����: ����5��ʼ�����򣬽����ڳ�ʼ��Ϊ
        - ������ = baudrate
        - �ֳ� = 8 λ
        - ��ֹͣλ
        - ����żУ��λ
        - ��ֹӲ��������
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void Initial_UART5(u32 baudrate)
{  
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

  /* ���ô���5 Tx��PC12��Ϊ���츴��ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* ���ô���5 Rx��PD2��Ϊ��������ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  /* ���ô���5 �ĸ��ֲ��� */
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(UART5, &USART_InitStructure);
  USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
  /* ����5��ʼ���� */
  USART_Cmd(UART5, ENABLE);
  USART_ClearFlag(UART5,USART_FLAG_TC);
}

/***********************************************************
* ����: USART_ReceiveData(UsartType USART_Unit* USART)
* ����: �жϴ��ڽ��������Ƿ����
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void USART_Unit_ReceiveData(UsartType* USART)
{
  if(USART_TimCount >= 2 && USART->USART_Receiving)                                     //10ms
  {  
    USART_TimCount = 0;
    if(USART->USART_ANum == USART->USART_RxCount)                                                 //���ݽ������
    {
      USART->USART_DataFinishFlag |= 0X01;
      USART->USART_Receiving &= ~0x01;                                                    //���ڽ������ݽ���
      USART->USART_RxLength = USART->USART_RxCount;                                   //��ȡ���յ����ݳ���
      USART->USART_RxCount &= ~0xff;
      
      memcpy(USART->USART_RxEffecDataBuf,&USART->USART_RxBuf,USART->USART_RxLength);
      
      Usart_SendNumString(USART1,USART->USART_RxEffecDataBuf,USART->USART_RxLength);
      
      memset(USART->USART_RxBuf,0,sizeof(USART->USART_RxBuf));//��ս��ջ�����
    }
    else
    {
      USART->USART_ANum = USART->USART_RxCount;
    }
  }
}

/***********************************************************
* ����: USART_Unit_Special_ReceiveData(UsartType USART_Unit* USART)
* ����: �жϴ��ڽ��������Ƿ����
* ����: ��
* ����: ��
* �޸�:
* ע��: ��Ϊ�˴ν��ܵ����ݱȽ����� ��ͨ�����ڴ����Ĳ�ͣ�Ĵ��䣬�����Ҫ����
        ���ܵ����������ж��Ƿ���յ���Ч����
***********************************************************/
void USART_Unit_Special_ReceiveData(UsartType* USART,USART_TypeDef* USARTx)
{
  uint8_t i = 0;
  
  if(USART->USART_Receiving)
  {
    for(i = 0;i < USART->USART_RxCount;i++)
    {
      if(USART->USART_RxBuf[i] == 0xff && (USART->USART_RxBuf[i+1] == 0xc0 || USART->USART_RxBuf[i+1] == 0xc7 || USART->USART_RxBuf[i+1] == 0xcb || USART->USART_RxBuf[i+1] == 0xcc))
      {
        SpecialEffecDataFlag |= 0x01;
        break;
      }
    }
    if(SpecialEffecDataFlag && USART->USART_RxBuf[i+2])
    {
      if(USART->USART_RxCount - i >= (USART->USART_RxBuf[i+2] + 2))
      {
        memcpy(USART->USART_RxEffecDataBuf,&USART->USART_RxBuf[i],USART->USART_RxBuf[i+2] + 2);
        SpecialEffecDataFlag &= ~0x01;
        
        USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);//�رմ����ж�
        USART->USART_Receiving &= ~0x01;
        USART->USART_DataFinishFlag |= 0x01;
        USART->USART_RxCount &= ~0xff;
        USART->USART_RxLength = USART->USART_RxBuf[i+2] + 2;
        memset(USART->USART_RxBuf,0,250);//��ս��ջ�����
      }
    }
  }
}

/***********************************************************
* ����: UART_NVIC_Configuration
* ����: �ж�����
* ����: ��
* ����: ��
* �޸�:
* ע��: ����ж��Լ������жϵ����ȼ�
***********************************************************/
void UART_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStruct;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //����
  NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStruct);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //
  NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStruct);
  
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //
//  NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
//  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//  
//  NVIC_Init(&NVIC_InitStruct);
//  
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //
//  NVIC_InitStruct.NVIC_IRQChannel = UART4_IRQn;
//  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//  
//  NVIC_Init(&NVIC_InitStruct);
//  
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //
//  NVIC_InitStruct.NVIC_IRQChannel = UART5_IRQn;
//  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//  
//  NVIC_Init(&NVIC_InitStruct);
}

/***********************************************************
* ����: PUTCHAR_PROTOTYPE
* ����: ���һ���ֽڵ�����
* ����: ��
* ����: ��
* �޸�:
* ע��: ���������iar��ʵ������putchar����(�ļ���ͷ����)��ʵ����putchar�������
�󣬱��������ܹ�ʹ��printf������
***********************************************************/
PUTCHAR_PROTOTYPE
{
  USART_SendData(USART2, (uint8_t) ch);
  
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
  {}
  
  return ch;
}

/***********************************************************
* ����: fgetc
* ����: �Ӵ��ڽ���һ���ֽ�
* ����: ��
* ����: ��
* �޸�:
* ע��: ֻҪʵ������������Ϳ���ʹ��scanf�ˡ�
***********************************************************/
int fgetc(FILE *f)
{

    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
    {}

    return (int)USART_ReceiveData(USART2);
}

/***********************************************************
* ����: Usart_SendByte()
* ����: ����һ���ַ�
* ����: pUSARTx ����  ch  ��Ҫ���͵��ַ�
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
  USART_SendData(pUSARTx,ch);
          
  while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/***********************************************************
* ����: Usart_SendString(USART_TypeDef * pUSARTx, uint8_t *str)
* ����: ����һ���ַ���
* ����: pUSARTx ����  ch  ��Ҫ���͵��ַ���
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
  unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      delay_us(200);
      k++;
  } while(*(str + k)!='\0');

  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/***********************************************************
* ����: Usart_SendNumString(USART_TypeDef * pUSARTx, uint8_t *str, uint8_t Num)
* ����: ���͹̶����ȵ��ַ���
* ����: pUSARTx ����  str  ��Ҫ���͵��ַ���  Num��Ҫ���͵ĳ���
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void Usart_SendNumString(USART_TypeDef * pUSARTx, uint8_t *str, uint8_t Num)           
{
  unsigned int k=0;
  for(k = 0;k < Num;k++)
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      delay_us(200);
  }
  
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/***********************************************************
* ����: USART_Unit_IRQ_ReceiveData(UsartType* USART,USART_TypeDef* USARTx)
* ����: �Ӵ����ж�����ȡ����  USARTx ���ں�
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void USART_Unit_IRQ_ReceiveData(UsartType* USART,USART_TypeDef* USARTx)
{
  uint8_t USART_RxData = 0;
  
  USART->USART_Receiving = 1; 
      
  USART_RxData = USART_ReceiveData(USARTx);
  
  if (USART->USART_RxCount < sizeof(USART->USART_RxBuf)-5)
  {
    USART->USART_RxBuf[USART->USART_RxCount++] = USART_RxData;
  }
  else 
  {
    USART->USART_RxCount = 0;
  }
}

/***********************************************************
* ����: USART1_IRQHandler()
* ����: USART1 �жϺ���
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void USART1_IRQHandler(void)        
{
  if(USART_GetFlagStatus(USART1,USART_IT_RXNE))   //�����ж� (���ռĴ����ǿ�) 
  {   
    USART_Unit_IRQ_ReceiveData(&USART_Unit[1],USART1); 
  } 
}

/***********************************************************
* ����: USART2_IRQHandler()
* ����: USART2 ESP8266�жϺ���
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void USART2_IRQHandler(void)       
{
  if(USART_GetFlagStatus(USART2,USART_IT_RXNE))   //�����ж� (���ռĴ����ǿ�) 
  {   
    USART_Unit_IRQ_ReceiveData(&USART_Unit[2],USART2);
  } 
}

/***********************************************************
* ����: USART3_IRQHandler()
* ����: USART3 �жϺ���
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void USART3_IRQHandler(void)      
{
  if(USART_GetFlagStatus(USART3,USART_IT_RXNE))   //�����ж� (���ռĴ����ǿ�) 
  {   
    USART_Unit_IRQ_ReceiveData(&USART_Unit[3],USART3);
  } 
}

/***********************************************************
* ����: UART4_IRQHandler()
* ����: UART4 �жϺ���
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void UART4_IRQHandler(void)        
{
  if(USART_GetFlagStatus(UART4,USART_IT_RXNE))   //�����ж� (���ռĴ����ǿ�) 
  {   
    USART_Unit_IRQ_ReceiveData(&USART_Unit[4],UART4);
  } 
}

/***********************************************************
* ����: UART5_IRQHandler()
* ����: UART5 �жϺ���
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void UART5_IRQHandler(void)        
{
  if(USART_GetFlagStatus(UART5,USART_IT_RXNE))   //�����ж� (���ռĴ����ǿ�) 
  {   
    USART_Unit_IRQ_ReceiveData(&USART_Unit[5],UART5);
  } 
}