#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

// put function declarations here:

TFT_eSPI tft = TFT_eSPI();           // TFT object
#define BGCOLOR 0x0000
#define TXTCOLOR 0xFFFF
#define TXTSIZE 2
char Testbyte = 16 ;
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
  tft.setRotation(2);
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

if (RESULT==1)
{
printError();
}


}

void loop() {
  // put your main code here, to run repeatedly:



}

void printError()
{
if (b0==1){
  tft.println(Errormessage1);
}
else if (b1==1){
  tft.println(Errormessage2);
}
else if (b2==1){
  tft.println(Errormessage3);
}
else if (b3==1){
  tft.println(Errormessage4);
}
else if (b4==1){
  tft.println(Errormessage5);
}
else if (b5==1){
  tft.println(Errormessage6);
}
else if (b6==1){
  tft.println("1");
}
else {
  tft.println("2");
}
}

