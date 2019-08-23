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
    void initData(String dir, ServoMovement *a, ServoMovement *b);
    ServoMovement *movementA;
    ServoMovement *movementB;
    long creationLocation;
    String direction;
};

// Leaf::Leaf(Leaf *leaf)
// {
//     movementA = leaf->movementA;
//     movementB = leaf->movementB;
// };

void Leaf::initData(String dir, ServoMovement *a, ServoMovement *b)
{
    direction = dir;
    movementA = a;
    movementB = b;
};