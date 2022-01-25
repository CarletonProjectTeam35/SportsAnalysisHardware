/*
  This example connects to a WPA2 Enterprise WiFi network.
  Then it prints the MAC address of the WiFi module,
  the IP address obtained, and other network details.

  Based on ConnectWithWPA.ino by dlf (Metodo2 srl) and Tom Igoe
*/
#include <SPI.h>
#include <WiFiNINA.h>
#include "ThingSpeak.h"
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;  // your WPA2 enterprise network SSID (name)
char pass[] = SECRET_PASS;  // your WPA2 enterprise password
int status = WL_IDLE_STATUS;     // the WiFi radio's status
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

String myStatus = "";


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA2 enterprise network:
    // - You can optionally provide additional identity and CA cert (string) parameters if your network requires them:
    //      WiFi.beginEnterprise(ssid, user, pass, identity, caCert)
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
  ThingSpeak.begin(client);  //Initialize ThingSpeak
}

void loop() {

  int number1 = random(500);
  int number2 = random(500);
  int number3 = random(500);
  int number4 = random(500);
  int number5 = random(500);
  int number6 = random(500);
  ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, number2);
  ThingSpeak.setField(3, number3);
  ThingSpeak.setField(4, number4);
  ThingSpeak.setField(5, number5);
  ThingSpeak.setField(6, number6);

  // figure out the status message
  if(number1 > number2){
    myStatus = String("field1 is greater than field2"); 
  }
  else if(number1 < number2){
    myStatus = String("field1 is less than field2");
  }
  else{
    myStatus = String("field1 equals field2");
  }
  
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(2000); // Wait 20 seconds to update the channel again
}
