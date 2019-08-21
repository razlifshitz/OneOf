class ServoMovement
{
public:
    ServoMovement(String dir, int dest, int speed);
    ServoMovement(ServoMovement *movement);
    String direction;
    int destination;
    int speed;
};

ServoMovement::ServoMovement(String dir, int dest, int speed)
{
    direction = dir;
    destination = dest;
    speed = speed;
};

ServoMovement::ServoMovement(ServoMovement *movement)
{
    direction = movement->direction;
    destination = movement->destination;
    speed = movement->speed;
};

class Leaf
{
public:
    Leaf();
    Leaf(Leaf *leaf);
    void initData(ServoMovement *a, ServoMovement *b);
    ServoMovement *movementA;
    ServoMovement *movementB;
};

Leaf::Leaf()
{
    movementA;
    movementB;
};
Leaf::Leaf(Leaf *leaf)
{
    movementA = leaf->movementA;
    movementB = leaf->movementB;
};
void Leaf::initData(ServoMovement *a, ServoMovement *b)
{
    movementA = a;
    movementB = b;
};