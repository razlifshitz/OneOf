class ServoMovement
{
public:
    ServoMovement();
    ServoMovement(ServoMovement *movement);
    // ServoMovement(String dir, int dest, int speed);
    int destination;
    int speed;
};

ServoMovement::ServoMovement()
{
    destination = (random() % (MAX_SERVO_DESTINATION - MIN_SERVO_DESTINATION + 1) + MIN_SERVO_DESTINATION);
    speed = (random() % (MAX_SERVO_SEEED - MIN_SERVO_SEEED + 1) + MIN_SERVO_SEEED);

    Serial.println(String("ServoMovement: destination: ") + (destination));
    Serial.println(String("ServoMovement: speed: ") + (speed));
};

// ServoMovement::ServoMovement(String dir, int dest, int speed)
// {
//     direction = dir;
//     destination = dest;
//     speed = speed;
// };

ServoMovement::ServoMovement(ServoMovement *movement)
{
    destination = movement->destination;
    speed = movement->speed;
};

class Leaf
{
public:
    // Leaf(Leaf *leaf);
    void initData(String dir /*, ServoMovement *a*/);
    //void initData(Leaf *leaf);
    ServoMovement movementA;
    ServoMovement movementB;
    long creationLocation;
    String direction;
};

// Leaf::Leaf(Leaf *leaf)
// {
//     movementA = leaf->movementA;
//     movementB = leaf->movementB;
// };

void Leaf::initData(String dir /*, ServoMovement *a*/)
{
    Serial.println(String("LEAF: direction: ") + (dir));
    //Serial.println(String("LEAF: movement.destination: ") + (a->destination) + String(" movement.speed: ") + (a->speed));

    direction = dir;
    // movementA = a;
    // movementB = b;
};

// void Leaf::initData(Leaf *leaf)
// {
//     Serial.println(String("LEAF: direction: ") + (leaf->direction));
//     Serial.println(String("LEAF: creationLocation: ") + (leaf->creationLocation));
//     Serial.println(String("LEAF: movement.destination: ") + (leaf->movementA->destination) + String(" movement.speed: ") + (leaf->movementA->speed));

//     direction = leaf->direction;
//     creationLocation = leaf->creationLocation;
//     direction = leaf->direction;
//     movementA = leaf->movementA;
//     //movementB = leaf->movementB;
// };