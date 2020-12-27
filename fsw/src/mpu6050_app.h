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

#include "common_types.h"

#include "cfe_tbl.h"
#include "mpu6050_platform_cfg.h"
#include "mpu6050_registers.h"
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
#define MPU6050_PATH_SIZE (OS_MAX_PATH_LEN)

/*
** Local Structure Declarations
*/

typedef struct
{
    MPU6050_AcceleormeterScale_t initialAccelScale;
    MPU6050_GyroScale_t initialGyroScale;
    uint8 deviceI2CAddr;
    char devicePath[MPU6050_PATH_SIZE];
} MPU6050_ConfigTbl_t;

typedef struct
{
    /* OS level file ID for read/write/ioctl calls */
    int FileID;

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

    CFE_TBL_Handle_t     ConfigTblHandle;
    MPU6050_ConfigTbl_t *ConfigTbl;

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

bool  MPU6050_VerifyCmdLength(CFE_MSG_Message_t*, uint16);

#endif /* _MPU6050_APP_H_ */

/*=======================================================================================
** End of file mpu6050_app.h
**=====================================================================================*/

