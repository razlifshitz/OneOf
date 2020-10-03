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
int minChangeInNumOfMoves = -1;
// count range
int maxNumOfCount = -1;
int minNumOfCount = -1;

long servoActiveDelay = -1;
long firstTime;
bool inDelayProcess;
int plateCounter = 0;  // Number of moves for the entire plate
int movesCounter = -1; // Number of moves in the current wave
bool toMoveUp = true;
int lastServoLoc;
int servoStartLocation; //todo: implementation is needed
long lastUpdate;

// -------------------------------------------- Logic Variables

// DESTINATION
int minFrom = 60; // Bottom range - minimum
int maxFrom = 64; // Bottom range - maximum
int minTo = 77;   // Upper range - minimum
int maxTo = 85;   // Upper range - maximum

// Delay length
int randDelay = -1;
int currentDelay = -1;

// Delay count of moves
int randCountOfMoves = -1;
int currentCountOfMoves = -1;

// Villroy and boch params
int currentQuarter = 0;
long encoderLocation;
long currentQurterActiveServo;
int currentServoDestination;
int previousServoDestination;
int servoDistancePast; // the distance that the servo has past in the current movement
int servoDistance;
int servoDistance75;
int servoDistance25;
bool isEncoderReachedDestination;
bool isServoReachedDestination;
int waveSpeed;
bool beforeStart; // flag that determine if the making of the plate hasn't started yet
// int numberOfDigging = 1;
int diggingSpeed = 135;
int currentEncoderSpeed;
bool isShpitzing;
int shpitzOdds = 5;
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

int getNextServoDestination(bool toMoveUp, int previousLocation)
{
  int newDestination = toMoveUp ? CalcRand(minTo, maxTo) : CalcRand(minFrom, maxFrom);

  // setting new values
  servoDistance = abs(newDestination - previousLocation);
  servoDistance75 = servoDistance * 0.75;
  servoDistance25 = servoDistance * 0.25;

  return newDestination;
}

int getNextServoSpeed()
{
  // FIXME: move to properties section when possible
  int numOfSpeedCategories = 4;
  WaveSpeed waveSpeeds[numOfSpeedCategories + 1];
  waveSpeeds[1].initData(1, 3, 15);    // category 1
  waveSpeeds[2].initData(2, 50, 80);   // category 2
  waveSpeeds[3].initData(3, 81, 100);  // category 3
  waveSpeeds[4].initData(4, 101, 120); // category 4

  return calcNextSpeed(waveSpeeds, numOfSpeedCategories);
}

int getNextShpitzSpeed()
{
  // FIXME: move to properties section when possible
  int numOfSpeedCategories = 4;
  WaveSpeed shpitzSpeeds[numOfSpeedCategories + 1];
  shpitzSpeeds[1].initData(1, 131, 150); // category 1
  shpitzSpeeds[2].initData(2, 151, 170); // category 2
  shpitzSpeeds[3].initData(3, 171, 190); // category 3
  shpitzSpeeds[4].initData(4, 191, 220); // category 4

  return calcNextSpeed(shpitzSpeeds, numOfSpeedCategories);
}

void calcNextLengthOfServoAction()
{
  // activate code when want random length of waves
  // currentQurterActiveServo = encoderLocation + CalcRand(EIGHTH_CLICKS_PER_ROUND_75, EIGHTH_CLICKS_PER_ROUND);

  currentQurterActiveServo = encoderLocation + EIGHTH_CLICKS_PER_ROUND;
}

void printMovement(bool delayPending)
{
  if (delayPending)
  {
    Serial.println(String("move No. ") + plateCounter + String(" From: ") + lastServoLoc + String(" To: ") + currentServoDestination + " direction: " + (toMoveUp ? "Up" : "Down") + " Speed: " + waveSpeed + " And Afterwards Delay will Start!");
  }
  else
  {
    Serial.println(String("move No. ") + plateCounter + String(" From: ") + lastServoLoc + String(" To: ") + currentServoDestination + " direction: " + (toMoveUp ? "Up" : "Down") + " Speed: " + waveSpeed);
  }
}

// void performServoDigging(int location)
// {
//   Serial.println("Digging");
//   delay(300);
//   for (int i = 1; i <= numberOfDigging; i++)
//   {
//     myservo.write(maxTo + 25, diggingSpeed, true);
//     myservo.write(location + 1, diggingSpeed, true);
//   }
//   delay(300);
// }

void onUpdate()
{
  // if (lastServoLoc != myservo.read())
  // {
  // Serial.println(String("1onUpdate--------previousServoDestination: ") + previousServoDestination + String(" lastServoLoc: ") + lastServoLoc + String("  myservo.read(): ") + myservo.read());

  lastServoLoc = myservo.read();
  encoderLocation = abs(encoder.read());
  servoDistancePast = abs(previousServoDestination - lastServoLoc);

  //   Serial.println(String("1onUpdate--------previousServoDestination: ") + previousServoDestination + String(" lastServoLoc: ") + lastServoLoc + String("  myservo.read(): ") + myservo.read());
  // }
}

