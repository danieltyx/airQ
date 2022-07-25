#include <DHT.h>
#include <DHT_U.h>

#define BLYNK_TEMPLATE_ID "TMPLbOc6du_b"
#define BLYNK_DEVICE_NAME "AirQ"
#define BLYNK_AUTH_TOKEN "A2v_2msXRGpzQhWtfADsP8ZbOwAlzYBY"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

// Libraries
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
//#include <Servo.h>

Servo myservo; 
const int servoPin = A0; 
const int dhtpin_i = A1; 
const int dhtpin_o = A2; 

DHT dhtout(dhtpin_o, DHT11); 
DHT dhtin(dhtpin_i, DHT11); 

char auth[] = BLYNK_AUTH_TOKEN;

// your WiFi credentials.
// set password to "" for open networks.
char ssid[] = "HP Setup";
char pass[] = "detkin101";
BlynkTimer timer; 

int ptempmax;
int ptempmin;  
float insidetemp;
float insidehumid; 
float outsidetemp; 
float outsidehumid;  

//WidgetTerminal terminal(V1); 

void updateinsidedata() {
  insidetemp = dhtin.readTemperature() * 1.8 + 32; 
  insidehumid = dhtin.readHumidity(); 
  Blynk.virtualWrite(V3, insidetemp); 
  Blynk.virtualWrite(V4, insidehumid); 
}

void updateoutsidedata() {
  outsidetemp = dhtout.readTemperature() * 1.8 +32; 
  outsidehumid = dhtout.readHumidity(); 
  Blynk.virtualWrite(V0, outsidetemp); 
  Blynk.virtualWrite(V5, outsidehumid); 
}

//read user input temp 
BLYNK_WRITE(V2) {
  ptempmax = param.asInt()+3;
  ptempmin = param.asInt()-3; 
  //servo.write(val); // fetch the slider value from the app
}

int tempreq; 


int findtempdiff() {
  String tempmessage; 
  
  if (insidetemp <= ptempmax && insidetemp >= ptempmin) {
    tempreq = 0.0; 
    tempmessage = "Inside is already good for temp. Window closed."; 
  }
  if (insidetemp > ptempmax && outsidetemp > ptempmax) {
    tempreq = 0.0; 
    tempmessage = "It's too hot. opening the window won't help. Turn on your AC."; 
  }
  if (insidetemp < ptempmin && outsidetemp < ptempmin) {
    tempreq = 0.0; 
    tempmessage = "It's too cold. opening the window won't help. Turn on your heater."; 
  }  
  if (outsidetemp <= ptempmax && outsidetemp >= ptempmin) {
    //if inside too hot/cold open accordingly 
    if (insidetemp > ptempmax) {
      float t = abs(insidetemp-outsidetemp); 
      tempreq = t; 
      tempmessage = "It's hot. It's cooler outside. The window is open percent: "; 
      
    }
    if (insidetemp < ptempmin) {
      float t = abs(ptempmin-insidetemp); 
      tempreq = t; 
      tempmessage = "It's cold. It's warmer outside. We're keeping your window open by percent: "; 
    }
  }
  return tempreq; 
}
/*
int findhumiddiff() {
  int humidreq; 
  String humidmessage; 
    if (insidehumid <= phumidmax && insidehumid >= phumidmin) {
      humidreq = 0.0; 
      humidmessage = "inside is already good for humidity, window closed."; 
    }
    //if both outside range close window 
    if (insidehumid > phumidmax && outsidehumid > phumidmax) {
   
      humidreq = 0.0; 
      humidmessage = "It's too humid. opening the window won't help."; 
    }
    if (insidehumid < phumidmin && outsidehumid < phumidmin) {
      humidreq = 0.0; 
      humidmessage = "It's too dry. opening the window won't help."; 
    }
    //if outside in range
    if (outsidehumid <= phumidmax && outsidehumid >= phumidmin) {
      //if inside too hot/cold open accordingly 
      if (insidehumid > phumidmax) {
        float h = abs(insidehumid-outsidehumid); 
        humidreq = h;         
        humidmessage = "It's too humid. It's dryer outside. The window is open percent: "; 
      }
      if (insidehumid < phumidmin) {
        float h = abs(phumidmin - insidehumid); 
        humidreq = h; 
        humidmessage = "It's cold. It's warmer outside. We're keeping your window open by percent: "; 
      }  
    }
    return humidreq; 
}*/
//read user pref for temp or humidity 
/*BLYNK_WRITE(V0) {
  if (param.asInt()==1) {
    //if set on temp 
    //send results to how much window is open 
    findtempdiff()
  }
}*/
int tempmove(float diff) {
  int results; 
  int pos; 
  if (diff < 1) {
     pos = 1000 + 99.0 * 1000 / 100.0; 
     results=1;
  }
  if (diff >= 1 && diff <= 5) {
     pos = 1000 + 75.0 * 1000 / 100.0; 
     results=25;
  }
  if (diff > 5 && diff <=10) {
     pos = 1000 + 50.0*1000/100.0; 
     results=50;
  }
  if (diff > 10 && diff <=15) {
     pos = 1000 + 25.0*1000/100.0; 
     results=75; 
  }
  if (diff > 15) {
     pos = 1000 + 1.0*1000/100.0; 
     results = 99; 
  }
  return results; 
  myservo.writeMicroseconds(pos); 
}


/*void updatePot(){
  Blynk.virtualWrite(V1, potVal);
}*/

void setup()
{
  // debug console
  Serial.begin(9600);

  // starts the connection with Blynk using the data provided at the top (Wi-Fi connection name, password, and auth token)
  Blynk.begin(auth, ssid, pass);
  dhtin.begin(); 
  dhtout.begin(); 
  
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 45, 145); // attaches the servo on pin 18 to the servo object
  // Update potentiometer virtual pin 10 times a second
  //timer.setInterval(100L, updatePot);
}

void loop()
{
  updateinsidedata(); 
  updateoutsidedata(); 
  findtempdiff(); 
  tempmove(tempreq); 
  //potVal = analogRead(A1);
  timer.run();
  Blynk.run();
}

