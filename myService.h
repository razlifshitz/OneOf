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
// GENERAL FUNCTIONS
//
void initDataBeforeFirstRun()
{
  attachServo();
  attachEncoder(&encoder);
}

//
// ENCODER FUNCTIONS
//

int setEncoderDirectionAndSpeed(Encoder *encoder, int dir, int speed)
{
  if (dir)
  {
    digitalWrite(DIR1_PWM_PIN, LOW);
    analogWrite(DIR2_PWM_PIN, speed);
  }
  else
  {
    digitalWrite(DIR2_PWM_PIN, LOW);
    analogWrite(DIR1_PWM_PIN, speed);
  }

  return speed;
}

int setEncoderSpeed(Encoder *encoder, int speed)
{
  return setEncoderDirectionAndSpeed(encoder, digitalRead(MOTOR_DIR_PIN), speed);
}

boolean hasEncoderReachedDestination(long lastEncoderLoc, long destination)
{
  boolean isStopMotor = (lastEncoderLoc) > destination;

#ifdef DEBUG_ENCODER
  Serial.println(String("encoder loc: ") + (lastEncoderLoc) + String(" destination: ") + (destination) + String(" isStopMotor: ") + (isStopMotor ? "TRUE" : "FALSE"));
#endif

  return isStopMotor;
}

//
// SERVO FUNCTIONS
//

int getNextServoDestination(bool toMoveUp, int previousLocation)
{
  int newDestination = toMoveUp ? CalcRand(minTo, maxTo) : CalcRand(minFrom, maxFrom);

  // setting new values
  servoDistance = abs(newDestination - previousLocation);
  servoDistance75 = servoDistance * 0.75;
  servoDistance25 = servoDistance * 0.25;

  return newDestination;
}

bool hasServoReachedDestination(int lastServoLoc, int destination, bool toMoveUp)
{
  //Serial.println(String("lastServoLoc: ") + (lastServoLoc) + String(" destination: ") + (destination) + String(" toMoveUp: ") + (toMoveUp ? "TRUE" : "FALSE"));

  bool result = toMoveUp
                    ? (lastServoLoc) >= destination
                    : (lastServoLoc) <= destination;

  //Serial.println(result ? "TRUE" : "FALSE");

  return result;
}
