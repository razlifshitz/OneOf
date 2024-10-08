// KEYWORDS
const String RIGHT = "right";
const String LEFT = "left";
const String UP = "up";
const String DOWN = "down";

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
const int MAIN_BRANCH_LOCATION = 92;

// ENCODER
const long NO_MANS_LAND = 150;
const long EDGE_LEAF_NO_MANS_LAND = 800;
const int MIN_ENCODER_SEEED = 130;
const int MAX_ENCODER_SEEED = 130;

// LEAFS
const int MIN_LEAFS = 2;
const int MAX_LEAFS = 5;
const int MIN_SERVO_SEEED = 2;
const int MAX_SERVO_SEEED = 2;
const int MIN_SERVO_DESTINATION_UP = 94; // Pnim Maagal
const int MAX_SERVO_DESTINATION_UP = 95;
const int MIN_SERVO_DESTINATION_DOWN = 88; // Hutz Maagal
const int MAX_SERVO_DESTINATION_DOWN = 89;

// GENERAL
const int DEFAULT_SPEED = 90;