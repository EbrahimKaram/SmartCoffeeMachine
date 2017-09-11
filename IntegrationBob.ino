/*
INTEGRATIONG CODE BY BILAL 
VERSION 0.3
0.1 : TESTING KNOB -REAY AND ULTRASONIC 
0.2 : INTEGRATING THE POTENTIOMETER

THE PURPORSE OF RUN 1 IS ULTRASONIC SENSOR CALLIBRATION 
THE PURPOSE  OF RUN 2 IS TO INTEGRATE THE POTENTIOMETER
THE PURPOSE  OF RUN 3  IS  TO INTEGRATE THE  LCD DISPLAY TO THE FINAL PRODUCT*/ 



#include "TM1637.h" ; // 4 digits display include
#include "Ultrasonic.h" ; // ultrasonic sensro libraries 
#include "Math.h";
#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include "SPI.h";
#endif
#include "WiFi.h";

/*MACRO DEFINITIONS: 
 * digital IOs at first and  then analog IOs
Digital pins  inputs and  outputs*/
#define RELAY_PIN 40  // connecting the realy that will control the coffeE machine as digital output . 
#define CLK 39        // 4-digit clock pin 
#define DIO 38        // 4-DIGIT display data pin 
 
/*ANALOG pins we have 5  analog  pins going from 23 till 27 included  */
#define KNOB_PIN 24           // Knob pin used for the potentiometer to be used as an analog inpt 
#define ULTRASONIC_PIN 27     // ultrasonic sensor  (measuring water height) used as analog  pin at pin 24 
#define BUZZER_PIN 25         // Buzzer Pin

/* Global Variables */
TM1637 tm1637(CLK, DIO);                  /* 4-Digit Display object */
Ultrasonic ultrasonic(ULTRASONIC_PIN);    /* Ultrasonic Ranger object */
int distance = 0;                         /* variable to store the distance to obstacles in front */
int waterLevel=0;
int8_t bits[4] = {0};                     /* array to store the single bits of the value */
                  

// the setup routine runs once when you press reset:
void setup() {
         pinMode(RELAY_PIN, OUTPUT); // initialize the digital pin as an output.
     //Setting Up the serial for debug purposes
         Serial.begin(9600) ; 
    /* Initialize 4-Digit Display */
         tm1637.init();
         tm1637.set(BRIGHT_TYPICAL);
}

// the loop routine runs over and over again forever:
void loop() {

  distance = ultrasonic.MeasureInCentimeters();
  waterLevel=10-distance; //How much water is left
  if(waterLevel<0)
  {waterLevel=0;
  }

 if (waterLevel<=0)
 {
  analogWrite(BUZZER_PIN, GetVoltage());
  Serial.println("Buzzer On");
  Serial.println(waterLevel);
  }
  else
  {
    analogWrite(BUZZER_PIN,0);
    Serial.println("Buzzer Off");
    }
    
  memset(bits, 0, 4);                             /* reset array before we use it */

  int cupsLeft=CupsLeft(waterLevel); // displays the cups left to use. 
  
    for(int i = 3; i >= 0; i--) 
    {
        /* Convert the value to individual decimal digits for display */
        bits[i] = cupsLeft % 10;
        cupsLeft = cupsLeft / 10;  
        tm1637.display(i, bits[i]);                 /* display on 4-Digit Display */
    }
    Serial.println(distance);
    delay(1000);   // wait for a second

if(analogRead(KNOB_PIN)>2048)
  {
         digitalWrite(RELAY_PIN, HIGH); // turn the relay on (HIGH is the voltage level)
         
  }
   else { 
          digitalWrite(RELAY_PIN, LOW);   // turn the relay o by making the voltage LOW
         } 
}

int GetVoltage ()
{
  int knobValue=analogRead(KNOB_PIN);
  int voltage= knobValue * (240.0/4095.0); //has to be a value be value between 0 and 240
  Serial.print("knobValue: ");
  Serial.print(knobValue);
  Serial.print("\n");
  return voltage;
  
  }

 int CupsLeft(int waterLeft) // /Value Ranges betweeen 1 to 6;
 {
  int cupsLeft;
  
  if (waterLeft<=0){
  cupsLeft=0;
  
    }
    else
    {
  cupsLeft= round((waterLeft+2.0)/2.0);
    }
  return cupsLeft;
  }


