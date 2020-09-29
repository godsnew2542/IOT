#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>                   // Include library
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#define OLED_RESET -1
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "N-book";
const char* password = "nnewnew65";
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_user = "";
const char* mqtt_pass = "";
const int mqtt_port = 1883;

const int postingInterval = 5000;
long lastMsg = 0;

int BH1750address = 0x23;       // Set BH1750 address
byte buff[2];

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();

  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  client.setServer(mqtt_server, mqtt_port);
}

void mqtt_publish() {
  //  BH1750
  uint16_t val = 0;
  BH1750_Init(BH1750address);             // Initial BH1750
  delay(200);
  if (2 == BH1750_Read(BH1750address))    // Read and check data from BH1750
  {
    val = ((buff[0] << 8) | buff[1]) / 1.2;
    OLED.clearDisplay();               //Clear display
    OLED.setTextColor(WHITE);          //Set text color
    OLED.setCursor(0, 0);              //Set display start position
    OLED.setTextSize(3);               //Set text size x1
    OLED.println("BH1750");          //Type message
    OLED.setCursor(0, 35);            //Set display postion
    OLED.println(String(val) + " lx"); // Show result value
    OLED.display();                    //Enable display

    // Prepare data to publish here
    char topic[] = "/iot/6030213011/lux";
    char msg[] = "";
    sprintf(msg, "%f", val);
    client.publish(topic, msg);
    Serial.printf("Published: topic[%s] lux[%s] \n", topic, msg);
    delay(5000);

  }
  delay(150);
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
      delay(500);
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

//BH1750
int BH1750_Read(int address)              // BH1750 read data function
{
  int i = 0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while (Wire.available())
  {
    buff[i] = Wire.read();                // Read one byte
    i++;
  }
  Wire.endTransmission();
  return i;
}
void BH1750_Init(int address)             // BH1750 initial function
{
  Wire.beginTransmission(address);
  Wire.write(0x10);                       // Start operation
  Wire.endTransmission();
}
