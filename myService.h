#ifdef VAR_SERVO_SPEED
  #define VAR_SERVO_SPEED 1
  #include <VarSpeedServo.h>
#endif

#ifndef PARAMS
#define PARAMS 1
#include "params.h"
#endif


int CalcRand(int nMin, int nMax) {
  return (rand() % (nMax - nMin + 1) + nMin);
}

int CalcNextRandVal (int firstParam, int secontParam, int limit, String paramName) {
    int attemptNumber = 0;
    int value = CalcRand(firstParam,secontParam);

    #ifdef DEBUG_CALC_NEXT_RAND_VAL
      Serial.println(String("value: ") + (value) + String(" firstParam: ") + (firstParam));
      Serial.println(String("secontParam: ") + (secontParam) + String(" limit: ") + (limit));
    #endif  
    
    while (abs(firstParam - secontParam) < limit)
    {
      attemptNumber++;
      if (attemptNumber < 10000) {
        value = CalcRand(firstParam,secontParam);

        
      #ifdef DEBUG_CALC_NEXT_RAND_VAL
        Serial.println(String("value: ") + (value) + String(" firstParam: ") + (firstParam));
        Serial.println(String("secontParam: ") + (secontParam) + String(" limit: ") + (limit));
      #endif  
      }
      else {
        Serial.println(String("EndLess Loop in param: " + paramName));
        return -1;
      }
    }

    #ifdef DEBUG_CALC_NEXT_RAND_VAL
      Serial.println(String("value " + paramName + " = ") + value);
    #endif 
    
    return value;
}

bool HandleDelayOfMovement(int* movesCounter, int* movesBeforeNextDelay, int* scheduledDelay, int minDelay, int maxDelay, int minChangeInDelay,
                           int minNumOfCount, int maxNumOfCount, int minChangeInNumOfMoves, long* activeDelay) {
  //Serial.println(String("movesCounter: ") + (*movesCounter) + String(" movesBeforeNextDelay: ") + (*movesBeforeNextDelay));
  // Checks if its time for make delay
  if (*movesCounter == *movesBeforeNextDelay) {

    // Delay's the amount of time as calculated
    #ifdef DEBUG_SERVO_DELAY
      Serial.println(String("scheduledDelay: ") + (*scheduledDelay));
    #endif    

    *activeDelay = *scheduledDelay;
    
    //delay(*scheduledDelay);

    /////////////////////////////// Calculates the data for the next delay
    *movesCounter = 0;

    // Delay Length calculation
    *scheduledDelay = CalcNextRandVal(minDelay, maxDelay, minChangeInDelay, "minChangeInDelay");

    if (*scheduledDelay == -1) {
      return false;
    }

    // Delay number of moves calculation
    *movesBeforeNextDelay = CalcNextRandVal(minNumOfCount, maxNumOfCount, minChangeInNumOfMoves, "minChangeInNumOfMoves");
    
    if (*movesBeforeNextDelay == -1) {
      return false;
    }
  }

    return true;
}

//boolean ToInitServoInLoc(VarSpeedServo* myServo, boolean action, int loc, boolean* alreadyHappend) {
//  Serial.println(String("action set to: ") + (action));
//  Serial.println(String("alreadyHappend set to: ") + (*alreadyHappend));
//  if (action && !alreadyHappend) {
//    myServo->attach(SERVO_PIN);
//    myServo->write(loc, 80, true);
//    myServo->detach();
//    *alreadyHappend = true;
//  }
//}


void motor_start(Encoder* encoder, int dir, int speedd) {
  Serial.println("Starting motor");
  pinMode(DIR1_PWM_PIN, OUTPUT);
  pinMode(DIR2_PWM_PIN, OUTPUT);
  encoder->write(0);

  if (dir) {
    digitalWrite(DIR1_PWM_PIN, LOW);
    analogWrite(DIR2_PWM_PIN, speedd);
  }
  else {
    digitalWrite(DIR2_PWM_PIN, LOW);
    analogWrite(DIR1_PWM_PIN, speedd);
  }
}

boolean motor_reachedEnd(Encoder* encoder) {
  long motorPos = abs(encoder->read());
  boolean isStopMotor = (motorPos) > CLICKS_PER_ROUND;

  #ifdef DEBUG_ENCODER
    Serial.println(String("Motor Pos: ") + (motorPos) + String(" CLICKS_PER_ROUND: ") + (CLICKS_PER_ROUND) + String(" isStopMotor: ") + (isStopMotor ? "TRUE" : "FALSE"));
  #endif  
  
  return isStopMotor;
}

boolean motor_stop() {
  Serial.println("Stopping motor");
  //stop motor
  digitalWrite(DIR1_PWM_PIN, LOW);
  digitalWrite(DIR2_PWM_PIN, LOW);

  delay(1000);

  // release motor hold
  pinMode(DIR1_PWM_PIN, INPUT);
  pinMode(DIR2_PWM_PIN, INPUT);
}

boolean isServoShouldStart(Encoder* encoder) {
  boolean isServoStart = (abs(encoder->read()) > SERVO_START_ANGLE);

  #ifdef DEBUG_ENCODER_IS_SERVO_SHOULD_START_CUPMMOD
    Serial.println(String("Motor Pos: ") + (abs(encoder->read())) + String(" SERVO_START_ANGLE: ") + (SERVO_START_ANGLE) + String(" isServoShouldStart: ") + (isServoStart ? "TRUE" : "FALSE"));
  #endif
  
  return isServoStart;
}
