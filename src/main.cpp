#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "BluetoothSerial.h"  // Include the Bluetooth Serial library

BluetoothSerial SerialBT;     // Create a Bluetooth Serial object

void setup() {
  Serial.begin(115200);        // Initialize serial monitor
  SerialBT.begin("ESP32_Reciever", true); // Set Bluetooth name and enable Master mode
  Serial.println("Bluetooth Receiver Started. Attempting to connect...");

  unsigned long startTime = millis(); // Record the start time
  const unsigned long timeout = 120000; // Set timeout period to 2 minutes (120000 ms)
  bool connected = false;

  // Keep trying to connect until successful or timeout
  while (millis() - startTime < timeout) {
    if (SerialBT.connect("ESP32_Transmitter")) { // Attempt to connect to ESP32_Transmitter
      connected = true;
      Serial.println("Connected to ESP32_Transmitter!");
      break;
    } else {
      Serial.println("Connection failed. Retrying...");
      delay(2000); // Wait 5 seconds before trying again
    }
  }

  // Check if we connected successfully or timed out
  if (!connected) {
    Serial.println("Failed to connect. Please restart!"); //denne fejl meddelses skal displayes på display
    while (true) { // Stop further attempts if timed out
      delay(1000);
    }
  }
}

void loop() {
  if (SerialBT.available()) { // Check if data is available from the server
    char receivedData = SerialBT.read(); // Read data from Bluetooth
    Serial.print("Received: ");
    Serial.println(receivedData); // Print received data to Serial Monitor
  }
}