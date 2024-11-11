#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();           // TFT object

TFT_eSprite errorlist = TFT_eSprite(&tft); // Sprite object Error messages

int tcount = 0;

// put function declarations here:

#define BGCOLOR 0x0000
#define TXTCOLOR 0xFFFF
#define TXTSIZE 2
char Testbyte = 0 ;
int b0, b1, b2, b3, b4, b5, b6, b7, RESULT;
String Errormessage1 = "Højre Blinklys";
String Errormessage2 = "Venstre Blinklys";
String Errormessage3 = "Højre Kørelys";
String Errormessage4 = "Venstre Kørelys";
String Errormessage5 = "Stop Lys";
String Errormessage6 = "Tåge Lys";
String Errormessage7 = "Inductor";
String Errormessage8 = "YOLO";

// Declare errorMsg globally
String errorMsg = "";  

void setup() {
  // put your setup code here, to run once:
 
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(BGCOLOR);
  tft.setTextSize(TXTSIZE);

  Serial.begin(115200); // Debug only

  // Create a sprite for Error messages
  errorlist.setColorDepth(8);
  errorlist.createSprite(80, 16);
  errorlist.fillSprite(TFT_DARKGREY);
  errorlist.setScrollRect(0, 0, 40, 16, TFT_DARKGREY); // Scroll the "Hello" in the first 40 pixels
  errorlist.setTextColor(TFT_WHITE); // White text, no background

}

void printError();

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 0; i < 256; i++)
  {
    Testbyte = i;

    b0 = (Testbyte & 0x01) ? 1:0;
    b1 = (Testbyte & 0x02) ? 1:0;
    b2 = (Testbyte & 0x04) ? 1:0;
    b3 = (Testbyte & 0x08) ? 1:0;
    b4 = (Testbyte & 0x10) ? 1:0;
    b5 = (Testbyte & 0x20) ? 1:0;
    b6 = (Testbyte & 0x40) ? 1:0;
    b7 = (Testbyte & 0x80) ? 1:0;

    // Push the sprite onto the TFT at specified coordinates
    errorlist.pushSprite(40, 70);

    // Now scroll the sprites scroll(dt, dy) where:
    // dx is pixels to scroll, left = negative value, right = positive value
    // dy is pixels to scroll, up = negative value, down = positive value
    errorlist.scroll(1);     // scroll stext 1 pixel right, up/down default is 0

    tcount--;
    if (tcount <=0)
    { // If we have scrolled 40 pixels the redraw text
    tcount = 40;
    errorlist.drawString(errorMsg, 6, 0, 2); // draw at 6,0 in sprite, font 2
    }

    printError();

    delay(1000);

  }


}


void printError() {
  String errorMsg = "";

  if (b0 == 1) errorMsg += Errormessage1 + ", ";
  if (b1 == 1) errorMsg += Errormessage2 + ", ";
  if (b2 == 1) errorMsg += Errormessage3 + ", ";
  if (b3 == 1) errorMsg += Errormessage4 + ", ";
  if (b4 == 1) errorMsg += Errormessage5 + ", ";
  if (b5 == 1) errorMsg += Errormessage6 + ", ";
  if (b6 == 1) errorMsg += Errormessage7 + ", ";
  if (b7 == 1) errorMsg += Errormessage8 + ", ";

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


