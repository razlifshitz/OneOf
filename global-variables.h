//
// GENERAL

bool active;
bool buttonPressed;
String state;
long lastUpdate;
bool dataCalculated;
int numberOfDrawnBranches;

///
/// LEAFS
// int upLeafsCount;
// int upDrawnLeafs;
// int downLeafsCount;
// int downDrawnLeafs;
// Leaf upLeafs[5];
// Leaf downLeafs[5];
// Leaf *leafToDraw;

//
// SERVO MOVEMENTS
int movementsCounter;
int upMovementsCounter;
int downMovementsCounter;
ServoMovement *currentMovement;
ServoMovement firstWave[5];
ServoMovement secondWaveUp[5];
ServoMovement secondWaveDown[5];

//
// ENCODER
long encoderLocation;
long encoderDestination;
String encoderDirection;
bool isEncoderMoving;
// int encoderSpeed;
int mainBranchTransitionsCounter;
bool drawAboveMainBranch;
long mainBranchTransitionsLocs[10];
bool wasTransitionCapture;

//
// SERVO

// DESTINATION
// int minFrom = 1; // Bottom range - minimum
// int maxFrom = 1; // Bottom range - maximum
// int minTo = 40;  // Upper range - minimum
// int maxTo = 70;  // Upper range - maximum

// bool toMoveUp = true;
// int servoDestination;
//int servoDistance;
// int servoDistance75;    // needed?
// int servoDistance25;    // needed?
// int servoStartLocation; //needed?
