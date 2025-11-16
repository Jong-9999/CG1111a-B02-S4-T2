#include <MeMCore.h>
#include <SoftwareSerial.h>

/* Pin Declarations */
#define DECODER_A_PIN A2 
#define DECODER_B_PIN A3 
#define LDR_PIN A1
#define LED 13

/* Colour Reading Declarations */
#define RGBWait 40
#define LDRWait 20 

/* Initialisation of color-sensing constants */
const char R = 0, G = 1, B = 2;
enum Colour {
    C_RED, C_GREEN, C_ORANGE, C_BLUE, C_PINK, C_WHITE
};
int currentColour[3] = {0};

/* Display Colour*/
MeRGBLed RGBled(0, 30);]
int status = 0;

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
  readColour(currentColour);
  Serial.print("Red: ");
  Serial.print(currentColour[0]);
  Serial.print(", Green: ");
  Serial.print(currentColour[1]);
  Serial.print(", Blue: ");
  Serial.println(currentColour[2]); 
}

int matchColour() {
    if (currentColour[2] > 875) { 
      return C_WHITE;
    } else if (currentColour[0] > 570) {  // Red Orange Pink Detection
      if (abs(currentColour[1] - currentColour[2]) < 30) { 
        return C_ORANGE;
      } else if (currentColour[1] < 700) {
        return C_RED;
      } else {
       return C_PINK;
      }
    } else if (abs(currentColour[2] - currentColour[1]) >= 120) {
      return C_BLUE;
    }
    return C_GREEN;
}

void display_colour(int pos) {
  int ledColors[6][3] = {
    {255, 0, 0},    // RED
    {0, 255, 0},    // GREEN
    {255, 100, 0},  // ORANGE
    {0, 0, 255},    // BLUE
    {200, 0, 200},  // PINK
    {255, 255, 255} // WHITE
  };

  RGBled.setColor(ledColors[pos][R], ledColors[pos][G], ledColors[pos][B]);
  RGBled.show();  
}

void setup(){
  pinMode(A7, INPUT); //button
  Serial.begin(9600);
  pinMode(DECODER_A_PIN,OUTPUT);   
  pinMode(DECODER_B_PIN,OUTPUT);
  RGBled.setpin(LED);  
}

void loop() {
  if (analogRead(A7) < 100) {
    status = 1 - status;
    delay(500);
  }
  if (status == 1){
    printColourValues();
    int colour = matchColour();
    display_colour(colour);
    delay(1000);
  }
}
