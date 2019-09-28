// clicks per rotation (64) * gear ration (70) * teeth ration (128/16=8)
// 360 degrees: 50000
#define CLICKS_PER_ROUND 67000L

// The Angle that the servo will start working - CUP MODE
#define SERVO_START_ANGLE 20000L

// possible values: 0-255
#define ROTATION_SPEED 60

// possible values: 0-255
#define LONG_PRESSED_ROTATION_SPEED 200

// direction can be 0 or 1
#define ROTATION_DIRECTION 1

#define SERVO_UPDATE_INTERVAL 150

boolean initServoInLoc = true;
int locationToInit = 0;

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

#define DEBUG_SERVO_MOVE_COUNTER 1
#define DEBUG_SERVO_DELAY 0
#define DEBUG_CALC_NEXT_RAND_VAL 0
#define DEBUG_ENCODER 0
#define DEBUG_ENCODER_IS_SERVO_SHOULD_START_CUPMMOD 0
#define STOPPER 0
