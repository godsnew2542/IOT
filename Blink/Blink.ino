// the setup function runs once when you press reset or power the board
#define BUILTIN D0
#define LED D4
#define sw1 D5
int sw1_state = 0;
void setup() {
  //  T1
  //  pinMode(BUILTIN, OUTPUT);
  //  pinMode(LED, OUTPUT);

  Serial.begin(115200);
  pinMode(sw1, INPUT);
  pinMode(LED, OUTPUT);

}

void loop() {
  sw1_state = not(digitalRead(sw1));
  //  Serial.println(sw1);
  if (sw1_state == 0) {
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, HIGH);
  }

  //  T1
  //  digitalWrite(BUILTIN, HIGH);
  //  delay(100);
  //  digitalWrite(LED, HIGH);
  //  delay(500);
  //  digitalWrite(BUILTIN, LOW);
  //  delay(200);
  //  digitalWrite(LED, LOW);
  //  delay(10);

}
