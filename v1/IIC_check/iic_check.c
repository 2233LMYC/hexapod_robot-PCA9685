//
// Created by LMTX on 2023/8/19.
//
#include "iic_check.h"
#include "i2c.h"

int iic_check(uint16_t deviceAddress)
{
  uint8_t buffer[1];

  HAL_I2C_Master_Transmit(&hi2c2, deviceAddress<<1, buffer, 0, 1000);
  HAL_StatusTypeDef status = HAL_I2C_GetError(&hi2c2);

  if (status == HAL_OK) {
// Device found
    return 0;
  } else {
// Device not found
    return 1;
  }
}
