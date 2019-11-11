/*=======================================================================================
** File Name:  mpu6050_app.c
**
** Title:  Function Definitions for MPU6050 Application
**
** $Author:    Jacob Killelea
** $Revision: 1.1 $
** $Date:      2019-10-22
**
** Purpose:  This source file contains all necessary function definitions to run MPU6050
**           application.
**
** Functions Defined:
**    Function X - Brief purpose of function X
**    Function Y - Brief purpose of function Y
**    Function Z - Brief purpose of function Z
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to all functions in the file.
**    2. List the external source(s) and event(s) that can cause the funcs in this
**       file to execute.
**    3. List known limitations that apply to the funcs in this file.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2019-10-22 | Jacob Killelea | Build #: Code Started
**
**=====================================================================================*/

/*
** Pragmas
*/

/*
** Include Files
*/
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// #include "mpu6050_registers.h"

#include "cfe.h"

#include "mpu6050_registers.h"
#include "mpu6050_platform_cfg.h"
#include "mpu6050_mission_cfg.h"
#include "mpu6050_app.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/

/*
** External Global Variables
*/

/*
** Global Variables
*/
MPU6050_AppData_t  g_MPU6050_AppData;

/*
** Local Variables
*/

/*
** Local Function Definitions
*/

/* Read a 16 bit register */
uint16 MPU6050_read16(int fd, uint8 reg)
{
    uint8_t buffer[2];
    buffer[0] = reg;
    write(fd, buffer, 1);                /* Select register     */ 
    read(fd, buffer, 2);                 /* Read 16 bits        */ 
    return (buffer[0] << 8) | buffer[1]; /* Return (big endian) */ 
}

/* Write an 8 bit register */
size_t MPU6050_write8(int fd, uint8 reg, uint8 val)
{
    uint8_t buffer[2] = {reg, val}; /* 8 bit register addr and 8 bit data */
    return write(fd, buffer, 2);
}

/* Write a 16 bit register */
size_t MPU6050_write16(int fd, uint8 reg, uint8 val1, uint8 val2)
{
    uint8_t buffer[3] = {reg, val1, val2}; /* 8 bit register addr and 16 bit data */
    return write(fd, buffer, 3);
}

int32 MPU6050_ResetDevice(CFE_SB_Msg_t *msg)
{
    CFE_EVS_SendEvent(MPU6050_ERR_EID, CFE_EVS_ERROR,
            "[MPU6050 %s:%d] Called a function that is not implemented yet! Exiting!",
            __FILE__, __LINE__);
    g_MPU6050_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    return -1;
}

int32 MPU6050_SetAccelScale(CFE_SB_Msg_t *msg)
{
    CFE_EVS_SendEvent(MPU6050_ERR_EID, CFE_EVS_ERROR,
            "[MPU6050 %s:%d] Called a function that is not implemented yet! Exiting!",
            __FILE__, __LINE__);
    g_MPU6050_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    return -1;
}

int32 MPU6050_SetGyroScale(CFE_SB_Msg_t *msg)
{
    CFE_EVS_SendEvent(MPU6050_ERR_EID, CFE_EVS_ERROR,
            "[MPU6050 %s:%d] Called a function that is not implemented yet! Exiting!",
            __FILE__, __LINE__);
    g_MPU6050_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    return -1;
}

