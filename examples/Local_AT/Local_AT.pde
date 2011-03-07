// Local_AT.pde


#include "DTM_XBee.h"



/*
 * This is an example of sending an AT command packet to the XBee modem
 * connected to this Arduino.
 * 
 * Assumptions:
 * 1) The XBee modems is set to packet mode (AP=1)
 * 2) The XBee modem is connected to the hardware serial port on pins 1 and 2.
 * 3) A LED is connected to pin 13 (the default for a Uno board)
 */


// This can use any hardware serial port (such as the extra ones on the Mega) or any other
// device that is descended from the Stream class.
XBee xbee( Serial );


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

	// Create a Local AT packet
	XBeeATCmdPacket cmd;
  
	// Set the command to "BD" which will return the modem's baud.
	cmd.set( 'B', 'D' );
	
	// Send it to the modem
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


	// Set the command to "MY" which will return the modem's short address.
	// Note that we re-use the same packet.
	cmd.set( "MY" );
	
	// Send it to the modem
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
}



void loop()
{
}

