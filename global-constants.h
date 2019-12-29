// KEYWORDS
const String RIGHT = "right";
const String LEFT = "left";
const String UP = "up";
const String DOWN = "down";
const int ERROR = -1;

// STATE
const String BEFORE_START = "BEFORE_START";
const String BEFORE_DRAWING = "BEFORE_DRAWING";
const String ENCODER_DRAW = "ENCODER_DRAW";
const String SERVO_DRAW = "SERVO_DRAW";
const String FINISH = "FINISH";

// ENCODER
const int ENCODER_SPEED = 20;

// SERVO
const int SERVO_SPEED = 10;
const int SERVO_MIN_DEST = 88;
const int SERVO_MAX_DEST = 92;
const int SERVO_DESTINATION_MIN_CHANGE = 1;

// DELAY
const int DELAY_FROM_ENCODER_TO_SERVO = 350;
const int DELAY_FROM_SERVO_TO_ENCODER = 500;

// GENERAL
const int DEFAULT_SPEED = 60;
const int DEFAULT_LOCATION = 90;