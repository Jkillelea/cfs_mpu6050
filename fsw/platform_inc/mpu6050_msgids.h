/*=======================================================================================
** File Name:  mpu6050_msgids.h
**
** Title:  Message ID Header File for MPU6050 Application
**
** $Author:    Jacob Killelea
** $Revision: 1.1 $
** $Date:      2019-10-22
**
** Purpose:  This header file contains declartions and definitions of all MPU6050's 
**           Message IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2019-10-22 | Jacob Killelea | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _MPU6050_MSGIDS_H_
#define _MPU6050_MSGIDS_H_

/***** TODO:  These Message ID values are default and may need to be changed by the developer  *****/
#define MPU6050_CMD_MID       0x11C0
#define MPU6050_SEND_HK_MID   0x11C1
#define MPU6050_WAKEUP_MID    0x11D0
#define MPU6050_OUT_DATA_MID  0x11D1
#define MPU6050_HK_TLM_MID    0x11BB

#endif /* _MPU6050_MSGIDS_H_ */

/*=======================================================================================
** End of file mpu6050_msgids.h
**=====================================================================================*/
    
