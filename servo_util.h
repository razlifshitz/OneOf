#ifdef VAR_SERVO_SPEED
  #define VAR_SERVO_SPEED 1
  #include <VarSpeedServo.h>
#endif

#include "paramsGenerator.h"
#include "myService.h"

VarSpeedServo myservo;

// -------------------------------------------- User Properties

//////////////////////////////
//////////CUP MODE////////////
////////////////////////////// 
bool isCupMod = false;

/*
 * RANGE DATA
 */

// Lower range
int minFrom = 1;
int maxFrom = 15;
// Upper range
int minTo = 40;
int maxTo = 70;


/*
 * SPEED DATA
 */

int maxSpeed = 110;
int minSpeed = 3;

// -------------------------------------------- Auto Generated Properties

/* 
 * DELAY DATA (The numbers are in Miliseconds)
 */

// LENGTH
int minDelay = -1;
int maxDelay = -1;
int minChangeInDelay = -1;

// NUMBER OF MOVES (before each delay)
int minNumOfCount = -1;
int maxNumOfCount = -1;
int minChangeInNumOfMoves = -1;

// -------------------------------------------- System Properties

int scheduledDelay = -1;      // Delay length of the next scheduled delay
long activeDelay = -1;        // Length of the delay that is perfoming right now

bool toMoveUp = true;         // Boolen that indicates whether the servo should go up or down
int movesCounter = 0;         // Number of moves in the current wave 
int movesBeforeNextDelay = 0; // Number of moves that the servo will perform before its next delay
int plateMovesCounter = 0;    // Servo moves counter for the entire plate


//--------------------------------------------


void servo_start() {
  Serial.println("servo_start()");
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
}

void servo_stop() {
  Serial.println("servo_stop()");
  myservo.detach();
}

bool iservoMoving(int* lastServoLoc) {
  long firstTime = millis();
  while (millis() - firstTime < 25) {
    
  }
  if (*lastServoLoc == myservo.read()) {
    return false;
  }
  else {
    return true;
  }
}

bool isDelayPending() {
  if (activeDelay == -1) {
    return false;
  }
  else {
    long firstTime = millis();
    while (millis() - firstTime < activeDelay) {
      // Executing the delay
        #ifdef DEBUG_SERVO_DELAY
          Serial.println((String("The servo has preformed ") + (millis() - firstTime) + String("ms/") + (activeDelay) + String("ms of delay")));
        #endif
    }
    activeDelay = -1;
    return false;
  }
}

bool servo_update() {
  //Serial.println("servo_update()");
  long lastServoLoc = myservo.read();
  long lastUpdated = millis();

  if (millis() - lastUpdated > SERVO_UPDATE_INTERVAL && 
      !iservoMoving(&lastServoLoc) && !isDelayPending()) {

  #ifdef DEBUG_SERVO_DELAY
    Serial.println("-------------------------");
  #endif

      toMoveUp = !toMoveUp;

      // First Move
      
      int posFrom = CalcRand(minFrom,maxFrom);
      int posTo = CalcRand(minTo,maxTo);
      int waveSpeed = CalcRand(minSpeed,maxSpeed);
      int nextPos = toMoveUp ? posTo : posFrom;

      plateMovesCounter++;
      
      #ifdef DEBUG_SERVO_MOVE_COUNTER
        Serial.println(String("Starting move: ") + (plateMovesCounter + String(" To location: ") + nextPos));
      #endif
      
      myservo.write(nextPos, waveSpeed, false);
      
      // Handle the delay
      if (!isCupMod) {
        // Delay count of moves
        movesCounter++;
        
        #ifdef DEBUG_SERVO_DELAY
          Serial.println(String("movesCounter: ") + (movesCounter + String(" movesBeforeNextDelay: ") + (movesBeforeNextDelay)));
        #endif
        
        if(!toMoveUp && !HandleDelayOfMovement(&movesCounter, &movesBeforeNextDelay, &scheduledDelay, minDelay, maxDelay, minChangeInDelay,
            minNumOfCount,maxNumOfCount,minChangeInNumOfMoves, &activeDelay)) {
          return false;
        }
        
        if (activeDelay != -1) {
          return true;
        }
      }
      
      lastUpdated = millis();
  }
  
  return true;
}
