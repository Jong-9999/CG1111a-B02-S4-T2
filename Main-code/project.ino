/**
 * @file    project.ino
 * @version V1.0.5
 * @date    15/11/2025
 * @brief   Description: This is the code for our 100% working mBot for the Amazing Race!
 */
#include <Arduino.h>
#include <MeMCore.h>
#include <PID_v1.h>

/* Define pins for mBot */
#define IR_PIN A0
#define LDR_PIN A1
#define DECODER_A_PIN A2 
#define DECODER_B_PIN A3 
#define ULTRASONIC_PIN 12 

/* Calibrated values for motion and navigation */
#define FWD_SPEED 255
#define TURN_SPEED 255
#define TURN_LEFT_TIME_MS 320 
#define TURN_RIGHT_TIME_MS 310
#define SECOND_TURN_LEFT_TIME_MS 300 
#define SECOND_TURN_RIGHT_TIME_MS 330  
#define UTURN_TIME_MS 650 

/* Values for colour detection */
#define RGBWait 40
#define LDRWait 20 

/* Initialisation of mBot ports */
MeDCMotor leftMotor(M1);
MeDCMotor rightMotor(M2);
MeLineFollower line_finder(PORT_2);
MeRGBLed rgbled(0, 2);
MeBuzzer buzzer; 

/* Global Variables */
int status = 0;
enum Colour {
    C_RED, C_GREEN, C_ORANGE, C_BLUE, C_PINK, C_WHITE
};
int currentColour[3] = {0, 0, 0}; // Store RGB Values during detection

enum Motion {
    FORWARD, CHECK
};
Motion State;
State = FORWARD; // Default state of motion is FORWARD

/* Variables used for PID Controller */
double Setpoint, Input, Output;
double Kp= 8.0 , Ki=0, Kd= 5.5;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
float correction = 0;

/* Variables used for filtering readings from IR and US */
int ir_filter = 0;
float us_filter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(A7, INPUT); // Button
  Setpoint = 6.5; // PID Setpoint for mBot to maintain 6.5 cm from right wheel of mBot
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-255, 255); // Set maximum range of correction to motor PWM values
  pinMode(DECODER_A_PIN,OUTPUT);    // 2-to-4 Decoder 
  pinMode(DECODER_B_PIN,OUTPUT); 
}

void loop() {
  if (analogRead(A7) < 100) {                                                          // Use mBot Button to toggle mBot to move
    status = 1 - status;
    delay(500);
  }
  if (status == 1) {
    if (State == FORWARD) {
      Input = distance_right();
      myPID.Compute();                                                                 // Start PID computation
      us_filter = correction;                                                          // Store last read US distance reading to filter garbage readings from US Sensor
      correction =  Output;                                                             
      if (Input < 0 || Input > 16 || abs(us_filter - correction) > 120 ) {             // Ultrasonic detector unable to detect wall or is reading garbage values trigger IR Sensor
          float voltage = read_ir();
          if (voltage >= 0.09 && abs(ir_filter - voltage) < 0.01) {                    // mBot is too close to the left trigger mBot to swerve right
            ir_filter = voltage;                                                       // Store last read voltage diff reading to filter garbage readings from IR sensor
            turn_right();
            delay(10);
          } else {                                                                     // mBot is not too close to the left move straight without PID
            move_forward();
          }
      } else {
        move_forward_correction((int)correction);                                      // (int) as arduino motor speed is in int
      }
      if (has_reached_waypoint()) {                                                    // Black line is detected 
        State = CHECK;
        stop();
      } 
    } else if (State == CHECK) {
      readColour();                                                                    // Updates currentColor[3] with R,G,B values
      Colour ColourDetected = matchColour();                                           // Determine the colour of the current tile based on match colour algorithm
      switch (ColourDetected)
      {
        case C_RED:
          turn_left_time(TURN_LEFT_TIME_MS);
          break;
        case C_PINK:
          compound_turn_left(TURN_LEFT_TIME_MS, SECOND_TURN_LEFT_TIME_MS);
          break;
        case C_GREEN:
          turn_right_time(TURN_RIGHT_TIME_MS);
          break;
        case C_BLUE:
          compound_turn_right(TURN_RIGHT_TIME_MS, SECOND_TURN_RIGHT_TIME_MS);
          break;
        case C_ORANGE:
          uturn_time(UTURN_TIME_MS);
          break;
        case C_WHITE:
          stop();
          endSong();
          status = 0;                                                                  // Terminates program 
        default:
          State = FORWARD;
          break;
      }
    }
  }
}
