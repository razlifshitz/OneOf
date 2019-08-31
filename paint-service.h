void onFinishAction()
{
    dataCalculated = false;
}

void calcLeafCreationLocations(Leaf leafs[], int leafsCount)
{
    long encoderLoc = getEncdoerLocation();

    long zone = EIGHTH_CLICKS_PER_ROUND / leafsCount;
    long minZone = encoderLoc;
    long maxZone = encoderLoc + zone;

    for (int i = 0; i < leafsCount; i++)
    {
        leafs[i].creationLocation = CalcRand(minZone + NO_MANS_LAND, maxZone - NO_MANS_LAND);
        minZone += zone;
        maxZone += zone;

        Serial.println(String("i: ") + i);
        Serial.println(String("creationLocation: ") + (leafs[i].creationLocation));
        Serial.println("----------");
    }
}

void generateLeafs(Leaf leafs[], int leafsCount, String direction)
{
    for (int i = 0; i < leafsCount; i++)
    {
        Serial.println(String("i: ") + i);

        //ServoMovement *moveA = new ServoMovement(); // Create moveA on the heap
        //ServoMovement moveB;

        leafs[i].initData(direction);

        Serial.println(String("direction: ") + (leafs[i].direction));
        Serial.println(String("movementA.destination:"));
        Serial.println(leafs[i].movementA.destination);
        Serial.println(String("movementA.speed:"));
        Serial.println(leafs[i].movementA.speed);

        //Serial.println(String("movementA.destination: ") + (leafs[i].movementA.destination) + String(" movementA.speed: ") + (leafs[i].movementA.speed));
        //Serial.println(String("movementB.destination: ") + (leafs[i].movementB->destination) + String("movementB.speed: ") + (leafs[i].movementB->speed));
        Serial.println("-------------");
    }

    calcLeafCreationLocations(leafs, leafsCount);
}

bool hasLeafsToDraw(String direction)
{
    bool result;

    if (direction == UP)
    {
        result = upLeafsCount > upDrawnLeafs;
    }
    else
    {
        result = downLeafsCount > downDrawnLeafs;
    }

    return result;
}

bool drawLeaf(ServoMovement currentMovement, bool toMoveUp, String encoderDirection)
{
    Serial.println("--------");
    Serial.println("drawLeaf");
    Serial.println("--------");
    Serial.println(String("drawLeaf: direction: ") + (encoderDirection));
    Serial.println(String("drawLeaf: destination: ") + (currentMovement.destination));
    Serial.println(String("drawLeaf: speed: ") + (currentMovement.speed));
    Serial.println(String("drawLeaf: isMoving: ") + (!myServo.isMoving() ? "TRUE" : "FALSE"));
    Serial.println("------------------");

    bool doneDrawing = false;

    if (!myServo.isMoving())
    {
        myServo.write(currentMovement.destination, currentMovement.speed);
    }

    if (hasServoReachedDestination(currentMovement.destination, toMoveUp))
    {
        Serial.println("drawLeaf: done drawing.");
        pauseEncoder();
        doneDrawing = true;
    }
    else
    {
        if (not isEncoderMoving)
        {
            Serial.println("drawLeaf: start Encoder");
            setEncoderDirectionAndSpeed(encoderDirection, 60);
        }
    }

    Serial.println(String("doneDrawing: ") + (doneDrawing));

    return downLeafs;
}