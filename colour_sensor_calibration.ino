#include <MeMCore.h>
#include <SoftwareSerial.h>

/* Pin Declarations */
#define DECODER_A_PIN A2 
#define DECODER_B_PIN A3 
#define LDR_PIN A1

/* Colour Reading Declarations */
#define RGBWait 40
#define LDRWait 20 

/* find the average reading for the requested number of times of scanning LDR */
int getAvgReading(int times){      
  int total =0;
  for(int i = 0; i < times; i++){  //take the reading as many times as requested and add them up
    total += analogRead(LDR_PIN);
    delay(LDRWait);
  }
  return total / times; //take the reading as many times as requested and add them up
}

/* function to get the avg red colour reading (Y3: Red) */
int findRed() {
  digitalWrite(DECODER_A_PIN, HIGH);
  digitalWrite(DECODER_B_PIN, HIGH);
  delay(RGBWait);
  return getAvgReading(5);
}

/* function to get the avg green colour reading (Y2: Green) */
int findGreen() {
  digitalWrite(DECODER_A_PIN, LOW);
  digitalWrite(DECODER_B_PIN, HIGH);
  delay(RGBWait);
  return getAvgReading(5);
}

/* function to get the avg blue colour reading (Y2: Blue) */
int findBlue() {
  digitalWrite(DECODER_A_PIN, HIGH);
  digitalWrite(DECODER_B_PIN, LOW);
  delay(RGBWait);
  return getAvgReading(5);
}

void readColour (int colour[]){
  colour[0] = findRed(); 
  colour[1] = findGreen();
  colour[2] = findBlue();
  return colour;
}

void printColourValues() {
  int currentColour[3];
  readColour(currentColour);
  Serial.print("Red: ");
  Serial.print(currentColour[0]);
  Serial.print(", Green: ");
  Serial.print(currentColour[1]);
  Serial.print(", Blue: ");
  Serial.println(currentColour[2]); 
}

void setup(){
  Serial.begin(9600);
  pinMode(DECODER_A_PIN,OUTPUT);   
  pinMode(DECODER_B_PIN,OUTPUT);  
}

void loop() {
  printColourValues();
}
