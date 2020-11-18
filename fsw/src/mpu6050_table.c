#include "cfe_tbl_filedef.h"
#include "mpu6050_app.h"

MPU6050_ConfigTbl_t MPU6050_Configuration_Table = {
    .initialAccelScale = MPU6050_ACCELSCALE_2G, // initial accelerometer sensitivity
    .initialGyroScale  = MPU6050_GYROSCALE_250DPS, // initial gyro sensitivity
    .deviceI2CAddr     = MPU6050_DEVICE_ADDR,

/* Linux path to I2C bus */
#ifdef __arm__
    /* Raspberry Pi */
    .devicePath = "/dev/i2c-1", // initial filepath for accelerometer
#else
    /* Laptop */
    .devicePath = "/dev/i2c-1" , // initial filepath for accelerometer
#endif
};

/*
** The macro below identifies:
**    1) the data structure type to use as the table image format
**    2) the name of the table to be placed into the cFE Table File Header
**    3) a brief description of the contents of the file image
**    4) the desired name of the table image binary file that is cFE compatible
*/
CFE_TBL_FILEDEF(MPU6050_Configuration_Table, MPU6050_APP.ConfigTbl, MPU6050 Config Param, mpu6050_table.tbl)
