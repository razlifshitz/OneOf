#ifdef VAR_SERVO_SPEED
#define VAR_SERVO_SPEED 1
#include <VarSpeedServo.h>
#endif

#include "paramsGenerator.h"
#include "myService.h"

VarSpeedServo myservo;

//////////////////////////////
//////////CUP MODE////////////
//////////////////////////////
bool isCupMod = true;
bool doHandle = false;

// -------------------------------------------- Properties

/*
 * MOVEMENT DATA
 */

// Bottom range
int minFrom = 40;
int maxFrom = 55;
// Upper range
int minTo = 65;
int maxTo = 85;

/* 
 * DELAY DATA (The numbers are in Miliseconds)
*/

// LENGTH
int minChangeInDelay = -1;
// Delay range
int maxDelay = -1;
int minDelay = -1;

// NUMBER OF MOVES (before each delay)
int minChangeInNumOfMoves = -1;
// count range
int maxNumOfCount = -1;
int minNumOfCount = -1;

/*
* OTHER DATA
*/

long lastUpdated = -1;
long servoActiveDelay = -1;
long firstTime;
bool inDelayProcess = false;
int plateCounter = 0;  // Number of moves for the entire plate
int movesCounter = -1; // Number of moves in the current wave
bool toMoveUp = true;
long lastServoLoc = myservo.read();
int servoStartLocation; //todo: implementation is needed
bool servoReachedDest;
int nextPos;
bool firstIteration;

// IKEA parametes
int firstMovementFrom = 30;
int firstMovementTo = maxTo;

// -------------------------------------------- Logic Variables

// Delay length
int randDelay = -1;
int currentDelay = -1;

// Delay count of moves
int randCountOfMoves = -1;
int currentCountOfMoves = -1;

//-------------------------------------- END DATA SETTING

void servo_start()
{
  Serial.println("servo_start()");
  myservo.attach(SERVO_PIN); // attaches the servo on pin 9 to the servo object
}

void servo_stop()
{
  Serial.println("servo_stop()");
  myservo.detach();
}

// todo check if we can remove the 'while'
// bool iservoMoving() {
//   // long firstTime = millis();
//   // while (millis() - firstTime < 25) {

//   // }
//   if (lastServoLoc == myservo.read()) {
//     return false;
//   }
//   else {
//     return true;
//   }
// }

//TODO CHANGE LOGIC
bool isPerformingDelay()
{
  if (servoActiveDelay != -1)
  {
    if (!inDelayProcess)
    {
      firstTime = millis();
      inDelayProcess = true;
    }
    if (millis() - firstTime < servoActiveDelay)
    {
      // Executing the delay
#ifdef DEBUG_SERVO_DELAY
      Serial.println((String("The servo has preformed ") + (millis() - firstTime) + String("ms/") + (servoActiveDelay) + String("ms of delay")));
#endif
    }
    else
    {
      // cancels the active delay
      servoActiveDelay = -1;
      inDelayProcess = false;
    }
  }

  return inDelayProcess;
}

bool servo_update()
{
  //Serial.println("servo_update()");
  lastServoLoc = myservo.read();

  if (!firstIteration && lastServoLoc == nextPos)
  {
    servoReachedDest = true;
  }

  if (millis() - lastUpdated > SERVO_UPDATE_INTERVAL &&
      (firstIteration || (servoReachedDest && !isPerformingDelay())))
  {
    toMoveUp = !toMoveUp;

    int numOfSpeedCategories = 5;
    WaveSpeed waveSpeeds[numOfSpeedCategories + 1];
    waveSpeeds[1].initData(1, 25, 40);
    waveSpeeds[2].initData(2, 50, 110);
    waveSpeeds[3].initData(3, 120, 170);
    waveSpeeds[4].initData(4, 171, 200);
    waveSpeeds[5].initData(5, 200, 235);

    int waveSpeed = calcNextSpeed(waveSpeeds, numOfSpeedCategories);

    // Movmement calculations

    int posFrom = CalcRand(minFrom, maxFrom);
    int posTo;
    if (firstIteration)
    {
      posTo = firstMovementTo;
    }
    else
    {
      posTo = CalcRand(minTo, maxTo);
    }

    nextPos = toMoveUp ? posTo : posFrom;

    plateCounter++;

#ifdef DEBUG_SERVO_MOVE_COUNTER
    Serial.println(String("move No. ") + plateCounter + String(" From: ") + lastServoLoc + String(" To: ") + nextPos + " direction: " + (toMoveUp ? "Up" : "Down") + " Speed: " + waveSpeed);
#endif

    myservo.write(nextPos, waveSpeed, false);
    servoReachedDest = false;

    // Handle the delay
    if (!isCupMod)
    {
      // Delay count of moves
      movesCounter++;

#ifdef DEBUG_SERVO_DELAY
      Serial.println(String("movesCounter: ") + (movesCounter + String("Next currentCountOfMoves: ") + (currentCountOfMoves)));
#endif

      if (!toMoveUp && !HandleDelayOfMovement(&movesCounter, &currentCountOfMoves, &currentDelay, minDelay, maxDelay, minChangeInDelay,
                                              minNumOfCount, maxNumOfCount, minChangeInNumOfMoves, &servoActiveDelay))
      {
        return false;
      }

      if (servoActiveDelay != -1)
      {
        return true;
      }
    }

    firstIteration = false;
    lastUpdated = millis();
  }

  return true;
}
