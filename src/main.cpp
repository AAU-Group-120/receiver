#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

// put function declarations here:

TFT_eSPI tft = TFT_eSPI();           // TFT object
#define BGCOLOR 0x0000
#define TXTCOLOR 0xFFFF
#define TXTSIZE 2
#define B0 B0
#define B1 B1
char Testbyte = 16 ;
int B0, B1, B2, B3, B4, B5, B6, B7, RESULT;
String Errormessage1 = "Højre Blinklys";
String Errormessage2 = "Venstre Blinklys";
String Errormessage3 = "Højre Kørelys";
String Errormessage4 = "Venstre Kørelys";
String Errormessage5 = "Stop Lys";
String Errormessage6 = "Tåge Lys";

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

  
     B0 = (Testbyte & 0x01) ? 1:0;
     B1 = (Testbyte & 0x02) ? 1:0;
     B2 = (Testbyte & 0x04) ? 1:0;
     B3 = (Testbyte & 0x08) ? 1:0;
     B4 = (Testbyte & 0x10) ? 1:0;
     B5 = (Testbyte & 0x20) ? 1:0;
     B6 = (Testbyte & 0x40) ? 1:0;
     B7 = (Testbyte & 0x80) ? 1:0;
  

RESULT = B0+B1+B2+B3+B4+B5+B6+B7;

if (RESULT==1)
{
if (B0==1){
  tft.println(Errormessage1);
}
else if (B1==1){
  tft.println(Errormessage2);
}
else if (B2==1){
  tft.println(Errormessage3);
}
else if (B3==1){
  tft.println(Errormessage4);
}
else if (B4==1){
  tft.println(Errormessage5);
}
else if (B5==1){
  tft.println(Errormessage6);
}
else if (B6==1){
  tft.println("1");
}
else {
  tft.println("2");
}
}


}

void loop() {
  // put your main code here, to run repeatedly:



}

