#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "cfe.h"
#include "cfe_evs.h"
#include "cfe_psp.h"
#include "mpu6050_app.h"

/* Read a 16 bit register */
uint16 MPU6050_read16(int fd, uint8 reg)
{
    uint8 buffer[2];
    buffer[0] = reg;
    write(fd, buffer, 1);                /* Select register     */
    read(fd, buffer, 2);                 /* Read 16 bits        */
    return (buffer[0] << 8) | buffer[1]; /* Return (big endian) */
}

/* Write an 8 bit register */
uint32 MPU6050_write8(int fd, uint8 reg, uint8 val)
{
    uint8 buffer[2] = {reg, val}; /* 8 bit register addr and 8 bit data */
    return write(fd, buffer, 2);
}

/* Write a 16 bit register */
uint32 MPU6050_write16(int fd, uint8 reg, uint8 val1, uint8 val2)
{
    uint8 buffer[3] = {reg, val1, val2}; /* 8 bit register addr and 16 bit data */
    return write(fd, buffer, 3);
}

/* Read a buffer of arbitrary size from the chip */
uint32 MPU6050_ReadArbitrary(int fd, uint8 startingAddr, uint8 *buffer, uint32 bufferLen)
{
    if (buffer == NULL)
    {
        return 0;
    }

    CFE_PSP_MemSet(buffer, 0, bufferLen);

    /* Select Register */
    buffer[0] = startingAddr;
    write(fd, buffer, 1);

    /* Read to fill buffer */
    return read(fd, buffer, bufferLen);
}

int32 MPU6050_SetAccelScale(MPU6050_AcceleormeterScale_t scale)
{
    CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_EventType_ERROR, "Unimpliemented!");
    return -1;
}

int32 MPU6050_SetGyroScale(MPU6050_GyroScale_t scale)
{
    CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_EventType_ERROR, "Unimpliemented!");
    return -1;
}

int32 MPU6050_ResetDevice(void)
{
    CFE_EVS_SendEvent(MPU6050_DEVICE_ERR_EID, CFE_EVS_EventType_ERROR, "Unimpliemented!");
    return -1;
}
