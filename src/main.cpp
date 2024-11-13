#include <Arduino.h>
#include <TFT_eSPI.h>          // Hardware-specific library for TFT
#include <SPI.h>
#include "BluetoothSerial.h"   // Bluetooth Serial library

BluetoothSerial SerialBT;      // Create a Bluetooth Serial object
TFT_eSPI tft = TFT_eSPI();     // Initialize TFT display object

void BTConnect();              // Function to handle Bluetooth connection

bool connected = false;        // Flag to track Bluetooth connection state

void setup() 
{
  Serial.begin(115200);              // Initialize Serial Monitor
  SerialBT.begin("ESP32_Receiver", true); // Set Bluetooth name and enable Master mode
  Serial.println("Bluetooth Receiver Started. Attempting to connect...");
  
  tft.init();                        // Initialize the TFT screen
  tft.setRotation(3);                // Set screen orientation if necessary
  tft.fillScreen(TFT_BLACK);         // Clear screen with black background
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set text color
  tft.setTextSize(2);
  
  BTConnect();                       // Attempt to connect to transmitter
}

void BTConnect()
{
  unsigned long startTime = millis(); // Record the start time
  const unsigned long timeout = 12000; // Set timeout period to 12 seconds (120000 ms)
  connected = false;
  unsigned long retryDelay = 500;

  delay(2000); // Optional delay to allow Bluetooth module initialization

  // Attempt to connect until successful or timeout
  while (millis() - startTime < timeout) 
  {
    if (SerialBT.connect("ESP32_Transmitter")) { // Attempt to connect to transmitter
      connected = true;
      Serial.println("Connected to ESP32_Transmitter!");
      tft.fillScreen(TFT_BLACK);       // Clear screen
      tft.drawString("Connected to ESP32_Transmitter!", 10, 10, 2); // Display success message
      break;
    } else {
      Serial.println("Connection failed. Retrying...");
      delay(retryDelay); // Delay before retrying
    }
  }

  // Check if connection was successful or timed out
  if (!connected) 
  {
    Serial.println("Failed to connect. Please restart!");
    tft.fillScreen(TFT_BLACK);
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
    tft.fillScreen(TFT_BLACK);        // Clear screen
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
    int data = receivedData.toInt();
    
    Serial.print("Received: ");
    Serial.println(data);           // Print received data to Serial Monitor
    tft.fillScreen(TFT_BLACK);      // Clear previous display
    tft.drawString("Received: " + String(data), 10, 10, 2); // Display received data on TFT
  }
}



void loop() {

  checkConnection();
  processData();

}
