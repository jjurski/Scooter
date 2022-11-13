//----------------------------------------------------------------
//-- Mark Rowland
//-- Troop 605 Robotics Merit Badge

#include <NewPing.h>
#include <VarSpeedServo.h>

const int DEBUG = true;
const int LEFT_WHEEL_PIN = 2;  //Arduino PIN connection
const int RIGHT_WHEEL_PIN = 3;
const int LEFT_LEG_PIN = 4;
const int RIGHT_LEG_PIN = 5;
const int SONIC_TRIGGER_PIN = 8;  // Arduino pin tied to trigger pin on the ultrasonic sensor.
const int SONIC_ECHO_PIN = 9;     // Arduino pin tied to echo pin on the ultrasonic sensor.

const int MAX_DISTANCE = 200;       // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
const int COLLISION_DISTANCE = 40;  // in centimeters
const int SHUTDOWN_TIMER = 10000;
const int WALK_MODE = true;

const int NA = 50;  //Not Apllicable
const int SLOW_SPEED = 20;
const int MED_SPEED = 40;
const int FAST_SPEED = 200;

const int LEFT_WHEEL_FAST = 120;
const int LEFT_WHEEL_FORWARD = 100;
const int LEFT_WHEEL_STOP = 89;
const int LEFT_WHEEL_BACKWARD = 80;
const int RIGHT_WHEEL_FAST = 60;
const int RIGHT_WHEEL_FORWARD = 75;
const int RIGHT_WHEEL_STOP = 90;
const int RIGHT_WHEEL_BACKWARD = 100;

const int RIGHT_LEG_UP = 10;
const int RIGHT_LEG_TILT = 70;
const int RIGHT_LEG_DOWN = 90;
const int RIGHT_LEG_EXTEND = 120;
const int LEFT_LEG_UP = 160;
const int LEFT_LEG_TILT = 100;
const int LEFT_LEG_DOWN = 75;
const int LEFT_LEG_EXTEND = 40;

