//
// File: CLedBuf.cpp
// Author: Andy Shepherd
// email: seg7led@bytecode.co.uk
// License: Public Domain
//

#ifndef LEDBUF_H
#define LEDBUF_H

#include <arduino.h>

class CLEDBuf
{
#define CLEDBUF_NUMDIGITS 4
//#define CLEDBUF_BUFSIZE  ((CLEDBUF_NUMDIGITS * 2) + 1)
public:
	CLEDBuf();
	
	void clear();
	uint8_t addDP();
	uint8_t addInt( int i );
	uint8_t addULong( unsigned long l );
	uint8_t addChar( char c );
	bool setCharAt( uint8_t pos, char ch, bool dp );
	bool setDpAt( uint8_t pos, bool dp );
	void shiftLeft( char ch, bool dp);
	void shiftRight( char ch, bool dp);

	uint8_t getNumDigits();
	uint8_t getCurrentDigitPos();
	char * getBuf();
	
private:
//	char m_buf[ CLEDBUF_BUFSIZE ];
	char m_buf[ CLEDBUF_NUMDIGITS ];
	bool m_buf_dp[ CLEDBUF_NUMDIGITS ];
	char m_bufCombined[((CLEDBUF_NUMDIGITS * 2) + 1)];  
	uint8_t  m_bufPos;
};

#endif  // LEDBUF_H
