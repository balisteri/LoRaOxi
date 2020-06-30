/**
   GeekFactory - "INNOVATING TOGETHER"
   Distribucion de materiales para el desarrollo e innovacion tecnologica
   www.geekfactory.mx

   Advanced example for GFButton library. This example ilustrates the callback programming
   pattern and also there are examples for most methods on the GFButton class.
*/

#include "GFButton.h"

// Create a button instance on pin 3
GFButton btn = GFButton(3);

/**
   PROGRAM INITIALIZATION
*/
void setup() {
  // Prepare serial interface
  Serial.begin(9600);

  // Attach callbacks to the button object
  btn.setPressHandler(button_on_press);
  btn.setHoldHandler(button_on_hold);
  btn.setReleaseHandler(button_on_release);
  btn.setClicksHandler(button_on_click);

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

  // Process button in main loop
  btn.process();

  // Perform other tasks here if necessary
}

/**
   CALLBACK FOR BUTTON PRESS EVENT
*/
void button_on_press(GFButton & btn)
{
  // Use of the press count field
  Serial.print(F("Button pressed "));
  Serial.print(btn.getPressCount());
  Serial.println(F(" times."));
}

/**
   CALLBACK FOR BUTTON HOLD EVENT
*/
void button_on_hold(GFButton & btn)
{
  // Use only the first gold event
  if (btn.isFirstHold())
  {
    // Use of the getHoldTime() method
    Serial.print(F("Button held for "));
    Serial.print(btn.getHoldTime() / 1000);
    Serial.println(F(" seconds."));
  } else {
    // Print dots as long as the button is pressed
    Serial.print('.');
  }
}

/**
   CALLBACK FOR BUTTON RELEASE EVENT
*/
void button_on_release(GFButton & btn)
{
  // Use of the getPin() method
  Serial.print(F("Button on pin "));
  Serial.print(btn.getPin());
  
  // example for wasLongPress() method
  if (btn.wasLongPress()) {
    Serial.println(F(" released after long press"));
  } else {
    Serial.println(F(" released after short press"));
  }
}

/**
   CALLBACK FOR CLICK EVENT
*/
void button_on_click(GFButton & button)
{
  // Use of the getClicks() method
  Serial.print(F("Button multiple clicks detected (within 1 sec): "));
  Serial.println(btn.getClicks());
}

