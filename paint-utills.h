String lastState = "";

void doPaint()
{
  //encoderLocation = abs(encoder.read());

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
    initDataBeforeFirstRun();
  }
  else if (state == BEFORE_DRAWING_MAIN_BRANCH)
  {
    beforeDrawingMainBranch();
  }
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
}
