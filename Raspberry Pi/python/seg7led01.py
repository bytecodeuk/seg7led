#!/usr/bin/python

import RPi.GPIO as gpio

m_pinCTS   = 22  # input
m_pinData  = 23  # output
m_pinRTS   = 24  # output
m_pinLatch = 25  # output

gpio.setmode(gpio.BCM)

gpio.setup( m_pinCTS, gpio.IN )
gpio.setup( m_pinData, gpio.OUT )
gpio.setup( m_pinRTS, gpio.OUT )
gpio.setup( m_pinLatch, gpio.OUT )


def digitalWrite( pin, state ):
        gpio.output( pin, state )
        
def digitalRead( pin ):
        return gpio.input( pin )

def getBitFromChar( bit, char ):
        retVal = gpio.LOW
        if (ord(char) & (1 << bit)) != 0:
                retVal = gpio.HIGH
        return retVal
        
# Send aCh to Seg7LED display
# param aCh - char to send.
def print8BitChar( aCh ):

        cts = False;

        digitalWrite( m_pinRTS, gpio.LOW );
        for  i in range( 0, 8 ):
                while cts == False:
                        cts = digitalRead( m_pinCTS );
                
                digitalWrite( m_pinData, getBitFromChar( i, aCh) );
                digitalWrite( m_pinRTS, gpio.HIGH );

                while cts == True:
                        cts = digitalRead( m_pinCTS );
                
                digitalWrite( m_pinRTS, gpio.LOW );             

        return True;
 

## Entry point 

digitalWrite( m_pinLatch, gpio.LOW )
print8BitChar( 'A' )
print8BitChar( '1' )
print8BitChar( '2' )
print8BitChar( '3' )
print8BitChar( '4' )
digitalWrite( m_pinLatch, gpio.HIGH )

gpio.cleanup()

