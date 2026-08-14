#include "RichShieldTM1637.h" 
#include "RichShieldLightSensor.h"
#include "RichShieldNTC.h"
#include "RichShieldDHT.h"
#include "RichShieldIRremote.h"
#include "RichShieldPassiveBuzzer.h"
#include <Wire.h>

#define KNOB_PIN A0
#define LDR_PIN A2
#define NTC_PIN A1
#define RECV_PIN 2
#define BUZZER_PIN 3
#define LED_RED 4
#define LED_GREEN 5
#define LED_BLUE 6
#define LED_YELLOW 7
#define DHT_TYPE DHT11  
#define BUTTON_K1 8
#define BUTTON_K2 9
#define CLK 10
#define DIO 11
#define DHT_PIN 12
TM1637 disp(CLK, DIO);
IRrecv IR(RECV_PIN);
DHT dht(DHT_PIN, DHT_TYPE);
NTC temper(NTC_PIN);  
#define BUTTON_CODE 0x45 
PassiveBuzzer buz(BUZZER_PIN);     //(Power button on Remote)
//    Thershold of sensors    // 
int HumThreshold = 65;            //Threshold Huminity (Default value is 65)
int TempThreshold = 23;           // Threshold Temperture (Defualt value is 23C)
int LdrSens = 4;                  //Scale from 1 to 10 (Defualt 4)
int ldrThreshold = 350;
int state = 1;
int previousMenu = -1;  // ADDED: tracks last menu, -1 forces a clear on first run
bool K1WasPressed = false;
bool K2WasPressed = false;
unsigned long previousBlinkTime = 0;  //Compare with previous cloak
bool ledBlinkState = false;           //iintial state
const long blinkInterval = 100;       //100ms interval for LED blink

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  disp.init();
  dht.begin();
  IR.enableIRIn();

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(BUTTON_K1, INPUT_PULLUP);
  pinMode(BUTTON_K2, INPUT_PULLUP);
  
}
//            MAIN LOOP             //
void loop() 
{
  int Menu = CheckNob();    //Check nob Value
  delay(100);

  if (Menu != previousMenu) //only clear LEDs on menu change
  {   
    clearLEDs();
    previousMenu = Menu;
  }

  if (Menu == 0)
  {   
    //    Main logic inside (Mokuns protion)    // 
    if(state == 1)
    {
    digitalWrite(LED_RED, HIGH);
    smartLight();
    tempDetection();
    smartFanControl();
    }
    if(state == 0)
    {
    clearLEDs();

    }
    
  OnOffSwitch();
  }
  //Smart light
  if(Menu == 1)
  {
    BlinkLEDs(LED_YELLOW);
    AdjLdr();
  }
  //Temp Detector
  if(Menu == 2)
  {
    BlinkLEDs(LED_BLUE);
    AdjTemp();

  }
  //Smart FAN
  if(Menu == 3)
  {
    BlinkLEDs(LED_GREEN);
    AdjHum();
  }

  if(Menu == 4)
  {
    BlinkLEDs(LED_RED);
    CheckPower();
  }

}
//            Functions            // (GynXern)
int CheckNob() {
  int knobVal;
  knobVal = analogRead(KNOB_PIN);

  //Serial.print("Knob Value is ");
  //Serial.println(knobVal);

  return (knobVal/205);
}

void clearLEDs() {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
      digitalWrite(LED_YELLOW, LOW);
      return;
}

void BlinkLEDs(int LED) {
  unsigned long currentTime = millis();
  if (currentTime - previousBlinkTime >= blinkInterval) 
  {
    previousBlinkTime = currentTime;
    ledBlinkState = !ledBlinkState;
    digitalWrite(LED, ledBlinkState);
  }
}

void AdjTemp() {

  bool K1Now = (digitalRead(BUTTON_K1) == 0);
  bool K2Now = (digitalRead(BUTTON_K2) == 0);

  if (K1Now && !K1WasPressed) 
  {          // only triggers on the moment it's first pressed
    TempThreshold += 1;
    if (TempThreshold > 29) TempThreshold = 29;
  }
  if (K2Now && !K2WasPressed) 
  {
    TempThreshold -= 1;
    if (TempThreshold < 18) TempThreshold = 18;
  }

  K1WasPressed = K1Now;                  // remember state for next loop pass
  K2WasPressed = K2Now;

  int8_t temp[4];
  int8_t temperature = TempThreshold;

  temp[0] = INDEX_BLANK;
	temp[1] = temperature / 10;
	temp[2] = temperature % 10;
	temp[3] = 12;	          //index of 'C' for celsius degree symbol.
	disp.display(temp);
  Serial.print("Temp: "); Serial.println(TempThreshold);
  return;
}

