long lastUpdate;

bool doPaint()
{
  lastUpdate = millis();
  lastServoLoc = myServo.read();
  encoderLocation = abs(encoder.read());

  // not continuing unless update update interval passed
  if (millis() - lastUpdate < SERVO_UPDATE_INTERVAL)
  {
    return true;
  }

  // entrance point. todo change to switch map<string, function> if possible
  if (state == BEFORE_START)
  {
    initDataBeforeFirstRun();
  }
  // else if (state == ENCODER_MOVING_TO_INITIAL_LOCATION)
  // {

  //   state = DRAWING_MAIN_BRANCH;
  // }
  else if (state == DRAWING_MAIN_BRANCH)
  {
    drawMainBranch();
  }
  else if (state == CALCULATE_LEAFS_SETTINGS)
  {
    calculateLeafsSettings();
  }
  else if (state == MOVING_TO_NEXT_LEAF_CREATION_SPOT)
  {
    moveToNextLeafCreationSpot();
  }
  else if (state == DRAWING_LEAF_PART_A)
  {
    drawLeafPartA();
  }
  else if (state == DRAWING_LEAF_PART_A)
  {
    drawLeafPartB();
  }
  else if (state == FINISH)
  {
    finishPain();
  }

  lastUpdate = millis();
  return true;
}
