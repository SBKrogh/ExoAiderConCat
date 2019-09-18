#ifndef TaskBT2_h
#define TaskBT2_h

#include <iostream>
#include "BluetoothSerial.h" 
#include <MPU9250.h>
#include <SPI.h>


using namespace std; 

class TaskBT2
{
    public:
        MPU9250FIFO _IMU1, _IMU2;
        TaskBT2(int CS_IMU1, int CS_IMU2)
            :_IMU1(SPI,CS_IMU1), _IMU2(SPI,CS_IMU2){} // Constructors and member initializer lists
        
        void SetTask(uint8_t ValueBT);
        bool RunTask();
        void ExecuteTask();
        void printIMU();
        void BeginIMU();
        std::vector<float> GetSensorData();

        
        

    protected:
        
    private:
        char int2char(int Value);
        std::vector<float> _DataBuffer;
        std::string _task, _str;
        //std::vector<float> _IMUBuffer;
        bool _run;
        // float _IMUBuffer1[3], _IMUBuffer2[3]; 
        

};

#endif