#ifndef MPU6050_REGISTERS_H_
#define MPU6050_REGISTERS_H_

// Addresses
#define MPU6050_DEVICE_ADDR 0x68
#define RegPowerManagment1  0x6B
#define RegPowerManagment2  0x6C
#define RegGyroConfig       0x1B
#define RegAccelConfig      0x1C
#define RegGyroX            0x43
#define RegGyroY            0x45
#define RegGyroZ            0x47
#define RegAccelX           0x3B
#define RegAccelY           0x3D
#define RegAccelZ           0x3F

// RegPowerManagment1 bits
#define PwrMgmt1Clksel      0 // bits 2:0
#define PwrMgmt1Temp_dis    3 // bit  3
#define PwrMgmt1Cycle       5
#define PwrMgmt1Sleep       6
#define PwrMgmt1DeviceReset 7

// RegPowerManagment2 bits
#define PwrMgmt2StbyZG     0
#define PwrMgmt2StbyYG     1
#define PwrMgmt2StbyXG     2
#define PwrMgmt2StbyZA     3
#define PwrMgmt2StbyYA     4
#define PwrMgmt2StbyXA     5
#define PwrMgmt2LPWakeCtrl 6 // bits 7:6

// RegAccelConfig bits
#define RegAccelConfigScale 3 // bits 4:3
#define RegGyroConfigScale 3  // bits 4:3

typedef enum
{
    MPU6050_ACCELSCALE_2G  = 0 << RegAccelConfigScale,
    MPU6050_ACCELSCALE_4G  = 1 << RegAccelConfigScale,
    MPU6050_ACCELSCALE_8G  = 2 << RegAccelConfigScale,
    MPU6050_ACCELSCALE_16G = 3 << RegAccelConfigScale,
} MPU6050_AcceleormeterScale_t;

typedef enum
{
    MPU6050_GYROSCALE_250DPS  = 0 << RegGyroConfigScale,
    MPU6050_GYROSCALE_500DPS  = 1 << RegGyroConfigScale,
    MPU6050_GYROSCALE_1000DPS = 2 << RegGyroConfigScale,
    MPU6050_GYROSCALE_2000DPS = 3 << RegGyroConfigScale,
} MPU6050_GyroScale_t;

#endif /* MPU6050_REGISTERS_H_ */

