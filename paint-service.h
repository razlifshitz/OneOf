void onFinishAction()
{
    dataCalculated = false;
}

void generateMovementsData(ServoMovement movements[],
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
}

void performMovement(ServoMovement *currentMovement)
{
    Serial.println(String("Perform Movement to destination: ") + currentMovement->destination + String(" speed: ") + currentMovement->speed);
    myServo.write(currentMovement->destination, currentMovement->speed, false);
}