#include "functions.h"
#include "takeImage.h"
#include "motors.h"
#include "pid.h"
#include "distancePid.h"
//#include "BluetoothSerial.h"


void setup() {

  Serial.begin(9600);    

  initCamera();
  initMotors();
  
  //goForwardMotor1(50);
  //goForwardMotor2(50);
}


void loop() {
  readRGBImage();
  distancePid(maxCounter);
  delay(100);
}

