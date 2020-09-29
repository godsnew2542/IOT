#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "N-book";
const char* password = "nnewnew65";
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_user = "";
const char* mqtt_pass = "";
const int mqtt_port = 1883;

int pinTone = D8;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  pinMode(LED, OUTPUT);
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
  String msg = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  Serial.println(msg);

  if (msg == "0") {
    analogWrite(pinTone, 255);
    analogWriteFreq(600);
    delay(500);
    analogWriteFreq(587);
    delay(200);
  }
  if (msg == "1") {
    noTone(pinTone);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Nnnnnnnnnnnnn", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      char* topic = "/iot/6030213011/alert";
      client.subscribe(topic);
    } else
      delay(5000);
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
