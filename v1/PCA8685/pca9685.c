#include "pca9685.h"
#include <math.h>
#include <stdio.h>

#define min(_X_, _Y_)	(_X_>=_Y_?_Y_:_X_)

static uint8_t read16(uint8_t addr,int ic);
static void write16(uint8_t addr, uint8_t d,int ic);

// ����Ϊtrue�ɴ�ӡһЩ������Ϣ������Ϊfalse�ɽ�����Щ��Ϣ��
#define ENABLE_DEBUG_OUTPUT false

// ��ʼ����Ƶ�ʣ�ת���Ƕȣ�-90~90��
void pca9685_init(float hz, int16_t angle)
{
  uint32_t off = 0;
  // ��һ���ܹؼ������û����һ��PCA9685�Ͳ�������������
  pca9685_reset();

  write16(PCA9685_MODE1,0x00,1);
  pca9685_setPWMFreq(hz);
  HAL_Delay(10);
  off = (uint32_t)(102.4+(90+angle)*2.2755556);

  pca9685_setPWM(0,0,off);
  pca9685_setPWM(1,0,off);
  pca9685_setPWM(2,0,off);
  pca9685_setPWM(3,0,off);
  pca9685_setPWM(4,0,off);
  pca9685_setPWM(5,0,off);
  pca9685_setPWM(6,0,off);
  pca9685_setPWM(7,0,off);
  pca9685_setPWM(8,0,off);
  pca9685_setPWM(9,0,off);
  pca9685_setPWM(10,0,off);
  pca9685_setPWM(11,0,off);
  pca9685_setPWM(12,0,off);
  pca9685_setPWM(13,0,off);
  pca9685_setPWM(14,0,off);
  pca9685_setPWM(15,0,off);


  HAL_Delay(100);
}

// �����λ
void pca9685_reset(void)
{
  write16(PCA9685_MODE1, 0x0,1);
}

// ����PCA9685�����Ƶ�ʣ�
// PCA9685��16·PWM���Ƶ����һ�µģ�
// �����ǲ���ʵ�ֲ�ͬ���Ų�ͬƵ�ʵġ�
void pca9685_setPWMFreq(float freq)
{
  //printf("Attempting to set freq ");
  //printf(freq);
  // �������ʵ���������ģ�����20ms��50Hz����������˵����˸�0.94Ϊ20ms
  // ������ʾ�����ۿ�����
  freq *= 1.005;
  double prescaleval = 25000000;
  prescaleval /= 4096.0;
  prescaleval /= freq;
  prescaleval -= 1;

  if (ENABLE_DEBUG_OUTPUT)
  {
    printf("Estimated pre-scale: %f\r\n", prescaleval);
  }
  // floor()���Ƿ���С�ڵ���һ���������ֵ����������
  // ��������������
  uint8_t prescale = floor(prescaleval + 0.5);

  if (ENABLE_DEBUG_OUTPUT)
  {
    printf("Final pre-scale: %d\r\n", prescale);
  }

  uint8_t oldmode1 = read16(PCA9685_MODE1,1);
  uint8_t newmode1 = (oldmode1&0x7F) | 0x10; // sleep
  write16(PCA9685_MODE1, newmode1,1); // go to sleep
  write16(PCA9685_PRESCALE, prescale,1); // set the prescaler
  write16(PCA9685_MODE1, oldmode1,1);
  HAL_Delay(5);
  write16(PCA9685_MODE1, oldmode1 | 0xa1,1);  //  This sets the MODE1 register to turn on auto increment.

  // This is why the beginTransmission below was not working.
  //  printf("Mode now 0x"); printf(read16(PCA9685_MODE1), HEX);
}

// ���PWMռ�ձȵĵ��ڡ�ͨ��on����Ϊ0���ı�off���ɡ�
// ��ΪPCA9685��12λ�ֱ��ʣ�����off��ֵ0~4096�ʹ�����ռ�ձ�0-100.
void pca9685_setPWM(uint8_t num, uint16_t on, uint16_t off) {
  //printf("Setting PWM "); printf(num); printf(": "); printf(on); printf("->"); printf(off);
  uint8_t d[4] = {(on & 0xFF), (on >> 8), (off & 0xFF), (off >> 8)};

  HAL_I2C_Mem_Write(&PCA9685_HI2C, PCA96851_IIC_ADDR_W, (LED0_ON_L + 4 * num), 1, d, 4, 0xff);
}

// num:��ţ�angle:�Ƕ�
void setAngle(uint8_t num, int16_t angle)
{
  uint32_t off = 0;
  // off��Χ�ǣ�0~4096
  // ���ռ�ձȷ�Χ�ǣ�0.5~2.5ms
  // ��������off�� 102.4~512��Χ��(0.5ms~2.5ms==>(0��~180��))
  // x/(512-102.4) = 1/180
  // ����ÿһ�Ȳ���ֵΪ��2.2755556
  off = (uint32_t)(102.4+(90+angle)*2.2755556);
  pca9685_setPWM(num, 0, off);
}

static uint8_t read16(uint8_t addr ,int ic)
{
  uint8_t d;

  HAL_I2C_Mem_Read(&PCA9685_HI2C, PCA96851_IIC_ADDR_R, addr, 1, &d, 1, 0xff);


  return d;
}

static void write16(uint8_t addr, uint8_t d ,int ic)
{
    HAL_I2C_Mem_Write(&PCA9685_HI2C, PCA96851_IIC_ADDR_W, addr, 1, &d, 1, 0xff);
}

