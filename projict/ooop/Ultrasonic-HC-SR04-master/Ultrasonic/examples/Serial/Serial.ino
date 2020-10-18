// Ultrasonic - Library for HR-SC04 Ultrasonic Ranging Module.
// Rev.4 (06/2012)
// J.Rodrigo ( www.jrodrigo.net )
// more info at www.ardublog.com

#include <Ultrasonic.h>

const int echo = D7;
int trig = D6;

Ultrasonic ultrasonic(trig,echo); // (Trig PIN,Echo PIN)

void setup() {
  Serial.begin(115200); 
}

void loop()
{
  Serial.print(ultrasonic.Ranging(CM)); // CM or INC
  Serial.println(" cm" );
  delay(100);
}
