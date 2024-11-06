#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

// put function declarations here:

TFT_eSPI tft = TFT_eSPI();           // TFT object
#define BGCOLOR 0x0000
#define TXTCOLOR 0xFFFF
#define TXTSIZE 2

void setup() {
  // put your setup code here, to run once:
 
  tft.init();

  Serial.begin(115200); // Debug only

  tft.begin();  // initialize
  tft.setRotation(3);
  tft.fillScreen(BGCOLOR);
  tft.setTextColor(TXTCOLOR);
  tft.setTextSize(TXTSIZE);
  tft.setCursor(0,0,2);
  tft.print("Inductor");


}

void loop() {
  // put your main code here, to run repeatedly:



}

