// ------------------- EEPROM CREDENTIALS FOR READ WRITE ROUTINES
//    SEE GITHUB https://github.com/javos65/ESP32-StorePass
//    
#include "Debug.h"
#include "Credentials.h"
#include <EEPROM.h> // ESP32 emulates Flash


// ------------------- EEPROM CREDENTIALS READ WRITE ROUTINES
   
/* verify ID, verify EEprom, Safe data, increase counter*/
byte Save_Credentials(struct EECredentials *O )
{
  //Debug_Credentials(O);
  if (int eeAddress = Open_EEprom()) {
     int u,c = O->counter+1;          //Debug(" * received counter +1 ");Debugln(c);
     EECredentials t;                 // dummy structure to test-read
     uint8_t* ptr = (uint8_t*)&t;       // make pointer to structure t
     for(u=0 ; u<sizeof(EECredentials);++u){ (*(ptr+u)) = EEPROM.readByte(eeAddress+u);} // read structure raw into t
     if(t.counter > 0) {c = t.counter +1;} // if valid counter in Eeprom, use it and increase
     if (O->identity == EECredentials_ID) {
         //Debug(" * saved new counter ");Debugln(c);
         O->counter=c; t.counter = c; t.identity=EECredentials_ID;
         SimpleCypher(O->ssid,t.ssid); SimpleCypher(O->wifipass,t.wifipass);
         SimpleCypher(O->login1,t.login1); SimpleCypher(O->pass1,t.pass1);
         SimpleCypher(O->mqttadr,t.mqttadr); SimpleCypher(O->mqttlogin,t.mqttlogin);SimpleCypher(O->mqttpass,t.mqttpass);
       for(u=0 ; u<sizeof(EECredentials);++u){ EEPROM.writeByte(eeAddress+u, *(ptr+u) );} // write structure t into eeprom  
       Debug(" * Saved Object to EEprom with ID ");Debug(EECredentials_ID);Debug(" at address 0x");Debugln(eeAddress,HEX);
       Close_EEprom();
       return(1);
       }
     else  {
       Debugln(" * Object not saved : wrong ID");
       Close_EEprom();
       return(0);      
       }
    } // end check size
  else  {
   Debugln(" * Object not saved : no EEprom memory");
   return(0);      
  }  
}

/* Check if eeprom size is large enough to save stucture*/
int Open_EEprom()
{
  int crsize = sizeof(EECredentials);
  int eesize = EE_ROUND*(  (int) ((crsize+EE_SPARE)/EE_ROUND) ) ;
  Debug(" * Container size = ");Debug(crsize); Debug(", EEprom size = ");Debugln(eesize);
  if ( EEPROM.begin( (size_t) eesize) ) { 
      //Debug("* EEprom size = ");Debug(eesize);Debug("B, CRsize = ");Debug(crsize);Debug("B, start = 0x");Debugln(eesize-crsize-1,HEX);
      //Debug("* SPARE size = ");Debug(eesize-crsize-1);Debugln("B, start = 0x00");
      return(eesize-crsize-1); } // return start adress
  else { 
    Debugln(" * No Eeprom not present.");
    return(0);
    }
}

int Close_EEprom()
{
EEPROM.end();
return(1);
}

int Check_EEsize()
{
  int crsize = sizeof(EECredentials);
  int eesize = EE_ROUND*(  (int) ((crsize+EE_SPARE)/EE_ROUND) ) ;
  //Debug("* EEprom size = ");Debug(eesize);Debug(", CRsize = ");Debug(crsize);Debug(", start = ");Debugln(eesize-crsize-1,HEX);
  //Debug(", SPARE size = ");Debug(eesize-crsize-1);Debugln(", start = 0x0000 ");
  return(eesize);  // return start adress
}

int Check_CRstart()
{
  int crsize = sizeof(EECredentials);
  int eesize = EE_ROUND*(  (int) ((crsize+EE_SPARE)/EE_ROUND) ) ;
  //Debug("* EEprom size = ");Debug(eesize);Debug(", CRsize = ");Debug(crsize);Debug(", start = ");Debugln(eesize-crsize-1,HEX);
  //Debug(", SPARE size = ");Debug(eesize-crsize-1);Debugln(", start = 0x0000 ");
  return(eesize-crsize-1);  // return start adress
}

