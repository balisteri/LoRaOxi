#include <GFButton.h>
#include<Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "MAX30100.h"

MAX30100 sensor;

#define REPORTING_PERIOD_MS   2000

String msg = String("");
int Pin = 7;
int i=0;
int ox=0;
int hr=0;
int lib=0;

PulseOximeter pox;
uint32_t tsLastReport = 0;
//________________________________________________________________
GFButton botao(10);
const int led = 5;


#define tempo_botao   2000
uint32_t report_botao = 0;

//________________________________________________________________

void onBeatDetected()
{
    Serial.println("Beat!"); 
}


void setup() 
{

 Serial.begin(38400);
 Serial1.begin(9600);

  pinMode(7, OUTPUT);
  digitalWrite(Pin, LOW);

 Serial.print("Testing MAX30100");

  if (!sensor.begin()) {
        Serial.print("FAILED: ");

        uint8_t partId = sensor.getPartId();
        if (partId == 0xff) {
            Serial.println("I2C error");
        } else {
            Serial.print("wrong part ID 0x");
            Serial.print(partId, HEX);
            Serial.print(" (expected: 0x");
            Serial.println(EXPECTED_PART_ID, HEX);
        }
        // Stop here
        for(;;);
    } else {
        Serial.println("Success");
    }

    Serial.print("Enabling HR/SPO2 mode..");
    sensor.setMode(MAX30100_MODE_SPO2_HR);
    Serial.println("done.");

    Serial.print("Configuring LEDs biases to 50mA..");
    sensor.setLedsCurrent(MAX30100_LED_CURR_50MA, MAX30100_LED_CURR_50MA);
    Serial.println("done.");

    delay(1000);

    Serial.print("Lowering the current to 7.6mA..");
    sensor.setLedsCurrent(MAX30100_LED_CURR_7_6MA, MAX30100_LED_CURR_7_6MA);
    Serial.println("done.");

    delay(1000);

    Serial.print("Shutting down..");
    sensor.shutdown();
    Serial.println("done.");

    delay(1000);

    Serial.print("Resuming normal operation..");
    sensor.resume();
    delay(500);
    Serial.println("done.");

    uint32_t tsTempSampStart = millis();
    Serial.print("Sampling die temperature..");
    sensor.startTemperatureSampling();
    while(!sensor.isTemperatureReady()) {
        if (millis() - tsTempSampStart > 1000) {
            Serial.println("ERROR: timeout");
            // Stop here
            for(;;);
        }
    }

    float temperature = sensor.retrieveTemperature();
    Serial.print("done, temp=");
    Serial.print(temperature);
    Serial.println("C");

    if (temperature < 5) {
        Serial.println("WARNING: Temperature probe reported an odd value");
    } else {
        Serial.println("All test pass.");
    }

    sensor.resetFifo();

    Serial.print("Shutting down..");
    sensor.shutdown();
    Serial.println("done.");

 

//________________________________________________________________


  botao.setPressHandler(botaoSolto);
  botao.setReleaseHandler(botaoPressionado);
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);

}

void loop() {
 botao.process();
 
 pox.update();

 if(millis() - tsLastReport > REPORTING_PERIOD_MS)
 {
  if(lib==1)
  {
  
  Serial.print("Heart rate:");
  Serial.println(pox.getHeartRate());
  hr=(int)(pox.getHeartRate());
  ox=(pox.getSpO2());
  if(ox<=100 && ox>0)
  {
    
  i = i+1;
  
  Serial.print("SpO2:");
  Serial.println(ox);
  msg = msg + String("O");   
  msg = msg + String(ox);
  msg = msg + String("B");
  msg = msg + String((int)(pox.getHeartRate()));
  Serial.print("Msg: ");
  Serial.println(msg);
  Serial1.print(msg);

 
  }
  if(i==10)
  {
   msg = String("");
   msg = msg + String("O");   
   msg = msg + String(ox);
   msg = msg + String("B");
   msg = msg + String(hr);
   i = 0;
   ox=0;
   hr=0;
   pox.shutdown();
   sensor.shutdown();
   digitalWrite(Pin, HIGH);
   Serial1.print(msg);
   Serial.print("Sending: ");
   digitalWrite(led,HIGH);
   Serial.println(msg);
   delay(3000);
   Serial1.print(msg);
   msg = String("");
   delay(3500);
   digitalWrite(Pin, LOW);
   digitalWrite(led,LOW); 

    lib=0;
    Serial.println("done.");
   }
    
 

  tsLastReport = millis();
  
  }
  
 }

}











void botaoPressionado(GFButton& botaoDoEvento)
{
  Serial.println("Botão foi Pressionado!");
  report_botao = millis();
}

void botaoSolto(GFButton& botaoDoEvento)
{
  Serial.println("Botão foi Solto!");
  if(millis() - report_botao > tempo_botao)
  {
    Serial.print("Botão foi pressionado por ");
    Serial.print((millis() - report_botao)/1000);
    Serial.println(" segundos!");




    
    lib=1;
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
}
