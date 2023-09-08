#include "pca9685.h"
#include <math.h>
#include <stdio.h>

#define min(_X_, _Y_)	(_X_>=_Y_?_Y_:_X_)

static uint8_t read16(uint8_t addr,int ic);
static void write16(uint8_t addr, uint8_t d,int ic);

// 设置为true可打印一些调试消息，设置为false可禁用这些消息。
#define ENABLE_DEBUG_OUTPUT false

// 初始化。频率，转动角度（-90~90）
void pca9685_init(float hz, int16_t angle)
{
  uint32_t off = 0;
  // 这一步很关键，如果没有这一步PCA9685就不会正常工作。
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

// 软件复位
void pca9685_reset(void)
{
  write16(PCA9685_MODE1, 0x0,1);
}

// 设置PCA9685的输出频率，
// PCA9685的16路PWM输出频率是一致的，
// 所以是不能实现不同引脚不同频率的。
void pca9685_setPWMFreq(float freq)
{
  //printf("Attempting to set freq ");
  //printf(freq);
  // 输出周期实际是有误差的，对于20ms（50Hz）的周期来说这里乘个0.94为20ms
  // 可以用示波器观看调试
  freq *= 1.005;
  double prescaleval = 25000000;
  prescaleval /= 4096.0;
  prescaleval /= freq;
  prescaleval -= 1;

  if (ENABLE_DEBUG_OUTPUT)
  {
    printf("Estimated pre-scale: %f\r\n", prescaleval);
  }
  // floor()总是返回小于等于一个给定数字的最大整数。
  // 这里是四舍五入
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

// 输出PWM占空比的调节。通常on都设为0，改变off即可。
// 因为PCA9685是12位分辨率，所以off的值0~4096就代表了占空比0-100.
void pca9685_setPWM(uint8_t num, uint16_t on, uint16_t off) {
  //printf("Setting PWM "); printf(num); printf(": "); printf(on); printf("->"); printf(off);
  uint8_t d[4] = {(on & 0xFF), (on >> 8), (off & 0xFF), (off >> 8)};

  HAL_I2C_Mem_Write(&PCA9685_HI2C, PCA96851_IIC_ADDR_W, (LED0_ON_L + 4 * num), 1, d, 4, 0xff);
}

// num:序号；angle:角度
void setAngle(uint8_t num, int16_t angle)
{
  uint32_t off = 0;
  // off范围是：0~4096
  // 舵机占空比范围是：0.5~2.5ms
  // 所以设置off在 102.4~512范围内(0.5ms~2.5ms==>(0°~180°))
  // x/(512-102.4) = 1/180
  // 所以每一度步进值为：2.2755556
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

