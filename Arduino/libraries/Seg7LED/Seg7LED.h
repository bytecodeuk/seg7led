//  

#ifndef SEG7LED_H
#define SEG7LED_H

#include <arduino.h>

#define CHAR_0 ((byte) 0b1111110)
#define CHAR_1 ((byte) 0b0110000)
#define CHAR_2 ((byte) 0b1101101)
#define CHAR_3 ((byte) 0b1111001)
#define CHAR_4 ((byte) 0b0110011)
#define CHAR_5 ((byte) 0b1011011)
#define CHAR_6 ((byte) 0b1011111)
#define CHAR_7 ((byte) 0b1110000)
#define CHAR_8 ((byte) 0b1111111)
#define CHAR_9 ((byte) 0b1111011)

#define CHAR_A ((byte) 0b1110111)
#define CHAR_b ((byte) 0b0011111)
#define CHAR_C ((byte) 0b1001110)
#define CHAR_d ((byte) 0b0111101)
#define CHAR_E ((byte) 0b1001111)
#define CHAR_F ((byte) 0b1000111)

#define CHAR_c ((byte) 0b0001101)
#define CHAR_e ((byte) 0b1101111)
#define CHAR_g ((byte) 0b1111011)
#define CHAR_h ((byte) 0b0010111)
#define CHAR_I ((byte) 0b0110000)
#define CHAR_i ((byte) 0b0010000)
#define CHAR_J ((byte) 0b0111000)
#define CHAR_L ((byte) 0b0001110)
#define CHAR_n ((byte) 0b0010101)
#define CHAR_o ((byte) 0b0011101)
#define CHAR_O ((byte) 0b1111110)
#define CHAR_P ((byte) 0b1100111)
#define CHAR_q ((byte) 0b1110011)
#define CHAR_r ((byte) 0b0000101)
#define CHAR_S ((byte) 0b1011011)
#define CHAR_t ((byte) 0b0001111)
#define CHAR_U ((byte) 0b0111110)
#define CHAR_u ((byte) 0b0011100)
#define CHAR_X ((byte) 0b0110111)
#define CHAR_Y ((byte) 0b0111011)
#define CHAR_Z ((byte) 0b1101101)

#define CHAR_MINUS  ((byte) 0b0000001)
#define CHAR_OPEN_BRACKET  ((byte) 0b1001110)
#define CHAR_CLOSE_BRACKET ((byte) 0b1111000)
#define CHAR_DP  ((byte) 0b10000000)
#define CHAR_BLANK ((byte) 0b00000000)
#define CHAR_UNDERSCORE ((byte) 0b0001000)
#define CHAR_EQUALS ((byte) 0b0001001)
#define CHAR_ALL ((byte) 0b11111111)

#define SEG_A  ((byte) 0b1000000)
#define SEG_B  ((byte) 0b0100000)
#define SEG_C  ((byte) 0b0010000)
#define SEG_D  ((byte) 0b0001000)
#define SEG_E  ((byte) 0b0000100)
#define SEG_F  ((byte) 0b0000010)
#define SEG_G  ((byte) 0b0000001)


class CLEDBuf
{
#define CLEDBUF_NUMDIGITS 4
#define CLEDBUF_BUFSIZE  ((CLEDBUF_NUMDIGITS * 2) + 1)
public:
	CLEDBuf();
	
	uint8_t addDP();
	uint8_t add( int i );
	uint8_t add( unsigned long l );
	uint8_t add( char c );
	void clear();
	uint8_t getNumDigits() { return (uint8_t) CLEDBUF_NUMDIGITS; }
	char * getBuf() { return buf; }
	
private:
	char buf[ CLEDBUF_BUFSIZE ];
	int bufPos;
	int numChars;
};


class Seg7LED
{
public:
	Seg7LED( int aPinLatch, int aPinRTS, int aPinData, int aPinCTS );

	bool printFloat( double f );
	bool printInt( int aVal );
	bool print4LEDChars( char * aStr );
	bool printNumericString( char * aStr );
	
	bool setting( char setting );
	
	bool DPSetPos( int aPos );
	bool DPClearPos( int aPos );
	bool DPClearAll();
	
private:
	bool print8BitChar( char aCh );
	bool floatToLEDBuf(double number, CLEDBuf & LEDBuf);

	int m_pinLatch;
	int m_pinRTS;
	int m_pinData;
	int m_pinCTS;
	int m_DPPos;
};

#endif // SEG7LED_H
