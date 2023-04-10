#ifndef BLUETOOTH_MOTOR_H_
#define BLUETOOTH_MOTOR_H_

#include "BluetoothSerial.h"
#include "positionPid.h"
#include "distancePid.h"
enum bluetoothEnums {DISTANCE_KP_ENUM = 1, DISTANCE_KI_ENUM = 2, DISTANCE_KD_ENUM = 3, DISTANCE_VALUE_ENUM = 4,
  POSITION_KP_ENUM = 5, POSITION_KI_ENUM = 6, POSITION_KD_ENUM = 7, POSITION_VALUE_ENUM = 8, CONFIGRED_ENUM = 9
};


BluetoothSerial SerialBT;

bool isConfigured = false;

void readPidParameters(){
  while (SerialBT.available() >= 2) {
    int key = SerialBT.read();
    int value = SerialBT.read();
    //Serial.println(value);
    double parameter = value*0.01;
    switch(key){
      case DISTANCE_KP_ENUM:
        KP_DISTANCE = parameter;
        
        break;
      case DISTANCE_KI_ENUM:
        KI_DISTANCE = parameter;
        break;
      case DISTANCE_KD_ENUM:
        KD_DISTANCE = parameter;
        break;
      case DISTANCE_VALUE_ENUM:
        break;
      case POSITION_KP_ENUM:
        KP_POSITION = parameter;
        break;
      case POSITION_KI_ENUM:
        KI_POSITION = parameter;
        break;
      case POSITION_KD_ENUM:
        KD_POSITION = parameter;
        break;
      case POSITION_VALUE_ENUM:
        break;
      case CONFIGRED_ENUM:
        isConfigured = true;
        break;
    }
  }

}

#endif