#include <Servo.h>
#include <DHT.h> 

Servo myServo; 
#define dhtpinO 4
#define dhtpinI 2
DHT dhtout(dhtpinO, DHT11); 
DHT dhtin(dhtpinI, DHT11); 

//temps in F
float insidehumid; 
float insidetemp; 
float outsidehumid; 
float outsidetemp; 

float ptempmax = 75.0; 
float ptempmin = 73.0;  
//user programmed temp range preference 
//ranges hard coded for testing 

// 75% for temp diff of (0,5] deg 
// 50% for temp diff of (5, 10]deg 
//25% for temp diff (10, 15] deg 
//fully retracted for temp diff (15, inf) 
int results; 
void tempmove(float tempdiff) {
  int pos; 
  if (tempdiff < 1) {
     pos = 1000 + 99.0 * 1000 / 100.0; 
     results = 1.0;
  }
  if (tempdiff > 1 && tempdiff <= 5) {
     pos = 1000 + 75.0 * 1000 / 100.0; 
     results = 25.0;
  }
  if (tempdiff > 5 && tempdiff <= 10) {
     pos = 1000 + 50.0 * 1000 / 100.0; 
     results = 50.0;
  }
  if (tempdiff > 10 && tempdiff <= 15) {
     pos = 1000 + 25.0 * 1000 / 100.0; 
     results = 75.0; 
  }
  if (tempdiff > 15) {
     pos = 1000 + 1.0 * 1000 / 100.0; 
     results = 99.0; 
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
  dhtout.begin();
  dhtin.begin();  
  Serial.begin(9600); 
}

void loop() {

  //delayMS
  delay(1000); 
  //variable delay between measurements, hardcoded for now  
  insidehumid = dhtin.readHumidity(); 
  insidetemp = dhtin.readTemperature()*1.8+32; 
  outsidehumid = dhtout.readHumidity(); 
  outsidetemp = dhtout.readTemperature()*1.8+32; 
  Serial.print("inside temp:"); 
  Serial.println(insidetemp); 
  Serial.print("inside humidity: "); 
  Serial.println(insidehumid); 
  Serial.print("outside temp: "); 
  Serial.println(outsidetemp); 
  Serial.print("outside humidity: "); 
  Serial.println(outsidehumid); 

  //if inside in range close window 
  int tempreq; //proposed position to reach ideal temp
  String tempmessage; 
  if (insidetemp <= ptempmax && insidetemp >= ptempmin) {
    tempreq = 0.0; 
    tempmessage = "inside is already good for temp, window closed."; 
    //tempmove(0.0); 
  }
  //if both outside range close window 
  if (insidetemp > ptempmax && outsidetemp > ptempmax) {
   // tempmove(0.0); 
    tempreq = 0.0; 
    tempmessage = "It's too hot. opening the window won't help. Turn on your AC."; 
  }
  if (insidetemp < ptempmin && outsidetemp < ptempmin) {
    //tempmove(0.0); 
    tempreq = 0.0; 
    tempmessage = "It's too cold. opening the window won't help. Turn on your heater."; 
  }


  //if outside in range
  if (outsidetemp <= ptempmax && outsidetemp>= ptempmin) {
    //if inside too hot/cold open accordingly 
    if (insidetemp>ptempmax) {
      float t = abs(insidetemp-outsidetemp); 
      tempreq = t; 
      
      //tempmove(t);
      tempmessage = "It's hot. It's cooler outside. The window is open percent: "; 
      /*Serial.print("It's hot. it's "); 
      Serial.print(t); 
      Serial.print(" degrees cooler outside. We're keeping your window  "); 
      Serial.print(results); 
      Serial.println(" percent open."); */
      
    }
    if (insidetemp<ptempmin) {
      float t = abs(ptempmin-insidetemp); 
      tempreq = t; 
      
      //tempmove(t); 
      tempmessage = "It's cold. It's warmer outside. We're keeping your window open by percent: "; 
      /*Serial.print("It's cold. it's "); 
      Serial.print(t); 
      Serial.print(" degrees warmer outside. We're keeping your window  "); 
      Serial.print(results); 
      Serial.println(" percent open."); */
    }
  }
  
  //hardcoded humidity preferences 
  int phumidmax =  37.0; 
  int phumidmin = 35.0; 

  //humidity tests 

  int humidreq; //proposed position to reach ideal humidity
    String humidmessage; 
    if (insidehumid <= phumidmax && insidehumid >= phumidmin) {
      humidreq = 0.0; 
      humidmessage = "inside is already good for humidity, window closed."; 
      //tempmove(0.0); 
      //Serial.println("inside is already good, window closed."); 
    }
    //if both outside range close window 
    if (insidehumid > phumidmax && outsidehumid > phumidmax) {
    // tempmove(0.0); 
      humidreq = 0.0; 
      humidmessage = "It's too humid. opening the window won't help."; 
      //Serial.println("It's too hot. opening the window won't help. Turn on your AC."); 
    }
    if (insidehumid < phumidmin && outsidehumid < phumidmin) {
      //tempmove(0.0); 
      humidreq = 0.0; 
      humidmessage = "It's too dry. opening the window won't help."; 
      //Serial.println("It's too cold. opening the window won't help. Turn on your heater.");
    }


    //if outside in range
    if (outsidehumid <= phumidmax && outsidehumid >= phumidmin) {
      //if inside too hot/cold open accordingly 
      if (insidehumid>phumidmax) {
        float h = abs(insidehumid-outsidehumid); 
        humidreq = h; 
        
        //tempmove(t);
        humidmessage = "It's too humid. It's dryer outside. The window is open percent: "; 
        /*Serial.print("It's hot. it's "); 
        Serial.print(t); 
        Serial.print(" degrees cooler outside. We're keeping your window  "); 
        Serial.print(results); 
        Serial.println(" percent open."); */
        
      }
      if (insidehumid < phumidmin) {
        float h = abs(phumidmin-insidehumid); 
        humidreq = h; 
        
        //tempmove(t); 
        humidmessage = "It's cold. It's warmer outside. We're keeping your window open by percent: "; 
        /*Serial.print("It's cold. it's "); 
        Serial.print(t); 
        Serial.print(" degrees warmer outside. We're keeping your window  "); 
        Serial.print(results); 
        Serial.println(" percent open."); */
      }
    }

    //temp takes precedence over humid commands
      tempmove(tempreq); 
      Serial.print(tempmessage); 
      Serial.println(results);     
 
}
