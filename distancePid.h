#ifndef DISTANCE_PID_H_
#define DISTANCE_PID_H_

#include "pid.h"
double KP_DISTANCE = 2;
double KI_DISTANCE = 0.001;
double KD_DISTANCE = 0.001;

#define SET_POINT_DISTANCE 70


double distanceCumError = 0;
double distanceLastError = 0;
double distanceRateError = 0;


void distancePid(int radius){
  int speed = computePID(radius, KP_DISTANCE, KI_DISTANCE, KD_DISTANCE, SET_POINT_DISTANCE, distanceCumError, distanceLastError);
  
  goMotor1(speed);
  goMotor2(speed);
  
}


#endif