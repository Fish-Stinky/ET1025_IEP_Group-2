#include "RichShieldTM1637.h" 
#include "RichShieldLightSensor.h"
#include "RichShieldDHT.h"
#include "RichShieldIRremote.h"
#include "RichShieldPassiveBuzzer.h"
#include <Wire.h>

#define LDR_PIN A2
#define KNOB_PIN A0
#define RECV_PIN 2
#define LED_RED 4
#define LED_GREEN 5
#define LED_BLUE 6
#define LED_YELLOW 7

#define DHT_TYPE   DHT11  
#define BUTTON_K1 8
#define BUTTON_K2 9
#define CLK 10
#define DIO 11
#define DHT_PIN    12
TM1637 disp(CLK, DIO);
IRrecv IR(RECV_PIN);
DHT dht(DHT_PIN, DHT_TYPE);

//Thershold of sensors
int HumThreshold = 65;            //Threshold Huminity (Default value is 65)
int TempThreshold = 23;           // Threshold Temperture (Defualt value is 23C)
int LdrSens = 5;                  //Scale from 1 to 10 (Defualt 5)
int ldrThreshold = map(LdrSens, 1, 10, 50, 950); //Actualy value of Ldr adjusted Ranging from 50 to 950
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
 

}
//            MAIN LOOP             //
void loop() 
{
  int Menu = CheckNob();

  if (Menu == 0)
  {
    clearLEDs();   
    //    Main logic inside (Mokuns protion)    // 

  }
  //Smart light
  if(Menu == 1)
  {
    clearLEDs();
    BlinkLEDs(LED_YELLOW);
    AdjLdr();
  }
  //Temp Detector
  if(Menu == 2)
  {
    clearLEDs();
    BlinkLEDs(LED_BLUE);
    AdjTemp();

  }
  //Smart FAN
  if(Menu == 3)
  {
    clearLEDs();
    BlinkLEDs(LED_GREEN);
    AdjHum();
  }
  if(Menu == 4)
  {
    clearLEDs();
    BlinkLEDs(LED_RED);

  }

}
//            Functions            //
int CheckNob()
{
  int knobVal;
  knobVal = analogRead(KNOB_PIN);

  Serial.print("Knob Value is ");
  Serial.println(knobVal);
  return (knobVal/205);
}

void clearLEDs()
{
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
      digitalWrite(LED_YELLOW, LOW);
      return;
}

void BlinkLEDs(int LED)
{
  digitalWrite(LED, HIGH);
  delay(250);
  digitalWrite(LED, LOW);
  delay(250);
  return;
}

void AdjTemp()
{

  if(digitalRead(BUTTON_K1) == 0)
  {
    TempThreshold += 1;
    if(TempThreshold > 28) TempThreshold = 28;
  while (digitalRead(BUTTON_K1) == 0);  
  }

  else if(digitalRead(BUTTON_K2) == 0)
  {
    TempThreshold -= 1;
    if(TempThreshold < 18) TempThreshold = 18;
  while (digitalRead(BUTTON_K2) == 0);  
  }

  int8_t temp[4];
  int8_t temperature = TempThreshold;

  temp[0] = INDEX_BLANK;
	temp[1] = temperature / 10;
	temp[2] = temperature % 10;
	temp[3] = 12;	          //index of 'C' for celsius degree symbol.
	disp.display(temp);
  
  return;
}

void AdjHum()
{
  if(digitalRead(BUTTON_K1) == 0)
  {
    HumThreshold += 5;
    if(HumThreshold < 90) HumThreshold = 90;
  while (digitalRead(BUTTON_K1) == 0);
  }

  else if(digitalRead(BUTTON_K2) == 0)
  {
    HumThreshold -= 5;
    if(HumThreshold < 40) HumThreshold = 40;
  while (digitalRead(BUTTON_K2) == 0);  
  }

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
  if (digitalRead(BUTTON_K1) == 0) 
  {
    LdrSens++;
    if (LdrSens > 10) LdrSens = 10;   // cap at 10
    ldrThreshold = map(LdrSens, 1, 10, 50, 950);  // convert 1-10 to actual threshold
    Serial.print("LdrSens: "); Serial.println(LdrSens);
    while (digitalRead(BUTTON_K1) == 0);       // wait for release
  }

  if (digitalRead(BUTTON_K2) == 0)
  {
    LdrSens--;
    if (LdrSens < 1) LdrSens = 1;     // floor at 1
    ldrThreshold = map(LdrSens, 1, 10, 50, 950);
    Serial.print("LdrSens: "); Serial.println(LdrSens);
    while (digitalRead(BUTTON_K2) == 0);
  }
}

