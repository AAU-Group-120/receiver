#include <Arduino.h>
#include <TFT_eSPI.h>          // Hardware-specific library for TFT
#include <SPI.h>
#include "BluetoothSerial.h"   // Bluetooth Serial library

TFT_eSPI tft = TFT_eSPI();            // TFT object
TFT_eSprite errorlist = TFT_eSprite(&tft); // Sprite object for Error messages

BluetoothSerial SerialBT;      // Create a Bluetooth Serial object

void BTConnect();              // Function to handle Bluetooth connection

bool connected = false;        // Flag to track Bluetooth connection state

// Set up variables and parameters
#define BGCOLOR 0x0000   // Background color
#define TXTCOLOR 0xFFFF  // Text color
#define TXTSIZE 2        // Text size (larger values = larger text)
int scrollPos = 0;       // Position for horizontal scrolling
int data = 0;

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
  
  tft.init();                        // Initialize the TFT screen
  tft.setRotation(3);                // Set screen orientation if necessary
  tft.fillScreen(BGCOLOR);         // Clear screen with black background
  
  BTConnect();                       // Attempt to connect to transmitter

  // Create a sprite for Error messages
  errorlist.setColorDepth(8);
  errorlist.createSprite(240, 135);  // Set sprite size to match display (240x135)
  errorlist.fillSprite(BGCOLOR);
  errorlist.setTextColor(TXTCOLOR); // Set text color
  errorlist.setTextSize(TXTSIZE);   // Set text size

}


void BTConnect()
{
  unsigned long startTime = millis(); // Record the start time
  const unsigned long timeout = 12000; // Set timeout period to 12 seconds (120000 ms)
  connected = false;
  unsigned long retryDelay = 2000;

  tft.fillScreen(BGCOLOR);// Clear screen
  tft.drawString("Connecting!", 10, 10, 2); // Display "Connecting!"

  delay(2000); // Optional delay to allow Bluetooth module initialization

  // Attempt to connect until successful or timeout
  while (millis() - startTime < timeout) 
  {
    if (SerialBT.connect("ESP32_Transmitter")) { // Attempt to connect to transmitter
      connected = true;
      Serial.println("Connected to ESP32_Transmitter!");
      tft.fillScreen(BGCOLOR);
      tft.drawString("Connected to ESP32_Transmitter!", 10, 10, 2); // Display success message
      delay(2000);
      break;
    } 
    else 
    {
      Serial.println("Connection failed. Retrying...");
      tft.fillScreen(BGCOLOR);       // Clear screen
      tft.drawString("Connection failed", 10, 10, 2);
      tft.drawString(" Retrying...", 10, 50, 2);
      delay(retryDelay); // Delay before retrying
    }
  }
  // Check if connection was successful or timed out
  if (!connected) 
  {
    Serial.println("Failed to connect. Please restart!");
    tft.fillScreen(BGCOLOR);
    tft.drawString("Failed to connect", 10, 10, 2); // Display failure message
    tft.drawString("Please restart!", 10, 50, 2); // Display failure message
    while (true) { // Stop further attempts if timed out
      delay(1000);
    }
  }
}

void checkConnection()
{
  // Check if the Bluetooth is still connected
  if (connected && !SerialBT.connected()) {
    // If we were connected but lost the connection
    connected = false; // Update connection state
    Serial.println("Lost connection");
    Serial.println("Trying to reconnect");
    tft.fillScreen(BGCOLOR);        // Clear screen
    tft.drawString("Lost connection", 10, 10, 2); // Display lost connection message
    tft.drawString("Trying to reconnect", 10, 50, 2); // Display lost connection message
  }

  // Reconnect if connection is lost
  if (!connected) {
    BTConnect(); // Try to reconnect
  }
}

void processData()
{
  // If data is available from the transmitter, process it
  if (connected && SerialBT.available()) 
  { 
    String receivedData = SerialBT.readString(); // Read data from Bluetooth
    data = receivedData.toInt();
    
    Serial.print("Received: ");
    Serial.println(data);           // Print received data to Serial Monitor
    //tft.fillScreen(TFT_BLACK);      // Clear previous display
    //tft.drawString("Received: " + String(data), 10, 10, 2); // Display received data on TFT
  }
}


// Function to generate a comma-separated error message based on active bits in the byte
String generateErrorMessage(int byteValue) {
  String errorMsg = "   ";

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

  // Replace the last comma with " og " if there is more than one error
  int lastComma = errorMsg.lastIndexOf(", ");
  if (lastComma != -1) {
    errorMsg = errorMsg.substring(0, lastComma) + " and" + errorMsg.substring(lastComma + 1);
  }

  return errorMsg;
}


void loop() {

  // Generate the error message based on the current byte value
  String errorMsg = generateErrorMessage(data);

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
    checkConnection();
    processData();
  }
  

  delay(25); // Adjust delay to control the smoothness of the scrolling
}



