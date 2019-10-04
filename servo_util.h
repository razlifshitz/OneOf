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
 * SPEED
 */
const int numOfSpeedCategories = 5;
MinMaxCategory speedCategories[numOfSpeedCategories] = {
	MinMaxCategory(110, 130),
	MinMaxCategory(131, 150),
	MinMaxCategory(151, 170),
	MinMaxCategory(171, 190),
	MinMaxCategory(191, 220)};

// Movement properties calculations
int posFrom = 15;
int posTo = 45;

/* 
 * DELAY DATA (The numbers are in Miliseconds)
*/

const int numOfDelayCategories = 4;
MinMaxCategory delayCategories[numOfDelayCategories] = {
	MinMaxCategory(150, 200),
	MinMaxCategory(201, 800),
	MinMaxCategory(801, 1500),
	MinMaxCategory(1700, 2500)};

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
int currentEncoderSpeed;
bool doAfterServoFinishPattern;

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
	int speed = getNextValueFromCategories(speedCategories, numOfSpeedCategories);

	nextPos = toMoveUp ? posTo : posFrom;

	if (DEBUG_SERVO_MOVE_COUNTER)
	{
		Serial.println(String("move No. ") + plateCounter + String(" From: ") + lastServoLoc + String(" To: ") + nextPos + " direction: " + (toMoveUp ? "Up" : "Down") + " Speed: " + speed);
	}

	myservo.write(nextPos, speed, false);
}

bool servo_update()
{
	lastServoLoc = myservo.read();

	if (!firstIteration && lastServoLoc == nextPos)
	{
		servoReachedDest = true;

		// This code block relates to when servo has finished pattern
		if (toMoveUp && doAfterServoFinishPattern)
		{
			doAfterServoFinishPattern = false;

			currentEncoderSpeed = setEncoderSpeed(&encoder, 0);
			delay(500);
			currentEncoderSpeed = setEncoderSpeed(&encoder, ROTATION_SPEED);
		}
	}

	if (millis() - lastUpdated > SERVO_UPDATE_INTERVAL &&
		(firstIteration || (servoReachedDest && !isPerformingDelay())))
	{
		firstIteration = false;
		toMoveUp = !toMoveUp;
		plateCounter++;

		// This code block relates to before servo has started pattern
		if (!toMoveUp)
		{
			doAfterServoFinishPattern = true;

			currentEncoderSpeed = setEncoderSpeed(&encoder, 0);
			delay(500);
			currentEncoderSpeed = setEncoderSpeed(&encoder, 30);
		}

		performServoPattern(toMoveUp);

		servoReachedDest = false;

		// Handle the delay
		if (!isCupMod)
		{
			// Delay count of moves
			movesCounter++;

			// if (DEBUG_SERVO_DELAY)
			// {
			// 	Serial.println(String("movesCounter: ") + (movesCounter + String("Next currentCountOfMoves: ") + (currentCountOfMoves)));
			// }

			if (toMoveUp && !HandleDelayOfMovement(&movesCounter, &currentCountOfMoves, &currentDelay, delayCategories, numOfDelayCategories,
												   minNumOfCount, maxNumOfCount, minChangeInNumOfMoves, &servoActiveDelay))
			{
				return false;
			}
		}

		lastUpdated = millis();
	}

	return true;
}
