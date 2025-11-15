/*
  motion.ino: Motion for the motors for forward movement, corrected movement, turning functions and stopping
*/

/* Constants to account for motor differences*/
#define CORRECTION_RIGHT 10
#define CORRECTION_LEFT 0

/* Default Motions */
void move_forward_correction(int correction) {
    leftMotor.run(-FWD_SPEED + correction + CORRECTION_LEFT);
    rightMotor.run(FWD_SPEED + correction - CORRECTION_RIGHT);
}

void move_forward() {
  leftMotor.run(-FWD_SPEED + CORRECTION_LEFT);
  rightMotor.run(FWD_SPEED - CORRECTION_RIGHT);
}

void turn_left() {
  leftMotor.run(TURN_SPEED);
  rightMotor.run(TURN_SPEED);
}

void turn_right() {
  leftMotor.run(-TURN_SPEED);
  rightMotor.run(-TURN_SPEED);
}

void stop() {
  leftMotor.run(0);
  rightMotor.run(0);
}

/* Turn actions based on colour detected */
void turn_right_time(int duration) {
  turn_right();
  delay(duration);
  stop();
  move_forward();
  delay(50);
  State = FORWARD;
}

void turn_left_time(int duration) {
  turn_left();
  delay(duration);
  stop();
  move_forward();
  delay(50);
  State = FORWARD;
}

void compound_turn_right(int duration1, int duration2) {
  // First turn
  turn_right();
  delay(duration1);
  stop();
  // Move forward
  move_forward();
  delay(950);
  // Second turn
  turn_right();
  delay(duration2);
  stop();
  move_forward();
  delay(50);
  State = FORWARD;
}

void compound_turn_left(int duration1, int duration2) {
  // First turn
  turn_left();
  delay(duration1);
  stop();
  // Move forward
  move_forward();
  delay(850);
  // Second turn
  turn_left();
  delay(duration2);
  stop();
  move_forward();
  delay(50);
  State = FORWARD;
}

void uturn_time(int duration) {
  turn_left();
  delay(duration);
  stop();
  State = FORWARD;
}

