/*
 https://github.com/brianrho/SIM800-for-TCP
 */

#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <sim800.h>
#define FONA_RST 4  // no need to connect this; 

char result1[6] = "000000";
char result2[6] = "000000";
char result3[6] = "000000";
char res[4] = "";


float temp,total;
int tempPin = 0;

String data;

String host = "www.ksrctiot.com"; 
char api[10]="c3mv6b6nug";

int buf1, buf2;

float flat, flon;

TinyGPS gps;

Modem modem = Modem(FONA_RST);
void gpt()
{
  
   Serial.begin(9600);
  
  Serial.println(TinyGPS::library_version());
  Serial.println("Bus Tracking SYSTEM");
  Serial.println();

  
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial2.available())
    {
      char c = Serial2.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
   
 buf1= (flat, 6);
  
buf2= (flon, 6);
  Serial.print("latitude="); 
  Serial.print(buf1);
  Serial.print("longitude=");
  Serial.println(buf2);
  }
 Serial.println("ends"); 
}

void setup() 
{
     while (! Serial);
    Serial.begin(9600);
    delay(1000);
    Serial.println(F("SIM800 TCP/HTTP demo"));
    Serial.println(F("Initializing modem... (May take a few seconds)"));
   Serial1.begin(9600);
    if (! modem.begin(Serial1))
  {
    Serial1.println(F("Couldn't find modem"));
      while(1);
    
  }

}
void loop() {
  
  
  Serial.println("Started GPRS and GSM");
  Serial.print(F("Checking for Cell network..."));
  while (modem.getNetworkStatus() != 1);
  Serial.println(F("Registered."));
  modem.setGPRSNetworkSettings(F("airtelgprs.com"));
  modem.enableGPRS(true);
  
 gpt();
 
  if (modem.TCPconnect("www.ksrctiot.com", 80))
   
  {
   char req[] = "GET /update.php?api=c3mv6b6nug&field1=000000000&field2=000000&field3=00000 HTTP/1.1\r\nHost:www.ksrctiot.com\r\n\r\n";
  Serial.println(strlen(req));
 int i;
 for(i=20;i<=29;i++)
 { 
      req[i]=api[i-20];
 
 }




  
dtostrf(buf1, 6, 5, result1);

for(i=38;i<=44;i++)
 {
       req[i]=result1[i-38];
 
 }
  Serial.println("GPS Latitude"); 
  Serial.println(result1);
 
  dtostrf(buf2, 6, 5, result2);

    for(i=55;i<=60;i++)
 {
 
      req[i]=result2[i-55];
 
 }
Serial.println("GPS Longitude"); 
  Serial.println(result2);
   delay(10);



 
 Serial.println(req);


  Serial.println("GPS Value Sent");
   modem.TCPsend(req, strlen(req));
    
    uint8_t recv[21];
    unsigned long lastRead = millis();
    while (millis() - lastRead < 1000){
      while (modem.TCPavailable()){
        uint8_t r = modem.TCPread(recv, 20);
        recv[r] = 0;
        Serial.write((char *)recv);
        lastRead = millis();
      }
    }
   
    modem.TCPclose();
    modem.enableGPRS(false);
  Serial.println("TCP CLosed");
  }
 


}


