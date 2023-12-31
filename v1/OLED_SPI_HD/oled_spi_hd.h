//
// Created by LMTX on 2023/8/19.
//

#ifndef V1_OLED_SPI_HD_H
#define V1_OLED_SPI_HD_H

#include "main.h"

#define SIZE 12
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	64

//-----------------测试LED端口定义----------------
#define OLED_RST_Pin		GPIO_PIN_4
#define OLED_DC_Pin			GPIO_PIN_6
#define OLED_CS_Pin			GPIO_PIN_0

#define OLED_RST_GPIO		GPIOA
#define OLED_DC_GPIO		GPIOA
#define OLED_CS_GPIO		GPIOB

//-----------------OLED端口定义----------------


#define OLED_RST_Clr() 	HAL_GPIO_WritePin(OLED_RST_GPIO,OLED_RST_Pin,GPIO_PIN_RESET)//RES
#define OLED_RST_Set() 	HAL_GPIO_WritePin(OLED_RST_GPIO,OLED_RST_Pin,GPIO_PIN_SET)

#define OLED_DC_Clr() 	HAL_GPIO_WritePin(OLED_DC_GPIO,OLED_DC_Pin,GPIO_PIN_RESET)//DC
#define OLED_DC_Set() 	HAL_GPIO_WritePin(OLED_DC_GPIO,OLED_DC_Pin,GPIO_PIN_SET)


#define OLED_CS_Clr()  	HAL_GPIO_WritePin(OLED_CS_GPIO,OLED_CS_Pin,GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  	HAL_GPIO_WritePin(OLED_CS_GPIO,OLED_CS_Pin,GPIO_PIN_SET)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);





#endif //V1_OLED_SPI_HD_H
