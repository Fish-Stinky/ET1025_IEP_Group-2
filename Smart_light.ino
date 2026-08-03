#define LDR_PIN A2
#define LED_YELLOW 7

int ldrThreshold = 300;
void setup() {
  pinMode(LED_YELLOW, OUTPUT);
}

void loop() {
  smartLight();
  delay(100);
}
void smartLight() {
  if (analogRead(LDR_PIN) < ldrThreshold) 
  {
    digitalWrite(LED_YELLOW, HIGH);
  } 
  else 
  {
    digitalWrite(LED_YELLOW, LOW);
  }
}
