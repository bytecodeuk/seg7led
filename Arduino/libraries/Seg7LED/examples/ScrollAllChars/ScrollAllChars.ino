/**
Author: Andrew Shepherd
Date: 11/02/12
 
Description:
  Scrolls Seg7LED through all the predefined chars.

Circuit:
  Arduino connected to Seg7LED board
**/

#include <Seg7LED.h>

const int pinLatch = 12;
const int pinRTS = 11;
const int pinData = 10;
const int pinCTS = 9;

Seg7LED LED( pinLatch, pinRTS, pinData, pinCTS );

void setup() 
{
}

char allChars[] = {
  CHAR_0, CHAR_1, CHAR_2, CHAR_3,       // 0
  CHAR_4, CHAR_5, CHAR_6, CHAR_7,       // 4
  CHAR_8, CHAR_9, CHAR_MINUS, CHAR_DP,  // 8
  
  CHAR_A, CHAR_b, CHAR_C, CHAR_d,       // 12
  CHAR_E, CHAR_F, CHAR_c, CHAR_e, 
  CHAR_g, CHAR_h, CHAR_I, CHAR_i, 
  CHAR_J, CHAR_L, CHAR_n, CHAR_o, 
  CHAR_O, CHAR_P, CHAR_q, CHAR_r, 
  CHAR_S, CHAR_t, CHAR_U, CHAR_u, 
  CHAR_X, CHAR_Y, CHAR_Z, CHAR_DP,
  
  CHAR_OPEN_BRACKET, CHAR_CLOSE_BRACKET, CHAR_UNDERSCORE, CHAR_EQUALS, 
  CHAR_ALL, 
  
  SEG_A, SEG_B, SEG_C, SEG_D,
  SEG_E, SEG_F, SEG_G, CHAR_DP,
  
  CHAR_BLANK      //  NB: CHAR_BLANK is also null (ie terminates string)  
};

const int delay_ms = 1000;
const int startPos = 0;

int bufPos = startPos;

void loop() 
{
char str[5];

    memset( str, 0, sizeof(str) );
    for( int i = 0; i < 4 && ((bufPos + i) < sizeof(allChars)); i++ )
    {
      str[i] = allChars[bufPos+i];
    }
    bufPos += 1;
    if( bufPos >= sizeof(allChars) )
    {
       bufPos = startPos; 
    }
    LED.sendChars( str );
    delay(delay_ms);
}
