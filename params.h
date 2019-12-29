// clicks per rotation (64) * gear ration (70) * teeth ration (128/16=8)
// 360 degrees: 50000
#define CLICKS_PER_ROUND 67500L

// long QUARTER_CLICKS_PER_ROUND = CLICKS_PER_ROUND * 0.25;
// long EIGHTH_CLICKS_PER_ROUND = QUARTER_CLICKS_PER_ROUND / 2;
// long NOT_PAINTING_AREA = EIGHTH_CLICKS_PER_ROUND * 0.05;

// possible values: 0-255
// #define ROTATION_SPEED 70

// possible values: 0-255
// #define LONG_PRESSED_ROTATION_SPEED 200

// direction can be 0 or 1
// #define ROTATION_DIRECTION 1

// #define SERVO_UPDATE_INTERVAL 25

// DC pin
#define ENCODER1_PIN 2
#define ENCODER2_PIN 3
#define DIR1_PWM_PIN 5
#define DIR2_PWM_PIN 6
#define PUSHBUTTON_PIN 10
#define MOTOR_DIR_PIN 11

// Servo pin
#define SERVO_PIN 9

// Debugger parameters

#define DEBUG_CALC_NEXT_RAND_VAL 1
#define DEBUG_ENCODER 0