NewPing Sonar(SONIC_TRIGGER_PIN, SONIC_ECHO_PIN, MAX_DISTANCE);  // NewPing setup of pins and maximum distance.
VarSpeedServo leftWheel, rightWheel, leftLeg, rightLeg;
///////////////////////////////////////////////////////////////////
//-- DIAGNOSTICS       ------------------------------------------//
///////////////////////////////////////////////////////////////////
void testLegs() {
  Serial.println("Right Leg Extend");
  rightLeg.write(RIGHT_LEG_EXTEND, SLOW_SPEED, true);
  delay(2000);
  Serial.println("Right Leg down");
  rightLeg.write(RIGHT_LEG_DOWN, SLOW_SPEED, true);
  delay(2000);
  Serial.println("Right Leg tilt");
  rightLeg.write(RIGHT_LEG_TILT, SLOW_SPEED, true);
  delay(2000);
  Serial.println("Right Leg up");
  rightLeg.write(RIGHT_LEG_UP, SLOW_SPEED, true);
  delay(2000);
  Serial.println("Left Leg Extend");
  leftLeg.write(LEFT_LEG_EXTEND, SLOW_SPEED, true);
  delay(2000);
  Serial.println("Left Leg Extend");
  leftLeg.write(LEFT_LEG_DOWN, SLOW_SPEED, true);
  delay(2000);
  Serial.println("Left Leg Extend");
  leftLeg.write(LEFT_LEG_TILT, SLOW_SPEED, true);
  delay(2000);
  Serial.println("Left Leg Extend");
  leftLeg.write(LEFT_LEG_UP, SLOW_SPEED, true);
  delay(2000);
  Serial.println("Right Wheel Turn");
  rightWheel.write(RIGHT_WHEEL_FORWARD, NA, true);
  delay(5000);
  rightWheel.write(RIGHT_WHEEL_STOP, NA, true);
  Serial.println("Left Wheel Turn");
  leftLeg.write(LEFT_WHEEL_FORWARD, NA, true);
  delay(5000);
  leftLeg.write(LEFT_WHEEL_STOP, NA, true);
}
///////////////////////////////////////////////////////////////////
//-- Basic Routines    ------------------------------------------//
///////////////////////////////////////////////////////////////////
int sonar(const char *label) {
  int Distance = Sonar.ping_cm();
  if (Distance == 0) {
    Distance = MAX_DISTANCE;
  }
  if (DEBUG) {
    Serial.print(label);
    Serial.print(" Ping: ");
    Serial.print(Distance);
    Serial.println("cm");
  }
  return Distance;
}
void attachServos() {
  leftWheel.attach(LEFT_WHEEL_PIN);
  rightWheel.attach(RIGHT_WHEEL_PIN);
  leftLeg.attach(LEFT_LEG_PIN);
  rightLeg.attach(RIGHT_LEG_PIN);
}
void detachServos() {
  leftWheel.detach();
  rightWheel.detach();
  leftLeg.detach();
  rightLeg.detach();
}
void wheelsForward(int duration) {
  leftWheel.write(LEFT_WHEEL_FORWARD, NA, false);
  rightWheel.write(RIGHT_WHEEL_FORWARD, NA, false);
  delay(duration);
}
void wheelsFastForward(int duration) {
  leftWheel.write(LEFT_WHEEL_FAST, NA, false);
  rightWheel.write(RIGHT_WHEEL_FAST, NA, false);
  delay(duration);
}
void wheelsBackward(int duration) {
  rightWheel.write(RIGHT_WHEEL_BACKWARD, NA, false);
  leftWheel.write(LEFT_WHEEL_BACKWARD, NA, false);
  wheelsStop(duration);
}
void wheelsRight(int duration) {
  leftWheel.write(LEFT_WHEEL_FORWARD, NA, false);
  rightWheel.write(RIGHT_WHEEL_BACKWARD, NA, false);
  wheelsStop(duration);
}
void wheelsLeft(int duration) {
  rightWheel.write(RIGHT_WHEEL_FORWARD, NA, false);
  leftWheel.write(LEFT_WHEEL_BACKWARD, NA, false);
  wheelsStop(duration);
}
void feetDown(int duration) {
  delay(duration);
  leftLeg.write(LEFT_LEG_DOWN, FAST_SPEED, false);
  rightLeg.write(RIGHT_LEG_DOWN, FAST_SPEED, false);
  leftLeg.wait();
  rightLeg.wait();
}
void feetUp(int duration) {
  delay(duration);
  leftLeg.write(LEFT_LEG_UP, FAST_SPEED, false);
  rightLeg.write(RIGHT_LEG_UP, FAST_SPEED, false);
  leftLeg.wait();
  rightLeg.wait();
}
void wheelsStop(int duration) {
  delay(duration);
  rightWheel.write(RIGHT_WHEEL_STOP, NA, false);
  leftWheel.write(LEFT_WHEEL_STOP, NA, false);
  leftWheel.wait();
  rightWheel.wait();
}
///////////////////////////////////////////////////////////////////
//-- Walking Routines  ------------------------------------------//
///////////////////////////////////////////////////////////////////
void findNewDirection() {
  do {
    wheelsLeft(200);
    delay(500);
  } while (sonar("FindNewDirection") < 1.5 * COLLISION_DISTANCE);
}
void glide(int duration) {
  feetDown(200);
  wheelsFastForward(duration);
  delay(200);
  wheelsBackward(duration / 2);
}
void leftPirouette(int duration, int toeExtend) {
  rightLeg.write(RIGHT_LEG_TILT, FAST_SPEED, false);
  leftLeg.write(LEFT_LEG_TILT, SLOW_SPEED, false);
  rightLeg.wait();
  leftLeg.wait();
  if (toeExtend) {
    rightLeg.write(RIGHT_LEG_EXTEND, SLOW_SPEED, true);
    delay(200);
  }
  leftWheel.write(LEFT_WHEEL_FORWARD, NA, false);
  delay(duration);
  leftWheel.write(LEFT_WHEEL_STOP, NA, true);
}
void rightPirouette(int duration, int toeExtend) {
  leftLeg.write(LEFT_LEG_TILT, FAST_SPEED, false);
  rightLeg.write(RIGHT_LEG_TILT, SLOW_SPEED, false);
  leftLeg.wait();
  rightLeg.wait();
  if (toeExtend) {
    leftLeg.write(LEFT_LEG_EXTEND, SLOW_SPEED, true);
    delay(200);
  }
  rightWheel.write(RIGHT_WHEEL_FORWARD, NA, false);
  delay(duration);
  rightWheel.write(RIGHT_WHEEL_STOP, NA, true);
}
void pirouette(int duration) {
  feetDown(200);
  leftPirouette(duration, true);
  delay(400);
  feetDown(200);
  rightPirouette(duration, true);
  feetDown(400);
}
void tilt(int steps) {
  feetDown(200);
  for (int i = 0; i < steps; i++) {
    leftLeg.write(LEFT_LEG_TILT, FAST_SPEED, false);
    rightLeg.write(RIGHT_LEG_TILT, SLOW_SPEED, true);
    leftLeg.wait();
    feetDown(200);
    rightLeg.write(RIGHT_LEG_TILT, FAST_SPEED, false);
    leftLeg.write(LEFT_LEG_TILT, SLOW_SPEED, true);
    rightLeg.wait();
    feetDown(200);
  }
  feetDown(200);
}
void jump(int steps) {
  feetDown(200);
  for (int i = 0; i < steps; i++) {
    leftLeg.write(LEFT_LEG_EXTEND, FAST_SPEED, false);
    rightLeg.write(RIGHT_LEG_EXTEND, FAST_SPEED, true);
    leftLeg.write(LEFT_LEG_DOWN, FAST_SPEED, false);
    rightLeg.write(RIGHT_LEG_DOWN, FAST_SPEED, true);
    delay(1000);
  }
}
void walkForwardOld(int steps) {
  feetDown(100);
  for (int i = 0; i < steps; i++) {
    leftPirouette(300, false);
    feetDown(100);
    rightPirouette(300, false);
    feetDown(100);
  }
  feetDown(100);
}
void walkForward(int steps) {
  feetDown(100);
  for (int i = 0; i < steps; i++) {
    rightLeg.write(RIGHT_LEG_EXTEND, FAST_SPEED, false);
    leftLeg.write(LEFT_LEG_TILT, FAST_SPEED, true);
    leftWheel.write(LEFT_WHEEL_FORWARD, NA, false);
    delay(300);
    leftWheel.write(LEFT_WHEEL_STOP, NA, true);
    feetDown(50);
    leftLeg.write(LEFT_LEG_EXTEND, FAST_SPEED, false);
    rightLeg.write(RIGHT_LEG_TILT, FAST_SPEED, true);
    rightWheel.write(RIGHT_WHEEL_FORWARD, NA, false);
    delay(300);
    rightWheel.write(RIGHT_WHEEL_STOP, NA, true);
    feetDown(50);
  }
  delay(100);
}

