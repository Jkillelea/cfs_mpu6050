/*=======================================================================================
** File Name:  mpu6050_private_ids.h
**
** Title:  ID Header File for MPU6050 Application
**
** $Author:    Jacob Killelea
** $Revision: 1.1 $
** $Date:      2019-10-22
**
** Purpose:  This header file contains declarations and definitions of MPU6050's private IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2019-10-22 | Jacob Killelea | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _MPU6050_PRIVATE_IDS_H_
#define _MPU6050_PRIVATE_IDS_H_

/*
** Pragmas
*/

/*
** Include Files
*/

/*
** Local Defines
*/

/* Event IDs */
#define MPU6050_RESERVED_EID  0

#define MPU6050_INF_EID        1
#define MPU6050_INIT_INF_EID   2
#define MPU6050_ILOAD_INF_EID  3
#define MPU6050_CDS_INF_EID    4
#define MPU6050_CMD_INF_EID    5

#define MPU6050_ERR_EID         51
#define MPU6050_INIT_ERR_EID    52
#define MPU6050_ILOAD_ERR_EID   53
#define MPU6050_CDS_ERR_EID     54
#define MPU6050_CMD_ERR_EID     55
#define MPU6050_PIPE_ERR_EID    56
#define MPU6050_MSGID_ERR_EID   57
#define MPU6050_MSGLEN_ERR_EID  58

#define MPU6050_EVT_CNT  14

/*
** Local Structure Declarations
*/

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

#endif /* _MPU6050_PRIVATE_IDS_H_ */

/*=======================================================================================
** End of file mpu6050_private_ids.h
**=====================================================================================*/
    