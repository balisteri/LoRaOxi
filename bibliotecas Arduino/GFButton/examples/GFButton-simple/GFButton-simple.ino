/**
   GeekFactory - "INNOVATING TOGETHER"
   Distribucion de materiales para el desarrollo e innovacion tecnologica
   www.geekfactory.mx

   Basic example for the GFButton library. This example shows the basic library usage with
   the polling (synchronous) API. This is the easiest and more intuitive way to use the library.
*/

#include "GFButton.h"

// Create two button instances on pins 2 & 3
GFButton buttonOn(2);
GFButton buttonOff(3);

/**
   PROGRAM INITIALIZATION
*/
void setup() {
  // Prepare serial interface
  Serial.begin(9600);

  // Show dialog to serial monitor
  Serial.println(F("----------------------------------------------------"));
  Serial.println(F("            GFBUTTON LIBRARY TEST PROGRAM           "));
  Serial.println(F("             https://www.geekfactory.mx             "));
  Serial.println(F("----------------------------------------------------"));
}

/**
   MAIN PROGRAM LOOP
*/
void loop() {
  // Check if "on" button is pressed
  if (buttonOn.isPressed()) {
    Serial.println(F("On button is pressed"));
    // Turn on the led
    digitalWrite(13, HIGH);
  }

  // Check if "off" button is pressed
  if (buttonOff.wasPressed()) {
    Serial.println(F("Off button was pressed"));
    // Turn off the led
    digitalWrite(13, LOW);
  }
}

