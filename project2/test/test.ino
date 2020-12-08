#include <TridentTD_LineNotify.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;

char auth[] = "BJpfq4mHiIrQ1b9_bhE1Jxgv1UKgcZL5";
char ssid[] = "iotwifi";
char pass[] = "1234567890";
char server[] = "192.168.102.74";  // IP for your Local Server
int port = 8080;
#define LINE_TOKEN  "7voFpEXR9ZuX7PzaxQnvNGhFbrQMOE8VLcQQnIkrVmm"

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Wire.h>
#include <SPI.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int ledPin = D0;
int pinValue;

int digitalPin = D2;
int valPIR = 0;
int pinTone = D8;

//ส่งข้อมูลจาก Blink into board port V0

BLYNK_WRITE(V0) { // Subscribe Virtual pin 0
  pinValue = param.asInt();
  Serial.print("V4 Value is : ");
  Serial.println(pinValue);

  if (pinValue == 0) {
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, HIGH);

  }
}

void setup() {
  // Debug console
  Serial.begin(115200);
  Serial.println(LINE.getVersion());

  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  OLED.display();

  Blynk.begin(auth, ssid, pass, server, port);

  pinMode(ledPin, OUTPUT); // sets the pin as output
  pinMode(digitalPin, INPUT); // sets the pin as input

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);
  LINE.notify("myarduino.net");

  Blynk.connect();
}

void PIR() {
  valPIR = digitalRead(digitalPin);
  Serial.print("valPIR = ");
  Serial.println(valPIR);

  OLED.clearDisplay();               //Clear display
  OLED.setTextColor(WHITE);          //Set text color
  OLED.setCursor(0, 35);            //Set display postion
  OLED.println(String(valPIR)); // Show result value
  OLED.display();

  if (valPIR == 1) {
    // ส่งแจ้งเตือน LINE
    String LineText;
    LineText = "มีคนเข้าใกล้ตู้รับบริจาค";
    //    Serial.println(LineText);
    LINE.notify(LineText);

    // เสียง
    analogWrite(pinTone, 255);
    delay(500);
    noTone(pinTone);
  } else {
    noTone(pinTone);
  }
  delay(100);
}

void loop() {
  Blynk.run();
  if (pinValue == 0) {
    PIR();
  }
}
