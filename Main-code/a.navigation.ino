/*
  navigation.ino: Take in values from navigation sensors to adjust movement of mBot while in the maze
*/

/* Ultrasonic Sensor*/
#define TIME_OUT 3000
#define SPEED_OF_SOUND 345

/* Get distance Ultrasonic Sensor reads */
float distance_right() {
  digitalWrite(ULTRASONIC_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_PIN, HIGH);
  delayMicroseconds(100);
  digitalWrite(ULTRASONIC_PIN, LOW);

  pinMode(ULTRASONIC_PIN, INPUT);

  long duration = pulseIn(ULTRASONIC_PIN, HIGH, TIME_OUT);
  float distance = (((float)SPEED_OF_SOUND * (float)duration / 10000.0) / 2.0) - 6.45;
  delay(1);

  return distance;
}

/* IR Sensor*/
float ir_measure_volts() {
  float sensorValue = analogRead(IR_PIN); // Read voltage reading from IR detector
  float voltage = sensorValue * (5.0 / 1023); 
  return voltage;
}

/* Get voltage diff values between ambient voltage and new voltage to determine proximity to wall by activating Y0 */
float read_ir() {
  digitalWrite(DECODER_A_PIN, LOW);
  digitalWrite(DECODER_B_PIN, LOW);
  delay(50);
  float IRVoltageNew = ir_measure_volts();
  digitalWrite(DECODER_A_PIN, HIGH);
  digitalWrite(DECODER_B_PIN, HIGH);
  delay(50);
  float IRVoltageAmbient = ir_measure_volts();
  return IRVoltageAmbient - IRVoltageNew; 
}

/* mBot line sensor check if black line has been detected*/
bool has_reached_waypoint() {
  int sensor_state = line_finder.readSensors();
  return sensor_state == S1_IN_S2_IN;
}
