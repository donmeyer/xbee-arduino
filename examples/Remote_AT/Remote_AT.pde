// Remote_AT.pde


#include "DTM_XBee.h"



/*
 * This is an example of transmitting an AT command packet to a remote XBee modem from an XBee modem
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


// Allocate a buffer to hold the bytes we receive from our modem.
// This can be smaller if you know that none of the data you plan to receive
// needs this much room.
byte buf[110];

// Now create the packet object.  It uses the buffer we created for storage.
XBeeReceivePacket rcv( buf, sizeof(buf) );



void setup()
{
	// We will use the LED on pin 13 to indicate when we succeed in transmitting a packet.
	pinMode(13, OUTPUT);

	// We talk to the XBee modem on the hardware serial port at its default baud.
	Serial.begin( 9600 );


	// Create a Remote command packet using a 16-bit address
	XBeeATCmdPacket cmd( remoteShortAddr );


	// Set the command to "BD" which will return the remote modem's baud.
	cmd.set( "BD" );
	xbee.sendAT( &cmd );

	// Now we check for an incoming packet.  If no packet has been seen after we wait 500ms
	// the receive call will return false.
	if( xbee.receiveWait( &rcv, 500 ) )
	{
    	// It returned true, so we got a packet from the mode,

		if( rcv.type == XBeeReceivePacket::AT_RESP )
		{
			// This is the status for our AT cmd.
			if( rcv.status == 0 )
			{
				// Status was zero, so that means the command was ok.
				digitalWrite(13, HIGH);  				
			}
		}
	}

	delay( 1000 );

    digitalWrite(13, LOW);  

	delay( 2000 );


	// Set the command to "MY" which will return the remote modem's short address.
	// Note that we re-use the same packet.
	cmd.set( 'M', 'Y' );
	xbee.sendAT( &cmd );

	// Now we check for an incoming packet.  If no packet has been seen after we wait 500ms
	// the receive call will return false.
	if( xbee.receiveWait( &rcv, 500 ) )
	{
    	// It returned true, so we got a packet from the mode,

		if( rcv.type == XBeeReceivePacket::AT_RESP )
		{
			// This is the status for our AT cmd.
			if( rcv.status == 0 )
			{
				// Status was zero, so that means the command was ok.
				digitalWrite(13, HIGH);  				
			}
		}
	}

	delay( 1000 );

    digitalWrite(13, LOW);  
}



void loop()
{
}

