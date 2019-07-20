#include "myService.h"

// -------------------------------------------- Properties

long lastUpdate;

//-------------------------------------- END DATA SETTING

bool servo_update()
{
  lastServoLoc = myservo.read();
  encoderLocation = abs(encoder.read());

  // not continuing unless update update interval passed
  if (millis() - lastUpdate < SERVO_UPDATE_INTERVAL)
  {
    return true;
  }

  if (encoderLocation <= HALF_QUARTER_CLICKS_PER_ROUND)
  {
    /* code */
  }

  lastUpdate = millis();
  return true;
}
