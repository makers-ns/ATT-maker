/*
  Basic ESP8266 MQTT example
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Update these with values suitable for your network.

const char* ssid = "TP-LINK-RZ";
const char* password = "AAAAAAAAAA";
const char* mqtt_server = "api.allthingstalk.io";

char deviceId[] = "sxBP79eJDwOelE3TFIufXTLY";
char token[] = "maker:4PoWRlpZZautW1VeVvQHWBD5LcibxnmtOnAqzT6";

char outTopicCounter[] = "device/sxBP79eJDwOelE3TFIufXTLY/asset/counter/state";
char outTopicText[] = "device/sxBP79eJDwOelE3TFIufXTLY/asset/poruka/state";
char inTopic[] = "device/sxBP79eJDwOelE3TFIufXTLY/asset/+/command";

String value1 = "\"value\":\"1\"";
String value0 = "\"value\":\"0\"";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
long int counter = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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

void callback(char* topic, byte* payload, unsigned int length) {

  StaticJsonBuffer<500> jsonBuffer;
  char json[500];

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    json[i] = (char)payload[i];
    //Serial.print((char)payload[i]);
  }
  json[length] = '\0';
  //Serial.println(json);
  JsonObject& root = jsonBuffer.parseObject(json);

  if (root.success()) {
    const char* atTime = root["at"];
    const char* vrednost = root["value"];
    Serial.println(atTime);
    Serial.println(vrednost);
    if (strcmp(vrednost,"1") == 0) {
      digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is acive low on the ESP-01)
    } else {
      digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    }
  } else {
    Serial.println("Parsing JSON failed");
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", token, "")) {
      Serial.println("connected");
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    ++counter;
    String value;
    value = String(counter);
    /*
      snprintf (msg, 75, "0|Counter : %s",value.c_str());
      Serial.print("Publish to poruka : ");
      Serial.println(msg);
      client.publish(outTopicText, msg);
      delay(100);
    */
    snprintf (msg, 75, "0|%s", value.c_str());
    //Serial.print("Publish to counter: ");
    //Serial.println(msg);
    client.publish(outTopicCounter, msg);

  }
}


char* buildContent(String value)
{
  char* message_buff;
  int length;

  if (value[0] == '[' || value[0] == '{') {
    length = value.length() + 16;
    message_buff = new char[length];
    sprintf(message_buff, "{\"value\":%s}", value.c_str());
  }
  else {
    length = value.length() + 3;
    message_buff = new char[length];
    sprintf(message_buff, "0|%s", value.c_str());
  }

  message_buff[length - 1] = 0;
  return message_buff;
}
