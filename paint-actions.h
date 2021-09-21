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

void calculateMovementsSettings()
{
    // FIRST WAVE MOVEMENT DATA
    generateMovementsData(firstWave,
                          5,
                          FIRST_WAVE_FROM_MIN_SERVO_DESTINATION,
                          FIRST_WAVE_FROM_MAX_SERVO_DESTINATION,
                          FIRST_WAVE_TO_MIN_SERVO_DESTINATION,
                          FIRST_WAVE_TO_MAX_SERVO_DESTINATION,
                          FIRST_WAVE_MIN_SERVO_SPEED,
                          FIRST_WAVE_MAX_SERVO_SPEED);

    // SECOND WAVE MOVEMENT DATA
    // movements above main branch
    generateMovementsData(secondWaveUp,
                          5,
                          SECOND_WAVE_FROM_MIN_SERVO_DESTINATION_UP,
                          SECOND_WAVE_FROM_MAX_SERVO_DESTINATION_UP,
                          SECOND_WAVE_TO_MIN_SERVO_DESTINATION_UP,
                          SECOND_WAVE_TO_MAX_SERVO_DESTINATION_UP,
                          SECOND_WAVE_MIN_SERVO_SPEED,
                          SECOND_WAVE_MAX_SERVO_SPEED);
    // movements below main branch
    generateMovementsData(secondWaveDown,
                          5,
                          SECOND_WAVE_FROM_MIN_SERVO_DESTINATION_DOWN,
                          SECOND_WAVE_FROM_MAX_SERVO_DESTINATION_DOWN,
                          SECOND_WAVE_TO_MIN_SERVO_DESTINATION_DOWN,
                          SECOND_WAVE_TO_MAX_SERVO_DESTINATION_DOWN,
                          SECOND_WAVE_MIN_SERVO_SPEED,
                          SECOND_WAVE_MAX_SERVO_SPEED);

    state = DRAWING_FIRST_WAVE;
}

void drawFirstWave()
{
    long encoderLoc = getEncdoerLocation();
    if (not dataCalculated)
    {
        dataCalculated = true;

        // counters
        firstWaveMovementsCounter = -1;
        mainBranchTransitionsCounter = -1;

        // main branch length
        encoderDestination = encoderLoc + EIGHTH_CLICKS_PER_ROUND;

        Serial.println(String("encoderLocation: ") + (encoderLoc));
        Serial.println(String("encoderDestination: ") + (encoderDestination));
    }

    // moving encoder to the calculated destinaion
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
            firstWaveMovementsCounter++;
            currentMovement = &firstWave[firstWaveMovementsCounter];

            // performs the movement
            performMovement(currentMovement);
        }

        // If servo crossed main branch, saves the endoer location
        int lastServoLoc = myServo.read();
        if (!wasTransitionCapture &&
            ((currentMovement->moveUp && lastServoLoc >= MAIN_BRANCH_LOCATION) ||
             (!currentMovement->moveUp && lastServoLoc <= MAIN_BRANCH_LOCATION)))
        {
            Serial.println(String("transition was captured! encoder location: ") + getEncdoerLocation() + String(" mainBranchTransitionsLocs[mainBranchTransitionsCounter]: ") + mainBranchTransitionsLocs[mainBranchTransitionsCounter]);
            Serial.println(String("currentMovement->moveUp: ") + (currentMovement->moveUp ? "TRUE" : "FALSE"));
            Serial.println(String("lastServoLoc >= MAIN_BRANCH_LOCATION: ") + ((lastServoLoc >= MAIN_BRANCH_LOCATION) ? "TRUE" : "FALSE"));
            Serial.println(String("lastServoLoc <= MAIN_BRANCH_LOCATION: ") + ((lastServoLoc <= MAIN_BRANCH_LOCATION) ? "TRUE" : "FALSE"));

            wasTransitionCapture = true;
            mainBranchTransitionsCounter++;
            mainBranchTransitionsLocs[mainBranchTransitionsCounter] = getEncdoerLocation();
        }
    }
}

void drawSecondWave()
{
    long encoderLoc = getEncdoerLocation();

    if (not dataCalculated)
    {
        dataCalculated = true;

        // counters
        secondWaveUpMovementsCounter = -1;
        secondWaveDownMovementsCounter = -1;

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
        // if encoder passed a servo's transition point with main branch, stoping servo and calculating
        // new servo movement
        if (encoderLoc <= mainBranchTransitionsLocs[mainBranchTransitionsCounter])
        {
            Serial.println(String("!! encoder passed a servo's transition point: ") + mainBranchTransitionsLocs[mainBranchTransitionsCounter] + String(" encoderLoc: ") + encoderLoc);

            // stopping servo movement
            myServo.stop();

            mainBranchTransitionsCounter--;
        }

        // If servo is not moving, getting next movement from array and perfoms the movement
        if (!myServo.isMoving())
        {
            // calculates whether should drawing above/below main branch
            drawAboveMainBranch = (mainBranchTransitionsCounter % 2) == 0;
            Serial.println(String("drawAboveMainBranch: ") + (drawAboveMainBranch ? "TRUE" : "FALSE"));

            // increasing the matching couner
            drawAboveMainBranch ? secondWaveUpMovementsCounter++ : secondWaveDownMovementsCounter++;

            // when current movment should be above main branch
            if (drawAboveMainBranch)
            {
                // setting the current movement variable
                currentMovement = &secondWaveUp[secondWaveUpMovementsCounter];
            }
            // when current movment should be below main branch
            else
            {
                currentMovement = &secondWaveDown[secondWaveDownMovementsCounter];
            }

            // performs the movement
            performMovement(currentMovement);
        }
    }
}

void finishPaint()
{
    numberOfDrawnBranches = (numberOfDrawnBranches + 1) % 8;

    pausePaint();

    finishPain();
}