void AdjHum() {
  bool K1Now = (digitalRead(BUTTON_K1) == 0);
  bool K2Now = (digitalRead(BUTTON_K2) == 0);

  if (K1Now && !K1WasPressed) 
  {          // only triggers on the moment it's first pressed
    HumThreshold += 5;
    if(HumThreshold > 90) HumThreshold = 90;
  }
  if (K2Now && !K2WasPressed) 
  {
    HumThreshold -= 5;
    if (HumThreshold < 40) HumThreshold = 40;
  }

  K1WasPressed = K1Now;                  // remember state for next loop pass
  K2WasPressed = K2Now;

  int8_t temp[4];
  int8_t humi = HumThreshold;
  temp[0] = INDEX_BLANK;
  temp[1] = humi / 10;
  temp[2] = humi % 10;
  temp[3] = 18;	          //index of 'H' for celsius degree symbol.
  disp.display(temp);
  return;
}

void AdjLdr() 
{
  bool K1Now = (digitalRead(BUTTON_K1) == 0);
  bool K2Now = (digitalRead(BUTTON_K2) == 0);

  if (K1Now && !K1WasPressed) 
  {          // only triggers on the moment it's first pressed
    LdrSens++;
    if (LdrSens > 10) LdrSens = 10;   // cap at 10
    ldrThreshold = map(LdrSens, 1, 10, 50, 950); 
  }
  if (K2Now && !K2WasPressed) 
  {
    LdrSens--;
    if (LdrSens < 1) LdrSens = 1;     // floor at 1
    ldrThreshold = map(LdrSens, 1, 10, 50, 950);
  }
  disp.display(LdrSens);
}


void CheckPower()
{
  if(state == 1) 
  {
  digitalWrite(LED_GREEN, HIGH);
  }
  else
  {
  digitalWrite(LED_YELLOW, HIGH);
  }
  Serial.print("State: "); Serial.println(state);
}
//            Function              // (Mokun)
void smartLight() {
  Serial.print("Light: "); Serial.println(analogRead(LDR_PIN));Serial.print("  // LdrTHRE: "); Serial.println(ldrThreshold);
  if (analogRead(LDR_PIN) < ldrThreshold) 
  {
    digitalWrite(LED_YELLOW, HIGH);
  } 
  else 
  {
    digitalWrite(LED_YELLOW, LOW);
  }
}

void tempDetection() {
  int8_t temperature;
  temperature = dht.readTemperature();
  Serial.print("Temp: "); Serial.println(temperature);Serial.print(" // TempTHRE: "); Serial.println(TempThreshold);
  if (TempThreshold <= temperature)
  {
    digitalWrite(LED_BLUE, HIGH);    
  } 
  else 
  {
    digitalWrite(LED_BLUE, LOW);           
  }

  int8_t temp[4];
 
  temp[0] = INDEX_BLANK;
  temperature %= 100;
	temp[1] = temperature / 10;
	temp[2] = temperature % 10;
	temp[3] = 12;	          //index of 'C' for celsius degree symbol.
	disp.display(temp);
}

void smartFanControl() {
  float humidity = dht.readHumidity();
  Serial.print("Humi: "); Serial.println(humidity);Serial.print("  // HumTHRE: "); Serial.println(HumThreshold);

  if (humidity > HumThreshold) 
  {
    digitalWrite(LED_GREEN, HIGH);
  } 
  else 
  {
    digitalWrite(LED_GREEN, LOW);
  }
}

void OnOffSwitch() 
{
  if (IR.decode())
  {
    if(IR.isReleased())
    {
    if (IR.keycode == BUTTON_CODE) 
      {
        state = (state == 1) ? 0 : 1;   // clean toggle between 0 and 1

        if (state == 1) {
          buz.playTone(2000, 100);      // higher pitch = turning ON
          digitalWrite(LED_RED, HIGH);
        } 
        else 
        {
          buz.playTone(500, 100);       // lower pitch = turning OFF
          digitalWrite(LED_RED, LOW);
        }
      }
    }
    IR.resume();
  }
}