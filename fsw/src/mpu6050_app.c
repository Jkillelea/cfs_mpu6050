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

#include "cfe_error.h"
#include "cfe_es.h"
#include "cfe_evs.h"
#include "cfe_msg.h"
#include "cfe_psp.h"
#include "cfe_sb.h"
#include "cfe_tbl.h"
#include "mpu6050_registers.h"
#include "mpu6050_platform_cfg.h"
#include "mpu6050_mission_cfg.h"
#include "mpu6050_app.h"
#include "mpu6050_hw_drv.h"

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
**
** Algorithm:
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
    memset((void*) g_MPU6050_AppData.EventTbl, 0x00, sizeof(g_MPU6050_AppData.EventTbl));

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
    g_MPU6050_AppData.EventTbl[14].EventID = MPU6050_DEVICE_ERR_EID;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(g_MPU6050_AppData.EventTbl, MPU6050_EVT_CNT, CFE_EVS_EventFilter_BINARY);
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
** Returns
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
**
** Algorithm:
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
        iStatus = CFE_SB_Subscribe(MPU6050_WAKEUP_MID, g_MPU6050_AppData.SchPipeId);

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
**
** Algorithm:
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
    memset((void*) &g_MPU6050_AppData.InData, 0x00, sizeof(g_MPU6050_AppData.InData));

    /* Init output data */
    memset((void*) &g_MPU6050_AppData.OutData, 0x00, sizeof(g_MPU6050_AppData.OutData));
    CFE_MSG_Init((CFE_MSG_Message_t *) &g_MPU6050_AppData.OutData, MPU6050_OUT_DATA_MID, sizeof(g_MPU6050_AppData.OutData));

    /* Init housekeeping packet */
    memset((void*) &g_MPU6050_AppData.HkTlm, 0x00, sizeof(g_MPU6050_AppData.HkTlm));
    CFE_MSG_Init((CFE_MSG_Message_t *) &g_MPU6050_AppData.HkTlm, MPU6050_HK_TLM_MID, sizeof(g_MPU6050_AppData.HkTlm));

    return (iStatus);
}


