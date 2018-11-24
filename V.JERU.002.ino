#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>

#include <Encoder.h>
#include <VarSpeedServo.h>

#ifndef PARAMS
#define PARAMS 1
#include "params.h"
#endif

#include "servo_util.h"
#include "button_actions.h"

boolean active;
boolean changed;
bool isLongPressed = false;
bool alreadyHappend = false;
Thread myThread = Thread();
Thread myEncoderThread = Thread();
long stopper;

Encoder encoder(ENCODER1_PIN, ENCODER2_PIN);

void ThreadServoUpdate() {
  //Serial.println("ThreadServoUpdate()");
  if (!servo_update()) {
    Serial.println("--------SERVO THREAD-----------------------------");
    
    #ifdef STOPPER
      Serial.println(String("TIME: ") + (millis() - stopper));
    #endif
    
    servo_stop();
    motor_stop();
    changed = false;
    active = false;
  }  
}

void ThreadEncoderUpdate() {
  //Serial.println("ThreadEncoderUpdate()");
  if (motor_reachedEnd(&encoder)) {
    Serial.println("-----ENCODER THREAD-------------------------");

    #ifdef STOPPER
      Serial.println(String("TIME: ") + (millis() - stopper));
    #endif
    
    servo_stop();
    motor_stop();
    changed = false;
    active = false;
  }  
}

void setup() {
  Serial.begin(115200);
  pinMode(PUSHBUTTON_PIN, INPUT_PULLUP);
  pinMode(MOTOR_DIR_PIN, INPUT_PULLUP);
  active = false;
  changed = false;
  myThread.onRun(ThreadServoUpdate);
  myEncoderThread.onRun(ThreadEncoderUpdate);
}

void initDataBeforeFirstRun() {
        minChangeInDelay = CalcRand(CHANGE_IN_DELAY_AMOUNT_L,CHANGE_IN_DELAY_AMOUNT_U);
        maxDelay = CalcRand(MAX_DELAY_AMOUNT_L,MAX_DELAY_AMOUNT_U);
        minDelay = CalcRand(MIN_DELAY_AMOUNT_L,MIN_DELAY_AMOUNT_U);
       
        minChangeInNumOfMoves = CalcRand(CHANGE_IN_DELAY_MOVES_L,CHANGE_IN_DELAY_MOVES_U);
        maxNumOfCount = CalcRand(MAX_DELAY_NUM_OF_MOVES_L,MAX_DELAY_NUM_OF_MOVES_U);
        minNumOfCount = CalcRand(MIN_DELAY_NUM_OF_MOVES_L,MIN_DELAY_NUM_OF_MOVES_U);

        scheduledDelay = CalcRand(minDelay,maxDelay);        
        movesBeforeNextDelay = CalcRand(minNumOfCount,maxNumOfCount);

        activeDelay = -1;
        movesCounter=0;
        plateMovesCounter=0;
        stopper = millis();
        
        servo_start(); 
        motor_start(&encoder, digitalRead(MOTOR_DIR_PIN), ROTATION_SPEED);
}

void loop() {
  int buttonState = checkButton();
    if (buttonState == 1 || buttonState == 3) {
    active = !active;
    changed = true;
    
    Serial.println(String("buttonState set to: ") + (buttonState));
    
    if (buttonState == 3) {
      isLongPressed = true;
    }
    else {
      isLongPressed = false;
    }
  }

  if (active) {
    if(changed) {
  	  if (isLongPressed) {
  		  motor_start(&encoder, digitalRead(MOTOR_DIR_PIN), LONG_PRESSED_ROTATION_SPEED);
        changed = false;
  	  }
      else {
        Serial.println("servo_start()");
        initDataBeforeFirstRun();
        changed = false;
      }
  }
    else {
      if (isLongPressed) {
        if(checkButton() != 0) {
          active = false;
          changed = true;
	        isLongPressed = false;
        }
      }
      else {
        // Checks if the motor reached the point when the servo should start running
        if (isCupMod) {
          //Serial.println("startServoInSpecificTime");
          if (isServoShouldStart(&encoder)) {
            myThread.run();
            if (false) {
              active = false;
              changed = true;
            }
          }
        }
        // Normal Behavior
        else {
          //Serial.println("Normal Behavior");
          myThread.run();
          myEncoderThread.run();
        }
      }
    }
  }
  else {
    if(changed) {
      motor_stop();
      servo_stop();
      changed = false;
    }
  }
}


