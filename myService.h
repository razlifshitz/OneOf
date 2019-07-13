#ifdef VAR_SERVO_SPEED
#define VAR_SERVO_SPEED 1
#include <VarSpeedServo.h>
#endif

#ifndef PARAMS
#define PARAMS 1
#include "params.h"
#endif

int CalcRand(int nMin, int nMax)
{
	return (random() % (nMax - nMin + 1) + nMin);
}

// todo: move this to the right place
class MinMaxCategory
{
public:
	MinMaxCategory();
	MinMaxCategory(int minSpeed, int maxSpeed);
	int minSpeed; // min speed range
	int maxSpeed; // max speed range
};

MinMaxCategory::MinMaxCategory() {}

MinMaxCategory::MinMaxCategory(int minS, int maxS)
{
	minSpeed = minS;
	maxSpeed = maxS;
};

int getNextValueFromCategories(MinMaxCategory *categories, int arrLength)
{
	int categoryNumber = CalcRand(0, arrLength - 1);
	// Serial.println(String("categories[categoryNumber].minSpeed: ") + (categories[categoryNumber].minSpeed));
	// Serial.println(String("categories[categoryNumber].maxSpeed: ") + (categories[categoryNumber].maxSpeed));

	return CalcRand(categories[categoryNumber].minSpeed, categories[categoryNumber].maxSpeed);
}

int CalcNextRandVal(int firstParam, int secontParam, int limit, String paramName)
{
	int attemptNumber = 0;
	int value = CalcRand(firstParam, secontParam);

	if (DEBUG_CALC_NEXT_RAND_VAL)
	{
		Serial.println(String("next value: ") + (value) + String(" lowEnd: ") + (firstParam));
		Serial.println(String("highEnd: ") + (secontParam) + String(" minimum Change: ") + (limit));
	}

	while (abs(firstParam - secontParam) < limit)
	{
		Serial.println(String("attemptNumber: ") + attemptNumber);
		attemptNumber++;
		if (attemptNumber < 10000)
		{
			value = CalcRand(firstParam, secontParam);

			if (DEBUG_CALC_NEXT_RAND_VAL)
			{
				Serial.println(String("next value: ") + (value) + String(" lowEnd: ") + (firstParam));
				Serial.println(String("highEnd: ") + (secontParam) + String(" minimum Change: ") + (limit));
			}
		}
		else
		{
			Serial.println(String("EndLess Loop in param: " + paramName));
			return -1;
		}
	}

	if (DEBUG_CALC_NEXT_RAND_VAL)
	{
		Serial.println(String("value " + paramName + " = ") + value);
	}

	return value;
}

bool HandleDelayOfMovement(int *movesCounter, int *currentCountOfMoves, int *currentDelay, MinMaxCategory *delayCategories, int numOfDelayCategories,
						   int minNumOfCount, int maxNumOfCount, int minChangeInNumOfMoves, long *servoActiveDelay)
{
	//Serial.println(String("movesCounter: ") + (*movesCounter) + String(" currentCountOfMoves: ") + (*currentCountOfMoves));
	// Checks if its time for make delay
	//Serial.println(String("Before compare moveCounter !!!!!!!"));

	if (*movesCounter >= *currentCountOfMoves)
	{
		// Delay's the amount of time as calculated
		if (DEBUG_SERVO_DELAY)
		{
			Serial.println(String("currentDelay: ") + (*currentDelay));
		}

		*servoActiveDelay = *currentDelay;

		//delay(*currentDelay);

		/////////////////////////////// Calculates the data for the next delay
		*movesCounter = 0;

		// Delay Length calculation
		*currentDelay = getNextValueFromCategories(delayCategories, numOfDelayCategories);

		if (*currentDelay == -1)
		{
			return false;
		}

		// Delay number of moves calculation
		*currentCountOfMoves = CalcNextRandVal(minNumOfCount, maxNumOfCount, minChangeInNumOfMoves, "minChangeInNumOfMoves");

		if (*currentCountOfMoves == -1)
		{
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

void motor_start(Encoder *encoder, int dir, int speedd)
{
	Serial.println("Starting motor");
	pinMode(DIR1_PWM_PIN, OUTPUT);
	pinMode(DIR2_PWM_PIN, OUTPUT);
	encoder->write(0);

	if (dir)
	{
		digitalWrite(DIR1_PWM_PIN, LOW);
		analogWrite(DIR2_PWM_PIN, speedd);
	}
	else
	{
		digitalWrite(DIR2_PWM_PIN, LOW);
		analogWrite(DIR1_PWM_PIN, speedd);
	}
}

boolean motor_reachedEnd(Encoder *encoder)
{
	long motorPos = abs(encoder->read());
	boolean isStopMotor = (motorPos) > CLICKS_PER_ROUND;

	if (DEBUG_ENCODER)
	{
		Serial.println(String("Motor Pos: ") + (motorPos) + String(" CLICKS_PER_ROUND: ") + (CLICKS_PER_ROUND) + String(" isStopMotor: ") + (isStopMotor ? "TRUE" : "FALSE"));
	}

	return isStopMotor;
}

boolean motor_stop()
{
	Serial.println("Stopping motor");
	//stop motor
	digitalWrite(DIR1_PWM_PIN, LOW);
	digitalWrite(DIR2_PWM_PIN, LOW);

	delay(1000);

	// release motor hold
	pinMode(DIR1_PWM_PIN, INPUT);
	pinMode(DIR2_PWM_PIN, INPUT);
}

boolean isServoShouldStart(Encoder *encoder)
{
	boolean isServoStart = (abs(encoder->read()) > SERVO_START_ANGLE);

	if (DEBUG_ENCODER_IS_SERVO_SHOULD_START_CUPMMOD)
	{
		Serial.println(String("Motor Pos: ") + (abs(encoder->read())) + String(" SERVO_START_ANGLE: ") + (SERVO_START_ANGLE) + String(" isServoShouldStart: ") + (isServoStart ? "TRUE" : "FALSE"));
	}

	return isServoStart;
}
