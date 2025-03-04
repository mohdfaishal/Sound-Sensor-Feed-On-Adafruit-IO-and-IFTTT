/***************************************************
  Adafruit MQTT Library ESP8266 Example
  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino
  Works great with Adafruit's Huzzah ESP board:
  ----> https://www.adafruit.com/product/2471
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 *  Modified by PDAControl
 *   Example val and Led control
 *  More info      : http://pdacontrolen.com
 *  Mas informacion: http://pdacontroles.com
 *  Channel  Youtube https://www.youtube.com/c/JhonValenciaPDAcontrol/videos   
  
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


// Lamp pin
//const int lamp_pin = 4;
int Led = 12;    // define LED Interface
int Led1 = 13;
int Led2 = 14;
int val = 0;

#define SOUND A0  // analog 0

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "ZMI_MFJ"
#define WLAN_PASS       "456dfmncgh"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "faishal94"
#define AIO_KEY         "062ec0cf04b14376b9bc4843aa7b9623"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/


// Setup a feed called 'Sound' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish sound = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/SoundFeed");


// Setup a feed called 'onoff' for subscribing to changes to the button
//Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff", MQTT_QOS_1);

/*************************** Sketch Code ************************************/


/*void onoffcallback(char *data, uint16_t len) {
  Serial.print("Hey we're in a onoff callback, the button value is: ");
  Serial.println(data);

     String message = String(data);
      message.trim();
      if (message == "ON") {digitalWrite(lamp_pin, HIGH);}
      if (message == "OFF") {digitalWrite(lamp_pin, LOW);}
  
}*/


void setup() {

    pinMode(Led, OUTPUT);
    pinMode(Led1, OUTPUT);
    pinMode(Led2, OUTPUT);
    
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());


  //onoffbutton.setCallback(onoffcallback);

  //mqtt.subscribe(&onoffbutton);

}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();


 int val = analogRead(SOUND);
  
  Serial.print(F("\nSending Sound val "));
  Serial.print(val,DEC);
  Serial.print("...");
  if (! sound.publish(val)) {
    delay(1000);
    Serial.println(F("Failed"));
  } else {
    delay(1000);
    Serial.println(F("OK!"));
  }
  
  if (val<65){
    digitalWrite(Led,HIGH);
    Serial.print("The value is ");
    Serial.println(val,DEC);
    Serial.println("Which is LOW LEVEL");
    delay(1000);
    digitalWrite(Led,LOW);
  }else if (val>65 && val<70){
    digitalWrite(Led1,HIGH);
    Serial.print("The value is ");
    Serial.println(val,DEC);
    Serial.println("Which is MEDIUM LEVEL");
    delay(1000);
    digitalWrite(Led1,LOW);
  } else if(val>70){
    digitalWrite(Led2,HIGH);
    Serial.print("The value is ");
    Serial.println(val,DEC);
    Serial.println("Which is HIGH LEVEL");
    delay(5000);
    digitalWrite(Led2,LOW);
  }

  // this is our 'wait for incoming subscription packets and callback em' busy subloop
  // try to spend your time here:
  mqtt.processPackets(500);
  
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  
 // if(! mqtt.ping()) {
 //   mqtt.disconnect();
 // }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 10 seconds...");
       mqtt.disconnect();
       delay(10000);  // wait 10 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