/*=====================================================================================
** Name: MPU6050_InitEvent
**
** Purpose: To initialize and register event table for MPU6050 application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_EVS_Register
**    CFE_ES_WriteToSysLog
**
** Called By:
**    MPU6050_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_MPU6050_AppData.EventTbl
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Jacob Killelea 
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 MPU6050_InitEvent()
{
    int32 iStatus = CFE_SUCCESS;

    /* Create the event table */
    memset((void*)g_MPU6050_AppData.EventTbl, 0x00, sizeof(g_MPU6050_AppData.EventTbl));

    g_MPU6050_AppData.EventTbl[0].EventID = MPU6050_RESERVED_EID;
    g_MPU6050_AppData.EventTbl[1].EventID = MPU6050_INF_EID;
    g_MPU6050_AppData.EventTbl[2].EventID = MPU6050_INIT_INF_EID;
    g_MPU6050_AppData.EventTbl[3].EventID = MPU6050_ILOAD_INF_EID;
    g_MPU6050_AppData.EventTbl[4].EventID = MPU6050_CDS_INF_EID;
    g_MPU6050_AppData.EventTbl[5].EventID = MPU6050_CMD_INF_EID;

    g_MPU6050_AppData.EventTbl[ 6].EventID = MPU6050_ERR_EID;
    g_MPU6050_AppData.EventTbl[ 7].EventID = MPU6050_INIT_ERR_EID;
    g_MPU6050_AppData.EventTbl[ 8].EventID = MPU6050_ILOAD_ERR_EID;
    g_MPU6050_AppData.EventTbl[ 9].EventID = MPU6050_CDS_ERR_EID;
    g_MPU6050_AppData.EventTbl[10].EventID = MPU6050_CMD_ERR_EID;
    g_MPU6050_AppData.EventTbl[11].EventID = MPU6050_PIPE_ERR_EID;
    g_MPU6050_AppData.EventTbl[12].EventID = MPU6050_MSGID_ERR_EID;
    g_MPU6050_AppData.EventTbl[13].EventID = MPU6050_MSGLEN_ERR_EID;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(g_MPU6050_AppData.EventTbl,
                               MPU6050_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("MPU6050 - Failed to register with EVS (0x%08X)\n", iStatus);
    }

    return (iStatus);
}

