#ifndef POSITION_PID_H_
#define POSITION_PID_H_

double KP_POSITION = 1;
double KI_POSITION = 1;
double KD_POSITION = 1;

#define SET_POINT_POSITION 30

double rightCumError = 0;
double rightLastError = 0;
double rightRateError = 0;

double leftCumError = 0;
double leftLastError = 0;
double leftRateError = 0;

void rightPid(int posX){
  int speed = computePID(posX, KP_POSITION, KI_POSITION, KD_POSITION, SET_POINT_POSITION, rightCumError, rightLastError);
  goMotor1(speed);
}

void leftPid(int posX){
  int speed = computePID(posX, KP_POSITION, KI_POSITION, KD_POSITION, SET_POINT_POSITION, leftCumError, leftLastError);
  goMotor2(speed);
}

void positionPid(int posX){
	int dif = SET_POINT_POSITION - posX;
	if(dif > 0){
		rightPid(dif);
	}
	else{
		leftPid(abs(dif));
	}
}
#endif