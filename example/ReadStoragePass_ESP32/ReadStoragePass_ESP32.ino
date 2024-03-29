/***
    eeprom_write and read example using C-structure to store Login and passwords for IoT applications with Wifi and Mqtt etc.
    text data is cyphered by simple algorithm, stored eeprom data is not text-readable.

    SEE GITHUB https://github.com/javos65/ESP32-StorePass

    Debug.h - simple serial.print debug call
    Credentials.h - defines structure , seed and id

    Use StoragePass to store your secret credentials
    Use ReadStoragePass_ESP32.ino for read only functions to add to your IOT code

    Written by Jay Fox - 2024
    Released under MIT licence.
***/

#include "Debug.h"
#include "Credentials.h"

EECredentials G_test = {"MYSSID","password","admin","password2","183.188.50.6","admin1","testcode",0x55,1}; // Global Variable Test structure

void setup() {
/*************SERIAL SETUP ***************/
  Serial.begin(115200);
  while (!Serial) ; // wait for serial port to connect. Needed for native USB port only
  delay(1000);
  Serial.println(" * Starting Program");
  
 Read_Credentials(&G_test);
 Serial.print("\n * Your Wifi Credentials [SSID],[Pass]=[");Serial.print(G_test.ssid);Serial.print("],[");Serial.print(G_test.wifipass);Serial.println("]");
}


void loop() {
  /* Empty loop */
}
