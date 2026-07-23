#include "RichShieldTM1637.h" 
#define CLK 10
#define DIO 11
TM1637 display(CLK, DIO);

#define KNOB_PIN A2
#define LED_RED 4
#define LED_GREEN 5
#define LED_BLUE 6
#define LED_YELLOW 7


void setup() {
  // put your setup code here, to run once:
  disp.init();
  Serial.begin(9600);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
 

}

void loop() 
{
      int knobVal;
  knobVal = analogRead(KNOB_PIN);

  Serial.print("Knob Value is ");
  Serial.println(knobVal);
  int Menu = knobVal/205

  if (Menu <= 1 )

    }
}

//            Functions            //
void displayTemperature(int8_t temperature)
{
  int8_t temp[4];
  if(temperature < 0)
	{
		temp[0] = INDEX_NEGATIVE_SIGN;
		temperature = abs(temperature);
	}
	else if(temperature < 100)temp[0] = INDEX_BLANK;
	else temp[0] = temperature/100;
	temperature %= 100;
	temp[1] = temperature / 10;
	temp[2] = temperature % 10;
	temp[3] = 12;	          //index of 'C' for celsius degree symbol.
	disp.display(temp);
}
