#define BLYNK_PRINT Serial

// DHT22
#include "DHT.h"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

DHT dht(D5, DHT22, 14);
BlynkTimer timer;


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Z7-jv5hlqWyn4gruJbn80f32B5iWtggo";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "N-book";
char pass[] = "name_nnew";

#define LED D0
//ส่งข้อมูลจาก Blink into board port V4

//BLYNK_WRITE(V4) { // Subscribe Virtual pin 4
//  int pinValue = param.asInt();
//  Serial.print("V4 Value is : ");
//  Serial.println(pinValue);
//
//  if (pinValue == 0) {
//    digitalWrite(LED, LOW);
//  } else {
//    digitalWrite(LED, HIGH);
//  }
//}


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
  Blynk.virtualWrite(V2, t);
  if (h > 60) {
    Blynk.notify("Alarm !");
  }
}

void setup() {
  dht.begin();

  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(2000L, Sensor);

  //setup LED
  pinMode(LED, OUTPUT);
}

void loop() {
  Blynk.run();
  timer.run();
}
