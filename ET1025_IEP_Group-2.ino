#include "RichShieldTM1637.h" 
#include "RichShieldLightSensor.h"
#include "RichShieldDHT.h"
#include <Wire.h>


#define DHT_PIN    12
#define DHT_TYPE   DHT11  
#define CLK 10

#define DIO 11
TM1637 disp(CLK, DIO);
#define LDR_PIN A2
#define KNOB_PIN A0

#define LED_RED 4
#define LED_GREEN 5
#define LED_BLUE 6
#define LED_YELLOW 7
DHT dht(DHT_PIN, DHT_TYPE);

int Hum_Def = 70;           //was humidityThreshold (shorten for easier writing)

int AdjHum = 0
int AdjTemp = 0


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  disp.init();
  dht.begin();

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

  }
  //Temp Detector
  if(Menu == 2)
  {
    clearLEDs();
    BlinkLEDs(LED_BLUE);
    distTempSet((int8_t)Hum_Def);

  }
  //Smart FAN
  if(Menu == 3)
  {
    clearLEDs();
    BlinkLEDs(LED_GREEN);

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
}





void BlinkLEDs(int LED)
{
  digitalWrite(LED, HIGH);
  delay(250);
  digitalWrite(LED, LOW);
  delay(250);
}

void distTempSet(int8_t temperature)
{


  int8_t temp[4];
  temperature %= 100;
  if(digitalRead(BUTTON_K2) == 0)
  {
    AdjTemp += 1
    
  }


  temp[0] = INDEX_BLANK
	temp[1] = temperature / 10;
	temp[2] = temperature % 10;
	temp[3] = 12;	          //index of 'C' for celsius degree symbol.
	disp.display(temp);
}

void distHumSet(int8_t humi)
{
  int8_t temp[4];
  if(humi < 100)temp[0] = INDEX_BLANK;
  else temp[0] = humi/100;
  humi %= 100;
  temp[1] = humi / 10;
  temp[2] = humi % 10;
  temp[3] = 18;	          //index of 'H' for celsius degree symbol.
  disp.display(temp);
}
