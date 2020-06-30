
#include<Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS   2000

PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected()
{
    Serial.println("Beat!"); 
}

void setup() {
 Serial.begin(9600);
 Serial.print("Inicializing pulse oximeter...");

    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("ERROR: Failed to initialize pulse oximeter");
        for(;;);
    }else {
        Serial.println("Success");
      }
      pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
      pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
 pox.update();
 if(millis() - tsLastReport > REPORTING_PERIOD_MS){
  Serial.print("Heart rate:");
  Serial.println(pox.getHeartRate());
  Serial.print("BPM/ SpO2:");
  Serial.println(pox.getSpO2());

  tsLastReport = millis();
  
 }
}
