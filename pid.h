#ifndef PID_H_
#define PID_H_

unsigned long currentTime = 0, previousTime = 0;
double elapsedTime;
double error;
double rateError=0;

#define MAX_FRAME_YATAY 45


int computePID(int inp, double kp, double ki, double kd, int setPoint, double & cumError, double & lastError){     
  currentTime = millis();
  elapsedTime = (double)(currentTime - previousTime);

  if(lastError * error < 0){
    cumError = 0;
  }


  error = setPoint - inp;   
 // Serial.print("inp: ");         
 // Serial.println(inp);
                           
  cumError += error * elapsedTime;       
  //Serial.print("error: ");         
  //Serial.println(error);
  rateError = (error - lastError)/elapsedTime;   

  int out = kp*error + ki*cumError + kd*rateError;    

  lastError = error;                                
  previousTime = currentTime;  

  //Serial.print("output: ");
    Serial.println(out);
  if(out < 200){
    out = 0;
  }
  else if(out > 200){
    out = 200;
  }
  Serial.println(out);

  return out;                                       
}



//yön için 
//sol motor: 
//max 60, istenen 30
//eğer girdi 30dan büyükse sağa gitmeli, sol motor çalışır
void controlLeftMotor(int current){
  
}

  


#endif