#include <Arduino.h>
#include "BluetoothSerial.h" 
#include "TaskBT2.h"

BluetoothSerial ESP_BT; //Object for Bluetooth
TaskBT2 Task(15, 33);   // pins for IMUs

std::vector<uint8_t> DataBufferBT;

void setup() { 
  Serial.begin(9600); //Start Serial monitor in 9600
  Serial.println(" ");
  Serial.println(" ");
  Task.BeginIMU();    // Initiate IMU


  ESP_BT.begin("Exo-Aider ESP32"); //Name of Bluetooth Device
  Serial.println("Bluetooth Device is Ready to Pair");

  pinMode (LED_BUILTIN, OUTPUT); //Specify that LED pin is output

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

// Test