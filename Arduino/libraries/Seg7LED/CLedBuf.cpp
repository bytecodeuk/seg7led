//
// File: CLedBuf.cpp
// Author: Andy Shepherd
// email: seg7led@bytecode.co.uk
// License: Public Domain
//

#define __TEST__
#ifdef __TEST__
	#include <stdio.h>
	#include <stdlib.h>
#endif

#include "CLedBuf.h"

CLEDBuf::CLEDBuf()
{
	clear();
}

void CLEDBuf::clear()
{
	for(uint8_t x = 0; x < sizeof( m_buf ); x++ )
	{
		m_buf[x] = 0;
		m_buf_dp[x] = 0;
	}
		
	m_bufPos = 0;
}

/**
 * Add Decimal point to buffer
 */
uint8_t CLEDBuf::addDP()
{
	if( m_bufPos < sizeof( m_buf_dp ) )
	{
		m_buf_dp[m_bufPos - 1] = 1;
	}
	
//	m_bufPos++;
	
	return 1;
}

bool CLEDBuf::setCharAt( uint8_t pos, char ch, bool dp )
{
	if( pos >=0 && pos < getNumDigits() )
	{
		m_buf[pos] = ch;
		m_buf_dp[pos] = dp;
		return true;
	}
	return false;
}

bool CLEDBuf::setDpAt( uint8_t pos, bool dp )
{
	if( pos >=0 && pos < getNumDigits() )
	{
		m_buf_dp[pos] = dp;
		return true;
	}
	return false;
}

void CLEDBuf::shiftLeft( char ch, bool dp)
{
	for(int x = 0; x < sizeof(m_buf)-1; x++)
	{
		m_buf[x] = m_buf[x+1];
		m_buf_dp[x] = m_buf_dp[x+1];
	}

	setCharAt( sizeof(m_buf)-1, ch, dp );
}

void CLEDBuf::shiftRight( char ch, bool dp)
{
	for(int x = sizeof(m_buf)-1; x > 0 ; x--)
	{
		m_buf[x] = m_buf[x-1];
		m_buf_dp[x] = m_buf_dp[x-1];
	}

	setCharAt( 0, ch, dp );
}

/**
 * Add unsigned long to buffer
 * @param l number to be added
 * @return length added to buffer
 */
uint8_t CLEDBuf::addULong( unsigned long l )
{
	char sBuf[18];
	int len = sprintf( sBuf,"%ld", l );
	int lenToCountDown = len;
	int pos = 0;
	
	while( lenToCountDown > 0 )
	{
		addChar( sBuf[pos++] );
		lenToCountDown--;
	}
	
	return len;
}

/**
 * Add single digit int to buffer
 * @param i number to be added
 * @return length added to buffer.  Always 1 if successful else 0.
 */
uint8_t CLEDBuf::addInt( int i )
{
	if( i < 10 && i >= 0)
	{
		char itoaBuf[2];
		itoa( i, itoaBuf, 10 );
		addChar( itoaBuf[0] );
    return 1;
	}
  
	return 0;
}

/**
 * Add single char to buffer
 * @param c char to be added
 * @return length added to buffer.  Always 1 if successful else 0.
 */
uint8_t CLEDBuf::addChar( char c )
{
	if( m_bufPos < sizeof( m_buf ) )
	{
		m_buf[ m_bufPos ] = c;
		
    m_bufPos++;
    
    return 1;
  }
  
  return 0;
}

uint8_t CLEDBuf::getCurrentDigitPos()
{
  return m_bufPos;
}

uint8_t CLEDBuf::getNumDigits()
{ 
  return (uint8_t) CLEDBUF_NUMDIGITS; 
}

char * CLEDBuf::getBuf()
{ 
	uint8_t x;
	uint8_t buf_pos = 0;
  
	// Clear buf
	for(x = 0; x < sizeof( m_bufCombined ); x++)
	{
		m_bufCombined[x] = 0;
	}
  
	for(int x = 0; x < sizeof( m_buf ); x++)
	{
		m_bufCombined[buf_pos++] = m_buf[x];
		if( m_buf_dp[x] != 0 )
		{
			m_bufCombined[buf_pos++] = '.';
		}
	}
  
	return m_bufCombined; 
}

