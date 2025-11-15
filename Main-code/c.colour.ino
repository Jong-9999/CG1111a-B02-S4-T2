/*
  colour.ino: Read values using the colour sensor and match them based on colour matching algorithm
*/

/* Find the average reading for the requested number of times of scanning LDR */
int getAvgReading(int times){      
  int reading;
  int total = 0;
  for (int i = 0; i < times; i++){ // Take readings as many times as requested and sum them up
    reading = analogRead(LDR_PIN); 
    total += reading;
    delay(LDRWait);
  }
  return total / times; // Calculate the average
}

/* Get the avg red colour reading (Y3: Red) */
int findRed() {
  digitalWrite(DECODER_A_PIN, HIGH);
  digitalWrite(DECODER_B_PIN, HIGH);
  delay(RGBWait);
  return getAvgReading(5);
}

/* Get the avg green colour reading (Y2: Green) */
int findGreen() {
  digitalWrite(DECODER_A_PIN, LOW);
  digitalWrite(DECODER_B_PIN, HIGH);
  delay(RGBWait);
  return getAvgReading(5);
}

/* Get the avg blue colour reading (Y1: Blue) */
int findBlue() {
  digitalWrite(DECODER_A_PIN, HIGH);
  digitalWrite(DECODER_B_PIN, LOW);
  delay(RGBWait);
  return getAvgReading(5);
}

/* Store the RGB Values for the current tile mBot is on */
void readColour (){
  currentColour[0] = findRed();
  currentColour[1] = findGreen();
  currentColour[2] = findBlue();
}

/* Colour determining algorithm passes a colour back */
Colour matchColour() {
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
