#include <DHT.h>

#define DHT_PIN    12
#define DHT_TYPE   DHT11  
#define LED_GREEN  5
DHT dht(DHT_PIN, DHT_TYPE);

int humidityThreshold = 70;
void setup() {
  pinMode(LED_GREEN, OUTPUT);
  dht.begin();

}
void loop() {
  smartFanControl();
  delay(100);
}
void smartFanControl() {
  float humidity = dht.readHumidity();

  if (humidity > humidityThreshold) 
  {
    digitalWrite(LED_GREEN, HIGH);
  } 
  else 
  {
    digitalWrite(LED_GREEN, LOW);
  }
}