/*=====================================================================================
** Name: MPU6050_InitPipe
**
** Purpose: To initialize all message pipes and subscribe to messages for MPU6050 application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_CreatePipe
**    CFE_SB_Subscribe
**    CFE_ES_WriteToSysLog
**
** Called By:
**    MPU6050_InitApp
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_MPU6050_AppData.usSchPipeDepth
**    g_MPU6050_AppData.cSchPipeName
**    g_MPU6050_AppData.SchPipeId
**    g_MPU6050_AppData.usCmdPipeDepth
**    g_MPU6050_AppData.cCmdPipeName
**    g_MPU6050_AppData.CmdPipeId
**    g_MPU6050_AppData.usTlmPipeDepth
**    g_MPU6050_AppData.cTlmPipeName
**    g_MPU6050_AppData.TlmPipeId
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Jacob Killelea 
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 MPU6050_InitPipe()
{
    int32 iStatus = CFE_SUCCESS;

    /* Init schedule pipe */
    g_MPU6050_AppData.usSchPipeDepth = MPU6050_SCH_PIPE_DEPTH;
    memset((void*)g_MPU6050_AppData.cSchPipeName, '\0', sizeof(g_MPU6050_AppData.cSchPipeName));
    strncpy(g_MPU6050_AppData.cSchPipeName, "MPU6050_SCH_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to Wakeup messages */
    iStatus = CFE_SB_CreatePipe(&g_MPU6050_AppData.SchPipeId,
                                 g_MPU6050_AppData.usSchPipeDepth,
                                 g_MPU6050_AppData.cSchPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(MPU6050_WAKEUP_MID,
                                     g_MPU6050_AppData.SchPipeId,
                                     CFE_SB_Default_Qos,
                                     1);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("MPU6050 - Sch Pipe failed to subscribe to MPU6050_WAKEUP_MID. (0x%08X)\n", iStatus);
            goto MPU6050_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("MPU6050 - Failed to create SCH pipe (0x%08X)\n", iStatus);
        goto MPU6050_InitPipe_Exit_Tag;
    }

    /* Init command pipe */
    g_MPU6050_AppData.usCmdPipeDepth = MPU6050_CMD_PIPE_DEPTH ;
    memset((void*)g_MPU6050_AppData.cCmdPipeName, '\0', sizeof(g_MPU6050_AppData.cCmdPipeName));
    strncpy(g_MPU6050_AppData.cCmdPipeName, "MPU6050_CMD_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&g_MPU6050_AppData.CmdPipeId,
                                 g_MPU6050_AppData.usCmdPipeDepth,
                                 g_MPU6050_AppData.cCmdPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(MPU6050_CMD_MID, g_MPU6050_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("MPU6050 - CMD Pipe failed to subscribe to MPU6050_CMD_MID. (0x%08X)\n", iStatus);
            goto MPU6050_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_Subscribe(MPU6050_SEND_HK_MID, g_MPU6050_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("MPU6050 - CMD Pipe failed to subscribe to MPU6050_SEND_HK_MID. (0x%08X)\n", iStatus);
            goto MPU6050_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("MPU6050 - Failed to create CMD pipe (0x%08X)\n", iStatus);
        goto MPU6050_InitPipe_Exit_Tag;
    }

    /* Init telemetry pipe */
    g_MPU6050_AppData.usTlmPipeDepth = MPU6050_TLM_PIPE_DEPTH;
    memset((void*)g_MPU6050_AppData.cTlmPipeName, '\0', sizeof(g_MPU6050_AppData.cTlmPipeName));
    strncpy(g_MPU6050_AppData.cTlmPipeName, "MPU6050_TLM_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to telemetry messages on the telemetry pipe */
    iStatus = CFE_SB_CreatePipe(&g_MPU6050_AppData.TlmPipeId,
                                 g_MPU6050_AppData.usTlmPipeDepth,
                                 g_MPU6050_AppData.cTlmPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* TODO:  Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, g_MPU6050_AppData.TlmPipeId);
        */
    }
    else
    {
        CFE_ES_WriteToSysLog("MPU6050 - Failed to create TLM pipe (0x%08X)\n", iStatus);
        goto MPU6050_InitPipe_Exit_Tag;
    }

MPU6050_InitPipe_Exit_Tag:
    return (iStatus);
}

/*=====================================================================================
** Name: MPU6050_InitData
**
** Purpose: To initialize global variables used by MPU6050 application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_InitMsg
**
** Called By:
**    MPU6050_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_MPU6050_AppData.InData
**    g_MPU6050_AppData.OutData
**    g_MPU6050_AppData.HkTlm
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Jacob Killelea 
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 MPU6050_InitData()
{
    int32 iStatus = CFE_SUCCESS;

    /* Init input data */
    memset((void*)&g_MPU6050_AppData.InData, 0x00, sizeof(g_MPU6050_AppData.InData));

    /* Init output data */
    memset((void*)&g_MPU6050_AppData.OutData, 0x00, sizeof(g_MPU6050_AppData.OutData));
    CFE_SB_InitMsg(&g_MPU6050_AppData.OutData,
                   MPU6050_OUT_DATA_MID, sizeof(g_MPU6050_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&g_MPU6050_AppData.HkTlm, 0x00, sizeof(g_MPU6050_AppData.HkTlm));
    CFE_SB_InitMsg(&g_MPU6050_AppData.HkTlm,
                   MPU6050_HK_TLM_MID, sizeof(g_MPU6050_AppData.HkTlm), TRUE);

    return (iStatus);
}

/*=====================================================================================
** Name: MPU6050_InitDevice
**
** Purpose: To initialize the MPU6050 accelerometer
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    Linux file IO commands - open, ioctl, etc
**
** Called By:
**    MPU6050_InitApp
**
** Global Inputs/Reads:
**    MPU6050_BUS_FILEPATH
**
** Global Outputs/Writes:
**    g_MPU6050_FileId
**    MPU6050_BUS_FILEPATH
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Find MPU6050 on bus, wake up, and place into the apropriate sensitivity mode
**
** Author(s):  Jacob Killelea 
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 MPU6050_InitDevice()
{
    int32 iStatus = CFE_SUCCESS;

    g_MPU6050_FileId = open(MPU6050_BUS_FILEPATH, O_RDWR);
    if (g_MPU6050_FileId < 0)
    {
        iStatus = CFE_ES_APP_ERROR;
        perror("Failed to open bus path!");
        CFE_ES_WriteToSysLog("MPU6050 - Failed to open bus path %s\n",
                MPU6050_BUS_FILEPATH);
        goto out;
    }

    iStatus = ioctl(g_MPU6050_FileId, I2C_SLAVE, MPU6050_DEVICE_ADDR);
    if (iStatus < 0)
    {
        iStatus = CFE_ES_APP_ERROR;
        perror("ioctl");
        CFE_ES_WriteToSysLog("MPU6050 - Ioctl failed to set slave address!\n");
        goto out;
    }

    /* Wake device */
    if(MPU6050_write8(g_MPU6050_FileId, RegPowerManagment1, 0) < 2)
    {
        iStatus = CFE_ES_APP_ERROR;
        CFE_ES_WriteToSysLog("MPU6050 - Failed to send wakeup command to device!\n");
        goto out;
    }

    /* +/- 2g */
    if(MPU6050_write8(g_MPU6050_FileId, RegAccelConfig, 0 << RegAccelConfigScale) < 2)
    {
        iStatus = CFE_ES_APP_ERROR;
        CFE_ES_WriteToSysLog("MPU6050 - Failed to set accelerometer sensitivity!\n");
        goto out;
    }

    /* +/- 250 deg/s */
    if(MPU6050_write8(g_MPU6050_FileId, RegGyroConfig, 0 << RegGyroConfigScale) < 2)
    {
        iStatus = CFE_ES_APP_ERROR;
        CFE_ES_WriteToSysLog("MPU6050 - Failed to set gyro sensitivity!\n");
        goto out;
    }

out:
    return (iStatus);
}

/*=====================================================================================
** Name: MPU6050_InitApp
**
** Purpose: To initialize all data local to and used by MPU6050 application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_WriteToSysLog
**    CFE_EVS_SendEvent
**    OS_TaskInstallDeleteHandler
**    MPU6050_InitEvent
**    MPU6050_InitPipe
**    MPU6050_InitData
**
** Called By:
**    MPU6050_AppMain
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Jacob Killelea 
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 MPU6050_InitApp()
{
    int32 iStatus = CFE_SUCCESS;

    g_MPU6050_AppData.uiRunStatus = CFE_ES_APP_RUN;

    iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("MPU6050 - Failed to register the app (0x%08X)\n", iStatus);
        goto MPU6050_InitApp_Exit_Tag;
    }

    if ((MPU6050_InitEvent()  != CFE_SUCCESS) || 
        (MPU6050_InitPipe()   != CFE_SUCCESS) || 
        (MPU6050_InitData()   != CFE_SUCCESS) ||
        (MPU6050_InitDevice() != CFE_SUCCESS))
    {
        iStatus = -1;
        goto MPU6050_InitApp_Exit_Tag;
    }

    /* Install the cleanup callback */
    OS_TaskInstallDeleteHandler((void*)&MPU6050_CleanupCallback);

MPU6050_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(MPU6050_INIT_INF_EID, CFE_EVS_INFORMATION,
                          "MPU6050 - Application initialized");
    }
    else
    {
        CFE_ES_WriteToSysLog("MPU6050 - Application failed to initialize\n");
    }

    return (iStatus);
}

