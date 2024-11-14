#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "BluetoothSerial.h"  // Include the Bluetooth Serial library

TFT_eSPI tft = TFT_eSPI();            // TFT object
TFT_eSprite errorlist = TFT_eSprite(&tft); // Sprite object for Error messages

BluetoothSerial SerialBT;     // Create a Bluetooth Serial object

// Set up variables and parameters
#define BGCOLOR 0x0000   // Background color
#define TXTCOLOR 0xFFFF  // Text color
#define TXTSIZE 2        // Text size (larger values = larger text)
int scrollPos = 0;       // Position for horizontal scrolling
int Testbyte = 0;        // Changing byte value

// Error messages associated with each bit in the byte
String Errormessage1 = "Right indicator";
String Errormessage2 = "Left indicator";
String Errormessage3 = "Right runninglight";
String Errormessage4 = "Left runninglight";
String Errormessage5 = "Brake lights";
String Errormessage6 = "Fog Light";
String Errormessage7 = "Error1";
String Errormessage8 = "Error2";

// Function to generate error message based on byte
String generateErrorMessage(int byteValue);

void setup() {
  Serial.begin(115200);        // Initialize serial monitor
  SerialBT.begin("ESP32_Reciever", true); // Set Bluetooth name and enable Master mode
  Serial.println("Bluetooth Receiver Started. Attempting to connect...");

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(BGCOLOR);

  // Create a sprite for Error messages
  errorlist.setColorDepth(8);
  errorlist.createSprite(240, 135);  // Set sprite size to match display (240x135)
  errorlist.fillSprite(BGCOLOR);
  errorlist.setTextColor(TXTCOLOR); // Set text color
  errorlist.setTextSize(TXTSIZE);   // Set text size

  unsigned long startTime = millis(); // Record the start time
  const unsigned long timeout = 120000; // Set timeout period to 2 minutes (120000 ms)
  bool connected = false;

  // Keep trying to connect until successful or timeout
  while (millis() - startTime < timeout) 
  {
    if (SerialBT.connect("ESP32_Transmitter")) 
    { // Attempt to connect to ESP32_Transmitter
      connected = true;
      Serial.println("Connected to ESP32_Transmitter!");
      break;
    } 
    else 
    {
      Serial.println("Connection failed. Retrying...");
      delay(2000); // Wait 5 seconds before trying again
    }
  }
  // Check if we connected successfully or timed out
  if (!connected) 
  {
    Serial.println("Failed to connect. Please restart!"); //denne fejl meddelses skal displayes på display
    while (true) 
    { // Stop further attempts if timed out
      delay(1000);
    }
  }
}

void loop() {
  if (SerialBT.available()) { // Check if data is available from the server
    String receivedData = SerialBT.readString(); // Read data from Bluetooth
    Serial.print("Received: ");
    Serial.println(receivedData); // Print received data to Serial Monitor
  }
  // Generate the error message based on the current byte value
  String errorMsg = generateErrorMessage(Testbyte);

  // Clear the sprite area
  errorlist.fillSprite(BGCOLOR);

  // Draw the message starting from the current scroll position
  errorlist.drawString(errorMsg, -scrollPos, 0, 2); // Draw text, starting position off-screen to the left

  // Push the updated sprite to the screen
  errorlist.pushSprite(0, 0);

  // Increment scroll position for next frame
  scrollPos += 2;  // Adjust scroll speed (higher values = faster scrolling)

  // Reset scroll position if the text has fully scrolled off the left side
  int textWidth = errorlist.textWidth(errorMsg, 2); // Calculate the text width
  if (scrollPos > textWidth) {
    scrollPos = 0;          // Restart scrolling from the beginning
    Testbyte = (Testbyte + 1) % 256;  // Increment the byte and wrap back to 0 after 255
  }

  delay(25); // Adjust delay to control the smoothness of the scrolling
}

// Function to generate a comma-separated error message based on active bits in the byte
String generateErrorMessage(int byteValue) {
  String errorMsg = "";

  if (byteValue & 0x01) errorMsg += Errormessage1 + ", ";
  if (byteValue & 0x02) errorMsg += Errormessage2 + ", ";
  if (byteValue & 0x04) errorMsg += Errormessage3 + ", ";
  if (byteValue & 0x08) errorMsg += Errormessage4 + ", ";
  if (byteValue & 0x10) errorMsg += Errormessage5 + ", ";
  if (byteValue & 0x20) errorMsg += Errormessage6 + ", ";
  if (byteValue & 0x40) errorMsg += Errormessage7 + ", ";
  if (byteValue & 0x80) errorMsg += Errormessage8 + ", ";

  // Remove the trailing ", " if there are any messages
  if (errorMsg.endsWith(", ")) {
    errorMsg = errorMsg.substring(0, errorMsg.length() - 2);
  }

  return errorMsg;
}

