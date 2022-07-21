#include <Servo.h>
#include <DHT.h> 

Servo myServo; 
#define dhtpinO 4
#define dhtpinI 2
DHT dht(dhtpinO, DHT11); 
DHT dht(dhtpinI, DHT11); 

//temps in F
float insidehumid; 
float insidetemp; 
float outsidehumid; 
float outsidetemp; 

float ptempmax = 72.0; 
float ptempmin = 65.0;  
//user programmed temp range preference 
//ranges hard coded for testing 

// 75% for temp diff of (0,5] deg 
// 50% for temp diff of (5, 10]deg 
//25% for temp diff (10, 15] deg 
//fully retracted for temp diff (15, inf) 
void tempmove(float tempdiff) {
  if (tempdiff<1) {
    int pos = 1000+99.0*1000/100.0; 
  }
  if (tempdiff > 1 && tempdiff <= 5) {
    int pos = 1000 + 75.0*1000/100.0; 
  }
  if (tempdiff > 5 && tempdiff<=10) {
    int pos = 1000 + 50.0*1000/100.0; 
  }
  if (tempdiff > 10 && tempdiff<=15) {
    int pos = 1000 + 25.0*1000/100.0; 
  }
  if (tempdiff>15) {
    int pos = 1000 + 1.0*1000/100.0; 
  }
  myServo.writeMicroseconds(pos); 

  /*if (strokepercent >= 1.0 && strokepercent <= 99.0) {
    int pos = 1000+ strokepercent*1000/100.0; 
    myServo.writeMicroseconds(pos); 

    //In this case, full range runs from about 1000-2000 usec.
  }*/
}
//stroke percentage 0.0 is fully retracted, 100.0 fully extended

void setup() {
  myServo.attach(8, 45, 145);
  dht.begin(); 
  Serial.begin(9600); 
}

void loop() {

  //delayMS
  delay(1000); 
  //variable delay between measurements, hardcoded for now  
  insidehumid=dht.readHumidity(); 
  insidetemp = dht.readTemperature()*1.8+32; 
  //Serial.println(insidetemp); 

  //if inside in range close window 
  if (insidetemp <= ptempmax && insidetemp >= ptempmin) {
    tempmove(0.0); 
  }
  //if outside in range
  if (outsidetemp <= ptempmax && outsidetemp>= ptempmin) {
    //if inside too hot/cold open accordingly 
    if (insidetemp>ptempmax) {
      float t = abs(insidetemp-ptempmax); 
      tempmove(t); 
    }
    if (insidetemp<ptempmin) {
      float t = abs(ptempmin-insidetemp); 
      tempmove(t); 
    }
  }
 
  
}
