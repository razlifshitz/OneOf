#ifdef VAR_SERVO_SPEED
  #define VAR_SERVO_SPEED 1
  #include <VarSpeedServo.h>
#endif

#ifndef PARAMS
#define PARAMS 1
#include "params.h"
#endif

// todo: move this to the right place
class WaveSpeed {
   public:
      void initData(int category, int minSpeed, int maxSpeed);
      int category;   // speed category
      int minSpeed;      // min speed range
      int maxSpeed;  // max speed range
};

void WaveSpeed::initData(int cat, int minS, int maxS) {
   category = cat;
   minSpeed = minS;
   maxSpeed = maxS;
};

long CalcRand(long nMin, long nMax) {
  return (random() % (nMax - nMin + 1) + nMin);
}

int CalcRand(int nMin, int nMax) {
  return (random() % (nMax - nMin + 1) + nMin);
}

int CalcNextRandVal (int firstParam, int secontParam, int limit, String paramName) {
    int attemptNumber = 0;
    int value = CalcRand(firstParam,secontParam);

      #ifdef DEBUG_CALC_NEXT_RAND_VAL
        Serial.println(String("next value: ") + (value) + String(" lowEnd: ") + (firstParam));
        Serial.println(String("highEnd: ") + (secontParam) + String(" minimum Change: ") + (limit));
      #endif
    
    while (abs(firstParam - secontParam) < limit)
    {
      //Serial.println(String("attemptNumber: ") + attemptNumber);
      attemptNumber++;
      if (attemptNumber < 10000) {
        value = CalcRand(firstParam,secontParam); 

      #ifdef DEBUG_CALC_NEXT_RAND_VAL
        Serial.println(String("next value: ") + (value) + String(" lowEnd: ") + (firstParam));
        Serial.println(String("highEnd: ") + (secontParam) + String(" minimum Change: ") + (limit));
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

bool HandleDelayOfMovement(int* movesCounter, int* currentCountOfMoves, int* currentDelay, int minDelay, int maxDelay, int minChangeInDelay,
                           int minNumOfCount, int maxNumOfCount, int minChangeInNumOfMoves, long* servoActiveDelay) {
  //Serial.println(String("movesCounter: ") + (*movesCounter) + String(" currentCountOfMoves: ") + (*currentCountOfMoves));
  // Checks if its time for make delay
  //Serial.println(String("Before compare moveCounter !!!!!!!"));

  if (*movesCounter >= *currentCountOfMoves) {
    //Serial.println(String("moveCounter are equals !!!!!!!!!!!!!!!!"));


    // Delay's the amount of time as calculated
    #ifdef DEBUG_SERVO_DELAY
      Serial.println(String("currentDelay: ") + (*currentDelay));
    #endif    

    *servoActiveDelay = *currentDelay;
    
    //delay(*currentDelay);

    /////////////////////////////// Calculates the data for the next delay
    *movesCounter = 0;

    // Delay Length calculation
    *currentDelay = CalcNextRandVal(minDelay, maxDelay, minChangeInDelay, "minChangeInDelay");

    if (*currentDelay == -1) {
      return false;
    }

    // Delay number of moves calculation
    *currentCountOfMoves = CalcNextRandVal(minNumOfCount, maxNumOfCount, minChangeInNumOfMoves, "minChangeInNumOfMoves");
    
    if (*currentCountOfMoves == -1) {
      return false;
    }
  }
  
  return true;
}

int calcNextSpeed(WaveSpeed* waveSpeeds, int arrLength) {
  int categoryNumber = CalcRand(1, arrLength);
  int result = CalcRand(waveSpeeds[categoryNumber].minSpeed, waveSpeeds[categoryNumber].maxSpeed);

  // Serial.println(String("categoryNumber: ") + (categoryNumber));
  // Serial.println(String("waveSpeeds[categoryNumber].minSpeed: ") + (waveSpeeds[categoryNumber].minSpeed));
  // Serial.println(String("waveSpeeds[categoryNumber].maxSpeed: ") + (waveSpeeds[categoryNumber].maxSpeed));
  // Serial.println(String("Speed: ") + (result));

  return result;
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

void setMotorSpeed(Encoder* encoder, int dir, int speed) {
  if (dir) {
    digitalWrite(DIR1_PWM_PIN, LOW);
    analogWrite(DIR2_PWM_PIN, speed);
  }
  else {
    digitalWrite(DIR2_PWM_PIN, LOW);
    analogWrite(DIR1_PWM_PIN, speed);
  }
}

void setMotorSpeed(Encoder* encoder, int speed) {
  setMotorSpeed(encoder,  digitalRead(MOTOR_DIR_PIN), speed);
}

void motor_start(Encoder* encoder, int dir, int speedd) {
  Serial.println("Starting motor");
  pinMode(DIR1_PWM_PIN, OUTPUT);
  pinMode(DIR2_PWM_PIN, OUTPUT);
  encoder->write(0);

  setMotorSpeed(encoder, dir, speedd);
}

bool hasServoReachedDestination(int lastServoLoc, int destination, bool toMoveUp) { 
  //Serial.println(String("lastServoLoc: ") + (lastServoLoc) + String(" destination: ") + (destination) + String(" toMoveUp: ") + (toMoveUp ? "TRUE" : "FALSE"));
 

 bool result = toMoveUp 
  ? (lastServoLoc) >= destination
  : (lastServoLoc) <= destination;

 //Serial.println(result ? "TRUE" : "FALSE");

  return result; 
}


// todo: change param encoder to encoderLocation
boolean motor_reachDestination(Encoder* encoder, long destination) {
  long motorPos = abs(encoder->read());
  boolean isStopMotor = (motorPos) > destination;

  #ifdef DEBUG_ENCODER
    Serial.println(String("Motor Pos: ") + (motorPos) + String(" CLICKS_PER_ROUND: ") + (CLICKS_PER_ROUND) + String(" isStopMotor: ") + (isStopMotor ? "TRUE" : "FALSE"));
  #endif  
  
  return isStopMotor;
}

boolean motor_reachedEnd(Encoder* encoder) {
  return motor_reachDestination(encoder, CLICKS_PER_ROUND);
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