/*=====================================================================================
** Name: MPU6050_CleanupCallback
**
** Purpose: To handle any neccesary cleanup prior to application exit
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    Executive Service when app is unloaded
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Jacob Killelea 
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void MPU6050_CleanupCallback()
{
    /* TODO:  Add code to cleanup memory and other cleanup here */
    CFE_ES_WriteToSysLog("MPU6050 - Cleanup Callback\n");
    close(g_MPU6050_FileId);
}
    
/*=====================================================================================
** Name: MPU6050_RcvMsg
**
** Purpose: To receive and process messages for MPU6050 application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization 
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    MPU6050_ProcessNewCmds
**    MPU6050_ProcessNewData
**    MPU6050_SendOutData
**
** Called By:
**    MPU6050_Main
**
** Global Inputs/Reads:
**    g_MPU6050_AppData.SchPipeId
**
** Global Outputs/Writes:
**    g_MPU6050_AppData.uiRunStatus
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Jacob Killelea 
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 MPU6050_RcvMsg(int32 iBlocking)
{
    int32           iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr  = NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MPU6050_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, g_MPU6050_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MPU6050_MAIN_TASK_PERF_ID);

    /* New data to process */
    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId) {
            case MPU6050_WAKEUP_MID:
                MPU6050_ProcessNewCmds();
                MPU6050_ProcessNewData();
                break;

            /* Add more cases here */

            default:
                CFE_EVS_SendEvent(MPU6050_MSGID_ERR_EID,
                        CFE_EVS_ERROR,
                        "MPU6050 - Recvd invalid SCH msgId (0x%08X)", MsgId);
                break;
                
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE || iStatus == CFE_SB_TIME_OUT)
    {
        /* Read accelerometer */
    }
    else
    {
        /* This is an example of exiting on an error.
         * Note that a SB read error is not always going to result in an app quitting.
         */
        g_MPU6050_AppData.uiRunStatus = CFE_ES_APP_ERROR;
        CFE_EVS_SendEvent(MPU6050_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "MPU6050: SB pipe read error (0x%08X), app will exit", iStatus);
    }


    return (iStatus);
}

