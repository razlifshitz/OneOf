void onFinishAction()
{
    dataCalculated = false;
}

void calcLeafCreationLocations(Leaf *leafs, int leafsCount)
{
    int zone = EIGHTH_CLICKS_PER_ROUND / leafsCount;
    int minZone = encoderLocation - zone;
    int maxZone = encoderLocation;

    for (int i = 0; i < leafsCount; i++)
    {
        leafs[i].creationLocation = CalcRand(minZone + NO_MANS_LAND, maxZone - NO_MANS_LAND);
        minZone -= zone;
        maxZone -= zone;
    }
}

void generateLeafs(Leaf *leafs, int leafsCount, String direction)
{
    for (int i = 0; i < leafsCount; i++)
    {
        ServoMovement moveA;
        ServoMovement moveB;

        leafs[i].initData(direction, &moveA, &moveB);

        Serial.println(String("direction: ") + (leafs[i].direction));
        Serial.println(String("movementA.destination: ") + (leafs[i].movementA->destination) + String("movementA.speed: ") + (leafs[i].movementA->speed));
        Serial.println(String("movementB.destination: ") + (leafs[i].movementB->destination) + String("movementB.speed: ") + (leafs[i].movementB->speed));
        Serial.println("----------");
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

bool drawLeaf(ServoMovement *currentMovement, bool toMoveUp, String encoderDirection)
{
    bool doneDrawing = false;

    if (!myServo.isMoving())
    {
        myServo.write(currentMovement->destination, currentMovement->speed);
    }

    if (hasServoReachedDestination(currentMovement->destination, toMoveUp))
    {
        doneDrawing = true;
    }
    else
    {
        if (not isEncoderMoving)
        {
            setEncoderDirectionAndSpeed(encoderDirection, 70);
        }
    }

    return downLeafs;
}