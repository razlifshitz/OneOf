//
// GENERAL

bool active;
bool buttonPressed;
String state;
long lastUpdate;
bool dataCalculated;
int numberOfDrawnBranches;

//
// SERVO MOVEMENTS
int firstWaveMovementsCounter;
int secondWaveUpMovementsCounter;
int secondWaveDownMovementsCounter;
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
int mainBranchTransitionsCounter;
bool drawAboveMainBranch;
long mainBranchTransitionsLocs[10];
bool wasTransitionCapture;
