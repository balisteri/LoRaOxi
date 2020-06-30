#include <GFButton.h>
#include<Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "MAX30100.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

MAX30100 sensor;

#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();


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
GFButton botao(11);
const int led = 5;


#define tempo_botao   2000
uint32_t report_botao = 0;



//________________________________________________________________


// If using software SPI (the default case):
#define OLED_MOSI  20   //D1
#define OLED_CLK   21   //D0
#define OLED_DC    9
#define OLED_CS    8
#define OLED_RESET 10

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);



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
  pox.begin();
  Serial.print("Heart rate:");
  Serial.println(pox.getHeartRate());
  hr=(int)(pox.getHeartRate());
  ox=(pox.getSpO2());
  if(ox<=100 && ox>0)
  {
    
  i = i+1;
  
  mlx.begin();
  Serial.print("Ambiente = ");
  Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tPessoa = "); 
  Serial.print(mlx.readObjectTempC()); 
  Serial.println("*C");
  
  pox.begin();
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
   msg = msg + String("A");
   mlx.begin();
   msg = msg + String((int)(100*(mlx.readAmbientTempC())));
   msg = msg + String("P");
   msg = msg + String((int)(100*(mlx.readObjectTempC())));
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
   delay(3500);
   digitalWrite(Pin, LOW);
   digitalWrite(led,LOW); 
//________________________________________________________________________________________________
    
      TWCR = 0; // reset TwoWire Control Register to default, inactive state
//________________________________________________________________________________________________
    
  
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Sending: \n");
  display.print(msg);
  display.display();
  delay(5000);
  
 //________________________________________________________________________________________________
 
    msg = String("");
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
