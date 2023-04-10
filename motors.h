#ifndef MOTORS_H_
#define MOTORS_H_

#include "Arduino.h"

#define MOTOR1_DIR1 12 //in1
#define MOTOR1_DIR2 14 //in2
#define MOTOR1_SIGNAL 13 //ena

#define MOTOR2_DIR1 15 //in3
#define MOTOR2_DIR2 16 //in4
#define MOTOR2_SIGNAL 2 //enb


void initMotors(){
  pinMode(MOTOR1_DIR1, OUTPUT);
  pinMode(MOTOR1_DIR2, OUTPUT);
  pinMode(MOTOR1_SIGNAL, OUTPUT);

  
  pinMode(MOTOR2_DIR1, OUTPUT);
  pinMode(MOTOR2_DIR2, OUTPUT);
  pinMode(MOTOR2_SIGNAL, OUTPUT);

}



void forwardMotor1(){  
	digitalWrite(MOTOR1_DIR1, LOW);
	digitalWrite(MOTOR1_DIR2, HIGH);
}

void backwardMotor1(){  

	digitalWrite(MOTOR1_DIR1, HIGH);
	digitalWrite(MOTOR1_DIR2, LOW);
}

void forwardMotor2(){  
	digitalWrite(MOTOR2_DIR1, LOW);
	digitalWrite(MOTOR2_DIR2, HIGH);
}

void backwardMotor2(){  
  digitalWrite(MOTOR2_DIR1, HIGH);
	digitalWrite(MOTOR2_DIR2, LOW);
}

void runMotor1(int speed){
  analogWrite(MOTOR1_SIGNAL, speed);
}

void runMotor2(int speed){
  analogWrite(MOTOR2_SIGNAL, speed);
}


void goForwardMotor1(int speed){
  forwardMotor1();
  runMotor1(speed);
}

void goForwardMotor2(int speed){
  forwardMotor2();
  runMotor2(speed);
}

void goBackwardMotor1(int speed){
  backwardMotor1();
  runMotor1(speed);
}

void goBackwardMotor2(int speed){
  backwardMotor2();
  runMotor2(speed);
}

void goMotor1(int speed){
  if(speed < 0)
    goBackwardMotor1(abs(speed));
  else 
    goForwardMotor1(speed);
}

void goMotor2(int speed){
  if(speed < 0)
    goBackwardMotor2(abs(speed));
  else 
    goForwardMotor2(speed);
}

#endif