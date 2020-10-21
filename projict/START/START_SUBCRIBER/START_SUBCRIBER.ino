#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "N-book";
const char* password = "nnewnew65";
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_user = "";
const char* mqtt_pass = "";
const int mqtt_port = 1883;

// Tone
int pinTone = D8;

// LED
#define LDE_red D0
#define LED_green D4

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // LED
  pinMode(LDE_red, OUTPUT);
  pinMode(LED_green, OUTPUT);
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

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle your payload here
  String dis = "";
  String BH = "";

  if (strcmp(topic, "/projice/iot/distance/")) {
    for (int i = 0; i < length; i++) {
      dis += (char)payload[i];
    }
    if (dis > "5") {
      digitalWrite(LDE_red, LOW);
      digitalWrite(LED_green, HIGH);
    } else {
      digitalWrite(LDE_red, HIGH);
      digitalWrite(LED_green, LOW);
    }
  }

  if (strcmp(topic, "/projice/iot/BH1750/")) {
    for (int i = 0; i < length; i++) {
      BH += (char)payload[i];
    }
    if (BH <= "3") {
      analogWrite(pinTone, 255);
      analogWriteFreq(600);
      delay(500);
      analogWriteFreq(587);
      delay(200);
    } else {
      noTone(pinTone);
    }
  }

}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Nnnnnnnnnnnnn", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      char* topic = "/projice/iot/+/";
      client.subscribe(topic);
    } else
      delay(100);
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
