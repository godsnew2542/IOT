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

const int postingInterval = 500;
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

// keyes laser
int laserPin = D10;

void setup() {
  Serial.begin(115200);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  OLED.display();

  // HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(laserPin, OUTPUT);

  delay(500);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void mqtt_publish() {

  digitalWrite(laserPin, HIGH); // Open the laser head

  uint16_t val = 0;
  BH1750_Init(BH1750address);             // Initial BH1750
  if (2 == BH1750_Read(BH1750address)) {
    val = ((buff[0] << 8) | buff[1]) / 1.2;
    OLED.clearDisplay();               //Clear display
    OLED.setTextColor(WHITE);          //Set text color
    OLED.setCursor(0, 0);              //Set display start position
    OLED.setTextSize(3);               //Set text size x1
    OLED.println("BH1750");          //Type message
    OLED.setCursor(0, 35);            //Set display postion
    OLED.println(String(val) + " lx"); // Show result value
    OLED.display();                    //Enable display
    char topic[] = "/projice/iot/BH1750/";
    char msg[] = "";
    sprintf(msg, "%d", val);
    client.publish(topic, msg);
    Serial.printf("Published: topic[%s] val: %s \n", topic, msg);
  }
}
void mqtt_publish_distance() {

  distance = read_hc_sr04();              // Read data from HC-SR04
  if (distance <= 200 && distance >= 0) {
    // Prepare data to publish here
    char topic[] = "/projice/iot/distance/";
    char msg[] = "";
    sprintf(msg, "%d", distance);
    client.publish(topic, msg);
    Serial.printf("Published: topic[%s] Distance =%s cm\n", topic, msg);
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
      delay(100);
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
    mqtt_publish_distance();
    lastMsg = now;
  }
}

int BH1750_Read(int address) {
  int i = 0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while (Wire.available()) {
    buff[i] = Wire.read();                // Read one byte
    i++;
  }
  Wire.endTransmission();
  return i;
}
void BH1750_Init(int address) {
  Wire.beginTransmission(address);
  Wire.write(0x10);                       // Start operation
  Wire.endTransmission();
}
