#include <Arduino.h>

//
// CONSTANTS
//

// KEYWORDS
const String RIGHT = "right";
const String LEFT = "left";

// STATE
const String BEFORE_START = "BEFORE_START";
const String BEFORE_DRAWING_MAIN_BRANCH = "BEFORE_DRAWING_MAIN_BRANCH";
const String DRAWING_MAIN_BRANCH = "DRAWING_MAIN_BRANCH";
const String CALCULATE_LEAFS_SETTINGS = "CALCULATE_LEAFS_SETTINGS";
const String MOVING_TO_NEXT_LEAF_CREATION_SPOT = "MOVING_TO_NEXT_LEAF_CREATION_SPOT";
const String DRAWING_LEAF_PART_A = "DRAWING_LEAF_PART_A";
const String DRAWING_LEAF_PART_B = "DRAWING_LEAF_PART_B";
const String FINISH = "FINISH";

// SERVO
const int mainBranchLocation = 50;

// ENCODER
const int NO_MANS_LAND = 3;

// PAINT
const int MIN_LEAFS = 2;
const int MAX_LEAFS = 5;

// GENERAL
const int DEFAULT_SPEED = 50;

//
// VARIABLES
//

//
// GENERAL

bool active;
String state;
long lastUpdate;
bool dataCalculated;

///
/// PAINT
int leafsNumber;

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
int lastServoLoc;
int servoDestination;
int servoDistance;
int servoDistance75;    // needed?
int servoDistance25;    // needed?
int servoStartLocation; //needed?
