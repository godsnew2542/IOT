#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"                    //Include DHT library
#include <Wire.h>                   // Include library
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char* ssid = "N-book";
const char* password = "nnewnew65";
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_user = "";
const char* mqtt_pass = "";
const int mqtt_port = 1883;

const int postingInterval = 5000;
long lastMsg = 0;

WiFiClient espClient;
PubSubClient client(espClient);

#define DHTTYPE DHT22
#define DHTPIN  D4
DHT dht (DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);

  dht.begin();
}

void mqtt_publish() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } else {
    Serial.println("H: " + String(h, 0) + " % - " + "T: " + String(t, 1) + " ");

    // Prepare data to publish here
    char topic[] = "/iot/6030213011/hello/";
    char msg[] = "";
    char msg1[] = "";
    sprintf(msg, "%f", t);
    sprintf(msg1, "%f", h);
    client.publish(topic, msg);
    Serial.printf("Published: topic[%s] message[%s] H:[%s]\n", topic, msg, msg1);
    delay(2000);
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
