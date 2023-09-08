//
// Created by LMTX on 2023/8/19.
//

#include "pca9685_sim.h"
#include "bsp_iic.h"


#include "math.h"

/****************
向PCA9685写一个字节
reg：寄存器的地址值
data:要写入的值
****************/
void pca9685_Write(unsigned char reg,unsigned char data){
  IIC_Start();
  IIC_Send_Byte(PCA9685_adrr);	//
  IIC_Wait_Ack();
  IIC_Send_Byte(reg);
  IIC_Wait_Ack();
  IIC_Send_Byte(data);
  IIC_Wait_Ack();
  IIC_Stop();
}
/****************
从PCA9685读一个字节
读取寄存器的值
reg:寄存器地址值
****************/
uint8_t pca9685_Read(unsigned char reg){
  uint8_t res;
  IIC_Start();
  IIC_Send_Byte(PCA9685_adrr);
  IIC_Wait_Ack();
  IIC_Send_Byte(reg);
  IIC_Wait_Ack();
  IIC_Start();
  IIC_Send_Byte(PCA9685_adrr|0x01);	//将地址值设置位读模式，地址值最后一位决定读写
  IIC_Wait_Ack();
  res=IIC_Read_Byte(0);
  IIC_Stop();
  return res;
}

/****************
设置PCA9685的PWM频率值
freq=50Hz   周期 t=1/50=0.02s=20ms
****************/
void setPWMFreq(uint8_t freq){
  uint8_t prescale,oldmode,newmode;
  double prescaleval;
  prescaleval = 25000000.0/(4096*freq*1.005);
  prescale = (uint8_t)floor(prescaleval+0.5)-1;	//floor --->round down（向下取整）

  oldmode = pca9685_Read(PCA9685_MODE1);
  newmode = (oldmode&0x7F) | 0x10; // sleep	设置预分频需要先进入低功耗模式
  pca9685_Write(PCA9685_MODE1, newmode); // go to sleep
  pca9685_Write(PCA9685_PRESCALE, prescale); // set the prescaler
  pca9685_Write(PCA9685_MODE1, oldmode);
  HAL_Delay(5);
  pca9685_Write(PCA9685_MODE1, oldmode | 0xa1); 		//0xa1:1010 0001
}

/****************
 *
设置每个通道的PWM值
num=0~15
on:
off:
****************/

void setPWM(uint8_t num, uint16_t on, uint16_t off) {
  pca9685_Write(LED0_ON_L+4*num,on&0xff);
  pca9685_Write(LED0_ON_H+4*num,on>>8);
  pca9685_Write(LED0_OFF_L+4*num,off&0xff);
  pca9685_Write(LED0_OFF_H+4*num,off>>8);
}

/****************
设置每个通道舵机的角度值
servoNum=0~15
servoAngle=0~180
****************/
void set_ServoAngle(uint8_t servoNum,uint16_t servoAngle)
{
  uint16_t pwm;
  pwm=(uint32_t)(102.4+(90+servoAngle)*2.2755556);

  setPWM(servoNum, 0, pwm);
}

/*********************************
PCA9685初始化并设置舵机初始角度值
**********************************/
void pca9685_Init(){
  uint8_t num;
  IIC_GPIO_Init();		//初始化IIC总线
  pca9685_Write(PCA9685_MODE1,0x0);	//在频率设置之前，必须有  (MODE1所有位置0)
  setPWMFreq(50);
  for(num=0;num<15;num++){
    set_ServoAngle(num,90);
  }
}
