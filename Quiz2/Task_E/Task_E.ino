#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "N-book";
const char* password = "nnewnew65";
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_user = "";
const char* mqtt_pass = "";
const int mqtt_port = 1883;

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LED D4

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  OLED.display();
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
    digitalWrite(LED, LOW);
  }
  if (msg == "1") {
    digitalWrite(LED, HIGH);
  }
  OLED.clearDisplay();               //Clear display
  OLED.setTextColor(WHITE);          //Set text color
  OLED.setCursor(0, 0);              //Set display start position
  OLED.setTextSize(2);               //Set text size x2
  OLED.println("LED: " + String(msg)); // Show result value
  OLED.display();                    //Enable display
  delay(2000);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Nnnnnnnnnnnnn", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      char* topic = "/iot/6030213011/message/";
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
