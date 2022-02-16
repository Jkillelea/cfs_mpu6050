cmake_minimum_required(VERSION 2.6.4)
project(MPU6050 C)

include_directories(fsw/mission_inc)
include_directories(fsw/platform_inc)

aux_source_directory(fsw/src APP_SRC_FILES)

# Create the app module
add_cfe_app(mpu6050 ${APP_SRC_FILES})
add_cfe_tables(mpu6050_table fsw/src/mpu6050_table.c)

# we depend on math library (-lm)
target_link_libraries(mpu6050 m)