/* Print debug information of Credentaisl structure,*/
byte Debug_Credentials(struct EECredentials *O)
{
     Debug(" * Credentials:[");Debug(O->ssid);Debug(",");Debug(O->wifipass);
     Debug("],[");Debug(O->login1);Debug(",");Debug(O->pass1);
     Debug("],[");Debug(O->mqttadr);Debug(",");Debug(O->mqttlogin);Debug(",");Debug(O->mqttpass);
     Debug("], id=");Debug(O->identity);Debug(", count=");Debug(O->counter);;Debug(", stop=");Debugln(O->stop);
     return(1);
}

/* Erase Object in EEprom*/
byte Erase_Credentials(struct EECredentials *O )
{
  if (int eeAddress = Open_EEprom()) {  
    if (O->identity == EECredentials_ID) {
      struct EECredentials x = {"x","x","x","x","x","x","x",0x0,0,0};
      uint8_t* ptr = (uint8_t*)&x;       // make pointer to structure t
      for(int u=0 ; u<sizeof(EECredentials);++u){ EEPROM.writeByte(eeAddress+u, *(ptr+u) );} // write structure t into eeprom  
      Debugln(" * Object erased from EEprom");
      Close_EEprom();
      return(1);
    }
    else  {
      Debugln(" * Erase Failed: wrong ID");
      Close_EEprom();
      return(0);      
      } 
  } 
  else { 
    //Debugln(" *Erase Failed : No Eeprom");
    return(0);
    }     
}

/* Read Credentials and verify ID */
byte Read_Credentials(struct EECredentials *O)
{
  if (int eeAddress = Open_EEprom()) {    
     EECredentials t; // dummy structure to test-read
     uint8_t* ptr = (uint8_t*)&t;       // make pointer to structure t
     for(int u=0 ; u<sizeof(EECredentials);++u){ (*(ptr+u)) = EEPROM.readByte(eeAddress+u);} // read structure raw into t
     //Debug_Credentials(&t); // raw read
     if (t.identity == EECredentials_ID){ // check ID
        O->counter = t.counter; O->identity=t.identity;
        SimpleDecypher(t.ssid,O->ssid); SimpleDecypher(t.wifipass,O->wifipass);
        SimpleDecypher(t.login1,O->login1); SimpleDecypher(t.pass1,O->pass1);
        SimpleDecypher(t.mqttadr,O->mqttadr); SimpleDecypher(t.mqttlogin,O->mqttlogin);SimpleDecypher(t.mqttpass,O->mqttpass);
        Debug(" * Credentials read from EEprom,with ID ");Debug(t.identity);Debug(" at address 0x");Debugln(eeAddress,HEX);
        Close_EEprom();
        return(1);
        }
     else {
        Debugln(" * EEprom read Failed : Wrong ID");
        Close_EEprom();
        return(0);
        }
  }
  else { 
    Debugln(" * EEprom Read Failed : No Eeprom");
    Close_EEprom();
    return(0);
    }         
}



/* Simple Cyphering the text code */
byte SimpleCypher(char * textin, char * textout)
{
int t=0;
bool st=true;
while(textin[t]!=0) {
   textout[t]=textin[t]+EE_SEED%(111)-t%17;
   t++; if( t>=EE_MAXCHAR){Debug(" * Cypher error: data size out of range") ;st=false;break;}
  }
  textout[t]=0;
  if (st==true){
  //Debug(" * Decyphered ");Debug(t);Debug(" - ");Debugln(textout);
  return(1);
  }
  else return(0);
}

/* Simple DeCyphering the text code */
byte SimpleDecypher(char * textin, char * textout)
{
int t=0;
bool st=true;
while(textin[t]!=0) {
   textout[t]=textin[t]-EE_SEED%(111)+t%17;
   t++; if( t>=EE_MAXCHAR){Debug(" * Decypher error : data too long") ;st=false;break;}
  }
  textout[t]=0;
  if (st==true){
  //Debug(" * Decyphered ");Debug(t);Debug(" - ");Debugln(textout);
  return(1);
  }
  else return(0);
}