/*=====================================================================================
** Name: MPU6050_ProcessNewData
**
** Purpose: To process incoming data subscribed by MPU6050 application
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**
** Called By:
**    MPU6050_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    None
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Jacob Killelea 
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void MPU6050_ProcessNewData()
{
    int            iStatus   = CFE_SUCCESS;
    CFE_SB_Msg_t*  TlmMsgPtr = NULL;
    CFE_SB_MsgId_t TlmMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&TlmMsgPtr, g_MPU6050_AppData.TlmPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            TlmMsgId = CFE_SB_GetMsgId(TlmMsgPtr);
            switch (TlmMsgId)
            {
                /* TODO:  Add code to process all subscribed data here 
                **
                ** Example:
                **     case NAV_OUT_DATA_MID:
                **         MPU6050_ProcessNavData(TlmMsgPtr);
                **         break;
                */

                default:
                    CFE_EVS_SendEvent(MPU6050_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "MPU6050 - Recvd invalid TLM msgId (0x%08X)", TlmMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(MPU6050_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "MPU6050: CMD pipe read error (0x%08X)", iStatus);
            g_MPU6050_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}

/*=====================================================================================
** Name: MPU6050_ProcessNewCmds
**
** Purpose: To process incoming command messages for MPU6050 application
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**    MPU6050_ProcessNewAppCmds
**    MPU6050_ReportHousekeeping
**
** Called By:
**    MPU6050_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    None
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Jacob Killelea 
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void MPU6050_ProcessNewCmds()
{
    int            iStatus   = CFE_SUCCESS;
    CFE_SB_Msg_t*  CmdMsgPtr = NULL;
    CFE_SB_MsgId_t CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, g_MPU6050_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case MPU6050_CMD_MID:
                    MPU6050_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                case MPU6050_SEND_HK_MID:
                    MPU6050_ReportHousekeeping();
                    break;

                /* TODO:  Add code to process other subscribed commands here
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         MPU6050_ProcessTimeDataCmd(CmdMsgPtr);
                **         break;
                */

                default:
                    CFE_EVS_SendEvent(MPU6050_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "MPU6050 - Recvd invalid CMD msgId (0x%08X)", CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(MPU6050_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "MPU6050: CMD pipe read error (0x%08X)", iStatus);
            g_MPU6050_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}

