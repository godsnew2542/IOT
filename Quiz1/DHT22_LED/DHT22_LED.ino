#include "DHT.h"                    //Include DHT library
#include <Wire.h>                   // Include library
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTTYPE DHT22
#define DHTPIN  D4

#define LED D0

DHT dht (DHTPIN, DHTTYPE);

void setup()   {
  Serial.begin(115200);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // initialize with the I2C addr 0x3C (for the 128x64)

  dht.begin();
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.

  pinMode(LED, OUTPUT);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } else {
    Serial.println("H: " + String(h, 0) + " % - " + "T: " + String(t, 1) + " ");
    if (t > 26) {
      digitalWrite(LED, HIGH);
    } else {
      digitalWrite(LED, LOW);
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

}
