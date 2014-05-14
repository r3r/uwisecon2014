//Low Level WHEEL Functions
void WHEELS_rightMotor(int dir, int pwm){
  //if (pwm) Serial.println("Left Motor On");
  #ifdef NOMOTION
     return;
  #endif
  if (dir) {
     analogWrite(LEFTENA, LOW);
     analogWrite(LEFTENB, pwm);
  }else{
     analogWrite(LEFTENA, pwm);
     analogWrite(LEFTENB, LOW);
     
  }
}

void WHEELS_leftMotor(int dir, int pwm){
  //if (pwm) Serial.println("Right Motor On");
  #ifdef NOMOTION
     return;
  #endif
  if (dir) {
     analogWrite(RIGHTENA, LOW);
     analogWrite(RIGHTENB, pwm);
  }else{
    analogWrite(RIGHTENA, pwm);
    analogWrite(RIGHTENB, LOW);     
  }
}

void WHEELS_spinLeft(){
  WHEELS_rightMotor(1, BASEPWM );
  WHEELS_leftMotor(0, BASEPWM );
}

void WHEELS_spinLeft(int time){
  WHEELS_spinLeft();
  changeTimer(time);
}

void changeTimer(int time){
  return;
  //timer.stop(timer_id);
  //timer_id = timer.after(time, WHEELS_stop);
}

void WHEELS_stop(){
   #ifdef NOMOTION
     return;
  #endif
  digitalWrite(LEFTENA, LOW);
  digitalWrite(LEFTENB, LOW);
  digitalWrite(RIGHTENA, LOW);
  digitalWrite(RIGHTENB, LOW);
  
}

void WHEELS_brake(){
   #ifdef NOMOTION
     return;
  #endif
  digitalWrite(LEFTENA, HIGH);
  digitalWrite(LEFTENB, HIGH);
  digitalWrite(RIGHTENA, HIGH);
  digitalWrite(RIGHTENB, HIGH);
}
  
//Mid Level WHEEL functions
void WHEELS_moveForward(int time){
  WHEELS_moveForward();
  changeTimer(time);
}
void WHEELS_moveForward(){
  WHEELS_rightMotor(1, BASEPWM - RIGHTMOTORBIAS);
  WHEELS_leftMotor(1, BASEPWM );
}
void WHEELS_turnRight(){
  WHEELS_rightMotor(1, BASEPWM * TURNRATIO);
  WHEELS_leftMotor(1, BASEPWM);
}

void WHEELS_turnRight(int time){
  WHEELS_turnLeft();
  changeTimer(time);
}

void WHEELS_spinRight(){
  WHEELS_rightMotor(0, BASEPWM );
  WHEELS_leftMotor(1, BASEPWM );
}

void WHEELS_spinRight(int time){
  WHEELS_spinRight();
  changeTimer(time);
}

void WHEELS_turnLeft(){
  WHEELS_rightMotor(1, BASEPWM);
  WHEELS_leftMotor(1, BASEPWM * TURNRATIO);
}

void WHEELS_turnLeft(int time){
  WHEELS_turnRight();
  changeTimer(time);
}
void WHEELS_moveReverse(int time){
  WHEELS_moveReverse();
  changeTimer(time); 
}

void WHEELS_moveReverse(){
  WHEELS_rightMotor(0, BASEPWM - RIGHTMOTORBIAS);
  WHEELS_leftMotor(0, BASEPWM );
}

void WHEELS_turnRightReverse(){
  WHEELS_leftMotor(0, BASEPWM);
  WHEELS_rightMotor(0, BASEPWM * TURNRATIO);
}

