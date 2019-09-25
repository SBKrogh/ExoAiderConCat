#include "MPU9250.h"

void InitiateDualIMU(MPU9250FIFO& IMU1, MPU9250FIFO& IMU2);
void ReadDualIMU(MPU9250FIFO& IMU1, MPU9250FIFO& IMU2, float* IMUBuffer1, float* IMUBuffer2);
