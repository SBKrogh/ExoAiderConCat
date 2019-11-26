#include <Arduino.h>
#include "BluetoothSerial.h" 
#include "TaskBT2.h"


#define CS_IMU1 15
#define CS_IMU2 33
#define CS_DAC 32
#define CS_ADC 14

BluetoothSerial ESP_BT; //Object for Bluetooth
TaskBT2 Task(CS_IMU1, CS_IMU2, CS_DAC, CS_ADC);   // pins for IMUs
std::vector<uint8_t> DataBufferBT;

float SetVoltage = 0;

void setup() { 

  pinMode (LED_BUILTIN, OUTPUT); //Specify LED pin as output

  Serial.begin(9600); //Start Serial monitor in 9600
  Serial.println("");

  Task.BeginIMU();     // Initiate IMU
  Task.BeginDAC();     // Initiate DAC - Default output is set to zero volt
  Task.BeginADC();     // Initiate ADC
  
  // Example: set DAC voltage channel [0 - 7] - input: [Channel, Voltage] 
  Task.SetDACVoltaget(0, SetVoltage); 
  Task.SetDACVoltaget(1, SetVoltage); 
  Task.SetDACVoltaget(2, SetVoltage);
  Task.SetDACVoltaget(3, SetVoltage);
  Task.SetDACVoltaget(4, SetVoltage);
  Task.SetDACVoltaget(5, SetVoltage);
  Task.SetDACVoltaget(6, SetVoltage);
  Task.SetDACVoltaget(7, SetVoltage);

  // Bluetooth 
  ESP_BT.begin("Exo-Aider ESP32");                                    //Name of Bluetooth Device
  Serial.println("Bluetooth Device is Ready to Pair");
  while(ESP_BT.hasClient() == false){digitalWrite(LED_BUILTIN, LOW);} // Wait until a connection is established
  
  Serial.println("Device Connected"); 
  // Task.setTask(); // Debugging erase this for
}


void loop() {

  while(ESP_BT.hasClient() == false){digitalWrite(LED_BUILTIN, LOW);} // Check for client, if non, wait
  digitalWrite(LED_BUILTIN, HIGH);

  if (ESP_BT.available())                                   // Check for received task over Bluetooth
  {
    Task.SetTask(ESP_BT.read());                            // Check task to execute
  }
  
  if (Task.RunTask()){                                      // Check if task exsists         
    Task.ExecuteTask();                                     // Execute task 
    DataBufferBT = Task.GetSensorDataBT();                  // Get task/sensor data
    ESP_BT.write(DataBufferBT.data(), DataBufferBT.size()); // Send data over Bluetooth
    delay(1);
  }
}

