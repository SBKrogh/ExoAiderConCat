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
        void BeginIMU();

        std::vector<float> GetSensorDataSerial();
        std::vector<uint8_t> GetSensorDataBT();

    protected:
        
    private:
        // uint8_t float2int();
        void float2uint8(float FloatToBeConverted);
        std::vector<uint8_t> _ReinterpretedValue = {0};

        char int2char(int Value);
        std::vector<uint8_t> _DataBufferBT;
        std::vector<float> _DataBufferSerial;
        uint8_t _ArrayBufferBT[];
        
        std::string _task, _str;
        bool _run;
};

#endif