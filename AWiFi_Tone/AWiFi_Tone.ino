int pinTone = D8;
#define sw1 D0
bool sw1_state = false;
void setup()
{
  pinMode(sw1, INPUT);

  //  analogWrite(pinTone, 255);
  delay(1000);
}
void loop()
{
  sw1_state = not(digitalRead(sw1));
  if (sw1_state) {
    analogWrite(pinTone, 255);
    analogWriteFreq(600);
    delay(500);
    analogWriteFreq(587);
    delay(200);
  } else {
    noTone(pinTone);

  }

}
