#include <Arduino.h>
#include "BluetoothSerial.h" 
// #include "IMU_Setup.h"
// #include "MPU9250.h"
#include "TaskBT2.h"



BluetoothSerial ESP_BT; //Object for Bluetooth
TaskBT2 Task(14, 32);
//MPU9250FIFO IMU1(SPI,14);
//MPU9250FIFO IMU2(SPI,32);

uint8_t incoming[32];
uint8_t dummy;

std::vector<float> DataBuffer;

float IMUBuffer1[3], IMUBuffer2[3]; 
float ax[2], ay[2], az[2];



void setup() {
  Serial.begin(9600); //Start Serial monitor in 9600
  ESP_BT.begin("Exo-Aider ESP32"); //Name of your Bluetooth Device
  Serial.println("Bluetooth Device is Ready to Pair");
  // InitiateDualIMU(IMU1, IMU2);

  pinMode (LED_BUILTIN, OUTPUT);//Specify that LED pin is output
  //while(ESP_BT.hasClient() == false){digitalWrite(LED_BUILTIN, LOW);} // Wait until a connection is present
  Serial.println("Device Connected");

  for (size_t i = 0; i < 32; i++)     // Random for loop for testing 
  {
    if (i % 2 == 0){
      incoming[i] = 0;
    } else{
      incoming[i] = i;
    }
  }
  Task.BeginIMU();
}


void loop() {

    
  while(ESP_BT.hasClient() == false){digitalWrite(LED_BUILTIN, LOW);} // Check for client
  digitalWrite(LED_BUILTIN, HIGH);

  if (ESP_BT.available())             // Check if we receive anything from Bluetooth
  {
    Task.SetTask(ESP_BT.read());      // Check task to execute
  }

  if (Task.RunTask()){
    Task.ExecuteTask();
    DataBuffer = Task.GetSensorData();

    for (auto& i: DataBuffer)
      std::cout << i << ' ';
    std::cout << endl;
  }


//if (incoming == 1){


  // ReadDualIMU(IMU1, IMU2, IMUBuffer1, IMUBuffer2);
  // Serial.print(IMUBuffer1[0]);
  // Serial.print("  ");
  // Serial.print(IMUBuffer1[1]);
  // Serial.print("  ");
  // Serial.print(IMUBuffer1[2]);
  // Serial.print("  ");
  // Serial.print(IMUBuffer2[0]);
  // Serial.print("  ");
  // Serial.print(IMUBuffer2[1]);
  // Serial.print("  ");
  // Serial.print(IMUBuffer2[2]);
  // Serial.println("  ");
  // delay(100);
//}

// if (incoming == 1){
//   ReadDualIMU(IMU1, IMU2, IMUBuffer1, IMUBuffer2);
//   ESP_BT.print(IMUBuffer1[0]);
//   ESP_BT.print(",");
//   ESP_BT.print(IMUBuffer1[1]);
//   ESP_BT.print(",");
//   ESP_BT.print(IMUBuffer1[2]);
//   ESP_BT.print(",");
//   ESP_BT.print(IMUBuffer2[0]);
//   ESP_BT.print(",");
//   ESP_BT.print(IMUBuffer2[1]);
//   ESP_BT.print(",");
//   ESP_BT.print(IMUBuffer2[2]);
//   ESP_BT.println(",");
//   delay(1);
// }

}

// void loop() {
  
//   if (ESP_BT.available()) //Check if we receive anything from Bluetooth
//   {
//     Serial.println("");
//     incoming = ESP_BT.read(); //Read what we recevive 
//     Serial.print("Received:"); Serial.println(incoming);

//     if (incoming == 49)
//         {
//         digitalWrite(LED_BUILTIN, HIGH);
//         ESP_BT.println("LED turned ON");
//         }
        
//     if (incoming == 48)
//         {
//         digitalWrite(LED_BUILTIN, LOW);
//         ESP_BT.println("LED turned OFF");
//         }     
//   }
//   delay(20);
// }