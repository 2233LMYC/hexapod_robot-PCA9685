//
// Created by LMTX on 2023/8/19.
//

#include "pca9685_sim.h"
#include "bsp_iic.h"


#include "math.h"

/****************
��PCA9685дһ���ֽ�
reg���Ĵ����ĵ�ֵַ
data:Ҫд���ֵ
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
��PCA9685��һ���ֽ�
��ȡ�Ĵ�����ֵ
reg:�Ĵ�����ֵַ
****************/
uint8_t pca9685_Read(unsigned char reg){
  uint8_t res;
  IIC_Start();
  IIC_Send_Byte(PCA9685_adrr);
  IIC_Wait_Ack();
  IIC_Send_Byte(reg);
  IIC_Wait_Ack();
  IIC_Start();
  IIC_Send_Byte(PCA9685_adrr|0x01);	//����ֵַ����λ��ģʽ����ֵַ���һλ������д
  IIC_Wait_Ack();
  res=IIC_Read_Byte(0);
  IIC_Stop();
  return res;
}

/****************
����PCA9685��PWMƵ��ֵ
freq=50Hz   ���� t=1/50=0.02s=20ms
****************/
void setPWMFreq(uint8_t freq){
  uint8_t prescale,oldmode,newmode;
  double prescaleval;
  prescaleval = 25000000.0/(4096*freq*1.005);
  prescale = (uint8_t)floor(prescaleval+0.5)-1;	//floor --->round down������ȡ����

  oldmode = pca9685_Read(PCA9685_MODE1);
  newmode = (oldmode&0x7F) | 0x10; // sleep	����Ԥ��Ƶ��Ҫ�Ƚ���͹���ģʽ
  pca9685_Write(PCA9685_MODE1, newmode); // go to sleep
  pca9685_Write(PCA9685_PRESCALE, prescale); // set the prescaler
  pca9685_Write(PCA9685_MODE1, oldmode);
  HAL_Delay(5);
  pca9685_Write(PCA9685_MODE1, oldmode | 0xa1); 		//0xa1:1010 0001
}

/****************
 *
����ÿ��ͨ����PWMֵ
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
����ÿ��ͨ������ĽǶ�ֵ
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
PCA9685��ʼ�������ö����ʼ�Ƕ�ֵ
**********************************/
void pca9685_Init(){
  uint8_t num;
  IIC_GPIO_Init();		//��ʼ��IIC����
  pca9685_Write(PCA9685_MODE1,0x0);	//��Ƶ������֮ǰ��������  (MODE1����λ��0)
  setPWMFreq(50);
  for(num=0;num<15;num++){
    set_ServoAngle(num,90);
  }
}
