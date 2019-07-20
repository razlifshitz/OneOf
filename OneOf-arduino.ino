// 3rd party libraries
#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>
#include <Encoder.h>
#include <VarSpeedServo.h>
#include "button_actions.h"

// my libraries
#include "motors-setup.srv.h"
#include "servo_util.h"

boolean active;
boolean changed;
//Thread myThread = Thread();
//ThreadController controller = ThreadController();

void ThreadServoUpdate()
{
  if (!servo_update())
  {
    detachServo();
    detachEncoder();
    changed = false;
    active = false;
  }
}

void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));
  pinMode(PUSHBUTTON_PIN, INPUT_PULLUP);
  pinMode(MOTOR_DIR_PIN, INPUT_PULLUP);
  active = false;
  changed = false;
  //myThread.onRun(ThreadServoUpdate);

  // Now, put bunch of Threads inside it, FEED it!
  //controller.add(&myThread); // Notice the '&' sign before the thread, IF it's not instantied as a pointer.
}

void initDataBeforeFirstRun()
{
  attachServo();
  attachEncoder(&encoder);
  lastUpdate = millis();
}

void loop()
{
  int buttonState = checkButton();
  if (buttonState == 1 || buttonState == 3)
  {

    // Button has been clicked

    active = !active;
    changed = true;

    Serial.println(String("buttonState set to: ") + (buttonState));
  }

  if (active)
  {
    // Before First run
    if (changed)
    {
      initDataBeforeFirstRun();
      changed = false;
    }
    // Working untill done
    else
    {
      // Normal Behavior
      ThreadServoUpdate();
    }
  }
  // not active enymore
  else
  {
    if (changed)
    {
      detachEncoder();
      detachServo();
      changed = false;
    }
  }
}
