//
// GENERAL FUNCTIONS
//

long CalcRand(long nMin, long nMax)
{
  return (random() % (nMax - nMin + 1) + nMin);
}

int CalcRand(int nMin, int nMax)
{
  return (random() % (nMax - nMin + 1) + nMin);
}

//
// ENCODER FUNCTIONS
//

//fixme change dir param to destination
int setEncoderDirectionAndSpeed(String dir, int speed)
{
  if (dir == RIGHT)
  {
    digitalWrite(DIR1_PWM_PIN, LOW);
    analogWrite(DIR2_PWM_PIN, speed);
  }
  else
  {
    digitalWrite(DIR2_PWM_PIN, LOW);
    analogWrite(DIR1_PWM_PIN, speed);
  }

  if (speed != 0)
  {
    isEncoderMoving = true;
  }

  return speed;
}

int setEncoderSpeed(int speed)
{
  return setEncoderDirectionAndSpeed(encoderDirection, speed);
}

int pauseEncoder()
{
  isEncoderMoving = false;
  return setEncoderSpeed(0);
}

long getEncdoerLocation()
{
  return encoder.read();
}

bool hasEncoderReachedDestination(long destination, bool toMoveRight)
{
  //fixme
  long encLoc = getEncdoerLocation();
  bool isStopMotor = toMoveRight
                         ? encLoc >= destination
                         : encLoc <= destination;

  // Serial.println(String("----------------------------"));
  // Serial.println(String("encoder loc: ") + (encLoc));
  // Serial.println(String("toMoveRight: ") + (toMoveRight));
  // Serial.println(String("destination: ") + (destination));
  // Serial.println(String("isStopMotor: ") + (isStopMotor ? "TRUE" : "FALSE"));

#ifdef DEBUG_ENCODER
  Serial.println(String("encoder loc: ") + (encLoc) + String(" destination: ") + (destination) + String(" isStopMotor: ") + (isStopMotor ? "TRUE" : "FALSE"));
#endif

  return isStopMotor;
}

bool moveEncoder(long destination, String direction)
{
  bool reached = false;

  if (not isEncoderMoving)
  {
    setEncoderDirectionAndSpeed(direction, DEFAULT_SPEED);
  }

  bool isRight = direction == "right";
  if (hasEncoderReachedDestination(destination, isRight))
  {
    reached = true;
    pauseEncoder();
  }

  return reached;
}

//
// SERVO FUNCTIONS
//

// int getNextServoDestination(bool toMoveUp, int previousLocation)
// {
//   int newDestination = toMoveUp ? CalcRand(minTo, maxTo) : CalcRand(minFrom, maxFrom);

//   // setting new values
//   servoDistance = abs(newDestination - previousLocation);
//   servoDistance75 = servoDistance * 0.75;
//   servoDistance25 = servoDistance * 0.25;

//   return newDestination;
// }

bool hasServoReachedDestination(int destination, bool toMoveUp)
{
  int lastServoLoc = myServo.read();

  bool result = toMoveUp
                    ? (lastServoLoc) >= destination
                    : (lastServoLoc) <= destination;

  // Serial.println(String("lastServoLoc: ") + (lastServoLoc) + String(" destination: ") + (destination) + String(" toMoveUp: ") + (toMoveUp ? "TRUE" : "FALSE"));
  // Serial.println(String("hasServoReachedDestination: ") + (result));
  // Serial.println(String("----------------------------------"));

  //Serial.println(result ? "TRUE" : "FALSE");

  return result;
}

//
// GENERAL FUNCTIONS
//
void pausePaint()
{
  active = false;

  if (myServo.isMoving())
  {
    myServo.stop();
  }

  if (isEncoderMoving)
  {
    pauseEncoder();
  }
}