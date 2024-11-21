#include <Arduino.h>
#include <TFT_eSPI.h>          
#include <SPI.h>
#include "BluetoothSerial.h"   

TFT_eSPI tft = TFT_eSPI();            
TFT_eSprite errorlist = TFT_eSprite(&tft); // Sprite for "Error messages"

BluetoothSerial SerialBT;      // initialise bluetooth communication

void BTConnect();              

bool connected = false;        // bool to check Bluetooth connection


#define BGCOLOR 0x0000   
#define TXTCOLOR 0xFFFF  
#define TXTSIZE 2        
int scrollPos = 0;       
uint8_t data = 0;

// Error messages associated with each bit in the byte
String Errormessage1 = "Right indicator";
String Errormessage2 = "Left indicator";
String Errormessage3 = "Right runninglight";
String Errormessage4 = "Left runninglight";
String Errormessage5 = "Brake lights";
String Errormessage6 = "Fog Light";
String Errormessage7 = "Error1";
String Errormessage8 = "Error2";

// Function to generate error message based on the byte
String generateErrorMessage(int alertBits);

void setup() {
  Serial.begin(115200);        
  SerialBT.begin("ESP32_Reciever", true); // set name of bluetooth device, and enable mastermode (to make sure it can both receive and send data)

  Serial.println("Bluetooth Receiver Started. Attempting to connect...");
  
  tft.init();                        
  tft.setRotation(3);                
  tft.fillScreen(BGCOLOR);
  
  BTConnect();     // Attempt to connect to transmitter

  // Create a sprite for Error messages
  errorlist.setColorDepth(8);
  errorlist.createSprite(240, 135);  
  errorlist.fillSprite(BGCOLOR);
  errorlist.setTextColor(TXTCOLOR); 
  errorlist.setTextSize(TXTSIZE);   

}


void BTConnect()
{
  unsigned long startTime = millis(); // Record the start time
  const unsigned long timeout = 12000; // Set max time period to connect, to 12 seconds 
  connected = false;
  unsigned long retryDelay = 2000;

  tft.fillScreen(BGCOLOR);
  tft.drawString("Connecting!", 10, 10, 2);

  delay(2000); // Delay to allow bluetooth startup

  // Attempt to connect until successful or timeout
  while (millis() - startTime < timeout) 
  {
    if (SerialBT.connect("ESP32_Transmitter")) // Attempt to connect to transmitter with the right name
    { 
      connected = true;
      Serial.println("Connected to ESP32_Transmitter!");
      tft.fillScreen(BGCOLOR);
      tft.drawString("Connected to ESP32_Transmitter!", 10, 10, 2);
      delay(2000);
      break;
    } 
    else 
    {
      Serial.println("Connection failed. Retrying...");
      tft.fillScreen(BGCOLOR);       
      tft.drawString("Connection failed", 10, 10, 2);
      tft.drawString(" Retrying...", 10, 50, 2);
      delay(retryDelay); // Delay before retrying
    }
  }
  
  if (!connected) // Check if connection was successful or timed out
  {
    Serial.println("Failed to connect. Please restart!");
    tft.fillScreen(BGCOLOR);
    tft.drawString("Failed to connect", 10, 10, 2); 
    tft.drawString("Please restart!", 10, 50, 2); 
    while (true)  // Stop further attempts if timed out 
    {
      delay(1000);
    }
  }
}

void checkConnection()// Check if the Bluetooth is still connected
{
  
  if (connected && !SerialBT.connected()) // If we were connected but lost the connection:
  {
    connected = false; // Update connection state
    Serial.println("Lost connection");
    Serial.println("Trying to reconnect");
    tft.fillScreen(BGCOLOR);        
    tft.drawString("Lost connection", 10, 10, 2); 
    tft.drawString("Trying to reconnect", 10, 50, 2);
    delay(1000);
  }

  
  if (!connected) // Reconnect if connection is lost 
  {
    BTConnect(); 
  }
}

void processData() //receive and process data
{
  if (connected && SerialBT.available()) // If data is available from the transmitter, process it
  { 
    String receivedData = SerialBT.readString(); // Read data from Bluetooth
    int lastTerminating = receivedData.lastIndexOf(0b10000000);
    if (lastTerminating != -1) 
    {
      data = receivedData[receivedData.length()-2];
    }
    unsigned char message = 0b110;
    SerialBT.write(message);
  }
}



String generateErrorMessage(int alertBits) // Function to generate a comma-separated error message based on active bits in the byte
{
  String errorMsg = "   ";

  if (alertBits & 0x01) errorMsg += Errormessage1 + ", ";
  if (alertBits & 0x02) errorMsg += Errormessage2 + ", ";
  if (alertBits & 0x04) errorMsg += Errormessage3 + ", ";
  if (alertBits & 0x08) errorMsg += Errormessage4 + ", ";
  if (alertBits & 0x10) errorMsg += Errormessage5 + ", ";
  if (alertBits & 0x20) errorMsg += Errormessage6 + ", ";
  if (alertBits & 0x40) errorMsg += Errormessage7 + ", ";
  if (alertBits & 0x80) errorMsg += Errormessage8 + ", ";

  
  if (errorMsg.endsWith(", ")) // Remove the last comma
  {
    errorMsg = errorMsg.substring(0, errorMsg.length() - 2);
  }
 
  int lastComma = errorMsg.lastIndexOf(", ");  // Replace the last comma with " and " if there is more than one error
  if (lastComma != -1) 
  {
    errorMsg = errorMsg.substring(0, lastComma) + " and" + errorMsg.substring(lastComma + 1);
  }

  return errorMsg;
}


void loop() {
  String errorMsg = generateErrorMessage(data);

  errorlist.fillSprite(BGCOLOR);
  errorlist.drawString(errorMsg, -scrollPos, 0, 2);
  errorlist.pushSprite(0, 0);
  scrollPos += 2;  

  // Reset scroll position when the text is out of vision
  int textWidth = errorlist.textWidth(errorMsg, 2); // Calculate the text width
  if (scrollPos > textWidth) 
  {
    scrollPos = 0;          
    checkConnection();
    processData();
  }
  

  delay(25);
}