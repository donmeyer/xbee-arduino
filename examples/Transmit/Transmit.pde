// Transmit.pde


#include "DTM_XBee.h"



XBee xbee;


char msg[] = "Hello World";



void setup()
{
	pinMode(13, OUTPUT);
	xbee.begin( 57600 );
}



void loop()
{
	digitalWrite(13, HIGH);  

	XBeeTransmitPacket txpacket( 0x1234, &msg, sizeof(msg) );

	xbee.send( &txpacket );
	
	delay( 200 );

	digitalWrite(13, LOW);  

	delay( 800 );
}