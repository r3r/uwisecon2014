#define FORWARDFACINGSENSOR A7
#define OBSTACLETHRESHOLD 20

int readUltraSonic(int pin){
  return analogRead(pin);
}

boolean isObstaclePresent(){
  return readUltraSonic(FORWARDFACINGSENSOR) <= OBSTACLETHRESHOLD;
}



