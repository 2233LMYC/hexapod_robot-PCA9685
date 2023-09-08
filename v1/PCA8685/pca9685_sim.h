//
// Created by LMTX on 2023/8/19.
//

#ifndef V1_PCA9685_SIM_H
#define V1_PCA9685_SIM_H

#include "main.h"


#define PCA9685_adrr 0x80
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

void pca9685_Write(unsigned char reg,unsigned char data);
uint8_t pca9685_Read(unsigned char reg);
void setPWMFreq(uint8_t freq);
void setPWM(uint8_t num, uint16_t on, uint16_t off);
void pca9685_Init(void);
void set_ServoAngle(uint8_t servoNum,uint16_t servoAngle);

#endif //V1_PCA9685_SIM_H
