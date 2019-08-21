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
    upLeafsNumber = CalcRand(MIN_LEAFS, MAX_LEAFS);
    downLeafsNumber = CalcRand(MIN_LEAFS, MAX_LEAFS);

    int leafsNumber = upLeafsNumber + downLeafsNumber;
    leafs[leafsNumber];

    for (int i = 0; i < leafsNumber; i++)
    {
        String direction;
        if (i < upLeafsNumber)
        {
            direction = UP;
        }
        else
        {
            direction = DOWN;
        }

        ServoMovement moveA(direction,
                            CalcRand(MIN_SERVO_DESTINATION, MAX_SERVO_DESTINATION),
                            CalcRand(MIN_SERVO_SEEED, MAX_SERVO_SEEED));
        ServoMovement moveB(direction,
                            CalcRand(MIN_SERVO_DESTINATION, MAX_SERVO_DESTINATION),
                            CalcRand(MIN_SERVO_SEEED, MAX_SERVO_SEEED));

        leafs[i].initData(&moveA, &moveB);

        Serial.println(String("movementA.direction: ") + (leafs[i].movementA->direction) + String("movementA.destination: ") + (leafs[i].movementA->destination) + String("movementA.speed: ") + (leafs[i].movementA->speed));
        Serial.println(String("movementB.direction: ") + (leafs[i].movementB->direction) + String("movementB.destination: ") + (leafs[i].movementB->destination) + String("movementB.speed: ") + (leafs[i].movementB->speed));
        Serial.println("----------");
    }

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