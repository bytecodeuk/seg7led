#!/usr/bin/python

import RPi.GPIO as gpio
from time import sleep

m_pinCTS   = 22  # input
m_pinData  = 23  # output
m_pinRTS   = 24  # output
m_pinLatch = 25  # output

def setupGPIO():
        gpio.setmode(gpio.BCM);

        gpio.setup( m_pinCTS, gpio.IN, pull_up_down=gpio.PUD_DOWN );
        
        gpio.setup( m_pinData, gpio.OUT );
        gpio.setup( m_pinRTS, gpio.OUT );
        gpio.setup( m_pinLatch, gpio.OUT );

def digitalWrite( pin, state ):
        gpio.output( pin, state )
        
def digitalRead( pin ):
        return gpio.input( pin )

def getBitFromChar( bit, char ):
        retVal = gpio.LOW
        if (ord(char) & (1 << bit)) != 0:
                retVal = gpio.HIGH
        return retVal

dm = 0.01;
        
# Send aCh to Seg7LED display
# param aCh - char to send.
def print8BitChar( aCh ):

        cts = False;

        digitalWrite( m_pinRTS, gpio.LOW );
#        sleep( dm );
        for  i in range( 0, 8 ):
                while cts == False:
                        cts = digitalRead( m_pinCTS );

#                sleep( dm );
                digitalWrite( m_pinData, getBitFromChar( i, aCh) );
#                sleep( dm );
                digitalWrite( m_pinRTS, gpio.HIGH );
#                sleep(dm );

                while cts == True:
                        cts = digitalRead( m_pinCTS );

#                sleep( dm );
                digitalWrite( m_pinRTS, gpio.LOW );
#                sleep( dm );
                
        return True;

def sendCount( iCount ):
        digitalWrite( m_pinLatch, gpio.LOW )
        sCount = "%04d" % (iCount);

        print8BitChar( 'A' )
        for a in range(0,4):
                c = sCount[a];
                print8BitChar( c );
                
        digitalWrite( m_pinLatch, gpio.HIGH )
        

## Entry point 

setupGPIO();

try:

	while True:
        	count = -999;
	        while count < 10000:
        	        sendCount( count );
                	count += 1;
               	 	sleep(0.1);

except:
	print "Caught CTRL-C..";

print "Cleaning up GPIO...";
gpio.cleanup()
print "Exited";
