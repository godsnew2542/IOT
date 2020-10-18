#include <Wire.h>                   // Include library
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// ADD
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int BH1750address = 0x23;       // Set BH1750 address
byte buff[2];

int pinTone = D8;

void setup() {
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop() {
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

    if (val > 1000) {
      analogWrite(pinTone, 255);
      analogWriteFreq(600);
      delay(200);
      noTone(pinTone);
    }
  }
  delay(150);
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
