#define WHITELINE
#define DIGITAL_SENSOR
//#define NOMOTION
#define DIGITAL_LINE 100
#define ANALOG_LINE 101
                        //Old      //New
#define REAR_LINE1 A5  //Number 5 //number 3
#define REAR_LINE2 A4 //Number 4 //number 6
#define REAR_LINE3 A3 //Number 7 // number 7
#define REAR_LINE4 A6 // number 6 // number 4
#define REAR_LINE5 A7 // number 3 //number 5
#define REAR_LINE_SENSOR_EN 52
//      Old    - New
//Sensor Line3 - Line1
//Sensor Line4 - Line4
//Sensor Line1 - Line5
//Sensor Line2 - Line2
//Sensor Line5 - Line3

#define LIGHTSURFACE 250
#define GREENTHRESHOLD 15

#define LINE1 34
#define LINE2 32
#define LINE3 30
#define LINE4 28 //Center
#define LINE5 26
#define LINE6 36
#define LINE7 24
#define LINE8 22

#define FORWARDLINEPWM 215 //max 216
#define BASEPWM 150
#define REVERSEPWM 160
#define RIGHTMOTORBIAS 15 //22
#define ERRORCONST 7
#define SCALEFACTOR 12
#define REVERSESCALEFACTOR 4
#define FIVE_SENSORS 
#define NUM_SENSORS 5


#define INTERSECTION_HYSTERISIS 80
#define MAINLINE_HYSTERISIS 100
#define SECOND_INTERSECTION 3

#define LEFTENA 3
#define LEFTENB 2
#define RIGHTENA 4
#define RIGHTENB 5

#define TURNRATIO 0.5

//Shooter and Aimer
//servo to push darts
#define TRIG 9

//Horizontal Controlling Stepper
#define SH_A1 41 //22 40
#define SH_A2 40 //23  41
#define SH_B1 43 //24 42
#define SH_B2 42 //25 43

//Vertical Controlling Stepper
#define SV_A1 44 //30 44
#define SV_A2 45 //31045
#define SV_B1 46 //32046
#define SV_B2 47 //33 47

//Enable pin
#define ENABLE_PIN 49 //27049

//Trigger Pin
#define FIRE 48 //48

#define PI_WAIT_THRESHOLD 5000
#define PI_BOOT_THRESHOLD 15000

int initialGreenThreshold;
 float alpha = 0.5;
