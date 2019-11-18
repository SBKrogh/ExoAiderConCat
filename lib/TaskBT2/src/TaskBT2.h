#ifndef TaskBT2_h
#define TaskBT2_h

#include <iostream>
#include "BluetoothSerial.h" 
#include <MPU9250.h>
#include <SPI.h>
#include <MAX525.h>


using namespace std; 

class TaskBT2
{
    public:
        MAX525 _DAC;
        MPU9250FIFO _IMU1, _IMU2;
        TaskBT2(uint8_t CS_IMU1, uint8_t CS_IMU2, uint8_t CS_DAC)
            :_IMU1(SPI,CS_IMU1), _IMU2(SPI,CS_IMU2)   // Constructors and member initializer lists
            ,_DAC(SPI,CS_DAC){} 
        
        void SetTask(uint8_t ValueBT);
        bool RunTask();
        void ExecuteTask();
        void BeginIMU();
        void BeginDAC();
        void SetDACVoltaget(uint8_t Channel, float Voltage);



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
        uint32_t _package_counter = 0;
};

#endif