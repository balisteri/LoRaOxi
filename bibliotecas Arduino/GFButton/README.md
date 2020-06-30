# GFButton Library #

Arduino library to easily manage buttons and keys as objects on the arduino sketch. The GFButton class provides methods for polling and event based programming styles.

The objective of this library is to move the button logic outside of the arduino sketch in order to keep the code organized.

## Basic library usage ##

The following example ilustrates the library usage. One button is used to turn on the led and other button is used to turn it off. When this example runs, pressing the ON button will print the message to the serial monitor many times, whereas the OFF button will print a message only once. See src/GFButton.h for full member documentation.

```cpp
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

```

## Project objectives ##

* Simplify the programming of push buttons, keys and momentary switches
* Move button related code outside the arduino sketch
* Provide advanced button fucntionality: double & triple clicks, button hold detection
* Provide polling and async (callback based) API
* Easy to configure and use

## Supported devices ##

This library was developed/tested on the following boards

* Arduino UNO R3
* Arduino Mega 2560 R3

