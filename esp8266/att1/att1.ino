#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ATT_IOT.h>
#include <SPI.h>  // required to have support for signed/unsigned long type.

const char* ssid     = "TP-LINK-RZ";
const char* password = "AAAAAAAAAA";

// define device credentials
char deviceId[] = "sxBP79eJDwOelE3TFIufXTLY";
char token[] = "maker:4PoWRlpZZautW1VeVvQHWBD5LcibxnmtOnAqzT6";

// define http and mqtt endpoints
#define httpServer "api.allthingstalk.io"  // API endpoint
#define mqttServer "api.allthingstalk.io"  // broker

ATTDevice device(deviceId, token);

//required for the device
void callback(char* topic, byte* payload, unsigned int length);
WiFiClient ethClient;
PubSubClient pubSub(mqttServer, 1883, callback,ethClient);  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);                                           // init serial link for debugging
  
  setup_wifi();
  delay(1000);                                                  //give the Ethernet shield a second to initialize:
  while(!device.subscribe(pubSub))  // make certain that we can receive messages over mqtt
    Serial.println("retrying"); 
}

unsigned long vreme;
unsigned int prevVal = 0;
int counter = 0;
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long curTime = millis();
  if (curTime > (vreme + 3000))  // update and send counter value every 3 seconds
  {
    counter++;
    device.send(String(counter), "counter");
    vreme = curTime;
  }
  device.process(); 
}



void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{ 
  String assetName = device.getAssetName(topic, strlen(topic));
  Serial.print("Data arrived from asset: ");
  Serial.println(assetName);

}
