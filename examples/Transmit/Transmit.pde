// Transmit.pde
// Copyright (c) 2011, Donald T. Meyer
// All rights reserved.
// This source code is licensed under the BSD 2-Clause License, full text available in license.txt.


#include "DTM_XBee.h"


/*
 * This is an example of transmitting a data packet to a remote XBee modem from an XBee modem
 * connected to this Arduino.
 * 
 * Assumptions:
 * 1) Both XBee modems are set to packet mode (AP=1)
 * 2) The XBee modem is connected to the hardware serial port on pins 1 and 2.
 * 3) A LED is connected to pin 13 (the default for a Uno board)
 * 4) There is a remote XBee modem for this one to talk to, and either it's address has been
 * set to 0x2425 or that setting in this sketch has been changed to match its setting.
 */


// This can use any hardware serial port (such as the extra ones on the Mega) or any other
// device that is descended from the Stream class.
XBee xbee( Serial );


// This is the short address of the remote XBee modem we plan to talk to.
//
// *** Change this to match the address of your specific remote modem (the factory default is 0) ***
//
word remoteShortAddr = 0x2425;


int txPauseCount;


// Allocate a buffer to hold the bytes we receive from our modem.
// This can be smaller if you know that none of the data you plan to receive
// needs this much room.
byte buf[110];

// Now create the packet object.  It uses the buffer we created for storage.
XBeeReceivePacket rcv( buf, sizeof(buf) );



void setup()
{
	// We will use the LED on pin 13 to indicate when we succeed in transmitting a packet.
	pinMode( 13, OUTPUT );

	// We talk to the XBee modem on the hardware serial port at its default baud.
	Serial.begin( 9600 );
}



void loop()
{
	if( ++txPauseCount >= 3 )
	{
		// Every third time that loop() is called, we send a packet to the other XBee modem
		char msg[] = "Hello World";
		XBeeTransmitPacket txpacket( remoteShortAddr, &msg, sizeof(msg) );		
		xbee.send( &txpacket );
		
		txPauseCount = 0;
	}

	// Now we check for an incoming packet.  If no packet has been seen after we wait 200ms
	// the receive call will return false.
	if( xbee.receiveWait( &rcv, 200 ) )
	{
    	// It returned true, so we got an inbound packet

		// Is this packet a TX status packet?
		if( rcv.type == XBeeReceivePacket::TX_STATUS )
		{
			// This is the status for our last transmit attempt.
			// Check to see if our transmit attempt was successful.
			if( rcv.status == 0 )
			{
				// Status was zero, so that means we transmitted successfully.
				digitalWrite(13, HIGH);  				
			}
		}
	}

	delay( 1000 );

    digitalWrite( 13, LOW );  
}
