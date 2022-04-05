/*=======================================================================================
** File Name:  mpu6050_platform_cfg.h
**
** Title:  Platform Configuration Header File for MPU6050 Application
**
** $Author:    Jacob Killelea
** $Revision: 1.1 $
** $Date:      2019-10-22
**
** Purpose:  This header file contains declartions and definitions of all MPU6050's
**           platform-specific configurations.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2019-10-22 | Jacob Killelea | Build #: Code Started
**
**=====================================================================================*/

#ifndef _MPU6050_PLATFORM_CFG_H_
#define _MPU6050_PLATFORM_CFG_H_

/*
** mpu6050 Platform Configuration Parameter Definitions
*/
#define MPU6050_SCH_PIPE_DEPTH  2
#define MPU6050_CMD_PIPE_DEPTH  10
#define MPU6050_TLM_PIPE_DEPTH  10

/* Accelerometer readings per second */
#define MPU6050_SAMPLE_RATE_HZ 10

/* Where to store the configuration table */
#define MPU6050_TBL_PATH "/cf/mpu6050_table.tbl"

/* TODO:  Add more platform configuration parameter definitions here, if necessary. */

#endif /* _MPU6050_PLATFORM_CFG_H_ */

/*=======================================================================================
** End of file mpu6050_platform_cfg.h
**=====================================================================================*/

