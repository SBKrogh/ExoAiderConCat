#include "Arduino.h"
#include "TaskBT2.h"
#include <iostream>
#include <vector>

//////////////////////////////////////////////////////////
            ////////// Private //////////
//////////////////////////////////////////////////////////


/* reinterprets the received bluetooth integer value to a char */
char TaskBT2::int2char(int Value){  
    int* p = new int(Value); 
    char* ch = reinterpret_cast<char*>(p); 
    return *ch;
}

/* reinterprets float to four byte uint8_t for BT communication */
void TaskBT2::float2uint8(float FloatToBeConverted){
    uint8_t *_array;                                                                  // uint8_t pointer
    _array = reinterpret_cast<uint8_t*>(&FloatToBeConverted);                         // reinterpret float to four uint8_t 
    _ReinterpretedValue.erase(_ReinterpretedValue.begin(),_ReinterpretedValue.end()); // Erase reinterpret buffer
    for (size_t i = 0; i < 4; i++){
        _ReinterpretedValue.push_back(_array[i]);                                     // store reinterpreted data   
    }
}

//////////////////////////////////////////////////////////
            ////////// Public //////////
//////////////////////////////////////////////////////////

/* Initialixe the ADCs*/
void TaskBT2::BeginADC(){
    Serial.println("Initialize ADC");
    _ADC.Begin();
}

/* Initialize the IMUs */
void TaskBT2::BeginIMU(){
    Serial.println("Initialize IMU");
    Serial.println(_IMU1.begin()); // Initiate IMU 1
    Serial.println(_IMU2.begin()); // Initiate IMU 2
}

/* Initialize DACs */
void TaskBT2::BeginDAC(){
    Serial.println("Initialize DAC");
    Serial.println(_DAC.begin_Daisy());
}

void TaskBT2::SetDACVoltaget(uint8_t Channel, float Voltage){
    _DAC.SetVoltage_Daisy(Channel, Voltage);
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

        return;
        }
    
    // Sample FSR data only [FSR1, FSR2, FSR3, FSR4, FSR5, FSR6, FSR7, FSR8]
    if(_task.compare("FSR") == 0){
        _ADC.noOpDaisy();                                               // Sample ADC channels
        _DataBufferBT.erase(_DataBufferBT.begin(),_DataBufferBT.end()); // Clear Bluetooth buffer
        std::vector<float> FSR = _ADC.ReturnADC_FSR();                  // Get FSR data
       
        for (size_t i = 0; i < FSR.size(); i++)
        {   
            float2uint8(FSR[i]);                                                                                 // Reinterprets IMU data float to uint8_t data 
             _DataBufferBT.insert(_DataBufferBT.end(), _ReinterpretedValue.begin(), _ReinterpretedValue.end());  // Store reinterpreted data in Bluetooth buffer
        }
        return;
        }

    // Sample EMG data only [EMG1, EMG2, EMG3, EMG4]
    if(_task.compare("EMG") == 0){
        _ADC.noOpDaisy();                                               // Sample ADC channels
        _DataBufferBT.erase(_DataBufferBT.begin(),_DataBufferBT.end()); // Clear Bluetooth buffer
        std::vector<float> EMG = _ADC.ReturnADC_EMG();                  // Get EMG data
       
        for (size_t i = 0; i < EMG.size(); i++)
        {   
            float2uint8(EMG[i]);                                                                                 // Reinterprets IMU data float to uint8_t data 
             _DataBufferBT.insert(_DataBufferBT.end(), _ReinterpretedValue.begin(), _ReinterpretedValue.end());  // Store reinterpreted data in Bluetooth buffer
        }
        return;
        }

    // Samples EMG, FSR and IMU data [FSR, EMG, IMU]
    if(_task.compare("All") == 0){

        _DataBufferBT.erase(_DataBufferBT.begin(),_DataBufferBT.end()); // Clear Bluetooth buffer
        _ADC.noOpDaisy();                                               // Sample ADC channels
        _IMU1.readSensor();                                             // Read IMU 1 data 
        _IMU2.readSensor();                                             // Read IMU 2 data

        /*FSR*/
        std::vector<float> FSR = _ADC.ReturnADC_FSR();                  // Get FSR data
        for (size_t i = 0; i < FSR.size(); i++){   
            float2uint8(FSR[i]);                                                                                 // Reinterprets IMU data float to uint8_t data 
             _DataBufferBT.insert(_DataBufferBT.end(), _ReinterpretedValue.begin(), _ReinterpretedValue.end());  // Store reinterpreted data in Bluetooth buffer
        }

        /*EMG*/
        std::vector<float> EMG = _ADC.ReturnADC_EMG();                  // Get EMG data
        for (size_t i = 0; i < EMG.size(); i++){   
            float2uint8(EMG[i]);                                                                                 // Reinterprets IMU data float to uint8_t data 
             _DataBufferBT.insert(_DataBufferBT.end(), _ReinterpretedValue.begin(), _ReinterpretedValue.end());  // Store reinterpreted data in Bluetooth buffer
        }

        /*IMU*/
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

        return;
        }

    // Stops data collection 
    if(_task.compare("Stop") == 0){_run = false; return;}

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

/* Return data for BT communication */
std::vector<uint8_t> TaskBT2::GetSensorDataBT(){
    return _DataBufferBT;
}

/* Set task for testing on MPU side */ 
void TaskBT2::setTask(){
    _task = "All";
    // _task = "IMU";
    // _task = "EMG";
    // _task = "FSR";
}


