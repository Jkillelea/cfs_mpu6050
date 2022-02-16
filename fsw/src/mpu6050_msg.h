/*=======================================================================================
** File Name:  mpu6050_msg.h
**
** Title:  Message Definition Header File for MPU6050 Application
**
** $Author:    Jacob Killelea
** $Revision: 1.1 $
** $Date:      2019-10-22
**
** Purpose:  To define MPU6050's command and telemetry message defintions
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2019-10-22 | Jacob Killelea | Build #: Code Started
**
**=====================================================================================*/

#ifndef _MPU6050_MSG_H_
#define _MPU6050_MSG_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe.h"
#include "cfe_msg.h"
#include "common_types.h"

/*
** Local Defines
*/

/*
** MPU6050 command codes
*/
#define MPU6050_NOOP_CC                            0
#define MPU6050_RESET_APP_CC                       1
#define MPU6050_RESET_DEVICE_CC                    2

/*
** Device scaling commands
*/
#define MPU6050_SET_DEVICE_ACCELEROMETER_SCALE_2G_CC  3
#define MPU6050_SET_DEVICE_ACCELEROMETER_SCALE_4G_CC  4
#define MPU6050_SET_DEVICE_ACCELEROMETER_SCALE_8G_CC  5
#define MPU6050_SET_DEVICE_ACCELEROMETER_SCALE_16G_CC 6
#define MPU6050_SET_DEVICE_GYRO_SCALE_250DPS_CC       7
#define MPU6050_SET_DEVICE_GYRO_SCALE_500DPS_CC       8
#define MPU6050_SET_DEVICE_GYRO_SCALE_1000DPS_CC      9
#define MPU6050_SET_DEVICE_GYRO_SCALE_2000DPS_CC     10

/*
** Local Structure Declarations
*/
typedef struct
{
    CFE_MSG_TelemetryHeader_t TlmHeader;
    uint32                    usCmdCnt;
    uint32                    usCmdErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */
} MPU6050_HkTlm_t;


#endif /* _MPU6050_MSG_H_ */

/*=======================================================================================
** End of file mpu6050_msg.h
**=====================================================================================*/

