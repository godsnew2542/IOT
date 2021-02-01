#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;

char auth[] = "qJhJilNwDldc9SB4tfqbvR9tIOgU_UpV";
char ssid[] = "N-Book";
char pass[] = "nnewnew011";
//char server[] = "192.168.102.74";  // IP for your Local Server
//int port = 8080;

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Wire.h>
#include <SPI.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// BH1750
int BH1750address = 0x23;       // Set BH1750 address
byte buff[2];

// HC-SR04
long distance = 0;
int trigPin = D7;
int echoPin = D8;

// keyes laser
//int laserPin = D10;

// Tone
int pinTone = D8;

void setup() {
  // Debug console
  Serial.begin(115200);

  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  OLED.display();

  Blynk.begin(auth, ssid, pass);
//  Blynk.begin(auth, ssid, pass, server, port);
  //  Blynk.begin(auth, ssid, pass, IPAddress(192,168,102,74), 8080);

    timer.setInterval(2000L, SensorBH1750);
  timer.setInterval(2000L, SensorSR04);

  // HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Blynk.connect();
}

void SensorBH1750() {
  //  digitalWrite(laserPin, HIGH); // Open the laser head

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
    Serial.printf(val + " lx");

     if (val <= 3) {
      analogWrite(pinTone, 255);
      delay(500);
      noTone(pinTone);
    } else {
      noTone(pinTone);
    }
  }
}

void SensorSR04() {
    distance = read_hc_sr04();              // Read data from HC-SR04
  if (distance <= 200 && distance >= 0) {
    // Prepare data to publish here
    char msg[] = "";
    sprintf(msg, "%d", distance);
    Serial.print(msg);
    Serial.print("cm");
    Serial.println();
    Blynk.virtualWrite(V1, msg);
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

void loop() {
  Blynk.run();
  timer.run();
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
