String lastState = "";

void doPaint()
{
  // printind state to monitor
  if (lastState != state)
  {
    Serial.println(String("state: ") + (state));
    lastState = state;
  }

  // entrance point. todo change to switch map<string, function> if possible
  if (state == BEFORE_START)
  {
    initDataBeforeFirstRun();
  }
  else if (state == BEFORE_DRAWING)
  {
    beforeDraw();
  }
  else if (state == ENCODER_DRAW)
  {
    encoderDraw();
  }
  else if (state == SERVO_DRAW)
  {
    servoDraw();
  }
  else if (state == FINISH)
  {
    finishPaint();
  }
}
