// Originally Cloned from https://github.com/madlabdk/touche

// Including the relevant libraries
#include <DFRobotDFPlayerMini.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Set up the pins to transmit (TX) and recieve (RX) data from the MP3 Controller
SoftwareSerial mySoftwareSerial(11, 10); //RX, TX

// Initialise the MP3 Controller
DFRobotDFPlayerMini myDFPlayer;

// Define and Initialise the variables for reading from the Capacative Touch Sensor
#define steps 128 // Number of frequencies to test across
float values[steps]; // Array of the values along each of the frequencies
float alpha;
int maxPos, maxVal; // The maximum position and maximum values
bool checkVal = true;


void setup ()
{
  pinMode (9, OUTPUT); // Set Digital Pin 9 to be output, (this is the pin signals are sent across)
  TCCR1A = 0; // Set the clock values of Timer 1 on the Arduino to be 0
  TCCR1B = 0; // Set the clock values of Timer 1 on the Arduino to be 0
  TCCR1A |= (1 << COM1A0);        // Toggle OC1A on Compare Match.
  TCCR1B |= (1 << WGM12);         // CTC mode

  mp3setup();
}

void loop () {
  if (Serial.available()) {
    alpha = (float)Serial.read() / 255.0f;
  }
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }

  maxPos = 0;
  maxVal = 0;

  // Start the logic to get the capacitance value along the different frequencies
  if (checkVal) {
    for (int i = 0; i < steps; i++) {
      TCCR1B &= 0xFE;                       // Turns off timer
      TCNT1 = 0;                            // Resets timer counter register
      OCR1A = i;                            // Sets new frequency step
      TCCR1B |= 0x01;                       // Turns on timer
      float curVal = analogRead(0); // Reads the current value from the capacitance sensor
      values[i] = values[i] * alpha + curVal * (1 - alpha);  // exponential moving avg

      // Find the signal peak
      if (values[i] > maxVal) {
        maxVal = values[i];
        maxPos = i;
      }
    }
    Serial.print(maxPos, DEC); // Outputs the step on which the maximum frequency occurs
    Serial.print(" ");
    Serial.println(maxVal, DEC); // Outputs the peak of the signal
    delay(175); // Delay to curb / define the outputs
  }
  // Logic to check if the sides of container are being touched
  /*if(((maxVal < 1024) && (maxVal > 1000)) && ((maxPos > 20) && (maxPos < 40))) // One Finger Touch
    {
    checkVal = false; // Set value to be false to stop reading input
    playFile(1,1000); // Play mp3 file (1) for length of file (1000) in ms
    checkVal = true; // Re-enable checking of values
    }*/
  playFiles(maxVal, maxPos, 300, 200, 25, 120, 1, 500);
//  playFiles(maxVal, maxPos, 1000, 950, 40, 60, 2, 1000);
//  playFiles(maxVal, maxPos, 950, 0, 60, 100, 3, 1000);
  // Logic to check if the container has 3 finger
  /*else if(((maxVal < 1000) && (maxVal > 950)) && ((maxPos >40) && (maxPos < 60)))
    {
    checkVal = false; // Set value to be false to stop reading input
    playFile(2,1000); // Play mp3 file (2) for length of file (1000) in ms
    checkVal = true; // Re-enable checking of values
    }
    // Logic to check if fingers are dipped in water
    else if(((maxVal < 1000) && (maxPos > 90)) || (maxPos < 5))
    {
    checkVal = false; // Set value to be false to stop reading input
    playFile(3,1000); // Play mp3 file (3) for length of file (1000) in ms
    checkVal = true; // Re-enable checking of values
    }*/
}
