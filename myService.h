//
// GENERAL FUNCTIONS
//

long calcRand(long nMin, long nMax)
{
  return (random() % (nMax - nMin + 1) + nMin);
}

int calcRand(int nMin, int nMax)
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
    setEncoderDirectionAndSpeed(direction, ENCODER_SPEED);
  }

  bool isRight = direction == RIGHT;
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