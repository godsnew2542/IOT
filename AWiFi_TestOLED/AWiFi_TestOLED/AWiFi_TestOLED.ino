#include <Wire.h>                   // Include library
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()   {
  Serial.begin(115200);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  // initialize with the I2C addr 0x3C (for the 128x64)
}

void loop() {
  OLED.clearDisplay();               //Clear display
  OLED.setTextColor(WHITE);          //Set text color
  OLED.setCursor(0,0);               //Set display start position
  OLED.setTextSize(1);               //Set text size x1
  OLED.println("Size1");             //Type message
  OLED.setCursor(35,20);             //Set display postion
  OLED.setTextSize(2);               //Set text size x2 
  OLED.println("Size2");             //Type message
  OLED.display();                    //Enable display
}
