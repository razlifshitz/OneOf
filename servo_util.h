#ifdef VAR_SERVO_SPEED
  #define VAR_SERVO_SPEED 1
  #include <VarSpeedServo.h>
#endif

#include "paramsGenerator.h"
#include "myService.h"

Encoder encoder(ENCODER1_PIN, ENCODER2_PIN);
VarSpeedServo myservo;

//////////////////////////////
//////////CUP MODE////////////
////////////////////////////// 
bool isCupMod = false;

// -------------------------------------------- Properties
/* 
 * DELAY DATA (The numbers are in Miliseconds)
*/

// LENGTH
int minChangeInDelay = -1;
// Delay range
int maxDelay = -1;
int minDelay = -1;

// NUMBER OF MOVES (before each delay)
int minChangeInNumOfMoves =  -1; 
// count range
int maxNumOfCount = -1;
int minNumOfCount = -1;

long servoActiveDelay = -1;
long firstTime;
bool inDelayProcess = false;
int plateCounter = 0;  // Number of moves for the entire plate
int movesCounter = -1; // Number of moves in the current wave 
bool toMoveUp = true;
long lastServoLoc = myservo.read();
int servoStartLocation; //todo: implementation is needed



// -------------------------------------------- Logic Variables

// Delay length
int randDelay = -1;
int currentDelay = -1;

// Delay count of moves
int randCountOfMoves = -1;
int currentCountOfMoves = -1;

// Villroy and boch params
int currentQuarter = 0;
int encoderPosition;
int currentQurterActiveServo;
bool isEncoderReachedDestination = true;
//-------------------------------------- END DATA SETTING


void servo_start() {
  Serial.println("servo_start()");
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
}

void servo_stop() {
  Serial.println("servo_stop()");
  myservo.detach();
}

int getNextServoDestination(bool toMoveUp) {
  // Bottom range
  int minFrom = 1;
  int maxFrom = 15;

  // Upper range
  int minTo = 40;
  int maxTo = 70;

  // Speed range
  int maxSpeed = 110;
  int minSpeed = 3;

  return toMoveUp ? CalcRand(minTo,maxTo) : CalcRand(minFrom,maxFrom);
}

int getNextServoSpeed() {
  int numOfSpeedCategories = 5;
  WaveSpeed waveSpeeds[numOfSpeedCategories];
  waveSpeeds[1].initData(1, 3, 15);
  waveSpeeds[2].initData(2, 16, 40);
  waveSpeeds[3].initData(3, 41, 70);
  waveSpeeds[4].initData(4, 71, 100);
  waveSpeeds[5].initData(5, 101, 110);
  
  return calcNextSpeed(waveSpeeds, numOfSpeedCategories);
}

void calcNextLengthOfServoAction () {
  currentQurterActiveServo = encoderPosition + CalcRand(QUARTER_CLICKS_PER_ROUND_25, QUARTER_CLICKS_PER_ROUND_75);
}

bool servo_update() {
  lastServoLoc = myservo.read();
  encoderPosition = abs(encoder.read());

  if (!isEncoderReachedDestination) {

      isEncoderReachedDestination = motor_reachDestination(&encoder, currentQurterActiveServo);

      toMoveUp = !toMoveUp;      
      int waveSpeed = getNextServoSpeed();
      int nextPos = getNextServoDestination(toMoveUp);

      plateCounter++;
      #ifdef DEBUG_SERVO_MOVE_COUNTER
        Serial.println(String("Starting move: ") + (plateCounter + String(" To location: ") + nextPos));
      #endif
      
      myservo.write(nextPos, waveSpeed, false);
      
      // // Handle the delay
      // if (!isCupMod) {
      //   // Delay count of moves
      //   movesCounter++;
        
      //   #ifdef DEBUG_SERVO_DELAY
      //     Serial.println(String("movesCounter: ") + (movesCounter + String("Next currentCountOfMoves: ") + (currentCountOfMoves)));
      //   #endif
        
      //   if(!toMoveUp && !HandleDelayOfMovement(&movesCounter,&currentCountOfMoves, &currentDelay, minDelay, maxDelay, minChangeInDelay,
      //       minNumOfCount,maxNumOfCount,minChangeInNumOfMoves, &servoActiveDelay)) {
      //     return false;
      //   }
        
      //   if (servoActiveDelay != -1) {
      //     return true;
      //   }
      // }
  } else if (encoderPosition <= currentQuarter * QUARTER_CLICKS_PER_ROUND) {
      // do nothing.. delay
  }
  else {
    currentQuarter++;

    if (currentQuarter <= 4) {
      // start next quarter
      isEncoderReachedDestination = false;
      calcNextLengthOfServoAction();
    }
  }
  
  return true;
}

