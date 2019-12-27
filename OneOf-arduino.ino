// 3rd party libraries
#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>
#include <Encoder.h>
#include <VarSpeedServo.h>
#include "button_actions.h"
#include <Arduino.h>

// my libraries
#include "params.h"
#include "global-constants.h"
#include "global-variables.h"
#include "motors-setup.srv.h"
#include "myService.h"
#include "paint-service.h"
#include "paint-actions.h"
#include "paint-utills.h"

//Thread myThread = Thread();
//ThreadController controller = ThreadController();

void setup()
{
	Serial.begin(115200);
	randomSeed(analogRead(0));
	pinMode(PUSHBUTTON_PIN, INPUT_PULLUP);
	pinMode(MOTOR_DIR_PIN, INPUT_PULLUP);
	active = false;
	state = BEFORE_START;
	lastUpdate = millis();
	buttonPressed = false;
	numberOfDrawnBranches = 0;

	//myThread.onRun(onButtonPressed);

	// Now, put bunch of Threads inside it, FEED it!
	//controller.add(&myThread); // Notice the '&' sign before the thread, IF it's not instantied as a pointer.
}

void loop()
{
	int buttonState = checkButton();
	if (buttonState == 1 || buttonState == 3)
	{
		// Button has been clicked
		Serial.println(String("buttonState set to: ") + (buttonState));

		if (buttonState == 3)
		{
			pausePaint();
		}
		else if (buttonState == 1)
		{
			if (state == ENCODER_DRAW)
			{
				state = SERVO_DRAW;
			}

			active = !active;
		}
	}

	if (active)
	{
		doPaint();
	}
	else
	{
		pausePaint();
	}
}
