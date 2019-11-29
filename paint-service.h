void onFinishAction()
{
    dataCalculated = false;
}

// void calcLeafCreationLocations(Leaf leafs[], int leafsCount)
// {
//     long encoderLoc = getEncdoerLocation();

//     long zone = EIGHTH_CLICKS_PER_ROUND / leafsCount;
//     long minZone = encoderLoc - zone;
//     long maxZone = encoderLoc;

//     for (int i = 0; i < leafsCount; i++)
//     {
//         long loc;
//         // edge leaf (start)
//         if (i == 0)
//         {
//             loc = CalcRand(minZone + NO_MANS_LAND, maxZone - EDGE_LEAF_NO_MANS_LAND);
//         }
//         // edge leaf (end)
//         else if (i == leafsCount)
//         {
//             loc = CalcRand(minZone + EDGE_LEAF_NO_MANS_LAND, maxZone - NO_MANS_LAND);
//         }
//         // regular leaf
//         else
//         {
//             loc = CalcRand(minZone + NO_MANS_LAND, maxZone - NO_MANS_LAND);
//         }

//         leafs[i].creationLocation = loc;
//         minZone -= zone;
//         maxZone -= zone;

//         Serial.println(String("i: ") + i);
//         Serial.println(String("creationLocation: ") + (leafs[i].creationLocation));
//         Serial.println("----------");
//     }
// }

void generateLeafs(ServoMovement movements[],
                   int movementscount, int fromMinDest, int fromMaxDest, int minToDest, int maxToDest, int minSpeed, int maxSpeed)
{
    bool toMoveUp = true;
    int minDest, maxDest;

    for (int i = 0; i < movementscount; i++)
    {
        Serial.println(String("i: ") + i);

        //ServoMovement *moveA = new ServoMovement(); // Create moveA on the heap
        //ServoMovement moveB;

        if (toMoveUp)
        {
            minDest = minToDest;
            maxDest = maxToDest;
        }
        else
        {
            minDest = fromMinDest;
            maxDest = fromMaxDest;
        }

        movements[i].init(minDest, maxDest, minSpeed, maxSpeed, toMoveUp);

        toMoveUp = !toMoveUp;
        // Serial.println(String("direction: ") + (leafs[i].direction));
        // Serial.println(String("movementA.destination:"));
        // Serial.println(leafs[i].movementA.destination);
        // Serial.println(String("movementA.speed:"));
        // Serial.println(leafs[i].movementA.speed);

        //Serial.println(String("movementA.destination: ") + (leafs[i].movementA.destination) + String(" movementA.speed: ") + (leafs[i].movementA.speed));
        //Serial.println(String("movementB.destination: ") + (leafs[i].movementB->destination) + String("movementB.speed: ") + (leafs[i].movementB->speed));
        // Serial.println("-------------");
    }

    // calcLeafCreationLocations(leafs, movementscount);
}

// bool hasLeafsToDraw(String direction)
// {
//     bool result;

//     if (direction == UP)
//     {
//         result = upLeafsCount > upDrawnLeafs;
//     }
//     else
//     {
//         result = downLeafsCount > downDrawnLeafs;
//     }

//     return result;
// }

bool performMovement(ServoMovement *currentMovement)
{
    myServo.write(currentMovement->destination, currentMovement->speed, false);
}

// bool drawLeaf(ServoMovement currentMovement, bool toMoveUp, String encoderDirection)
// {
//     //     Serial.println("--------");
//     //     Serial.println("drawLeaf");
//     //     Serial.println("--------");
//     //     Serial.println(String("drawLeaf: direction: ") + (encoderDirection));
//     //     Serial.println(String("drawLeaf: destination: ") + (currentMovement.destination));
//     //     Serial.println(String("drawLeaf: speed: ") + (currentMovement.speed));
//     //     Serial.println(String("drawLeaf: isMoving: ") + (myServo.isMoving() ? "TRUE" : "FALSE"));
//     //     Serial.println("------------------");

//     bool doneDrawing = false;

//     if (!hasServoReachedDestination(currentMovement.destination, toMoveUp))
//     {
//         if (not isEncoderMoving)
//         {
//             int speed = CalcRand(MIN_ENCODER_SEEED, MAX_ENCODER_SEEED);
//             //Serial.println("drawLeaf: start Encoder");
//             setEncoderDirectionAndSpeed(encoderDirection, speed);
//         }

//         //Serial.println("start moving");
//         myServo.write(currentMovement.destination, currentMovement.speed);

//         return false;
//     }
//     else
//     {
//         //Serial.println("drawLeaf: done drawing.");
//         pauseEncoder();
//         return true;
//     }

//     // return false;
// }