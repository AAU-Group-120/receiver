#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

// put function declarations here:

TFT_eSPI tft = TFT_eSPI();           // TFT object
#define BGCOLOR 0x0000
#define TXTCOLOR 0xFFFF
#define TXTSIZE 2
char Testbyte = 25 ;
int b0, b1, b2, b3, b4, b5, b6, b7, RESULT;
String Errormessage1 = "Højre Blinklys";
String Errormessage2 = "Venstre Blinklys";
String Errormessage3 = "Højre Kørelys";
String Errormessage4 = "Venstre Kørelys";
String Errormessage5 = "Stop Lys";
String Errormessage6 = "Tåge Lys";

void printError ();

void setup() {
  // put your setup code here, to run once:
 
  tft.init();

  Serial.begin(115200); // Debug only

  tft.begin();  // initialize
  tft.setRotation(3);
  tft.fillScreen(BGCOLOR);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(TXTSIZE);
  tft.setCursor(0,0,0);

  
  b0 = (Testbyte & 0x01) ? 1:0;
  b1 = (Testbyte & 0x02) ? 1:0;
  b2 = (Testbyte & 0x04) ? 1:0;
  b3 = (Testbyte & 0x08) ? 1:0;
  b4 = (Testbyte & 0x10) ? 1:0;
  b5 = (Testbyte & 0x20) ? 1:0;
  b6 = (Testbyte & 0x40) ? 1:0;
  b7 = (Testbyte & 0x80) ? 1:0;
  

RESULT = b0+b1+b2+b3+b4+b5+b6+b7;


printError();


}

void loop() {
  // put your main code here, to run repeatedly:
}


void printError() {
  String errorMsg = "";

  if (b0 == 1) errorMsg += Errormessage1 + ", ";
  if (b1 == 1) errorMsg += Errormessage2 + ", ";
  if (b2 == 1) errorMsg += Errormessage3 + ", ";
  if (b3 == 1) errorMsg += Errormessage4 + ", ";
  if (b4 == 1) errorMsg += Errormessage5 + ", ";
  if (b5 == 1) errorMsg += Errormessage6 + ", ";

  // Remove the last ", " if there are any messages
  if (errorMsg.endsWith(", ")) {
    errorMsg = errorMsg.substring(0, errorMsg.length() - 2);
  }

  // Replace the last comma with " og " if there is more than one error
  int lastComma = errorMsg.lastIndexOf(", ");
  if (lastComma != -1) {
    errorMsg = errorMsg.substring(0, lastComma) + " og" + errorMsg.substring(lastComma + 1);
  }

  if (errorMsg != "") {
    tft.fillScreen(BGCOLOR);
    tft.println(errorMsg);
  } else {
    tft.fillScreen(BGCOLOR);
  }
}


