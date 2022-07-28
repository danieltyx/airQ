/*
 Dust Sensor project for arduino
 sensor GP2Y1010AU0F
 www.keyestudio.com
*/

int measurePin = 26; //Connect dust sensor to Arduino A0 pin
int ledPower = 25; 
int samplingTime = 280; 
int deltaTime = 40; 
int sleepTime = 9680; 
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
void setup(){
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
}
void loop(){
  digitalWrite(ledPower, LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin); // read the dust value
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);  
  delayMicroseconds(sleepTime); 
  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity = 170 * calcVoltage - 0.1;
  int pm; 
  if (dustDensity < 150) {
    pm = 1;
  }
  if (dustDensity >= 150 && dustDensity < 300) {
    pm = 2;
  }
  if (dustDensity >= 300 && dustDensity < 1050) {
    pm = 2;
  }
  if (dustDensity >= 1050 && dustDensity < 3000) {
    pm = 2;
  }
  if (dustDensity >= 3000) {
    pm = 2;
  }
  Serial.print("The dust concentration is: ");
  Serial.print(dustDensity);
  Serial.println(" ug/m3\n");  
  Serial.print("dust rating: "); 
  Serial.println(pm); 
  delay(1000);
}//***********