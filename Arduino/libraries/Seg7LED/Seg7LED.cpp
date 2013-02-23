//
// File: Seg7LED.cpp
// Author: Andy Shepherd
// email: seg7led@bytecode.co.uk
// License: Public Domain
//

#include "Seg7LED.h"

Seg7LED::Seg7LED( int aPinLatch, int aPinRTS, int aPinData, int aPinCTS )
{
	// 
	m_pinLatch = aPinLatch;
	m_pinRTS = aPinRTS;
	m_pinData = aPinData;
	m_pinCTS = aPinCTS;

	pinMode( m_pinLatch, OUTPUT );
	pinMode( m_pinRTS, OUTPUT );
	pinMode( m_pinData, OUTPUT );
	pinMode( m_pinCTS, INPUT );
}

// Send aCh to Seg7LED display
// param aCh - char to send.
bool Seg7LED::print8BitChar( char aCh )
{
bool cts = false;

	digitalWrite( m_pinRTS, LOW );
	for ( int i = 0; i < 8; i++ )
	{
		while ( !cts )
		{
			cts = digitalRead( m_pinCTS );
		}
		
		digitalWrite( m_pinData, !!(aCh & (1 << i)) );
		digitalWrite( m_pinRTS, HIGH );

		while ( cts )
		{
			cts = digitalRead( m_pinCTS );
		}
		
		digitalWrite( m_pinRTS, LOW );		
	}

	return true;
}

#define BUFFERSIZE 20
bool Seg7LED::printInt( int aVal )
{
	if ( aVal > 9999 || aVal < -999 )
	{
		return false;
	}

	digitalWrite( m_pinLatch, LOW );

	print8BitChar( 'A' );

	char str[BUFFERSIZE];
	memset( str, 0, BUFFERSIZE );
	sprintf( str,"%04d", aVal );

	int len = strlen( str );
	if ( len < BUFFERSIZE )
	{
		for ( int charNum = 0; charNum < len; ++charNum )
		{
			print8BitChar( str[charNum] );
		}
	}

	digitalWrite( m_pinLatch, HIGH );

	return true;
}

#define BUFFERSIZE_READVALUE 10
bool Seg7LED::printNumericString( char * aStr )
{
	digitalWrite( m_pinLatch, LOW );

	print8BitChar( 'A' );

	int aStrLen = min(strlen( aStr ), BUFFERSIZE_READVALUE);
	for ( int i = 0;  i < aStrLen; i++ )
	{
		print8BitChar( aStr[i] );
	}
		
	digitalWrite( m_pinLatch, HIGH );

	return true;
}

//
//
// param aStr - Array of 4 LED chars
bool Seg7LED::print4LEDChars( char * aStr )
{
	digitalWrite( m_pinLatch, LOW );

	print8BitChar( 'A' );

	char buf[BUFFERSIZE];
	memset( buf, 0, BUFFERSIZE );

	for ( int i = 0;  i < 4; i++ )
	{
		int i2 = i * 2;
		buf[ i2 ] = '\\';
		buf[ i2 + 1] = aStr[i];
	}

	for ( int charNum = 0; charNum < 8; ++charNum )
	{
		print8BitChar( buf[charNum] );
	}
		
	digitalWrite( m_pinLatch, HIGH );

	return true;
}

bool Seg7LED::printFloat( double f )
{
CLEDBuf LEDBuf;

	floatToLEDBuf( f, LEDBuf.getNumDigits(), LEDBuf );
	return printNumericString( LEDBuf.getBuf() );
}

// This method is based on Arduino code: Print::printFloat() in Print.cpp
// Credit to: David A. Mellis
void Seg7LED::floatToLEDBuf(double number, uint8_t decimal_places,  CLEDBuf & LEDBuf ) 
{ 
	uint8_t digits = LEDBuf.getNumDigits();
	LEDBuf.clear();
  
	// Handle negative numbers
	if (number < 0.0)
	{
		LEDBuf.addChar('-');
		number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	for (uint8_t i=0; i<decimal_places; ++i)
  {
		rounding /= 10.0;
  }

	number += rounding;

	// Extract the integer part of the number and print it
	unsigned long int_part = (unsigned long)number;
	double remainder = number - (double)int_part;
	LEDBuf.addULong(int_part);

	// Print the decimal point, but only if there are digits beyond
	if (LEDBuf.getCurrentDigitPos() < digits) 
	{
		LEDBuf.addDP();
	}
  else
  {
    return;
  }

	// Extract digits from the remainder one at a time
	while (decimal_places-- > 0)
	{
		remainder *= 10.0;
		int toPrint = int(remainder);
		LEDBuf.addInt(toPrint);
		remainder -= toPrint; 
	} 
}

//------------------------------------------------------------------------
