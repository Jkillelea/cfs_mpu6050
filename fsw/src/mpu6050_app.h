/*=======================================================================================
** File Name:  mpu6050_app.h
**
** Title:  Header File for MPU6050 Application
**
** $Author:    Jacob Killelea
** $Revision: 1.1 $
** $Date:      2019-10-22
**
** Purpose:  To define MPU6050's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2019-10-22 | Jacob Killelea | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _MPU6050_APP_H_
#define _MPU6050_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "mpu6050_platform_cfg.h"
#include "mpu6050_mission_cfg.h"
#include "mpu6050_private_ids.h"
#include "mpu6050_private_types.h"
#include "mpu6050_perfids.h"
#include "mpu6050_msgids.h"
#include "mpu6050_msg.h"



/*
** Local Defines
*/
#define MPU6050_TIMEOUT_MSEC    1000

/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[MPU6050_EVT_CNT];

    /* CFE scheduling pipe */
    CFE_SB_PipeId_t  SchPipeId; 
    uint16           usSchPipeDepth;
    char             cSchPipeName[OS_MAX_API_NAME];

    /* CFE command pipe */
    CFE_SB_PipeId_t  CmdPipeId;
    uint16           usCmdPipeDepth;
    char             cCmdPipeName[OS_MAX_API_NAME];
    
    /* CFE telemetry pipe */
    CFE_SB_PipeId_t  TlmPipeId;
    uint16           usTlmPipeDepth;
    char             cTlmPipeName[OS_MAX_API_NAME];

    /* Task-related */
    uint32  uiRunStatus;
    
    /* Input data - from I/O devices or subscribed from other apps' output data.
       Data structure should be defined in mpu6050/fsw/src/mpu6050_private_types.h */
    MPU6050_InData_t   InData;

    /* Output data - to be published at the end of a Wakeup cycle.
       Data structure should be defined in mpu6050/fsw/src/mpu6050_private_types.h */
    MPU6050_OutData_t  OutData;

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in mpu6050/fsw/src/mpu6050_msg.h */
    MPU6050_HkTlm_t  HkTlm;

    /* TODO:  Add declarations for additional private data here */
} MPU6050_AppData_t;

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
int32 MPU6050_InitApp(void);
int32 MPU6050_InitEvent(void);
int32 MPU6050_InitData(void);
int32 MPU6050_InitPipe(void);
int32 MPU6050_InitDevice(void);

void  MPU6050_AppMain(void);

void  MPU6050_CleanupCallback(void);

int32  MPU6050_RcvMsg(int32 iBlocking);

void  MPU6050_ProcessNewData(void);
void  MPU6050_ProcessNewCmds(void);
void  MPU6050_ProcessNewAppCmds(CFE_SB_Msg_t*);

void  MPU6050_ReportHousekeeping(void);
void  MPU6050_SendOutData(void);

boolean  MPU6050_VerifyCmdLength(CFE_SB_Msg_t*, uint16);

/* Read and write from the I2C bus using an open file descriptor
 * TODO: Change from size_t to something in cfe's headers
 * */
uint16 MPU6050_read16(int fd, uint8 reg);
size_t MPU6050_write8(int fd, uint8 reg, uint8 val);
size_t MPU6050_write16(int fd, uint8 reg, uint8 val1, uint8 val2);
size_t MPU6050_ReadArbitrary(int fd, uint8 startingAddr, uint8 *buffer, size_t bufferLen);

/* Set or reset parts of the device
 * */
int32 MPU6050_ResetDevice(CFE_SB_Msg_t *msg);
int32 MPU6050_SetAccelScale(CFE_SB_Msg_t *msg);
int32 MPU6050_SetGyroScale(CFE_SB_Msg_t *msg);

#endif /* _MPU6050_APP_H_ */

/*=======================================================================================
** End of file mpu6050_app.h
**=====================================================================================*/
    
