void initDataBeforeFirstRun()
{
    attachServo();
    attachEncoder(&encoder);
    state = DRAWING_MAIN_BRANCH; // fixme
    encoderDirection = RIGHT;
    // servoDestination = mainBranchLocation;
}

bool drawMainBranch()
{
    if (not isEncoderMoving)
    {
        setEncoderSpeed(70);
    }

    if (hasEncoderReachedDestination(encoderLocation, HALF_QUARTER_CLICKS_PER_ROUND))
    {
        pauseEncoder();
        state = CALCULATE_LEAFS_SETTINGS;
    }
}

bool calculateLeafsSettings()
{

    state = MOVING_TO_NEXT_LEAF_CREATION_SPOT;
}

bool moveToNextLeafCreationSpot()
{

    state = DRAWING_LEAF_PART_A;
}

bool drawLeafPartA()
{

    state = DRAWING_LEAF_PART_B;
}

bool drawLeafPartB()
{

    state = FINISH;
}