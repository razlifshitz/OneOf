String lastState = "";

bool doPaint()
{
  lastServoLoc = myServo.read();
  encoderLocation = abs(encoder.read());

  // printind state to monitor
  if (lastState != state)
  {
    Serial.println(String("state: ") + (state));
    lastState = state;
  }

  // not continuing unless update update interval passed
  if (millis() - lastUpdate < SERVO_UPDATE_INTERVAL)
  {
    //Serial.println(String("millis() ") + (millis()) + String(" lastUpdate ") + (lastUpdate) + String(" = ") + (millis() - lastUpdate));

    return true;
  }

  // entrance point. todo change to switch map<string, function> if possible
  if (state == BEFORE_START)
  {
    Serial.println(String("initDataBeforeFirstRun()"));
    initDataBeforeFirstRun();
  }
  else if (state == BEFORE_DRAWING_MAIN_BRANCH)
  {
    beforeDrawingMainBranch();
    state = DRAWING_MAIN_BRANCH;
  }
  else if (state == DRAWING_MAIN_BRANCH)
  {
    Serial.println(String("drawMainBranch()"));
    drawMainBranch();
  }
  else if (state == CALCULATE_LEAFS_SETTINGS)
  {
    Serial.println(String("calculateLeafsSettings()"));
    calculateLeafsSettings();
  }
  else if (state == MOVING_TO_NEXT_LEAF_CREATION_SPOT)
  {
    Serial.println(String("moveToNextLeafCreationSpot()"));

    moveToNextLeafCreationSpot();
  }
  else if (state == DRAWING_LEAF_PART_A)
  {
    Serial.println(String("drawLeafPartA()"));

    drawLeafPartA();
  }
  else if (state == DRAWING_LEAF_PART_A)
  {
    Serial.println(String("drawLeafPartB()"));

    drawLeafPartB();
  }
  else if (state == FINISH)
  {
    Serial.println(String("finishPain()"));

    finishPain();
  }

  lastUpdate = millis();
  return true;
}
