/***********************************************************
***********************************************************/

#ifndef LEDS_H
#define LEDS_H

#define D1 1 
#define D2 2

//Ϊ�˿���ֲ�ԣ� ��LED�����ţ��˿ڣ�ʼ�ն�������
#define D1_GPIO_PIN GPIO_Pin_1  
#define D2_GPIO_PIN GPIO_Pin_0
#define D1_GPIO_PORT GPIOA
#define D2_GPIO_PORT GPIOA
#define D1_GPIO_ACC RCC_APB2Periph_GPIOA
#define D2_GPIO_ACC RCC_APB2Periph_GPIOA

void leds_init(void);
void D1_on(void);
void D2_on(void);
void D1_off(void);
void D2_off(void);
void D1_toggle(void);
void D2_toggle(void);
#endif