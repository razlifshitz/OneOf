#include <VarSpeedServo.h>

#ifndef PARAMS
#define PARAMS 1
#include "params.h"
#endif

#include "button_actions.h"

boolean active;
VarSpeedServo myservo;

void servo_start() {
  Serial.println("servo_start()");
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
}

void setup() {
  Serial.begin(115200);
  pinMode(PUSHBUTTON_PIN, INPUT_PULLUP);
  pinMode(MOTOR_DIR_PIN, INPUT_PULLUP);
  active = false;
  servo_start(); 
}

void loop() {
  int buttonState = checkButton();
  if (buttonState == 1 || buttonState == 3) {
    active = !active;
  }
  
  int from = 1;
  int to = 70;
  int destination;
  
  if (active == true) {
    myservo.write(to, 60, false);
  } else {
    myservo.write(from, 60, false);
  }

}


