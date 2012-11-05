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

bool Seg7LED::printNumericString( char * aStr )
{
	digitalWrite( m_pinLatch, LOW );

	print8BitChar( 'A' );

	int aStrLen = strlen( aStr );
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

bool Seg7LED::DPSetPos( int aPos )
{
	if( aPos > 3 || aPos < 0 )
	{
		return false;
	}
	
	bitSet( m_DPPos, aPos );
	return true;
}

bool Seg7LED::DPClearPos( int aPos )
{
	if( aPos > 3 || aPos < 0 )
	{
		return false;
	}

	bitClear( m_DPPos, aPos );
	return true;	
}

bool Seg7LED::DPClearAll()
{
	m_DPPos = 0;
	return true;
}

bool Seg7LED::printFloat( double f )
{
CLEDBuf LEDBuf;

	floatToLEDBuf( f, LEDBuf );
	return printNumericString( LEDBuf.getBuf() );
}

bool Seg7LED::floatToLEDBuf(double number, CLEDBuf & LEDBuf) 
{ 
	uint8_t digits = LEDBuf.getNumDigits();
	size_t n = 0;
	LEDBuf.clear();
  
	// Handle negative numbers
	if (number < 0.0)
	{
		n += LEDBuf.add('-');
		number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	for (uint8_t i=0; i<digits; ++i)
		rounding /= 10.0;

	number += rounding;

	// Extract the integer part of the number and print it
	unsigned long int_part = (unsigned long)number;
	double remainder = number - (double)int_part;
	n += LEDBuf.add(int_part);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0) 
	{
		n += LEDBuf.addDP();
	}

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		int toPrint = int(remainder);
		n += LEDBuf.add(toPrint);
		remainder -= toPrint; 
	} 

	return true;
}

bool Seg7LED::setting( char setting )
{
	digitalWrite( m_pinLatch, LOW );

	print8BitChar( 'S' );
	print8BitChar( setting );
		
	digitalWrite( m_pinLatch, HIGH );

	return true;	
}

//------------------

CLEDBuf::CLEDBuf()
{
	clear();
}

void CLEDBuf::clear()
{
	for(uint8_t x = 0; x < sizeof( buf ); x++ )
	{
		buf[x] = 0;
	}
		
	bufPos = 0;
	numChars = 0;
}

uint8_t CLEDBuf::addDP()
{
	if( bufPos < (sizeof( buf ) -1) )
	{
		buf[bufPos] = '.';
	}
	
	bufPos++;
	
	return 1;
}

uint8_t CLEDBuf::add( unsigned long l )
{
	char sBuf[18];
	int len = sprintf( sBuf,"%ld", l );
	int lenToCountDown = len;
	int pos = 0;
	
	while( lenToCountDown > 0 )
	{
		add( sBuf[pos++] );
		lenToCountDown--;
	}
	
	return len;
}

uint8_t CLEDBuf::add( int i )
{
	if( i < 10 && i >= 0)
	{
		char itoaBuf[2];
		itoa( i, itoaBuf, 10 );
		add( itoaBuf[0] );
	}
	
	return 1;
}

uint8_t CLEDBuf::add( char c )
{
	if( bufPos < (sizeof( buf ) -1) && numChars < CLEDBUF_NUMDIGITS )
	{
		buf[ bufPos ] = c;
	}
		
	bufPos++;
	numChars++;
	
	return 1;
}
