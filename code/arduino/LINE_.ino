void LINE_setup(){
  digitalWrite(REAR_LINE_SENSOR_EN, HIGH);
}

int getRear4LineReading(){
  return readLine(REAR_LINE4, ANALOG_LINE);
}

int getRear4LineReading(int tm){
  float time = millis() + tm;
  int reading = 0;
  while(millis() - time < 0){
     reading = ((1-alpha) * reading) + (alpha * getRear4LineReading());
   
     delay(5);
  }
  return reading;
}
boolean isGreenOn(){
  return (initialGreenThreshold - getRear4LineReading(100) > GREENTHRESHOLD);
}

//Line Sensor Functions
int readLine(int pin, int type){
  if (type == DIGITAL_LINE)
    return digitalRead(pin);
  if (type == ANALOG_LINE)
    return analogRead(pin);
}

void getReadings(int readings[8]){
  for(int i = 0; i<8; i++){
    readings[i] = readLine(lineArray[i], DIGITAL_LINE);
  }
}

void getRearReadings(int readings[5]){
  for(int i = 0; i<5; i++){
    readings[i] = readLine(rearLineArray[i], ANALOG_LINE);
  }
}

boolean isOnLine(int reading, int type){
  if (type == DIGITAL_LINE){
    #ifdef WHITELINE
      return reading == LOW;
    #endif
    #ifdef BLACKLINE
      return reading == HIGH;
    #endif
  }
  
  if (type == ANALOG_LINE){
    #ifdef WHITELINE
      return reading < LIGHTSURFACE;
    #endif
    #ifdef BLACKLINE
      return reading > LIGHTSURFACE;
    #endif
  }
}

boolean isIntersection(int readings[]){
   for(int i = 0; i<NUM_SENSORS; i++){
   if (!isOnLine(readings[i], DIGITAL_LINE)){
     return false;
   }
  }
  return true;
}
boolean isIntersection(){
  int readings[8];
  getReadings(readings);
  return isIntersection(readings);
}

boolean isLeftExtremeOnLine(){
  int readings[8];
  getReadings(readings);
  return isOnLine(readings[0], DIGITAL_LINE);
}
boolean isRightExtremeOnLine(){
  int readings[8];
  getReadings(readings);
  return isOnLine(readings[4], DIGITAL_LINE);
}
boolean isLeftMidOnLine(){
  int readings[8];
  getReadings(readings);
  return isOnLine(readings[1], DIGITAL_LINE);
}

boolean isRightMidRearOnLine(){
  //When Facing Front
  int reading;
  reading = readLine(REAR_LINE5, ANALOG_LINE);
  return isOnLine(reading, ANALOG_LINE);
}
boolean isLeftExtremeRearOnLine(){
  //When Facing front
  int reading;
  reading = readLine(REAR_LINE1, ANALOG_LINE);
  return isOnLine(reading, ANALOG_LINE);
}


boolean isRightExteremRearOnLine(){
  //Right when facing front
  int reading;
  reading = readLine(REAR_LINE3, ANALOG_LINE);
  return isOnLine(reading, ANALOG_LINE);
}
boolean isCenter(){
  int readings[8];
  getReadings(readings);
  return isOnLine(readings[2], DIGITAL_LINE);
}

boolean isRearSensorOnLine(){
  int readings[5];
  readings[0] = readLine(REAR_LINE5, ANALOG_LINE);
  readings[1] = readLine(REAR_LINE2, ANALOG_LINE);
  readings[2] = readLine(REAR_LINE4, ANALOG_LINE);
  return isOnLine(readings[0], ANALOG_LINE) || isOnLine(readings[1], ANALOG_LINE);
}

boolean isParallaxLeftAndPololuRight(){
  int parallaxReadings[8];
  int pololuReadings[5];
  getReadings(parallaxReadings);
  getRearReadings(pololuReadings);
  
  boolean parallax = isOnLine(parallaxReadings[0], DIGITAL_LINE) || isOnLine(parallaxReadings[1], DIGITAL_LINE);
  boolean pololu = isOnLine(pololuReadings[4], ANALOG_LINE) || isOnLine(pololuReadings[3], ANALOG_LINE);
  
  return parallax && pololu;
}

boolean isParallaxRightAndPololuLeft(){
  int parallaxReadings[8];
  int pololuReadings[5];
  getReadings(parallaxReadings);
  getRearReadings(pololuReadings);
  
  boolean parallax = isOnLine(parallaxReadings[4], DIGITAL_LINE) || isOnLine(parallaxReadings[3], DIGITAL_LINE);
  boolean pololu = isOnLine(pololuReadings[0], ANALOG_LINE) || isOnLine(pololuReadings[1], ANALOG_LINE);
  
  return parallax && pololu;
}

boolean isRear5OnLine(){
  return isOnLine(readLine(REAR_LINE5, ANALOG_LINE), ANALOG_LINE);
}

boolean isRear4OnLine(){
  return isOnLine(readLine(REAR_LINE4, ANALOG_LINE), ANALOG_LINE);
}
void setLeftMostRearSensorOnLine(boolean found){
  //Left When Front Facing
  int readings[5];
  getRearReadings(readings);
  for(int i =4; i>=0; i--){
    if(isOnLine(readings[i], ANALOG_LINE)){
      leftMostRearSensorOnLine = rearLineArray[i];
      if(found)
        return;
      found = true;
      
    }
  }
}

boolean isLeftMostRearSensorOnLine(){
  return isOnLine(readLine(leftMostRearSensorOnLine, ANALOG_LINE), ANALOG_LINE);
}

boolean isRearLeftOnLine(){
  //Left WHen Front Facing
  return isOnLine(readLine(REAR_LINE1,ANALOG_LINE), ANALOG_LINE);
}

  
