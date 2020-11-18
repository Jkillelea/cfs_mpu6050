#ifndef MPU6050_HW_DRV_H_
#define MPU6050_HW_DRV_H_

#include "cfe.h"
#include "mpu6050_registers.h"

/* Read a 16 bit register */
uint16 MPU6050_read16(int fd, uint8 reg);

/* Write an 8 bit register */
size_t MPU6050_write8(int fd, uint8 reg, uint8 val);

/* Write a 16 bit register */
size_t MPU6050_write16(int fd, uint8 reg, uint8 val1, uint8 val2);

/* Read a buffer of arbitrary size from the chip */
size_t MPU6050_ReadArbitrary(int fd, uint8 startingAddr, uint8 *buffer, size_t bufferLen);

/* Set or reset parts of the device */
int32 MPU6050_ResetDevice(void);
int32 MPU6050_SetAccelScale(MPU6050_AcceleormeterScale_t scale);
int32 MPU6050_SetGyroScale(MPU6050_GyroScale_t scale);

#endif /* end of include guard: MPU6050_HW_DRV_H_ */

