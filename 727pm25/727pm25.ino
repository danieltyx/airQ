int dustPin=A1;
float dustVal=0; 
int ledPower=21;
int delayTime=280;
int delayTime2=40;
float offTime=9680;
int res = 0;
int level=0;
void setup(){
Serial.begin(9600);
pinMode(ledPower,OUTPUT);
pinMode(dustPin, INPUT);
}
 
void loop(){
// ledPower is any digital pin on the arduino connected to Pin 2 on the sensor
digitalWrite(ledPower,LOW); 
delayMicroseconds(delayTime);
dustVal=analogRead(dustPin); 
delayMicroseconds(delayTime2);
digitalWrite(ledPower,HIGH); 
delayMicroseconds(offTime);
 
delay(1000);
if (dustVal>36.455)

res = (float(dustVal/1024)-0.0356)*120000*0.035;

// Serial.println(res);
if(res<300)
{
  level = 1;
}
else if(res<1000)
{
  level = 2;
}
else if(res<2000)
{
  level = 3;
}
else if(res<3000)
{
  level = 4;
}
else
{
  level = 5;
}
Serial.println(res);
Serial.println(level);
}