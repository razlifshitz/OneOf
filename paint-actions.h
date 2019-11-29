void initDataBeforeFirstRun()
{
    attachEncoder(&encoder);
    attachServo();

    state = BEFORE_DRAWING_MAIN_BRANCH;
    dataCalculated = false;
}

void beforeDrawingMainBranch()
{
    if (not dataCalculated)
    {
        dataCalculated = true;
        // length of waves to skip
        encoderDestination = (numberOfDrawnBranches == 0) ? (getEncdoerLocation() + EIGHTH_CLICKS_PER_ROUND) : (getEncdoerLocation() + QUARTER_CLICKS_PER_ROUND);

        // Serial.println(String("encoderLocation: ") + (encoderLoc));
        // Serial.println(String("encoderDestination: ") + (encoderDestination));
    }

    // move servo to main branch location
    if (myServo.read() != MAIN_BRANCH_LOCATION)
    {
        myServo.write(MAIN_BRANCH_LOCATION, DEFAULT_SPEED);
    }

    // moving the encoder 1/8 of plate to skip the part of the waves of the plate
    if (moveEncoder(encoderDestination, RIGHT))
    {
        state = CALCULATE_LEAFS_SETTINGS;

        // pausing machine and waiting for another button press
        Serial.println(String("pausePaint()"));
        pausePaint();
        onFinishAction();
        delay(SERVO_UPDATE_INTERVAL);
        // buttonPressed = true;
    }
}

void drawFirstWave()
{
    long encoderLoc = getEncdoerLocation();
    if (not dataCalculated)
    {
        dataCalculated = true;

        // counters
        movementsCounter = 0;
        mainBranchTransitionsCounter = -1;
        // main branch length
        encoderDestination = encoderLoc + EIGHTH_CLICKS_PER_ROUND;

        Serial.println(String("encoderLocation: ") + (encoderLoc));
        Serial.println(String("encoderDestination: ") + (encoderDestination));
    }

    // painting the main branch by moving the encoder.
    // servo is in place.
    if (moveEncoder(encoderDestination, RIGHT))
    {
        // stopping servo movement
        myServo.stop();

        Serial.println(String("encoderLocation on finish first wave: ") + (encoderLoc));

        state = DRAWING_SECOND_WAVE;
        onFinishAction();
    }
    else
    {
        // If servo is not moving, getting next movement from array and perfoms the movement
        if (!myServo.isMoving())
        {
            wasTransitionCapture = false;

            // setting the current movement variable
            currentMovement = &firstWave[movementsCounter];

            // performs the movement
            performMovement(currentMovement);
            movementsCounter++;
        }

        // If servo crossed main branch, saves the endoer location
        int lastServoLoc = myServo.read();
        if (!wasTransitionCapture &&
            ((currentMovement->moveUp && lastServoLoc >= MAIN_BRANCH_LOCATION) ||
             (!currentMovement->moveUp && lastServoLoc <= MAIN_BRANCH_LOCATION)))
        {
            wasTransitionCapture = true;
            mainBranchTransitionsCounter++;
            mainBranchTransitionsLocs[mainBranchTransitionsCounter] = getEncdoerLocation();
        }
    }
}

void calculateMovementsSettings()
{
    // wave count
    // upLeafsCount = 10;
    // downLeafsCount = 10;

    // Serial.println(String("upLeafsCount:") + upLeafsCount);
    // Serial.println(String("downLeafsCount: ") + downLeafsCount);

    // generatings leafs data
    generateLeafs(firstWave,
                  5,
                  FIRST_WAVE_FROM_MIN_SERVO_DESTINATION,
                  FIRST_WAVE_FROM_MAX_SERVO_DESTINATION,
                  FIRST_WAVE_TO_MIN_SERVO_DESTINATION,
                  FIRST_WAVE_TO_MAX_SERVO_DESTINATION,
                  FIRST_WAVE_MIN_SERVO_SPEED,
                  FIRST_WAVE_MAX_SERVO_SPEED);
    generateLeafs(secondWaveUp,
                  5,
                  SECOND_WAVE_FROM_MIN_SERVO_DESTINATION_UP,
                  SECOND_WAVE_FROM_MAX_SERVO_DESTINATION_UP,
                  SECOND_WAVE_TO_MIN_SERVO_DESTINATION_UP,
                  SECOND_WAVE_TO_MAX_SERVO_DESTINATION_UP,
                  SECOND_WAVE_MIN_SERVO_SPEED,
                  SECOND_WAVE_MAX_SERVO_SPEED);
    generateLeafs(secondWaveDown,
                  5,
                  SECOND_WAVE_FROM_MIN_SERVO_DESTINATION_DOWN,
                  SECOND_WAVE_FROM_MAX_SERVO_DESTINATION_DOWN,
                  SECOND_WAVE_TO_MIN_SERVO_DESTINATION_DOWN,
                  SECOND_WAVE_TO_MAX_SERVO_DESTINATION_DOWN,
                  SECOND_WAVE_MIN_SERVO_SPEED,
                  SECOND_WAVE_MAX_SERVO_SPEED);

    state = DRAWING_FIRST_WAVE;
}

