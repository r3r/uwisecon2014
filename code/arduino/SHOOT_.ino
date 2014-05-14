Servo trigger;
Stepper horizontal(150, SH_A1, SH_B1, SH_A2, SH_B2);
Stepper vertical = Stepper(150, SV_A1, SV_B1, SV_A2, SV_B2);

void verticalTest(){
  enableSteppers(true);
  vertical.step(-550);
  vertical.step(+550);
  enableSteppers(false);
  
}

void horizontalTest(){
  enableSteppers(true);
  horizontal.step(100);
  horizontal.step(-100);
  enableSteppers(false);
}


void findPins(){
  enableSteppers(true);
  int i, j, k, l;
  i = 40;
  j = 41;
  k = 42;
  l = 43;
  for(int i = 40; i<44; i++){
    for(int j = 40; j<44; j++){
      for(int k = 40; k<44; k++){
        for(int l = 40; l<44; l++){
          if ((l == i) || (l == j) || (l == k) || (k==j) || (k==1) || (j==i)){
            continue;
          }
          Stepper test(150, i,j,k,l);
          test.setSpeed(100);
          test.step(100);
          Serial.print(i);
          Serial.print('\t');
          Serial.print(j);
          Serial.print('\t');
          Serial.print(k);
          Serial.print('\t');
          Serial.println(l);
          delay(100);
          
        }
      }
    }
  }
}

void shootTest(){
  enableSteppers(true);
  digitalWrite(FIRE, HIGH);
  delay(1000);
  servoTest();
  delay(1000);
  digitalWrite(FIRE, LOW);
  enableSteppers(false);
}

void servoTest(){
  trigger.write(160);
  delay(500);
  trigger.write(90);
  delay(500);
}

void SHOOT_setup(){
  pinMode(ENABLE_PIN,OUTPUT); 
  pinMode(FIRE,OUTPUT);
  pinMode(SH_A1,OUTPUT);
  pinMode(SH_A2,OUTPUT);
  pinMode(SH_B1,OUTPUT);
  pinMode(SH_B2,OUTPUT); 
  pinMode(SV_A1,OUTPUT);
  pinMode(SV_A2,OUTPUT);
  pinMode(SV_B1,OUTPUT);
  pinMode(SV_B2,OUTPUT);
  
  horizontal.setSpeed(100);
  //horizontal.step(255);
  vertical.setSpeed(100);
  trigger.attach(TRIG);
}
void shoot()
{
  delay(1000);
  trigger.write(180);
  delay(1000);
  trigger.write(100);
}

void toggleFrictionRollers(boolean on){
  if(on){
    digitalWrite(FIRE,HIGH);
  }
  else {
    digitalWrite(FIRE,LOW);
  }
}

void enableSteppers(boolean en){
  if(en) digitalWrite(ENABLE_PIN, HIGH);
  else digitalWrite(ENABLE_PIN, HIGH);
}
void waitForCommand(){
  long start, now;
  start = millis();
  while(Serial.available()<2){
    now = millis();
    if((now-start)>2) break;
  } 
}

//convert string to int for sending no of steps
int readIntStr(){
  char digit1, digit2, digit3;
  int valout;
  //int number = Serial.parseInt();
  digit1 = Serial.read() - '0';
  digit2 = Serial.read() - '0';
  digit3 = Serial.read() - '0';
  
  valout =  (digit1*100) + (digit2*10) + (digit3);
  Serial.println(valout);
  return valout;
}

void aimAndShoot(){
  char pp ;
    

  Serial.flush();
  Serial.println("A");
  long time = millis();
  while(millis() - time < PI_WAIT_THRESHOLD){
    
   
    //Serial.flush();
    //waitForCommand();
  
     
    
    
    //int num = (int)Serial.read();
   
   if (Serial.available() > 3){
    
    pp = Serial.read();
    
    switch(pp){
       case 'R' : enableSteppers(true);            
            //horizontal.step(-num);
            //Serial.println(num);
            
            //new input of steps to make
            horizontal.step(-readIntStr());
            
            enableSteppers(false);
            break;
            
      case 'L' : enableSteppers(true);
            //horizontal.step(num);
            //Serial.println(num);
            
            //new input of steps to make
            horizontal.step(readIntStr());
            
            enableSteppers(false);
            break;
            
      case 'U' : enableSteppers(true);
            //vertical.step(-num);
            //Serial.println(num);
            
            //new input of steps to make
            vertical.step(-readIntStr());
            enableSteppers(false);
            break;
            
      case 'D' : enableSteppers(true);
            //vertical.step(num);
            //Serial.println(num);
            
            //new input of steps to make
            vertical.step(readIntStr());
            
            enableSteppers(false);
            break;
            
      case 'F' : enableSteppers(true);
            toggleFrictionRollers(true);
            delay(500);
            shoot();
            delay(1500);    
            toggleFrictionRollers(false);
            Serial.println("L");
            //delay(1000);
            enableSteppers(false);
            return;
    }
    Serial.flush();
    time = millis();
   }
  }
}
