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
 * Servo Movement bounderies 
*/
// // Bottom range
// int minFrom = 40;
// int maxFrom = 50;
// // Upper range
// int minTo = 60;
// int maxTo = 80;

int minDest = 50;
int maxDest = 115;

const int numOfDestinaitonCategories = 5;
MinMaxCategory destinationCategories[numOfDestinaitonCategories] = {
	MinMaxCategory(1, 5),
	MinMaxCategory(6, 15),
	MinMaxCategory(16, 30),
	MinMaxCategory(31, 45),
	MinMaxCategory(46, 65)};

/*
 * SPEED
 */
const int numOfSpeedCategories = 5;
MinMaxCategory waveSpeeds[numOfSpeedCategories] = {
	MinMaxCategory(20, 40),
	MinMaxCategory(41, 70),
	MinMaxCategory(71, 110),
	MinMaxCategory(111, 150),
	MinMaxCategory(151, 200)};

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
bool servoReachedDest;
int nextPos;
bool firstIteration;

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

			if (DEBUG_SERVO_DELAY)
			{
				Serial.println((String("The servo has preformed ") + (millis() - firstTime) + String("ms/") + (servoActiveDelay) + String("ms of delay")));
			}
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

void performServoPattern(int toMoveUp)
{
	// Movement properties calculations
	// int posFrom = CalcRand(minFrom, maxFrom);
	// int posTo = CalcRand(minTo, maxTo);
	Serial.println("Selected destination category:");
	int difference = getNextCategoryValue(destinationCategories, numOfDestinaitonCategories);
	Serial.println("Selected speed category:");
	int speed = getNextCategoryValue(waveSpeeds, numOfSpeedCategories);

	//nextPos = toMoveUp ? posTo : posFrom;
	if (toMoveUp)
	{
		nextPos = (lastServoLoc + difference > maxDest) ? maxDest : (lastServoLoc + difference);
	}
	else
	{
		//nextPos = (lastServoLoc - difference < minDest) ? minDest : (lastServoLoc - difference);
		nextPos = minDest;
	}

	if (DEBUG_SERVO_MOVE_COUNTER)
	{
		Serial.println(String("move No. ") + plateCounter + String(" From: ") + lastServoLoc + String(" To: ") + nextPos + " direction: " + (toMoveUp ? "Up" : "Down") + " Speed: " + speed);
	}

	myservo.write(nextPos, speed, false);
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
		(firstIteration || (servoReachedDest /*&& !isPerformingDelay()*/)))
	{
		firstIteration = false;
		toMoveUp = !toMoveUp;
		plateCounter++;

		// Performes servo movement
		performServoPattern(toMoveUp);

		servoReachedDest = false;

		// Handle the delay
		if (!isCupMod)
		{
			// Delay count of moves
			movesCounter++;

			if (DEBUG_SERVO_DELAY)
			{
				Serial.println(String("movesCounter: ") + (movesCounter + String("Next currentCountOfMoves: ") + (currentCountOfMoves)));
			}

			if (!toMoveUp && !HandleDelayOfMovement(&movesCounter, &currentCountOfMoves, &currentDelay, minDelay, maxDelay, minChangeInDelay,
													minNumOfCount, maxNumOfCount, minChangeInNumOfMoves, &servoActiveDelay))
			{
				return false;
			}
		}

		lastUpdated = millis();
	}

	return true;
}