void drawSecondWave()
{
    long encoderLoc = getEncdoerLocation();

    if (not dataCalculated)
    {
        dataCalculated = true;

        // counters
        upMovementsCounter = 0;
        downMovementsCounter = 0;

        // moving back to to head of the main branch,
        // painting leafs on the way.
        encoderDestination = getEncdoerLocation() - EIGHTH_CLICKS_PER_ROUND;

        Serial.println(String("encoderLocation: ") + (encoderLoc));
        Serial.println(String("encoderDestination: ") + (encoderDestination));
    }

    if (moveEncoder(encoderDestination, LEFT))
    {
        // stopping servo movement
        myServo.stop();

        Serial.println(String("encoderLocation on finish sencond wave: ") + (encoderLoc));

        // encoder reached head of main branch
        state = FINISH;
        onFinishAction();
    }
    else
    {
        if (encoderLoc <= mainBranchTransitionsLocs[mainBranchTransitionsCounter])
        {
            // stopping servo movement
            myServo.stop();

            mainBranchTransitionsCounter--;
        }

        // If servo is not moving, getting next movement from array and perfoms the movement
        if (!myServo.isMoving())
        {
            drawAboveMainBranch = (mainBranchTransitionsCounter % 2) == 0;

            // current movment should be above main branch
            if (drawAboveMainBranch)
            {
                // setting the current movement variable
                currentMovement = &secondWaveUp[upMovementsCounter];
            }
            // current movment should be below main branch
            else
            {
                currentMovement = &secondWaveDown[downMovementsCounter];
            }

            // performs the movement
            performMovement(currentMovement);
            drawAboveMainBranch ? upMovementsCounter++ : downMovementsCounter++;
        }
    }
}

// void moveToNextLeafCreationSpot()
// {
//     long encoderLoc = getEncdoerLocation();

//     if (not dataCalculated)
//     {
//         dataCalculated = true;

//         // counters
//         upDrawnLeafs = 0;
//         downDrawnLeafs = 0;

//         // moving back to to head of the main branch,
//         // painting leafs on the way.
//         encoderDestination = getEncdoerLocation() - EIGHTH_CLICKS_PER_ROUND;

//         Serial.println(String("encoderLocation: ") + (encoderLoc));
//         Serial.println(String("encoderDestination: ") + (encoderDestination));
//     }

//     if (moveEncoder(encoderDestination, LEFT))
//     {
//         Serial.println(String("encoderLocation: ") + (encoderLoc));

//         // encoder reached head of main branch
//         state = FINISH;
//         onFinishAction();
//     }
//     // encoder reached location to draw leaf (UP)
//     else if (hasLeafsToDraw(UP) && encoderLoc <= upLeafs[upDrawnLeafs].creationLocation)
//     {
//         // Serial.println("--------");
//         // Serial.println("hasLeafsToDraw");
//         // Serial.println("--------");
//         Serial.println(String("upDrawnLeafs: ") + (upDrawnLeafs));
//         Serial.println(String("hasLeafsToDraw: ") + (hasLeafsToDraw(UP) ? "TRUE" : "FALSE"));
//         Serial.println(String("encoderLocation: ") + (encoderLoc));
//         Serial.println(String("creationLocation: ") + (upLeafs[upDrawnLeafs].creationLocation));
//         // Serial.println("--------");

//         Serial.println("Reached start of UP leaf number " + String(upDrawnLeafs));
//         Serial.println("Servo loc: " + String(myServo.read()));

//         pauseEncoder();
//         delay(500);

//         leafToDraw = &upLeafs[upDrawnLeafs];

//         //Serial.println(String("encoder reached location: leafToDraw.destination: ") + (&leafToDraw)->movementA->destination);

//         state = DRAWING_LEAF_PART_A;
//     }
//     // encoder reached location to draw leaf (DOWN)
//     else if (hasLeafsToDraw(DOWN) && encoderLoc <= downLeafs[downDrawnLeafs].creationLocation)
//     {
//         Serial.println("Reached start of DOWN leaf number " + String(downDrawnLeafs));
//         Serial.println("Servo loc: " + String(myServo.read()));

//         pauseEncoder();

//         delay(500);

//         leafToDraw = &downLeafs[downDrawnLeafs];

//         state = DRAWING_LEAF_PART_A;
//     }
// }

// void drawLeafPartA()
// {
//     bool doneDrawing = drawLeaf((leafToDraw->movementA), ((leafToDraw->direction) == UP), RIGHT);
//     //Serial.println(String("doneDrawing move A: ") + (doneDrawing ? "TRUE" : "FALSE"));

//     if (doneDrawing)
//     {
//         Serial.println("Servo loc: " + String(myServo.read()));

//         // Done leaf part A
//         state = DRAWING_LEAF_PART_B;
//     }
// }

// void drawLeafPartB()
// {
//     bool doneDrawing = drawLeaf((leafToDraw->movementB), ((leafToDraw->direction) == DOWN), LEFT);
//     //Serial.println(String("doneDrawing move B: ") + (doneDrawing ? "TRUE" : "FALSE"));

//     if (doneDrawing)
//     {
//         // Done leaf part B

//         Serial.println("Servo loc: " + String(myServo.read()));

//         // updating the drawn leaf value
//         leafToDraw->direction == UP ? upDrawnLeafs++ : downDrawnLeafs++;

//         delay(500);

//         state = MOVING_TO_NEXT_LEAF_CREATION_SPOT;
//     }
// }

void finishPaint()
{
    numberOfDrawnBranches = (numberOfDrawnBranches + 1) % 4;

    pausePaint();

    finishPain();
}