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

// void printMovement(bool delayPending)
// {
//   if (delayPending)
//   {
//     Serial.println(String("move No. ") + plateCounter + String(" From: ") + lastServoLoc + String(" To: ") + nextServoLocation + " direction: " + (toMoveUp ? "Up" : "Down") + " Speed: " + waveSpeed + " And Afterwards Delay will Start!");
//   }
//   else
//   {
//     Serial.println(String("move No. ") + plateCounter + String(" From: ") + lastServoLoc + String(" To: ") + nextServoLocation + " direction: " + (toMoveUp ? "Up" : "Down") + " Speed: " + waveSpeed);
//   }
// }

//
// ENCODER FUNCTIONS
//

int setEncoderDirectionAndSpeed(String dir, int speed)
{
  if (dir == "right")
  {
    digitalWrite(DIR1_PWM_PIN, LOW);
    analogWrite(DIR2_PWM_PIN, speed);
  }
  else
  {
    digitalWrite(DIR2_PWM_PIN, LOW);
    analogWrite(DIR1_PWM_PIN, speed);
  }

  isEncoderMoving = true;

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

boolean hasEncoderReachedDestination(long lastEncoderLoc, long destination)
{
  boolean isStopMotor = (lastEncoderLoc) > destination;

#ifdef DEBUG_ENCODER
  Serial.println(String("encoder loc: ") + (lastEncoderLoc) + String(" destination: ") + (destination) + String(" isStopMotor: ") + (isStopMotor ? "TRUE" : "FALSE"));
#endif

  return isStopMotor;
}

bool moveEncoderEighthRound(long destination)
{
  bool reached = false;

  if (not isEncoderMoving)
  {
    setEncoderSpeed(70);
  }

  if (hasEncoderReachedDestination(encoderLocation, destination))
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

bool hasServoReachedDestination(int lastServoLoc, int destination, bool toMoveUp)
{
  //Serial.println(String("lastServoLoc: ") + (lastServoLoc) + String(" destination: ") + (destination) + String(" toMoveUp: ") + (toMoveUp ? "TRUE" : "FALSE"));

  bool result = toMoveUp
                    ? (lastServoLoc) >= destination
                    : (lastServoLoc) <= destination;

  //Serial.println(result ? "TRUE" : "FALSE");

  return result;
}

//
// GENERAL FUNCTIONS
//
void pausePaint()
{
  if (myServo.isMoving())
  {
    myServo.stop();
  }

  if (isEncoderMoving)
  {
    pauseEncoder();
  }
}