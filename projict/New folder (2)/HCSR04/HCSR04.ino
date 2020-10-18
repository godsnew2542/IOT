#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Wire.h>
#include <SPI.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

long distance = 0;
#define trigPin D5
#define echoPin D6

void setup()
{

  Serial.begin(115200);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  OLED.display();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void loop()
{
  distance = read_hc_sr04();              // Read data from HC-SR04
  if (distance <= 200 && distance >= 0)   // Set distance display range
  {
    Serial.print("Distance = ");
    Serial.println(distance + String("cm  "));

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
long  read_hc_sr04()                      // Read the distance value for HC-SR04
{
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