/*=====================================================================================
** Name: MPU6050_InitTable
**
** Purpose: To initialize the MPU6050 app tables
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_TBL routines
**
** Called By:
**    MPU6050_InitApp
**
** Global Inputs/Reads:
**    /cf/mpu6050_table.tbl
**
** Global Outputs/Writes:
**    g_MPU6050_AppData.ConfigTblHandle
**    g_MPU6050_AppData.ConfigTbl
**
** Limitations, Assumptions, External Events, and Notes:
** 1: Table has compiled and loaded successfully
** 2: Table paramters (path, name, etc) match.
**
** Algorithm:
**
** Author(s):  Jacob Killelea
**
** History:  Date Written  2020-09-19
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 MPU6050_InitTable(void)
{
    int32 iStatus = CFE_SUCCESS;

    /* Ask cFS to allocate space for the table */
    iStatus = CFE_TBL_Register(&g_MPU6050_AppData.ConfigTblHandle, "ConfigTbl", sizeof(MPU6050_ConfigTbl_t), CFE_TBL_OPT_DEFAULT, NULL);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Failed to register table");
        return iStatus;
    }

    /* Ask cFS to load the table into memory */
    iStatus = CFE_TBL_Load(g_MPU6050_AppData.ConfigTblHandle, CFE_TBL_SRC_FILE, MPU6050_TBL_PATH);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Failed to load table");
        CFE_TBL_ReleaseAddress(g_MPU6050_AppData.ConfigTblHandle);
        return iStatus;
    }

    /* Ask cFS to give us a pointer to the table */
    iStatus = CFE_TBL_GetAddress((void **) &g_MPU6050_AppData.ConfigTbl, g_MPU6050_AppData.ConfigTblHandle);
    if (iStatus != CFE_SUCCESS && iStatus != CFE_TBL_INFO_UPDATED)
    {
        CFE_ES_WriteToSysLog("Failed to get table address (errcode %x)", iStatus);
        return iStatus;
    }

    // CFE_ES_WriteToSysLog("Table is at address %p", (void *) g_MPU6050_AppData.ConfigTbl);

    /* Nofity us when the table needs updates */
    iStatus = CFE_TBL_NotifyByMessage(g_MPU6050_AppData.ConfigTblHandle, MPU6050_SEND_HK_MID, 0, 0);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Failed to set up table management (%x)", iStatus);
        return iStatus;
    }

    return iStatus;
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
**    g_MPU6050_AppData.FileID
**    MPU6050_BUS_FILEPATH
**
** Limitations, Assumptions, External Events, and Notes:
**
** Algorithm:
**
** Author(s):  Jacob Killelea
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 MPU6050_InitDevice(void)
{
    int32 iStatus = CFE_SUCCESS;

    g_MPU6050_AppData.FileID = open(g_MPU6050_AppData.ConfigTbl->devicePath, O_RDWR);
    if (g_MPU6050_AppData.FileID < 0)
    {
        iStatus = CFE_ES_RunStatus_APP_ERROR;
        perror("Failed to open bus path!");
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_EventType_ERROR, "MPU6050 - Failed to open bus path %s\n",
                g_MPU6050_AppData.ConfigTbl->devicePath);
        return iStatus;
    }

    iStatus = ioctl(g_MPU6050_AppData.FileID, I2C_SLAVE, g_MPU6050_AppData.ConfigTbl->deviceI2CAddr);
    if (iStatus < 0)
    {
        iStatus = CFE_ES_RunStatus_APP_ERROR;
        perror("ioctl");
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_EventType_ERROR, "MPU6050 - Ioctl failed to set slave address!\n");
        return iStatus;
    }

    /* Wake device */
    if(MPU6050_write8(g_MPU6050_AppData.FileID, RegPowerManagment1, 0) < 2)
    {
        iStatus = CFE_ES_RunStatus_APP_ERROR;
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_EventType_ERROR, "MPU6050 - Failed to send wakeup command to device!\n");
        return iStatus;
    }

    /* +/- 2g */
    if(MPU6050_write8(g_MPU6050_AppData.FileID, RegAccelConfig, g_MPU6050_AppData.ConfigTbl->initialAccelScale) < 2)
    {
        iStatus = CFE_ES_RunStatus_APP_ERROR;
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_EventType_ERROR,
                "MPU6050 - Failed to set accelerometer sensitivity!\n");
        return iStatus;
    }

    /* +/- 250 deg/s */
    if(MPU6050_write8(g_MPU6050_AppData.FileID, RegGyroConfig, g_MPU6050_AppData.ConfigTbl->initialGyroScale) < 2)
    {
        iStatus = CFE_ES_RunStatus_APP_ERROR;
        CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_EventType_ERROR,
                "MPU6050 - Failed to set gyro sensitivity!\n");
        return iStatus;
    }

    return iStatus;
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
**
** Algorithm:
**
** Author(s):  Jacob Killelea
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 MPU6050_InitApp()
{
    int32 iStatus = CFE_SUCCESS;

    g_MPU6050_AppData.uiRunStatus = CFE_ES_RunStatus_APP_RUN;

    /* Register with EVS */
    iStatus = MPU6050_InitEvent();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(MPU6050_INIT_ERR_EID, CFE_EVS_EventType_ERROR, "InitEvent failed");
        return iStatus;
    }

    /* Register pies with SB */
    iStatus = MPU6050_InitPipe();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(MPU6050_INIT_ERR_EID, CFE_EVS_EventType_ERROR, "InitPipe failed");
        return iStatus;
    }

    /* Zero input, output, and hk structs */
    iStatus = MPU6050_InitData();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(MPU6050_INIT_ERR_EID, CFE_EVS_EventType_ERROR, "InitData failed");
        return iStatus;
    }

    /* Load configuration table */
    iStatus = MPU6050_InitTable();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(MPU6050_INIT_ERR_EID, CFE_EVS_EventType_ERROR, "InitTable failed");
        return iStatus;
    }

    /* Get the physical device ready */
    iStatus = MPU6050_InitDevice();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(MPU6050_INIT_ERR_EID, CFE_EVS_EventType_ERROR, "InitDevice failed");
        return iStatus;
    }

    /* Install the cleanup callback */
    OS_TaskInstallDeleteHandler(MPU6050_CleanupCallback);

    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(MPU6050_INIT_INF_EID, CFE_EVS_EventType_INFORMATION, "MPU6050 - Application initialized");
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
**
** Algorithm:
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
    if (g_MPU6050_AppData.FileID >= 0)
    {
        close(g_MPU6050_AppData.FileID);
        g_MPU6050_AppData.FileID = -1;
    }
}

