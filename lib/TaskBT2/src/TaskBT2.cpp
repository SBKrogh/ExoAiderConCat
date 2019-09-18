#include "Arduino.h"
#include "TaskBT2.h"
#include <iostream>
#include <vector>
#include <IMU_Setup.h>


// Private
char TaskBT2::int2char(int Value){ // reinterprets the received integer value to a char 
    int* p = new int(Value); 
    char* ch = reinterpret_cast<char*>(p); 
    return *ch;
}

// Public
void TaskBT2::BeginIMU(){
    _IMU1.begin();
    _IMU2.begin();
}

void TaskBT2::SetTask(uint8_t ValueBT){     // Setting the task of interest
    char c = int2char(ValueBT);
    _str.push_back(c);
    std::size_t found = _str.find("//");
    if(found != std::string::npos){
        _task.erase(_task.begin(),_task.end());
        _task = _str.substr(0,found);
        _str.erase(_str.begin(),_str.end());

        cout << "Task found: " << _task << endl;
        _run = true;
    }
}

bool TaskBT2::RunTask(){                    // Check for executable task 
    return _run;
}

void TaskBT2::ExecuteTask(){
    
    // Sample IMU data only [IMU1x, IMU1y, IMU1z, IMU2x, IMU2y, IMU2z]
    if(_task.compare("IMU") == 0){
        
        _IMU1.readSensor(); // Read IMU data 
        _IMU2.readSensor(); // Read IMU data
        
        _DataBuffer.erase(_DataBuffer.begin(),_DataBuffer.end()); // Clear IMU buffer 

        _DataBuffer.push_back(_IMU1.getAccelX_mss());           // Get IMU1 XYZ
        _DataBuffer.push_back(_IMU1.getAccelY_mss());
        _DataBuffer.push_back(_IMU1.getAccelZ_mss());
        _DataBuffer.push_back(_IMU2.getAccelX_mss());           // Get IMU2 XYZ
        _DataBuffer.push_back(_IMU2.getAccelY_mss());
        _DataBuffer.push_back(_IMU2.getAccelZ_mss());
        return;}
    
    // Sample FSR data only [FSR1, FSR2, FSR3, FSR4, FSR5, FSR6, FSR7, FSR8]
    if(_task.compare("FSR") == 0){return;}
    // Sample EMG data only [EMG1, EMG2, EMG3, EMG4]
    if(_task.compare("EMG") == 0){return;}
    // Samples EMG, FSR and IMU data [EMG, FSR, IMU]
    if(_task.compare("All") == 0){return;}
    // Stops data collection 
    if(_task.compare("Stop") == 0){_run = false; return;}

    cout << "The task did not exsist" << endl;
    _run = false;
    
    return;
}

void TaskBT2::printIMU(){
    //cout << _IMUBuffer1;
}

std::vector<float> TaskBT2::GetSensorData(){
    return _DataBuffer;
}