///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);  // Open serial monitor at 115200 baud to see ping results.
  attachServos();
  leftLeg.write(LEFT_LEG_DOWN, FAST_SPEED, true);
  rightLeg.write(RIGHT_LEG_DOWN, FAST_SPEED, true);
  leftWheel.write(LEFT_WHEEL_STOP, NA, true);
  rightWheel.write(RIGHT_WHEEL_STOP, NA, true);
  delay(3000);
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
  int distance, rightDistance, leftDistance;
  int count = 0;
  Serial.println("Start");
  if (WALK_MODE) {
    glide(2000);
    jump(3);
    tilt(8);
    pirouette(2000);
    walkForward(8);
    feetUp(200);
  }
  do {
    distance = sonar("Main");
    if (distance < COLLISION_DISTANCE) {
      Serial.println("COLLISION");
      wheelsStop(0);
      wheelsBackward(400);
      wheelsRight(400);
      rightDistance = sonar("Right");
      wheelsLeft(600);
      leftDistance = sonar("Left");
      if ((rightDistance > leftDistance) && (rightDistance > COLLISION_DISTANCE)) {
        wheelsRight(700);
      } else if (leftDistance < COLLISION_DISTANCE) {
        Serial.println("find new direction");
        findNewDirection();
      }
    } else if (distance < MAX_DISTANCE / 2) {
      wheelsForward(400);
    } else if ((count > 100) && (!(random(10) % 8))) {
      switch (random(5)) {
        case 0: glide(2000); break;
        case 1: tilt(8); break;
        case 2: leftPirouette(2000,true); break;
        case 3: rightPirouette(2000, true); break;
        default: walkForward(8);
      }
      count=0;
      feetUp(200);
    } else {
      wheelsFastForward(800);
    }
    Serial.println(count);
  } while (++count < SHUTDOWN_TIMER);
  detachServos();
  exit(0);
}
