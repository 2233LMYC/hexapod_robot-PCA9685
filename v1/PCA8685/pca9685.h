#ifndef _PCA9685_H
#define _PCA9685_H

#include "stdbool.h"
#include "main.h"
#include "i2c.h"

// I2C 句柄，用户自己修改
#define PCA9685_HI2C	hi2c2
// I2C 地址
//#define PCA96851_IIC_ADDR_W	0x80
//#define PCA96851_IIC_ADDR_R	0x81
// I2C 地址
#define PCA96851_IIC_ADDR_W	0x82
#define PCA96851_IIC_ADDR_R	0x83

// 寄存器地址
#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD



// 函数
void pca9685_reset(void);
void pca9685_setPWMFreq(float freq);
void pca9685_setPWM(uint8_t num, uint16_t on, uint16_t off);

// 初始化。频率，转动角度（-90~90）
void pca9685_init(float hz, int16_t angle);
// num:序号；angle:角度（-90~90）
void setAngle(uint8_t num,int16_t angle);

#endif	/* _PCA9685_PW_H */

