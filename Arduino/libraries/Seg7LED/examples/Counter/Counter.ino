/**
Author: Andrew Shepherd
Date: 19/11/11
 
Description:
  Counter to demostrate the Seg7LED board.  Counts -999 to 9999 repeatedly
  at approx 1/100th second per count.

Circuit:
  Arduino connected to Seg7LED board
**/

#include <Seg7LED.h>

const int pinLatch = 12;
const int pinRTS = 11;
const int pinData = 10;
const int pinCTS = 9;

Seg7LED LED( pinLatch, pinRTS, pinData, pinCTS );

int val = -999;

void setup() 
{
}

void loop() 
{
  LED.printInt( val );
 
  val++;
  if( val > 9999 )
    val = -999;
    
  delay(10);
}

