void onFinishAction()
{
    dataCalculated = false;
}

void initDataBeforeFirstRun()
{
    attachServo();
    attachEncoder(&encoder);
    state = BEFORE_DRAWING_MAIN_BRANCH;
    dataCalculated = false;
}

void beforeDrawingMainBranch()
{
    if (not dataCalculated)
    {
        encoderDirection = RIGHT;
        encoderDestination = encoderLocation + EIGHTH_CLICKS_PER_ROUND;
    }

    // move servo to main branch location
    if (lastServoLoc != mainBranchLocation)
    {
        myServo.write(mainBranchLocation, DEFAULT_SPEED);
    }

    // moving the encoder 1/8 of plate to skip the part of the waves of the plate
    if (moveEncoderEighthRound(encoderDestination))
    {
        state = DRAWING_MAIN_BRANCH;
        onFinishAction();
    }
}

void drawMainBranch()
{
    if (not dataCalculated)
    {
        encoderDirection = RIGHT;
        encoderDestination = encoderLocation + EIGHTH_CLICKS_PER_ROUND;
    }

    // moving the encoder 1/8 of plate to skip the part of the waves of the plate
    if (moveEncoderEighthRound(encoderDestination))
    {
        state = CALCULATE_LEAFS_SETTINGS;
        onFinishAction();
    }
}

void calculateLeafsSettings()
{

    state = MOVING_TO_NEXT_LEAF_CREATION_SPOT;
}

void moveToNextLeafCreationSpot()
{

    state = DRAWING_LEAF_PART_A;
}

void drawLeafPartA()
{

    state = DRAWING_LEAF_PART_B;
}

void drawLeafPartB()
{

    state = FINISH;
}