/*=====================================================================================
** Name: MPU6050_ProcessNewAppCmds
**
** Purpose: To process command messages targeting MPU6050 application
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr - new command message pointer
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_GetCmdCode
**    CFE_EVS_SendEvent
**
** Called By:
**    MPU6050_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_MPU6050_AppData.HkTlm.usCmdCnt
**    g_MPU6050_AppData.HkTlm.usCmdErrCnt
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Jacob Killelea 
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void MPU6050_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    uint32 uiCmdCode = 0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case MPU6050_NOOP_CC:
                g_MPU6050_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(MPU6050_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "MPU6050 - Recvd NOOP cmd (%d)", uiCmdCode);
                break;

            case MPU6050_RESET_APP_CC:
                g_MPU6050_AppData.HkTlm.usCmdCnt = 0;
                g_MPU6050_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_EVS_SendEvent(MPU6050_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "MPU6050 - Recvd RESET cmd (%d)", uiCmdCode);
                break;

            case MPU6050_RESET_DEVICE_CC:
                // TODO
                MPU6050_ResetDevice(MsgPtr);
                break;

            case MPU6050_SET_DEVICE_ACCELEROMETER_SCALE_CC:
                // TODO
                MPU6050_SetAccelScale(MsgPtr);
                break;

            case MPU6050_SET_DEVICE_GYRO_SCALE_CC:
                // TODO
                MPU6050_SetGyroScale(MsgPtr);
                break;

            /* TODO:  Add code to process the rest of the MPU6050 commands here */

            default:
                g_MPU6050_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(MPU6050_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "MPU6050 - Recvd invalid cmdId (%d)", uiCmdCode);
                break;
        }
    }
}

/*=====================================================================================
** Name: MPU6050_ReportHousekeeping
**
** Purpose: To send housekeeping message
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    MPU6050_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  GSFC, Jacob Killelea
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void MPU6050_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_MPU6050_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_MPU6050_AppData.HkTlm);
}

/*=====================================================================================
** Name: MPU6050_SendOutData
**
** Purpose: To publish 1-Wakeup cycle output data
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    MPU6050_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Jacob Killelea
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void MPU6050_SendOutData()
{
    /* TODO:  Add code to update output data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_MPU6050_AppData.OutData);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_MPU6050_AppData.OutData);
}
    
/*=====================================================================================
** Name: MPU6050_VerifyCmdLength
**
** Purpose: To verify command length for a particular command message
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr      - command message pointer
**    uint16         usExpLength - expected command length
**
** Returns:
**    boolean bResult - result of verification
**
** Routines Called:
**    TBD
**
** Called By:
**    MPU6050_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Jacob Killelea 
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
boolean MPU6050_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    boolean bResult  = FALSE;
    uint16  usMsgLen = 0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            CFE_EVS_SendEvent(MPU6050_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "MPU6050 - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            g_MPU6050_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}

/*=====================================================================================
** Name: MPU6050_AppMain
**
** Purpose: To define MPU6050 application's entry point and main process loop
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_RunLoop
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    CFE_ES_ExitApp
**    CFE_ES_WaitForStartupSync
**    MPU6050_InitApp
**    MPU6050_RcvMsg
**
** Called By:
**    TBD
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Jacob Killelea 
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void MPU6050_AppMain()
{
    /* Register the application with Executive Services */
    CFE_ES_RegisterApp();

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MPU6050_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (MPU6050_InitApp() != CFE_SUCCESS)
    {
        g_MPU6050_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    }
    else
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(MPU6050_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(MPU6050_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(MPU6050_MAIN_TASK_PERF_ID);
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&g_MPU6050_AppData.uiRunStatus) == TRUE)
    {
        MPU6050_RcvMsg(1000 / MPU6050_SAMPLE_RATE_HZ);

        /* Read from MPU6050 and send out data */
        float readingAccelX =
            ((float) MPU6050_read16(g_MPU6050_FileId, RegAccelX)) / INT16_MAX;
        float readingAccelY =
            ((float) MPU6050_read16(g_MPU6050_FileId, RegAccelY)) / INT16_MAX;
        float readingAccelZ =
            ((float) MPU6050_read16(g_MPU6050_FileId, RegAccelZ)) / INT16_MAX;
        OS_printf("%f\t%f\t%f\n", readingAccelX, readingAccelY, readingAccelZ);
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MPU6050_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(g_MPU6050_AppData.uiRunStatus);
} 

/*=======================================================================================
** End of file mpu6050_app.c
**=====================================================================================*/