/*=====================================================================================
** Name: MPU6050_RcvMsg
**
** Purpose: To receive and process messages for MPU6050 application
**
** Arguments:
**    int32 Timeout - The number of milliseconds to wait before timing out.
**                      CFE_SB_PEND_FOREVER can be used to wait indefinitely for a message
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_MSG_GetMsgId
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
**
** Algorithm:
**
** Author(s):  Jacob Killelea
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 MPU6050_RcvMsg(int32 Timeout)
{
    int32           iStatus = CFE_SUCCESS;
    CFE_SB_Buffer_t*   MsgPtr  = NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MPU6050_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_ReceiveBuffer(&MsgPtr, g_MPU6050_AppData.SchPipeId, Timeout);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MPU6050_MAIN_TASK_PERF_ID);

    /* New data to process */
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_MSG_GetMsgId(&MsgPtr->Msg, &MsgId);
        if (iStatus != CFE_SUCCESS)
        {
                CFE_EVS_SendEvent(MPU6050_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                        "MPU6050 - Could not get message ID (0x%08X)", iStatus);
        }

        switch (MsgId)
        {
            case MPU6050_WAKEUP_MID:
                MPU6050_ProcessNewCmds();
                MPU6050_ProcessNewData();
                break;

            /* Add more cases here */

            default:
                CFE_EVS_SendEvent(MPU6050_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
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
        g_MPU6050_AppData.uiRunStatus = CFE_ES_RunStatus_APP_ERROR;
        CFE_EVS_SendEvent(MPU6050_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
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
**    CFE_MSG_GetMsgId
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
**
** Algorithm:
**
** Author(s):  Jacob Killelea
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void MPU6050_ProcessNewData()
{
    int            iStatus   = CFE_SUCCESS;
    CFE_SB_Buffer_t*  TlmMsgPtr = NULL;
    CFE_SB_MsgId_t TlmMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_ReceiveBuffer(&TlmMsgPtr, g_MPU6050_AppData.TlmPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = CFE_MSG_GetMsgId(&TlmMsgPtr->Msg, &TlmMsgId);
            if (iStatus == CFE_SUCCESS)
            {
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
                        CFE_EVS_SendEvent(MPU6050_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                                "MPU6050 - Recvd invalid TLM msgId (0x%08X)", TlmMsgId);
                        break;
                }
            }
            else
            {
                    CFE_EVS_SendEvent(MPU6050_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                                      "MPU6050 - Could not get TLM msgId (0x%08X)", iStatus);
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(MPU6050_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                  "MPU6050: CMD pipe read error (0x%08X)", iStatus);
            g_MPU6050_AppData.uiRunStatus = CFE_ES_RunStatus_APP_ERROR;
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
**    CFE_MSG_GetMsgId
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
**
** Algorithm:
**
** Author(s):  Jacob Killelea
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void MPU6050_ProcessNewCmds()
{
    int            iStatus   = CFE_SUCCESS;
    CFE_SB_Buffer_t*  CmdMsgPtr = NULL;
    CFE_SB_MsgId_t CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_ReceiveBuffer(&CmdMsgPtr, g_MPU6050_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            iStatus = CFE_MSG_GetMsgId(&CmdMsgPtr->Msg, &CmdMsgId);
            if (iStatus == CFE_SUCCESS)
            {
                switch (CmdMsgId)
                {
                    case MPU6050_CMD_MID:
                        MPU6050_ProcessNewAppCmds(&CmdMsgPtr->Msg);
                        break;

                    case MPU6050_SEND_HK_MID:
                        if (CFE_TBL_Manage(g_MPU6050_AppData.ConfigTblHandle) != CFE_SUCCESS)
                        {
                            CFE_EVS_SendEvent(MPU6050_ILOAD_ERR_EID, CFE_EVS_EventType_ERROR,
                                    "Failed to manage table!");
                        }
                        MPU6050_ReportHousekeeping();
                        break;

                    default:
                        CFE_EVS_SendEvent(MPU6050_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                                "MPU6050 - Recvd invalid CMD msgId (0x%08X)", CmdMsgId);
                        break;
                }
            }
            else
            {
                CFE_EVS_SendEvent(MPU6050_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                        "MPU6050 - Could not get CMD msgId (0x%08X)", iStatus);
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(MPU6050_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                  "MPU6050: CMD pipe read error (0x%08X)", iStatus);
            g_MPU6050_AppData.uiRunStatus = CFE_ES_RunStatus_APP_ERROR;
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
**
** Algorithm:
**
** Author(s):  Jacob Killelea
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void MPU6050_ProcessNewAppCmds(CFE_MSG_Message_t *MsgPtr)
{
    CFE_MSG_FcnCode_t uiCmdCode = 0;

    if (MsgPtr != NULL)
    {
        CFE_MSG_GetFcnCode(MsgPtr, &uiCmdCode);
        switch (uiCmdCode)
        {
            case MPU6050_NOOP_CC:
                g_MPU6050_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(MPU6050_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                                  "MPU6050 - Recvd NOOP cmd (%d)", uiCmdCode);
                break;

            case MPU6050_RESET_APP_CC:
                g_MPU6050_AppData.HkTlm.usCmdCnt = 0;
                g_MPU6050_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_EVS_SendEvent(MPU6050_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                                  "MPU6050 - Recvd RESET cmd (%d)", uiCmdCode);
                break;

            case MPU6050_RESET_DEVICE_CC:
                MPU6050_ResetDevice();
                break;

            case MPU6050_SET_DEVICE_ACCELEROMETER_SCALE_2G_CC:
                g_MPU6050_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(MPU6050_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                                  "MPU6050 - Setting accelerometer scale to +/- 2g");
                g_MPU6050_AppData.ConfigTbl->initialAccelScale = MPU6050_ACCELSCALE_2G;
                MPU6050_write8(g_MPU6050_AppData.FileID, RegAccelConfig, MPU6050_ACCELSCALE_2G);
                break;

            case MPU6050_SET_DEVICE_ACCELEROMETER_SCALE_4G_CC:
                g_MPU6050_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(MPU6050_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                                  "MPU6050 - Setting accelerometer scale to +/- 4g");
                g_MPU6050_AppData.ConfigTbl->initialAccelScale = MPU6050_ACCELSCALE_4G;
                MPU6050_write8(g_MPU6050_AppData.FileID, RegAccelConfig, MPU6050_ACCELSCALE_4G);
                break;

            case MPU6050_SET_DEVICE_ACCELEROMETER_SCALE_8G_CC:
                g_MPU6050_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(MPU6050_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                                  "MPU6050 - Setting accelerometer scale to +/- 8g");
                g_MPU6050_AppData.ConfigTbl->initialAccelScale = MPU6050_ACCELSCALE_8G;
                MPU6050_write8(g_MPU6050_AppData.FileID, RegAccelConfig, MPU6050_ACCELSCALE_8G);
                break;

            case MPU6050_SET_DEVICE_ACCELEROMETER_SCALE_16G_CC:
                g_MPU6050_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(MPU6050_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                                  "MPU6050 - Setting accelerometer scale to +/- 16g");
                g_MPU6050_AppData.ConfigTbl->initialAccelScale = MPU6050_ACCELSCALE_16G;
                MPU6050_write8(g_MPU6050_AppData.FileID, RegAccelConfig, MPU6050_ACCELSCALE_16G);
                break;

            case MPU6050_SET_DEVICE_GYRO_SCALE_250DPS_CC:
                g_MPU6050_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(MPU6050_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                                  "MPU6050 - Setting gyroscope scale to +/- 250 degs/s");
                g_MPU6050_AppData.ConfigTbl->initialGyroScale = MPU6050_GYROSCALE_250DPS;
                MPU6050_write8(g_MPU6050_AppData.FileID, RegGyroConfig, MPU6050_GYROSCALE_250DPS);
                break;

            case MPU6050_SET_DEVICE_GYRO_SCALE_500DPS_CC:
                g_MPU6050_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(MPU6050_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                                  "MPU6050 - Setting gyroscope scale to +/- 500 degs/s");
                g_MPU6050_AppData.ConfigTbl->initialGyroScale = MPU6050_GYROSCALE_500DPS;
                MPU6050_write8(g_MPU6050_AppData.FileID, RegGyroConfig, MPU6050_GYROSCALE_500DPS);
                break;

            case MPU6050_SET_DEVICE_GYRO_SCALE_1000DPS_CC:
                g_MPU6050_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(MPU6050_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                                  "MPU6050 - Setting gyroscope scale to +/- 1000 degs/s");
                g_MPU6050_AppData.ConfigTbl->initialGyroScale = MPU6050_GYROSCALE_1000DPS;
                MPU6050_write8(g_MPU6050_AppData.FileID, RegGyroConfig, MPU6050_GYROSCALE_1000DPS);
                break;

            case MPU6050_SET_DEVICE_GYRO_SCALE_2000DPS_CC:
                g_MPU6050_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(MPU6050_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                                  "MPU6050 - Setting gyroscope scale to +/- 2000 degs/s");
                g_MPU6050_AppData.ConfigTbl->initialGyroScale = MPU6050_GYROSCALE_2000DPS;
                MPU6050_write8(g_MPU6050_AppData.FileID, RegGyroConfig, MPU6050_GYROSCALE_2000DPS);
                break;

            /* TODO:  Add code to process the rest of the MPU6050 commands here */

            default:
                g_MPU6050_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(MPU6050_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
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
**
** Algorithm:
**
** Author(s):  GSFC, Jacob Killelea
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void MPU6050_ReportHousekeeping()
{
    /* TODO:  Add code to update housekeeping data, if needed, here.  */

    CFE_SB_TimeStampMsg((CFE_MSG_Message_t*) &g_MPU6050_AppData.HkTlm);
    CFE_SB_TransmitMsg((CFE_MSG_Message_t*)  &g_MPU6050_AppData.HkTlm, true);
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
**    CFE_SB_TimeStampMsg
**    CFE_SB_SendMsg
**
** Called By:
**    MPU6050_RcvMsg
**
** Global Inputs/Reads:
**    g_MPU6050_AppData.OutData
**
** Global Outputs/Writes:
**    g_MPU6050_AppData.OutData.uiCounter
**
** Limitations, Assumptions, External Events, and Notes:
** 1: Does not account for uiCounter rollover
**
** Algorithm:
**
** Author(s):  Jacob Killelea
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void MPU6050_SendOutData()
{
    g_MPU6050_AppData.OutData.uiCounter++;

    CFE_SB_TimeStampMsg((CFE_MSG_Message_t*) &g_MPU6050_AppData.OutData);
    CFE_SB_TransmitMsg((CFE_MSG_Message_t*)  &g_MPU6050_AppData.OutData, true);
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
**
** Algorithm:
**
** Author(s):  Jacob Killelea
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
bool MPU6050_VerifyCmdLength(CFE_MSG_Message_t* MsgPtr, uint16 expectedLen)
{
    bool              bResult = true;
    CFE_Status_t      iStatus = CFE_SUCCESS;
    CFE_SB_MsgId_t    msgId   = 0;
    CFE_MSG_FcnCode_t fcnCode = 0;


    if (MsgPtr != NULL)
    {
        CFE_MSG_Size_t msgSize;
        iStatus = CFE_MSG_GetSize(MsgPtr, &msgSize);

        if (iStatus == CFE_SUCCESS)
        {
            if (expectedLen != msgSize)
            {
                bResult = false;

                iStatus = CFE_MSG_GetMsgId(MsgPtr, &msgId);
                if (iStatus != CFE_SUCCESS)
                {
                    CFE_EVS_SendEvent(MPU6050_MSGLEN_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to get message ID!");
                }

                iStatus = CFE_MSG_GetFcnCode(MsgPtr, &fcnCode);
                if (iStatus != CFE_SUCCESS)
                {
                    CFE_EVS_SendEvent(MPU6050_MSGLEN_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to get function code!");
                }

                CFE_EVS_SendEvent(MPU6050_MSGLEN_ERR_EID, CFE_EVS_EventType_ERROR,
                        "MPU6050 - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                        "msgLen=%zu, expectedLen=%d",
                        msgId, fcnCode, msgSize, expectedLen);

                g_MPU6050_AppData.HkTlm.usCmdErrCnt++;
            }
        }
        else
        {
            CFE_EVS_SendEvent(MPU6050_MSGLEN_ERR_EID, CFE_EVS_EventType_ERROR, "Failed to get message size!");
        }
    }
    else
    {
        CFE_EVS_SendEvent(MPU6050_MSGLEN_ERR_EID, CFE_EVS_EventType_ERROR, "MsgPtr is null!");
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
**
** Algorithm:
**
** Author(s):  Jacob Killelea
**
** History:  Date Written  2019-10-22
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void MPU6050_AppMain()
{
    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(MPU6050_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (MPU6050_InitApp() != CFE_SUCCESS)
    {
        g_MPU6050_AppData.uiRunStatus = CFE_ES_RunStatus_APP_ERROR;
    }
    else
    {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(MPU6050_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(MPU6050_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(MPU6050_MAIN_TASK_PERF_ID);
    }

    /* Application main loop */
    while (CFE_ES_RunLoop(&g_MPU6050_AppData.uiRunStatus) == true)
    {

        MPU6050_RcvMsg(1000 / MPU6050_SAMPLE_RATE_HZ);

        /* Read from MPU6050 and send out data */
        float readingAccelX =
            ((float) (int16) MPU6050_read16(g_MPU6050_AppData.FileID, RegAccelX)) / INT16_MAX;
        float readingAccelY =
            ((float) (int16) MPU6050_read16(g_MPU6050_AppData.FileID, RegAccelY)) / INT16_MAX;
        float readingAccelZ =
            ((float) (int16) MPU6050_read16(g_MPU6050_AppData.FileID, RegAccelZ)) / INT16_MAX;
        float readingGyroX =
            ((float) (int16) MPU6050_read16(g_MPU6050_AppData.FileID, RegGyroX)) / INT16_MAX;
        float readingGyroY =
            ((float) (int16) MPU6050_read16(g_MPU6050_AppData.FileID, RegGyroY)) / INT16_MAX;
        float readingGyroZ =
            ((float) (int16) MPU6050_read16(g_MPU6050_AppData.FileID, RegGyroZ)) / INT16_MAX;

        g_MPU6050_AppData.OutData.gyroXDegsSec = readingGyroX;
        g_MPU6050_AppData.OutData.gyroYDegsSec = readingGyroY;
        g_MPU6050_AppData.OutData.gyroZDegsSec = readingGyroZ;
        g_MPU6050_AppData.OutData.accelXGees   = readingAccelX;
        g_MPU6050_AppData.OutData.accelYGees   = readingAccelY;
        g_MPU6050_AppData.OutData.accelZGees   = readingAccelZ;

        OS_printf("%f\t%f\t%f\t%f\t%f\t%f\n", readingAccelX, readingAccelY, readingAccelZ,
                                              readingGyroX, readingGyroY, readingGyroZ);

        MPU6050_SendOutData();
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(MPU6050_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(g_MPU6050_AppData.uiRunStatus);
}

/*=======================================================================================
** End of file mpu6050_app.c
**=====================================================================================*/

