#!/usr/bin/python

#
# (c) copyright Andrew Shepherd 2013
# File: seg7led-timer.py
# Author: Andy Shepherd
# Version: V1.00
# Date: 16/01/13
# Description: Python script to demonstrate the Seg7Led board on Raspberry Pi. This script counts from -999 to 9999
# in 1/100 of a second displaying the current count on the Seg7Led board. (timing is approximate)
#
# License: This source may be used for anything you like.  No warranty is given.
#
# Connect Rasberry Pi as defined in source below + GND and 5V.
# Note: These connections are for the Raspberry Pi (c)2011 (version made in China).  They may need
# to be different on other versions.
#
# To run: sudo python seg7led-timer.py
#

import RPi.GPIO as gpio
from time import sleep

m_pinCTS   = 22  # input
m_pinData  = 23  # output
m_pinRTS   = 24  # output
m_pinLatch = 25  # output

def setupGPIO():
        gpio.setmode(gpio.BCM);

        gpio.setup( m_pinCTS, gpio.IN );

        gpio.setup( m_pinData, gpio.OUT );
        gpio.setup( m_pinRTS, gpio.OUT );
        gpio.setup( m_pinLatch, gpio.OUT );

        digitalWrite( m_pinLatch, gpio.HIGH )
        digitalWrite( m_pinRTS, gpio.LOW );
        digitalWrite( m_pinData, gpio.LOW );

# Function to output a value to a GPIO pin (Arduino style)
def digitalWrite( pin, state ):
        gpio.output( pin, state )

# Function to read a value from a GPIO pin (Arduino style)
def digitalRead( pin ):
        return gpio.input( pin )

# Function to get a 'bit' value from s 'char'.
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
                # Wait for CTS to go high (Seg7 is ready for next bit)
                while cts == False:
                        cts = digitalRead( m_pinCTS );

                # Send data bit
                digitalWrite( m_pinData, getBitFromChar( i, aCh) );

                # Set RTS HIGH (Tell Seg7 that bit it set)
                digitalWrite( m_pinRTS, gpio.HIGH );

                # Wait for CTS to go LOW (Seg7 has received bit)
                while cts == True:
                        cts = digitalRead( m_pinCTS );

                # Set data to low (Not essential but makes decoding on a logic analyser easier
                digitalWrite( m_pinData, gpio.LOW );

                # Set RTS to low until next data bit is ready
                digitalWrite( m_pinRTS, gpio.LOW );

        return True;

# Function to send an int to Seg7Led board
# - Converts int to string and send it char by char.
# - String is preceded with an 'A' char which tells the 
# Seg7Led board to expect an ASCII string.
def sendInt( iCount ):

        # Convert pass int to 4 digit string
        sCount = "%04d" % (iCount);

        # Open latch
        digitalWrite( m_pinLatch, gpio.LOW )        
        
        # Send 'A' to specify ASCII string being sent
        print8BitChar( 'A' )

        # Loop through the 4 digits
        for a in range(0,4):
                # Get char at current loop position
                c = sCount[a];
                # Send char
                print8BitChar( c );

        # Close latch
        digitalWrite( m_pinLatch, gpio.HIGH )


## Entry point

setupGPIO();

try:
  # Count from -999 to 9999 until CTRL-C is passed to escape
	while True:
        	count = -999;
	        while count < 10000:
        	        sendInt( count );
                	count += 1;
               	 	sleep(0.01);

except:
	print "Caught CTRL-C..";

print "Cleaning up GPIO...";
gpio.cleanup()
print "Exited";
