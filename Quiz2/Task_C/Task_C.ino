#include "DHT.h"                    //Include DHT library
#include <Wire.h>                   // Include library
#include <SPI.h>

const char* ssid = "N-book";
const char* password = "nnewnew65";
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_user = "";
const char* mqtt_pass = "";
const int mqtt_port = 1883;

const int postingInterval = 5000;
long lastMsg = 0;

#define DHTTYPE DHT22
#define DHTPIN  D4
DHT dht (DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void mqtt_publish() {

  // Prepare data to publish here
  char topic[] = "/iot/6030213011/temp";

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } else {
    Serial.println("H: " + String(h, 0) + " % - " + "T: " + String(t, 1) + " ");
    Serial.println("H: " + String(h, 0));

    client.publish(topic, msg);
    Serial.printf("Published: topic[%s] temp[%s]\n", topic, (h, 0));
  }
}

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Nnewnew6030213011", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else
      delay(5000);
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > postingInterval) {
    mqtt_publish();
    lastMsg = now;
  }
}
