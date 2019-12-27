void initDataBeforeFirstRun()
{
    attachEncoder(&encoder);
    attachServo();

    state = BEFORE_DRAWING;
}

void beforeDraw()
{
    myServo.write(DEFAULT_LOCATION, DEFAULT_SPEED);

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
    pauseEncoder();
    delay(DELAY_FROM_ENCODER_TO_SERVO);
    int servoDestination = calcNextRandVal(SERVO_MIN_DEST, SERVO_MAX_DEST, SERVO_DESTINATION_MIN_CHANGE, "servoDestination");

    if (servoDestination != ERROR)
    {
        myServo.write(servoDestination, SERVO_SPEED, true);
        delay(DELAY_FROM_SERVO_TO_ENCODER);
        state = ENCODER_DRAW;
    }
    else
    {
        state = FINISH;
    }
}

void finishPaint()
{
    pausePaint();
    finishPain();
}