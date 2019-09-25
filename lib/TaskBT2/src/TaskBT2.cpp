#include "Arduino.h"
#include "TaskBT2.h"
#include <iostream>
#include <vector>
//#include <IMU_Setup.h>

//////////////////////////////////////////////////////////
            ////////// Private //////////
//////////////////////////////////////////////////////////


/* reinterprets the received bluetooth integer value to a char */
char TaskBT2::int2char(int Value){  
    int* p = new int(Value); 
    char* ch = reinterpret_cast<char*>(p); 
    return *ch;
}

/* reinterprets IMU float to four byte uint8_t for BT communication */
void TaskBT2::float2uint8(float FloatToBeConverted){
    uint8_t *_array;  
    _array = reinterpret_cast<uint8_t*>(&FloatToBeConverted);
    _ReinterpretedValue.erase(_ReinterpretedValue.begin(),_ReinterpretedValue.end());
    for (size_t i = 0; i < 4; i++)
    {
        _ReinterpretedValue.push_back(_array[i]);
    }
}

//////////////////////////////////////////////////////////
            ////////// Public //////////
//////////////////////////////////////////////////////////

/* Initialize the IMU */
void TaskBT2::BeginIMU(){
    _IMU1.begin(); // Initiate IMU 1
    _IMU2.begin(); // Initiate IMU 2
}

/* Receive the task specified by the master */
void TaskBT2::SetTask(uint8_t ValueBT){     
    char c = int2char(ValueBT);                     // Change uint8_t to human readable data 
    _str.push_back(c);                              // Store char in vector (temporary data) 
    std::size_t found = _str.find("//");            // Check for stop character
    if(found != std::string::npos){                 // If stop character found == true
        _task.erase(_task.begin(),_task.end());     // Erase previous task
        _task = _str.substr(0,found);               // Specify new task 
        _str.erase(_str.begin(),_str.end());        // Erase temporary data

        cout << "Task found: " << _task << endl;    // Print new task received
        _run = true;                                // Raise flag to execute specifig task 
    }
}

/* Return true if a task should be executed */
bool TaskBT2::RunTask(){                    
    return _run;                           
}

/* Execute the task specified by the Master */
void TaskBT2::ExecuteTask(){
    
    // Sample IMU data only [IMU1x, IMU1y, IMU1z, IMU2x, IMU2y, IMU2z]
    if(_task.compare("IMU") == 0){ 
        
        _IMU1.readSensor(); // Read IMU 1 data 
        _IMU2.readSensor(); // Read IMU 2 data
        
        _DataBufferBT.erase(_DataBufferBT.begin(),_DataBufferBT.end()); // Clear Bluetooth buffer

        float2uint8(_IMU1.getAccelX_mss());
        _DataBufferBT.insert(_DataBufferBT.end(), _ReinterpretedValue.begin(), _ReinterpretedValue.end());   // Store reinterpreted data in Bluetooth buffer
        float2uint8(_IMU1.getAccelY_mss());                                                                  // Reinterprets IMU data float to uint8_t data 
        _DataBufferBT.insert(_DataBufferBT.end(), _ReinterpretedValue.begin(), _ReinterpretedValue.end());   // Store reinterpreted data in Bluetooth buffer
        float2uint8(_IMU1.getAccelZ_mss());                                                                  // Reinterprets IMU data float to uint8_t data 
        _DataBufferBT.insert(_DataBufferBT.end(), _ReinterpretedValue.begin(), _ReinterpretedValue.end());   // Store reinterpreted data in Bluetooth buffer
        float2uint8(_IMU2.getAccelX_mss());                                                                  // Reinterprets IMU data float to uint8_t data 
        _DataBufferBT.insert(_DataBufferBT.end(), _ReinterpretedValue.begin(), _ReinterpretedValue.end());   // Store reinterpreted data in Bluetooth buffer
        float2uint8(_IMU2.getAccelY_mss());                                                                  // Reinterprets IMU data float to uint8_t data 
        _DataBufferBT.insert(_DataBufferBT.end(), _ReinterpretedValue.begin(), _ReinterpretedValue.end());   // Store reinterpreted data in Bluetooth buffer
        float2uint8(_IMU2.getAccelZ_mss());                                                                  // Reinterprets IMU data float to uint8_t data 
        _DataBufferBT.insert(_DataBufferBT.end(), _ReinterpretedValue.begin(), _ReinterpretedValue.end());   // Store reinterpreted data in Bluetooth buffer

        _package_counter = _package_counter + 1;
        return;
        }
    
    // Sample FSR data only [FSR1, FSR2, FSR3, FSR4, FSR5, FSR6, FSR7, FSR8]
    if(_task.compare("FSR") == 0){return;}
    // Sample EMG data only [EMG1, EMG2, EMG3, EMG4]
    if(_task.compare("EMG") == 0){return;}
    // Samples EMG, FSR and IMU data [EMG, FSR, IMU]
    if(_task.compare("All") == 0){return;}
    // Stops data collection 
    if(_task.compare("Stop") == 0){_run = false; Serial.println(_package_counter); _package_counter = 0; return;}

    cout << "The task did not exsist" << endl;
    _run = false;
    
    return;
}



/* Return DataBuffer if Serial communication is used */
std::vector<float> TaskBT2::GetSensorDataSerial(){
        _IMU1.readSensor(); // Read IMU 1 data 
        _IMU2.readSensor(); // Read IMU 2 data
        _DataBufferSerial.erase(_DataBufferSerial.begin(),_DataBufferSerial.end());
        _DataBufferSerial.push_back(_IMU1.getAccelX_mss());           // Get IMU1 XYZ
        _DataBufferSerial.push_back(_IMU1.getAccelY_mss());
        _DataBufferSerial.push_back(_IMU1.getAccelZ_mss());
        _DataBufferSerial.push_back(_IMU2.getAccelX_mss());           // Get IMU2 XYZ
        _DataBufferSerial.push_back(_IMU2.getAccelY_mss());
        _DataBufferSerial.push_back(_IMU2.getAccelZ_mss());
    return _DataBufferSerial;
}


std::vector<uint8_t> TaskBT2::GetSensorDataBT(){
    return _DataBufferBT;
}

