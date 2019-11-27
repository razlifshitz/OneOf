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
int minChangeInNumOfMoves = -1;
// count range
int maxNumOfCount = -1;
int minNumOfCount = -1;

long lastUpdated = -1;
long servoActiveDelay = -1;
long firstTime;
bool inDelayProcess = false;
int plateCounter = 0;  // Number of moves for the entire plate
int movesCounter = -1; // Number of moves in the current wave
bool toMoveUp = true;
long lastServoLoc = myservo.read();
int servoStartLocation; //todo: implementation is needed
int minServoDelayDest = 51;
int maxServoDelayDest = 60;
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
bool iservoMoving()
{
  long firstTime = millis();
  while (millis() - firstTime < 25)
  {
  }
  if (lastServoLoc == myservo.read())
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool isPerformingDelay()
{
  if (servoActiveDelay != -1)
  {
    if (!inDelayProcess)
    {
      firstTime = millis();
      inDelayProcess = true;
      int servoDelayLoc = CalcRand(minServoDelayDest, maxServoDelayDest);
      myservo.write(servoDelayLoc, 60, true);
      Serial.println("Servo delay location: " + servoDelayLoc);
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
  if (millis() - lastUpdated > SERVO_UPDATE_INTERVAL &&
      !iservoMoving() && !isPerformingDelay())
  {

#ifdef DEBUG_SERVO_DELAY
    Serial.println("-------------------------");
#endif
    toMoveUp = !toMoveUp;

    // Bottom range
    int minFrom = 30;
    int maxFrom = 50;

    // Upper range
    int minTo = 63;
    int maxTo = 83;

    // Speed range
    int maxSpeed = 110;
    int minSpeed = 3;

    int numOfSpeedCategories = 5;
    WaveSpeed waveSpeeds[numOfSpeedCategories];
    waveSpeeds[1].initData(1, 25, 60);
    waveSpeeds[2].initData(2, 61, 75);
    waveSpeeds[3].initData(3, 76, 97);
    waveSpeeds[4].initData(4, 98, 115);
    waveSpeeds[5].initData(5, 120, 150);

    int sssspeed = calcNextSpeed(waveSpeeds, numOfSpeedCategories);
    //Serial.println(String("sssspeed: ") + sssspeed);

    // First Move

    int posFrom = CalcRand(minFrom, maxFrom);
    int posTo = CalcRand(minTo, maxTo);
    //int waveSpeed = CalcRand(minSpeed,maxSpeed);
    int waveSpeed = sssspeed;
    int nextPos = toMoveUp ? posTo : posFrom;

    plateCounter++;

#ifdef DEBUG_SERVO_MOVE_COUNTER
    Serial.println(String("Starting move: ") + (plateCounter + String(" To location: ") + nextPos + String("speed: ") + waveSpeed));
#endif

    myservo.write(nextPos, waveSpeed, false);

    // Handle the delay
    if (!isCupMod)
    {
      // Delay count of moves
      movesCounter++;

#ifdef DEBUG_SERVO_DELAY
      Serial.println(String("movesCounter: ") + (movesCounter + String("Next currentCountOfMoves: ") + (currentCountOfMoves)));
#endif

      if (toMoveUp && !HandleDelayOfMovement(&movesCounter, &currentCountOfMoves, &currentDelay, minDelay, maxDelay, minChangeInDelay,
                                             minNumOfCount, maxNumOfCount, minChangeInNumOfMoves, &servoActiveDelay))
      {
        return false;
      }

      if (servoActiveDelay != -1)
      {
        return true;
      }
    }

    lastUpdated = millis();
  }

  return true;
}
