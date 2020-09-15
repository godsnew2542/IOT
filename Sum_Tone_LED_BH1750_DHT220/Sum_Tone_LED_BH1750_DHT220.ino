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

// DHT220
#define DHTTYPE DHT22
#define DHTPIN  D4
//#define LED_DISLAY_REd  D5
//#define LED_DISLAY_GREEN  D6

// BH1750
int BH1750address = 0x23;       // Set BH1750 address
byte buff[2];

// LED
#define LED_DISLAY_GREEN  D6

// Tone
int pinTone = D8;

DHT dht (DHTPIN, DHTTYPE);
void setup()   {
  Serial.begin(115200);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  dht.begin();
  OLED.display();
    pinMode(LED_DISLAY_GREEN, OUTPUT);
  delay(2000); // Pause for 2 seconds

}
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  uint16_t val = 0;
  BH1750_Init(BH1750address);             // Initial BH1750

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from sensor!");
    return;
  } else {
    if (2 == BH1750_Read(BH1750address)) {
      val = ((buff[0] << 8) | buff[1]) / 1.2;
      Serial.println("H: " +
                     String(h, 0) +
                     " % - " +
                     "T: " +
                     String(t, 1) +
                     "Val: " +
                     "String(val)");

      if (h > 70 && val > 5000) {
        digitalWrite(LED_DISLAY_GREEN, LOW);
        analogWrite(pinTone, 255);
        analogWriteFreq(600);
        delay(200);
        noTone(pinTone);
      } else {
        digitalWrite(LED_DISLAY_GREEN, HIGH);
      }

      OLED.clearDisplay();               //Clear display
      OLED.setTextColor(WHITE);          //Set text color
      OLED.setCursor(0, 0);              //Set display start position
      OLED.setTextSize(2);               //Set text size x2
      OLED.println("H: " + String(h, 0) + " %"); // Show result value

      OLED.print("T: " + String(t, 1) + " ");
      OLED.write(247);                    // อักษรพิเศษ
      OLED.print("C");
      OLED.println("BH1750");          //Type message
      //    OLED.setCursor(0, 35);            //Set display postion
      OLED.println(String(val) + " lx"); // Show result value
      OLED.display();                    //Enable display
    }
    delay(100);
  }

}
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
