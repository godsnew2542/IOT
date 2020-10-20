#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Wire.h>
#include <SPI.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Set wifi
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

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

// BH1750
int BH1750address = 0x23;       // Set BH1750 address
byte buff[2];

// HC-SR04
long distance = 0;
#define trigPin D5
#define echoPin D6

void setup() {
  Serial.begin(115200);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  OLED.display();

  // HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  delay(100);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void mqtt_publish() {

  distance = read_hc_sr04();              // Read data from HC-SR04
  if (distance <= 200 && distance >= 0) {
//    Serial.print("Distance = ");
//    Serial.println(distance + String("cm "));

    // Prepare data to publish here
    char topic[] = "/projice/iot/distance/";
    char msg[] = "";
    sprintf(msg, "%d", distance);
    client.publish(topic, msg);
    Serial.printf("Published: topic[%s] Distance =%s cm\n", topic, msg);

    OLED.clearDisplay();
    OLED.setTextColor(WHITE);
    OLED.setCursor(15, 0);
    OLED.setTextSize(2);
    OLED.print("HC-SR04");   // Print some message
    OLED.setCursor(15, 35);
    OLED.setTextSize(3);
    OLED.print(String(distance) + " cm");
    OLED.display();
  }
}
long  read_hc_sr04() {
  long duration, distance;
  digitalWrite(trigPin, LOW);             // Trig signal sending
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);      // Echo signal receiving
  distance = (duration / 2) / 29.1;       // Calculate the distance result
  return  distance;                       // Return value
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
      delay(400);
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
