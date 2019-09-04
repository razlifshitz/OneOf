//
// GENERAL

bool active;
bool buttonPressed;
String state;
long lastUpdate;
bool dataCalculated;

///
/// LEAFS
int numberOfDrawnBranches;
int upLeafsCount;
int upDrawnLeafs;
int downLeafsCount;
int downDrawnLeafs;
Leaf upLeafs[5]; //todo try replace it with constant
Leaf downLeafs[5];
Leaf *leafToDraw;

//
// ENCODER
long encoderLocation;
long encoderDestination;
String encoderDirection;
bool isEncoderMoving;
// int encoderSpeed;

//
// SERVO

// DESTINATION
int minFrom = 1; // Bottom range - minimum
int maxFrom = 1; // Bottom range - maximum
int minTo = 40;  // Upper range - minimum
int maxTo = 70;  // Upper range - maximum

bool toMoveUp = true;
int servoDestination;
int servoDistance;
int servoDistance75;	// needed?
int servoDistance25;	// needed?
int servoStartLocation; //needed?
