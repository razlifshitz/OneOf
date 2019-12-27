void initDataBeforeFirstRun()
{
    attachEncoder(&encoder);
    attachServo();

    state = BEFORE_DRAWING;
    dataCalculated = false;
}

void beforeDrawingMainBranch()
{
    // move servo to main branch location
    if (myServo.read() != MAIN_BRANCH_LOCATION)
    {
        myServo.write(MAIN_BRANCH_LOCATION, DEFAULT_SPEED);
    }

    state = ENCODER_DRAW;

    pausePaint();
}

void encoderDraw()
{
    if (moveEncoder(CLICKS_PER_ROUND, RIGHT))
    {
        state = FINISH;
    }
}

void servoDraw()
{
    myServo.write(CalcRand(SERVO_MIN_DEST, SERVO_MAX_DEST), SERVO_SPEED, true);
    delay(300);
    state = ENCODER_DRAW;
}

void finishPaint()
{
    pausePaint();
    finishPain();
}