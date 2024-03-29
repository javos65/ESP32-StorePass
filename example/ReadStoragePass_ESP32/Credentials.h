// ------------------- EEPROM CREDENTIALS FOR READ WRITE ROUTINES
//
//   SEE GITHUB https://github.com/javos65/ESP32-StorePass
// 
//  *EEPROM**********************************************
//  *                   eesize                          *
//  *****************************************************
//  * 0x00                   *       crsize             * 
//  * EE_SPARE               *  EE_CREDENTIALS          *
//  *****************************************************
//
 
#ifndef __CRED_H
#define __CRED_H

#include <Arduino.h>
#define byte uint8_t
#define EECredentials_ID 0x77  // EEprom save ID for object to retrieve
#define EE_SEED 77             // Seed for Cyphering
#define EE_MAXCHAR 32          // structure for EEprom storage:  7 x 32byte + 4 bytes = 228 bytes length + overhead = 236 bytes in total
#define EE_SPARE 128           // Spare memory in EEprom as from 0x000
#define EE_ROUND 16            // Round up EEprom size

struct EECredentials {         
char ssid[EE_MAXCHAR];
char wifipass[EE_MAXCHAR];
char login1[EE_MAXCHAR];
char pass1[EE_MAXCHAR];
char mqttadr[EE_MAXCHAR];
char mqttlogin[EE_MAXCHAR];
char mqttpass[EE_MAXCHAR];
byte identity;
int counter;
byte stop; // dummy
};

// Declare external functions in Credentials.cpp that cal be called from the sketch //

extern byte Debug_Credentials(struct EECredentials *O);

extern byte Read_Credentials(struct EECredentials *O);
extern int Open_EEprom();
extern int Close_EEprom();
extern int Check_EEsize();
extern int Check_CRstart();

extern byte SimpleDecypher(char * textin, char * textout);

#endif
