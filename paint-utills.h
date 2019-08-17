// -------------------------------------------- Properties

long lastUpdate;

//-------------------------------------- END DATA SETTING

bool doPaint()
{
  lastUpdate = millis();
  lastServoLoc = myservo.read();
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
    state = ENCODER_MOVING_TO_INITIAL_LOCATION;
  }
  else if (state == ENCODER_MOVING_TO_INITIAL_LOCATION)
  {

    state = DRAWING_MAIN_BRANCH;
  }
  else if (state == DRAWING_MAIN_BRANCH)
  {

    state = CALCULATE_LEAFS_SETTINGS;
  }
  else if (state == CALCULATE_LEAFS_SETTINGS)
  {

    state = MOVING_TO_NEXT_LEAF_CREATION_SPOT;
  }
  else if (state == MOVING_TO_NEXT_LEAF_CREATION_SPOT)
  {

    state = DRAWING_LEAF_PART_A;
  }
  else if (state == DRAWING_LEAF_PART_A)
  {

    state = DRAWING_LEAF_PART_B;
  }
  else if (state == DRAWING_LEAF_PART_A)
  {

    state = DRAWING_LEAF_PART_B;
  }
  else if (state == FINISH)
  {
    finishPain();
  }

  lastUpdate = millis();
  return true;
}
