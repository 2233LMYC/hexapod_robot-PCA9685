#ifndef __WS2818B_H
#define __WS2818B_H

#include "main.h"
#include "gpio.h"

#define 	RGB_LED 	GPIO_PIN_8
#define		RGB_LED_HIGH	(HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET))
#define 	RGB_LED_LOW		(HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET))

void RGB_LED_Init(void);
void RGB_LED_Write0(void);
void RGB_LED_Write1(void);
void RGB_LED_Reset(void);
void RGB_LED_Write_Byte(uint8_t byte);
void RGB_LED_Write_24Bits(uint8_t green,uint8_t red,uint8_t blue);
void RGB_LED_Red(void);
void RGB_LED_Green(void);
void RGB_LED_Blue	(void);



#endif
 