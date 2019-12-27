void initDataBeforeFirstRun()
{
    attachEncoder(&encoder);
    attachServo();

    state = BEFORE_DRAWING;
}

void beforeDraw()
{
    myServo.write(calcRand(SERVO_MIN_DEST, SERVO_MAX_DEST), DEFAULT_SPEED);

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
    delay(300);
    myServo.write(calcRand(SERVO_MIN_DEST, SERVO_MAX_DEST), SERVO_SPEED, true);
    delay(300);
    state = ENCODER_DRAW;
}

void finishPaint()
{
    pausePaint();
    finishPain();
}