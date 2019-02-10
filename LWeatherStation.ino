/*************************************************** 
  LOCAL WEATEHR STATION:::
  
  This is an example for the BMP180 Barometric Pressure & Temp Sensor and DHT11 temp and Humidity sensor
  
  BMP180 useS I2C to communicate, 2 pins are required to  
  interface
  // Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
  // Connect GND to Ground
  // connect i2c bus with SDA=D2 and SCL=D1 of NodeMCU 

  // Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
  // Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
  // EOC is not used, it signifies an end of conversion
  // XCLR is a reset pin, also not used here

  DHT11 uses normal gpio pins to interface with NodeMCU
  // Connect VCC or + of the DHT11 sensor to 3.3V (NOT 5.0V!)
  // Connect GND or - of the DHT11 to Ground
  // connect out of DHT11 to any gpio of nodeMCU (here to  d4 of nodemcu)

  returns
  
  
 ****************************************************/
//  INCLUDES libraries for DHT11
#include "DHT.h"
#define DHTPIN 2     
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// INCLUDES libraries for BMP180
#include <Wire.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;


#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 13); // RX, TX

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager
#include <WiFiClientSecure.h>

int count=0,i,m,j,k;
 


//////////////////////////////////////// ALL DECLARATIONS for CLOUD //////////////////////////////
const char* host = "api.thingsai.io";                                 // OR host = devapi2.thethingscloud.com
const char* post_url = "/devices/deviceData";       // OR /api/v2/thingscloud2/_table/data_ac
const char* time_server = "baas.thethingscloud.com";             //this is to convert timestamp
const int httpPort = 80;
const int httpsPort = 443;
const char*  server = "api.thingsai.io";  // Server URL

char timestamp[10];

WiFiClient client;


/////////////////////////////////////////////////////////////////////////////////////////////////////////
void configModeCallback (WiFiManager *myWiFiManager) 
{
  Serial.println("Entered config mode");             //*-*-*-*-*-*-*-*-*-*-*-*-*-*if control enters this function then net is not connected
  Serial.println(WiFi.softAPIP());                  // "WiFi.softAPIP() is for AP" , "WiFi.localIP() is for STA",
                                                                
  Serial.println(myWiFiManager->getConfigPortalSSID());             //if you used auto generated SSID, print it
}
/////////////////////////////////////// TIMESTAMP CALCULATION function///////////////////////////////////////
int GiveMeTimestamp()
{
  unsigned long timeout = millis();

  while (client.available() == 0)
  {
    if (millis() - timeout > 50000)
    {
      client.stop();
      return 0;
    }
  }

while (client.available())
      {
        String line = client.readStringUntil('\r');                    //indexOf() is a funtion to search for smthng , it returns -1 if not found
        int pos = line.indexOf("\"timestamp\"");                       //search for "\"timestamp\"" from beginning of response got and copy all data after that , it'll be your timestamp
        if (pos >= 0)                                                     
        {
          int j = 0;
          for(j=0;j<10;j++)
          {
            timestamp[j] = line[pos + 12 + j];
          }
        }
      }
}  
////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() 
{
  delay(1000);
  dht.begin();
  bmp.begin();

  
  Serial.begin(115200);     //(19200,SERIAL_8E1) - data size = 8 bits , parity = Even , stop bit =  1bit
  mySerial.begin(115200);
  WiFiManager wifiManager;
 
  wifiManager.setAPCallback(configModeCallback);
                                                                                                    
  if(!wifiManager.autoConnect("iPankaj","80518051"))                   //wifiManager.autoConnect("AP-NAME", "AP-PASSWORD"); (OR) wifiManager.autoConnect("AP-NAME"); only ID no password (OR) wifiManager.autoConnect(); this will generate a ID by itself
  {
    Serial.println("failed to connect and hit timeout");     //control comes here after long time of creating Access point "NodeMCU" by NodeMCU and still it has not connected
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  } 

  //if you come here you have connected to the WiFi
  Serial.println("connected...yeey :");
 
}

