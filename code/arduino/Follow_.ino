//Sensor Line3 - Line1
//Sensor Line4 - Line4
//Sensor Line1 - Line5
//Sensor Line2 - Line2
//Sensor Line5 - Line3
//LineSensor Array
int lineArray[8] = {LINE1, LINE2, LINE3, LINE4, LINE5, LINE6, LINE7, LINE8};
//int rearLineArray[5] = {REAR_LINE3, REAR_LINE4, REAR_LINE1, REAR_LINE2, REAR_LINE5};
//int rearLineArray[5] = {REAR_LINE1, REAR_LINE4, REAR_LINE5, REAR_LINE2, REAR_LINE3};
int rearLineArray[5] = {REAR_LINE3, REAR_LINE2, REAR_LINE5, REAR_LINE4, REAR_LINE1};
int leftMostRearSensorOnLine = rearLineArray[3];
//int weights[8] = {2,4,7,10,12,13,14,15}
int weights[8] = {-3,-1,0,1,2,3,4,5};
int rearWeights[5] = {-2,-1,0, 1, 2};

void FOLLOW_setup(){
  for(int i = 0; i< 8; i++){
    pinMode(lineArray[i], INPUT);
  }
  for(int i =0 ; i<5; i++){
    pinMode(rearLineArray[i], INPUT);
  }
}
int Follow_calculateError(){
  int readings[8];
  getReadings(readings);
  int weight = 0;
  for (int i = 0; i < NUM_SENSORS; i++){
    if (isOnLine(readings[i], DIGITAL_LINE)){
      weight = weights[i] + weight;
    }
  }
  return (weight) * SCALEFACTOR;
}

void Follow_followLine(){
  int error = Follow_calculateError();
  WHEELS_rightMotor(1, FORWARDLINEPWM - 10 - error);
  WHEELS_leftMotor(1, FORWARDLINEPWM + error);
}
int Follow_calculateReverseError(){
  int readings[5];
  getRearReadings(readings);
  int weight = 0;
  for (int i = 0; i < 5; i++){
    if (isOnLine(readings[i], ANALOG_LINE)){
      weight = rearWeights[i] + weight;
    }
  }
  return (weight) * REVERSESCALEFACTOR;
}

int Follow_calculateReverseError2(){
  int readings[8];
  getReadings(readings);
  int weight = 0;
  for (int i = 0; i < NUM_SENSORS; i++){
    if (isOnLine(readings[i], DIGITAL_LINE)){
      weight = rearWeights[i] + weight;
    }
  }
  return (weight) * REVERSESCALEFACTOR;
}
void  Follow_followReverse(){
/*  if(isParallaxLeftAndPololuRight()){
    WHEELS_spinRight(); //Since facing reverse direction
    while(isParallaxLeftAndPololuRight()){}
    WHEELS_brake();
  }
  if(isParallaxRightAndPololuLeft()){
    WHEELS_spinLeft(); //Since facing reverse direction
    while(isParallaxRightAndPololuLeft()){}
    WHEELS_brake();
  }*/
  int error = Follow_calculateReverseError();
  if(error < 0){
    WHEELS_leftMotor(0, REVERSEPWM - error);
    WHEELS_rightMotor(0,0);
  } else if (error > 0){
    WHEELS_rightMotor(0, REVERSEPWM - RIGHTMOTORBIAS + error);
    WHEELS_leftMotor(0,0);
  }
  else {
    WHEELS_rightMotor(0, BASEPWM - RIGHTMOTORBIAS + error);
    WHEELS_leftMotor(0, BASEPWM - error);
  }
}

