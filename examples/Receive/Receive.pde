// Receive.pde


#include "DTM_XBee.h"

/*
 * This is an example of receiving a data packet from a remote XBee modem to an XBee modem
 * connected to this Arduino.
 * 
 * Assumptions:
 * 1) Both XBee modems are set to packet mode (AP=1)
 * 2) The XBee modem is connected to the hardware serial port on pins 1 and 2.
 * 3) A LED is connected to pin 13 (the default for a Uno board)
 * 4) There is a remote XBee modem sending packets to this one.
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
	// We will use the LED on pin 13 to indicate when we succeed in receiving a packet.
	pinMode( 13, OUTPUT );

	// We talk to the XBee modem on the hardware serial port at its default baud.
	Serial.begin( 9600 );
}


void loop()
{
	// Now we check for an incoming packet.  If no packet has been seen after we wait 200ms
	// the receive call will return false.
	if( xbee.receiveWait( &rcv, 200 ) )
	{
    	// It returned true, so we got an inbound packet

		// Is this packet a RX Data packet?
		if( rcv.type == XBeeReceivePacket::RX_DATA )
		{
			// This is a data packet.
			// Of course in a real program we'd do something with that data, but here
			// we just indicate that we received one.
			digitalWrite( 13, HIGH );  				
		}
	}

	delay( 1000 );

    digitalWrite( 13, LOW );  
}
