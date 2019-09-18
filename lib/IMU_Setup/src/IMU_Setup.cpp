#include "Arduino.h"
#include "IMU_Setup.h"

int status;

void InitiateDualIMU(MPU9250FIFO& IMU1, MPU9250FIFO& IMU2){
// IMU1
  status = IMU1.begin();
  if(status < 0){
    Serial.println("IMU1 initialization unsuccessful");
    Serial.println("Check IMU1 wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
  }
  else{
    Serial.println("");
    Serial.println("IMU1 initialization successful");
  }

  status = IMU1.setAccelRange(MPU9250::ACCEL_RANGE_8G); // Set accelerometer range 
  status = IMU1.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ); // Set lowpass filter Hz_cut 
  status = IMU1.calibrateAccel();

 // IMU2
   status = IMU2.begin();
  if(status < 0){
    Serial.println("IMU2 initialization unsuccessful");
    Serial.println("Check IMU2 wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
  }
  else{
    Serial.println("IMU2 initialization successful");
  }

  status = IMU2.setAccelRange(MPU9250::ACCEL_RANGE_8G); // Set accelerometer range 
  status = IMU2.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ); // Set lowpass filter Hz_cut 
  status = IMU2.calibrateAccel();
}


void ReadDualIMU(MPU9250FIFO& IMU1, MPU9250FIFO& IMU2, float* IMUBuffer1, float* IMUBuffer2){
  IMU1.readSensor();
  IMUBuffer1[0] = IMU1.getAccelX_mss();
  IMUBuffer1[1] = IMU1.getAccelY_mss();
  IMUBuffer1[2] = IMU1.getAccelZ_mss();
  
  IMU2.readSensor();
  IMUBuffer2[0] = IMU2.getAccelX_mss();
  IMUBuffer2[1] = IMU2.getAccelY_mss();
  IMUBuffer2[2] = IMU2.getAccelZ_mss();
}