void loop() 
{
 // int analogValue = analogRead(outputpin);
{
 
  
/////////////////////////////////////// SEND THE QUERY AND RECEIVE THE RESPONSE///////////////////////  

          // LOGIC FOR DHT11
  
            delay(2000);
            float h = dht.readHumidity();
            float t = dht.readTemperature();
            float f = dht.readTemperature(true);
          
            if (isnan(h) || isnan(t) || isnan(f))      // isnan ==> "is not a number"...returns 1 if " h  or t or f are not a number"     
              {
                  Serial.println("Failed to read from DHT sensor!");
                  return;
              }
          
            float hic = dht.computeHeatIndex(t, h, false);
          
            Serial.print("Humidity: ");
            Serial.print(h);
            Serial.print(" %\t");
            Serial.print(" %\n");
            Serial.print("Temperature: ");
            Serial.print(t);
            Serial.print(" *C ");
            Serial.print(" %\n");
            Serial.print("Heat index: ");
            Serial.print(hic);
            Serial.print(" *C ");
            Serial.print(" %\n");
            delay(2000);
          
            //LOGIC FOR BMP180
              float pr= bmp.readPressure();
              float alt= bmp.readAltitude();
              float realAlt = bmp.readAltitude(101500);
              Serial.print("Pressure = ");
              Serial.print(pr);
              Serial.println(" Pa");
              
              // Calculate altitude assuming 'standard' barometric
              // pressure of 1013.25 millibar = 101325 Pascal
              Serial.print("Altitude = ");
              Serial.print(alt);
              Serial.println(" meters");
          
              Serial.print("Pressure at sealevel (calculated) = ");
              Serial.print(bmp.readSealevelPressure());
              Serial.println(" Pa");
          
            // you can get a more precise measurement of altitude
            // if you know the current sea level pressure which will
            // vary with weather and such. If it is 1015 millibars
            // that is equal to 101500 Pascals.
              Serial.print("Real altitude = ");
              Serial.print(realAlt);
              Serial.println(" meters");
              
              Serial.println();
              delay(3000);

          

  Serial.print("connecting to ");
  Serial.println(host);                          //defined upside :- host = devapi2.thethingscloud.com or 139.59.26.117

///////////////////////////////////// TIMESTAMP CODE SNIPPET /////////////////////////
Serial.println("inside get timestamp\n");
  if (!client.connect(time_server, httpPort)) 
  {
    return;                                                        //*-*-*-*-*-*-*-*-*-*
  }

  client.println("GET /api/timestamp HTTP/1.1");                            //Whats this part doing, i didnt get
  client.println("Host: baas.thethingscloud.com");
  client.println("Cache-Control: no-cache");
  client.println("Postman-Token: ea3c18c6-09ba-d049-ccf3-369a22a284b8");
  client.println();

GiveMeTimestamp();                        //it'll call the function which will get the timestamp response from the server
Serial.println("timestamp receieved");
Serial.println(timestamp);
///////////////////////////////////////////////////////////////////////////////

  Serial.println("inside ThingsCloudPost");

 
  String PostValue = "{\"device_id\":  61121696123, \"slave_id\": 2";
         PostValue = PostValue + ",\"dts\":" +timestamp;
   PostValue = PostValue +",\"data\":{\"Humidity\":" + h +",\"Temperature\":" + t +", \"Pressure\":" + pr +",\"Real Altitude\":" + realAlt +", \"Heat index\":" + hic +" }"+"}";
  
  
  Serial.println(PostValue);

/* create an instance of WiFiClientSecure */
    WiFiClientSecure client;
    
    Serial.println("Connect to server via port 443");
    if (!client.connect(server, 443)){
        Serial.println("Connection failed!");
    } else {
        Serial.println("Connected to server!");
        /* create HTTP request */

        client.println("POST /devices/deviceData HTTP/1.1");
        client.println("Host: api.thingsai.io");
        //client.println("Connection: close");
        client.println("Content-Type: application/json");
        client.println("cache-control: no-cache");
        client.println("Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.IjVhMzBkZDFkN2QwYjNhNGQzODkwYzQ4OSI.kaY6OMj5cYlWNqC2PNTkXs9PKy6_m9tdW5AG7ajfVlY");
        client.print("Content-Length: ");
        client.println(PostValue.length());
        client.println();
        client.println(PostValue);
//////////////////////////////////POSTING the data on to the cloud is done and now get the response form cloud server//////////////////
 Serial.print("Waiting for response ");
        while (!client.available()){
            delay(50); //
            Serial.print(".");
        }  
        /* if data is available then receive and print to Terminal */
        while (client.available()) {
            char c = client.read();
            Serial.write(c);
        }

        /* if the server disconnected, stop the client */
        if (!client.connected()) {
            Serial.println();
            Serial.println("Server disconnected");
            client.stop();
        }
    }
Serial.println("//////////////////////    THE END     /////////////////////");
delay(3000);
}
}