void Follow_doIntersection()
{
 //Stop Wheels
 WHEELS_brake();
 if(INTERSECTION_HYSTERISIS) delay(INTERSECTION_HYSTERISIS * 2);
 //Turn Right Until Rear Left Sensor is off the main white lin
 WHEELS_rightMotor(0, BASEPWM);
 if(intersectionsSeen == 1)setLeftMostRearSensorOnLine(true);
 else setLeftMostRearSensorOnLine(true);
 while(isLeftMostRearSensorOnLine()){}
 WHEELS_brake();
 
 //Go Forward Until Rear sensor is beyond the perpendicular white line
 WHEELS_moveForward();
 if(!isRearSensorOnLine()){ while(!isRearSensorOnLine()){}}
 if(isRearSensorOnLine()){ while(isRearSensorOnLine()){}}
 while(!isRearSensorOnLine()){} while(isRearSensorOnLine()){}
 WHEELS_brake();
 
 
 //Reverse Left Until Almost facing perpendicular line
 WHEELS_leftMotor(0, BASEPWM + 50);
 while(!isLeftExtremeOnLine()){}
 WHEELS_brake();
 
 if(INTERSECTION_HYSTERISIS) delay(INTERSECTION_HYSTERISIS);
 
 WHEELS_leftMotor(0, BASEPWM + 50);
 while(isLeftExtremeOnLine()){}
 WHEELS_brake();
 
 if(INTERSECTION_HYSTERISIS) delay(INTERSECTION_HYSTERISIS);
 
 WHEELS_leftMotor(0, BASEPWM + 50);
 while(!isLeftExtremeOnLine()){}
 WHEELS_brake();
 
 if(INTERSECTION_HYSTERISIS) delay(INTERSECTION_HYSTERISIS);
 
 
  //Forward Until Left Mid Front Sensor is not on line - set up to follow perpendicular line
 WHEELS_rightMotor(1, BASEPWM + 80);
 if(isLeftExtremeOnLine()){
    while(isLeftExtremeOnLine()){}
 }
 else {
   if(isLeftMidOnLine()){ 
     while(isLeftMidOnLine()){}
   }
 }
 /*if(intersectionsSeen != SECOND_INTERSECTION){
   if(isLeftMidOnLine()){ 
     while(isLeftMidOnLine()){}
   }
 }
 else{
   if(isLeftExtremeOnLine()){
     while(isLeftExtremeOnLine()){}
   }
 }*/
 // delay(500);
 //Adjustment for Curved Line

 /*if(intersectionsSeen == SECOND_INTERSECTION){
   
   if(isLeftMidOnLine()) { while(isLeftMidOnLine()){}}
   if(isCenter()) { while(isCenter()){}}
 }*/
 WHEELS_brake();
 
 if(INTERSECTION_HYSTERISIS) delay(INTERSECTION_HYSTERISIS);
 //while(true){printLineReadings(); delay(500);}
 
}

void Follow_turnToMainLine(){
  WHEELS_brake();
  if(MAINLINE_HYSTERISIS) delay(MAINLINE_HYSTERISIS);
  WHEELS_spinRight();
  if(isRear5OnLine()){while(isRear5OnLine()){}}
  else if(isRear4OnLine()){while(isRear4OnLine()){}}
  WHEELS_brake();
  if(MAINLINE_HYSTERISIS) delay(MAINLINE_HYSTERISIS);
  WHEELS_moveForward();
  while(!isRightExtremeOnLine()){}
  WHEELS_brake();
  if(MAINLINE_HYSTERISIS) delay(MAINLINE_HYSTERISIS);
  WHEELS_moveForward();
  while(isRightExtremeOnLine()){}
  WHEELS_brake();
  if(MAINLINE_HYSTERISIS) delay(MAINLINE_HYSTERISIS);
  WHEELS_spinRight();
  while(!isRightExtremeOnLine()){}
   WHEELS_brake();
  if(MAINLINE_HYSTERISIS) delay(MAINLINE_HYSTERISIS);
  WHEELS_spinRight();
  while(isRightExtremeOnLine()){}
  WHEELS_brake();
  if(MAINLINE_HYSTERISIS) delay(MAINLINE_HYSTERISIS);
 
 
  WHEELS_moveForward();
  if(isRearLeftOnLine()){while(isRearLeftOnLine()){}}
  while(!isRearLeftOnLine()){}
  if(intersectionsSeen != SECOND_INTERSECTION){
    while(isRearLeftOnLine()){}
    while(!isRearLeftOnLine()){}
  }
  WHEELS_brake();
  if(MAINLINE_HYSTERISIS) delay(MAINLINE_HYSTERISIS);
  
  
  WHEELS_spinRight();
  while(!isCenter()){}
  WHEELS_brake();
  if(MAINLINE_HYSTERISIS) delay(MAINLINE_HYSTERISIS);
  WHEELS_spinRight();
  while(!isCenter()){}
  WHEELS_brake();
  if(MAINLINE_HYSTERISIS) delay(MAINLINE_HYSTERISIS);
  
  
}
  
void Follow_doObstacle(){
  WHEELS_brake();
}
  