bool servo_update()
{
  // #ifdef DEBUG_SERVO_MOVE_COUNTER
  //   if (myservo.read() != lastServoLoc) {
  //     Serial.println(String("Servo Location: ") + lastServoLoc);
  //   }
  // #endif

  onUpdate();

  // not continuing unless update update interval passed
  if (millis() - lastUpdate < SERVO_UPDATE_INTERVAL)
  {
    return true;
  }

  // As Long as Encoder hasn't reached to destination where servo should delay, servo will work.
  // if (!isEncoderReachedDestination)
  // {
  // isEncoderReachedDestination = motor_reachDestination(&encoder, currentQurterActiveServo);

  // checking if servo reached destination.
  // doing nothing if servo hasn't reached destination
  if (!isServoReachedDestination)
  {
    isServoReachedDestination = hasServoReachedDestination(lastServoLoc, currentServoDestination, toMoveUp);

    // if (!toMoveUp)
    // {
    //   Serial.println(String("servoDistancePast: ") + servoDistancePast + String(" servoDistance75: ") + servoDistance75);
    // }
    // else
    // {
    //   Serial.println(String("servoDistancePast: ") + servoDistancePast + String(" servoDistance25: ") + servoDistance25);
    // }

    // Stoppin the servo in the wanted locations
    if (!isShpitzing && !toMoveUp && servoDistancePast > servoDistance75)
    {
      // Serial.println("STOPPING DC");
      currentEncoderSpeed = setMotorSpeed(&encoder, 0);
    }
    else if (!isShpitzing && toMoveUp && servoDistancePast > servoDistance25)
    {
      // Serial.println("STARTING DC");
      currentEncoderSpeed = setMotorSpeed(&encoder, ROTATION_SPEED);
    }

    // TODO: is it necessary?
    // continuing the movement to the current destination
    // myservo.write(currentServoDestination, waveSpeed, false);
  }

  // servo reached destination, calc next servo move
  else
  {
    // todo: think about it
    //delay(300);
    isServoReachedDestination = false;

    // if (!toMoveUp /*&& currentEncoderSpeed == 0*/)
    // {
    //   performServoDigging(currentServoDestination);
    //   lastServoLoc = myservo.read();
    //   Serial.println(String("LOC AFTER DIGGING lastServoLoc: ") + lastServoLoc);
    // }

    // calc next servo move
    toMoveUp = !toMoveUp;

    int numTest = CalcRand(1, shpitzOdds);
    Serial.println(String("numTest: ") + numTest + String(" direction: ") + (toMoveUp ? "Up" : "Down"));

    // if in middle of shpitz movement
    if (isShpitzing && !toMoveUp)
    {
      Serial.println("SHPITZ - SECOND MOVE");

      setMotorSpeed(&encoder, 0);
      delay(500);
      setMotorSpeed(&encoder, 30);

      waveSpeed = getNextShpitzSpeed();
      currentServoDestination = minFrom;
    }
    // check if should perform shpitz
    else if (numTest == 1 && toMoveUp)
    {
      Serial.println("SHPITZ - FIRST MOVE");
      isShpitzing = true;
      setMotorSpeed(&encoder, 0);
      delay(500);
      setMotorSpeed(&encoder, 30);

      waveSpeed = getNextShpitzSpeed();
      currentServoDestination = maxTo + 20;
    }
    else
    {
      isShpitzing = false;

      if (toMoveUp)
      {
        // stopping DC as in regular move when servo is in low zone
        setMotorSpeed(&encoder, 0);
        Serial.println("REGULAR MOVE - FIRST MOVE")
      }
      else
      {
        Serial.println("REGULAR MOVE - FIRST MOVE");
      }

      waveSpeed = getNextServoSpeed();
      currentServoDestination = getNextServoDestination(toMoveUp, previousServoDestination);
    }

    previousServoDestination = lastServoLoc;

    // write to monitor
    plateCounter++;
#ifdef DEBUG_SERVO_MOVE_COUNTER
    printMovement(false);
#endif

    // start new servo move
    myservo.write(currentServoDestination, waveSpeed, false);
  }
  // }
  // The Encoder has reached to destination where servo should delay, tell servo to go to low
  // location and delay.
  //   else if (!beforeStart && encoderLocation <= currentQuarter * QUARTER_CLICKS_PER_ROUND)
  //   {
  //     waveSpeed = CalcRand(30, 70);
  //     currentServoDestination = minTo;
  //     isServoReachedDestination = hasServoReachedDestination(lastServoLoc, currentServoDestination, true);

  //     if (!inDelayProcess)
  //     {
  //       inDelayProcess = true;

  // #ifdef DEBUG_SERVO_MOVE_COUNTER
  //       printMovement(true);
  // #endif

  //       currentEncoderSpeed = setMotorSpeed(&encoder, ROTATION_SPEED);
  //       myservo.write(currentServoDestination, waveSpeed, false);
  //     }
  //   }
  // prepares next movement parameters
  // else
  // {
  //   // initialize inDelayProcess value
  //   inDelayProcess = false;
  //   currentQuarter++;

  //   if (currentQuarter <= 4)
  //   {
  //     // start next quarter
  //     isEncoderReachedDestination = false;
  //     previousServoDestination = lastServoLoc;
  //     calcNextLengthOfServoAction();
  //   }
  // }

  lastUpdate = millis();
  return true;
}
