#define BLYNK_PRINT Serial

// DHT22
#include "DHT.h"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LED D0
DHT dht(D5, DHT22, 14);
BlynkTimer timer;

char auth[] = "vC3F0rBaRNg1YZlHWkQHY07w9pWIs3Fm";

char ssid[] = "N-book";
char pass[] = "name_nnew";

WidgetLED led(V1);

// set up DHT22
void Sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.printf("H = %.1f : T = %.1f", h, t);
  Serial.println();

  //ส่งจาก Board ไปยัง Blink
  Blynk.virtualWrite(V0, h);
  if (h > 60) {
    led.on();
    Blynk.notify("turn on LED !!");
  } else {
    led.off();
  }
  OLED.clearDisplay();               //Clear display
  OLED.setTextColor(WHITE);          //Set text color
  OLED.setCursor(0, 0);              //Set display start position
  OLED.setTextSize(2);               //Set text size x2
  OLED.println("H: " + String(h, 0) + " %"); // Show result value
  OLED.setCursor(0, 30);            //Set display postion
  OLED.print("T: " + String(t, 1) + " ");
  OLED.write(247);                    // อักษรพิเศษ
  OLED.print("C");
  OLED.display();                    //Enable display
  delay(2000);
}

void setup() {
  dht.begin();

  // Debug console
  Serial.begin(115200);

  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  Blynk.begin(auth, ssid, pass);

  OLED.display();

  timer.setInterval(2000L, Sensor);

  //setup LED
  pinMode(LED, OUTPUT);
}

void loop() {
  Blynk.run();
  timer.run();
}
