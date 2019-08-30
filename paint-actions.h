void initDataBeforeFirstRun()
{
    attachEncoder(&encoder);
    state = BEFORE_DRAWING_MAIN_BRANCH;
    dataCalculated = false;
}

void beforeDrawingMainBranch()
{
    if (not dataCalculated)
    {
        dataCalculated = true;

        // length of waves to skip
        encoderDestination = encoderLocation + EIGHTH_CLICKS_PER_ROUND;
    }

    // move servo to main branch location
    if (myServo.read() != mainBranchLocation)
    {
        myServo.write(mainBranchLocation, DEFAULT_SPEED);
    }

    // moving the encoder 1/8 of plate to skip the part of the waves of the plate
    if (moveEncoder(encoderDestination, RIGHT))
    {
        state = DRAWING_MAIN_BRANCH;

        // pausing machine and waiting for another button press
        Serial.println(String("pausePaint()"));
        pausePaint();
        onFinishAction();
    }
}

void drawMainBranch()
{
    if (not dataCalculated)
    {
        dataCalculated = true;

        // main branch length
        encoderDestination = encoderLocation + EIGHTH_CLICKS_PER_ROUND;
    }

    // painting the main branch by moving the encoder.
    // servo is in place.
    if (moveEncoder(encoderDestination, RIGHT))
    {
        state = CALCULATE_LEAFS_SETTINGS;
        onFinishAction();
    }
}

bool temp = true;

void killMeNow()
{
    // for (int i = 0; i < upLeafsCount; i++)
    // {
    //     Serial.println("-------killMeNow-------------");
    //     Serial.println(String("i: ") + i);
    //     Serial.println(String("direction: ") + (upLeafs[i].direction));
    //     Serial.println(String("movementA.destination: ") + (upLeafs[i].movementA->destination));
    //     Serial.println(String("movementA.speed: ") + (upLeafs[i].movementA->speed));
    // }
}

void calculateLeafsSettings()
{
    // leafs count
    upLeafsCount = CalcRand(MIN_LEAFS, MAX_LEAFS);
    downLeafsCount = CalcRand(MIN_LEAFS, MAX_LEAFS);

    Serial.println(String("upLeafsCount:") + upLeafsCount);
    Serial.println(String("downLeafsCount: ") + downLeafsCount);

    // generatings leafs data
    generateLeafs(upLeafs, upLeafsCount, UP);
    //generateLeafs(downLeafs, downLeafsCount, DOWN);

    state = MOVING_TO_NEXT_LEAF_CREATION_SPOT;

    temp = false;
    Serial.println("1");
    killMeNow();
}

void moveToNextLeafCreationSpot()
{
    Serial.println("2");
    killMeNow();

    if (not dataCalculated)
    {
        dataCalculated = true;
        attachServo();

        // counters
        upDrawnLeafs = 0;
        downDrawnLeafs = 0;

        // moving back to to head of the main branch,
        // painting leafs on the way.
        encoderDestination = encoderLocation + EIGHTH_CLICKS_PER_ROUND;
    }

    Serial.println("3");
    killMeNow();

    if (moveEncoder(encoderDestination, LEFT))
    {
        // encoder reached head of main branch
        state = FINISH;
        onFinishAction();
    }
    // encoder reached location to draw leaf (UP)
    else if (hasLeafsToDraw(UP) && encoderLocation >= upLeafs[upDrawnLeafs].creationLocation)
    {
        Serial.println(String("upDrawnLeafs: ") + upDrawnLeafs);
        Serial.println(String("hasLeafsToDraw: ") + (hasLeafsToDraw(UP) ? "TRUE" : "FALSE") + String(" encoderLocation: ") + (encoderLocation) + String(" creationLocation: ") + (upLeafs[upDrawnLeafs].creationLocation));

        Serial.println("Reached start of leaf");
        pauseEncoder();
        delay(SERVO_UPDATE_INTERVAL);
        Serial.println("4");
        killMeNow();
        Serial.println(String("encoder reached location: creationLocation: ") + (upLeafs[upDrawnLeafs].creationLocation));
        // Serial.println(String("encoder reached location: destination: ") + String(upLeafs[upDrawnLeafs].movementA->destination));
        // Serial.println(String("encoder reached location: speed: ") + String(upLeafs[upDrawnLeafs].movementA->speed));
        //leafToDraw.initData(&upLeafs[upDrawnLeafs]);
        //leafToDraw.initData(upLeafs[upDrawnLeafs].direction, upLeafs[upDrawnLeafs].movementA);

        leafToDraw = &upLeafs[3];

        //Serial.println(String("encoder reached location: leafToDraw.destination: ") + (&leafToDraw)->movementA->destination);

        state = DRAWING_LEAF_PART_A;
    }
    // encoder reached location to draw leaf (DOWN)
    // else if (hasLeafsToDraw(DOWN) && encoderLocation >= downLeafs[downDrawnLeafs].creationLocation)
    // {
    //     Serial.println("Reached start of leaf");
    //     pauseEncoder();
    //     leafToDraw = downLeafs[downDrawnLeafs];
    //     state = DRAWING_LEAF_PART_A;
    // }
}

void drawLeafPartA()
{
    if (drawLeaf(&(leafToDraw->movementA), leafToDraw->direction == UP, RIGHT))
    {
        // Done leaf part A
        state = DRAWING_LEAF_PART_B;
    }
}

void drawLeafPartB()
{
    if (drawLeaf(&(leafToDraw->movementB), leafToDraw->direction == DOWN, LEFT))
    {
        // Done leaf part B

        // updating the drawn leaf value
        leafToDraw->direction == UP ? upDrawnLeafs++ : downDrawnLeafs++;

        state = MOVING_TO_NEXT_LEAF_CREATION_SPOT;
    }
}