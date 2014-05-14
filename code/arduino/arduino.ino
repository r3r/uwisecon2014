/*Author : Ritesh Reddy - Computer Science Class of 2015 UWI, Mona.
  Title: Arduino Line Follower for Southeastcon2014
*/

#include <Stepper.h>
#include <Servo.h>

#include "Constants.h"

boolean inter = false;
boolean flip = false;
boolean flipInter = false;

int intersectionsSeen = 0;
void setup(){
  SHOOT_setup();
  LINE_setup();
  FOLLOW_setup();
  Serial.begin(9600);
  digitalWrite(ENABLE_PIN, HIGH);
  //waitForPiBoot();
 
  initialGreenThreshold = 0;
  initialGreenThreshold = getRear4LineReading(100);
  ///Serial.print("initial");Serial.println(initialGreenThreshold);
  delay(50);
  initialGreenThreshold = getRear4LineReading(200);
  ///Serial.print("initial");Serial.println(initialGreenThreshold);
  while (!isGreenOn()){
    delay(5);
  }
}
void loop() {
 
 if(isIntersection()){
  if(!flip){
     if(inter){ //Code for Stopping on Blue Marker and Shooting Command
       delay(70);
       WHEELS_brake();
       aimAndShoot();
       //delay(5000);
       inter = false;       
       flip = true;
     }
     else { //Code to turn at intersection
       intersectionsSeen++;
       if(intersectionsSeen == 5) while(true){delay(40);WHEELS_brake();}
       if(intersectionsSeen==1) {
         //while(true){WHEELS_brake();}
         while(isIntersection()){delay(5);}
       }
       else {     
         inter = true;
         Follow_doIntersection();
       }
     }
  
 }
   else {
     if(!flipInter){ //After SHOOTING...returning to mainline
       WHEELS_moveReverse();
       if(!isIntersection()){while(!isIntersection()){delay(10);}}
       while(isIntersection()){delay(10);}
       delay(10);
       flipInter = true;
       Follow_followReverse();
     }  
     else {
       Follow_turnToMainLine();
       flipInter = false;
       flip = false;
       inter = false;
     }
   }
 
 }

 if(flip){
   Follow_followReverse();
 }
 else {
   Follow_followLine();
 }
 ///Serial.println(isIntersection());*/
 ///printLineReadings();
  ///printRearLineReadings();
  ///qprintFiltered4();
  ///delay(500);
 
}

void waitForPiBoot(){
  while (!Serial.available()){
  }
  if(Serial.available()) {
    Serial.read();
    Serial.println('W');
  }
}

