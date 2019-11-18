#include <Arduino.h>
#include "BluetoothSerial.h" 
#include "TaskBT2.h"

#define CS_IMU1 33
#define CS_IMU2 15
#define CS_DAC 32
#define CS_ADC 14

BluetoothSerial ESP_BT; //Object for Bluetooth

TaskBT2 Task(CS_IMU1, CS_IMU2, CS_DAC);   // pins for IMUs

std::vector<uint8_t> DataBufferBT;

void setup() { 
  Serial.begin(9600); //Start Serial monitor in 9600
  Serial.println(" ");
  Serial.println(" ");
  Task.BeginIMU();     // Initiate IMU
  Task.BeginDAC();     // Initiate DAC


  ESP_BT.begin("Exo-Aider ESP32"); //Name of Bluetooth Device
  Serial.println("Bluetooth Device is Ready to Pair");

  pinMode (LED_BUILTIN, OUTPUT); //Specify that LED pin is output

  // Example: set DAC voltage channel [0 - 7] - input: [Channel, Voltage]
  Task.SetDACVoltaget(0,0); 
  Task.SetDACVoltaget(1,1); 
  Task.SetDACVoltaget(2,2);
  Task.SetDACVoltaget(3,3);
  Task.SetDACVoltaget(4,4);
  Task.SetDACVoltaget(5,5);
  Task.SetDACVoltaget(6,4);
  Task.SetDACVoltaget(7,3);

  while(ESP_BT.hasClient() == false){digitalWrite(LED_BUILTIN, LOW);} // Wait until a connection is established
  Serial.println("Device Connected"); 
}


void loop() {
  while(ESP_BT.hasClient() == false){digitalWrite(LED_BUILTIN, LOW);} // Check for client, if non, wait
  digitalWrite(LED_BUILTIN, HIGH);

  if (ESP_BT.available())             // Check for received task over Bluetooth
  {
    Task.SetTask(ESP_BT.read());      // Check task to execute
  }

  if (Task.RunTask()){                // Check if task exsists         
    Task.ExecuteTask();               // Execute task 
    DataBufferBT = Task.GetSensorDataBT(); // Get task/sensor data
    ESP_BT.write(DataBufferBT.data(), DataBufferBT.size()); // Send data over Bluetooth
    delay(1);
  }
}

