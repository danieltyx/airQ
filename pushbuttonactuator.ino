/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-button
 */

#define BUTTON_PIN 21  // GIOP21 pin connected to button
#include <ESP32Servo.h>

Servo myServo; 
#define servopin A0

//stroke percentage 0 is fully retracted, 100 fully extended
int findStrokePercent(int strokePercent) {
  return map(strokePercent,0,100,0,180);
}

// Variables will change:
int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize the pushbutton pin as an pull-up input
  // the pull-up input pin will be HIGH when the switch is open and LOW when the switch is closed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  myServo.attach(servopin, 1000, 2000);

}

void loop() {
  // read the state of the switch/button:
  currentState = digitalRead(BUTTON_PIN);

  if (lastState == HIGH && currentState == LOW) {
    Serial.println("The button is pressed");
    myServo.write(findStrokePercent(60)); 
    
  }
    
    
  else if (lastState == LOW && currentState == HIGH) {
    Serial.println("The button is released");
    myServo.write(findStrokePercent(0)); 

  }
    

  // save the the last state
  lastState = currentState;
}
