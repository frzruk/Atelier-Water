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
  
  // Logic to check if the sides of container are being touched
  while((maxVal < 500) && (maxVal > 200))
  {
  myDFPlayer.loop(4);  // Play the 4th file on the TF card in mp3 controller
  }
  // Logic to check if the container has a finger in the water
  while(maxVal < 200)
  {
  myDFPlayer.loop(5); // Play the 5th file on the TF card in the mp3 controller
  }
  
}
