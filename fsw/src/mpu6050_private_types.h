/*=======================================================================================
** File Name:  mpu6050_private_types.h
**
** Title:  Type Header File for MPU6050 Application
**
** $Author:    Jacob Killelea
** $Revision: 1.1 $
** $Date:      2019-10-22
**
** Purpose:  This header file contains declarations and definitions of all MPU6050's private
**           data structures and data types.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2019-10-22 | Jacob Killelea | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _MPU6050_PRIVATE_TYPES_H_
#define _MPU6050_PRIVATE_TYPES_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe.h"
#include "cfe_msg.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/

typedef struct
{
    CFE_MSG_CommandHeader_t ucCmdHeader;
} MPU6050_NoArgCmd_t;


typedef struct
{
    uint32  counter;

    /* TODO:  Add input data to this application here, such as raw data read from I/O
    **        devices or data subscribed from other apps' output data.
    */

} MPU6050_InData_t;

typedef struct
{
    CFE_MSG_TelemetryHeader_t ucTlmHeader;
    uint32  uiCounter;
    double  accelXGees;   /* Acceleration in X, Y, and Z body frame (g's) */
    double  accelYGees;
    double  accelZGees;
    double  gyroXDegsSec; /* Gyro angular rates, X, Y, and Z BF (degs/sec)*/
    double  gyroYDegsSec;
    double  gyroZDegsSec;
} MPU6050_OutData_t;

/* TODO:  Add more private structure definitions here, if necessary. */

/*
** External Global Variables
*/

/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/

#endif /* _MPU6050_PRIVATE_TYPES_H_ */

/*=======================================================================================
** End of file mpu6050_private_types.h
**=====================================================================================*/

