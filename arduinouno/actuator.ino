
#include <Servo.h>

Servo myServo; 
#define potpin 8; 

void findstrokepercent(float strokepercent) {
  if (strokepercent >= 1.0 && strokepercent <= 99.0) {
    int pos = 1000 + strokepercent * 1000 / 100.0; 
    myServo.writeMicroseconds(pos); 

    //In this case, full range runs from about 1000-2000 usec.
  }
  
}
//stroke percentage 0.0 is fully retracted, 100.0 fully extended


void setup() {
  myServo.attach(8, 45, 145);
}

void loop() {
  //keeps moving servo from 1 to 99 percent and back 
  //int delayMS=1500; 
  for (int i=0; i<99; i+=5) {
    findstrokepercent(i); 
    delay(500); 
  }
  for (int i=99; i>1; i-=5) {
    findstrokepercent(i); 
    delay(500); 
  }

}
