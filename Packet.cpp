

#include "Packet.h"



/**
 * Base class ctor.
 * 
 * Local packet ctor.  Should only be used for AT commands.
**/
XBeePacket::XBeePacket()
:	addrType( ADDR_LOCAL )
{
}



/**
 * Base class ctor.
**/
XBeePacket::XBeePacket( word _shortAddr )
:	addrType( ADDR_SHORT ),
	shortAddr( _shortAddr )
{
}


/**
 * Base class ctor.
**/
XBeePacket::XBeePacket( unsigned long _highAddr, unsigned long _lowAddr )
:	addrType( ADDR_LONG ),
	highAddr( _highAddr ),
	lowAddr( _lowAddr ),
	shortAddr( 0xFFFE )			// Needed for the remote AT command
{	
}



void XBeePacket::parseShortAddr( byte *p )
{
	shortAddr = *p<<8;
	p++;
	shortAddr |= *p;

	addrType = ADDR_SHORT;
}


void XBeePacket::parseLongAddr( byte *p )
{
	highAddr = *p<<24;
	p++;
	highAddr |= *p<<16;
	p++;
	highAddr |= *p<<8;
	p++;
	highAddr |= *p;
	
	lowAddr = *p<<24;
	p++;
	lowAddr |= *p<<16;
	p++;
	lowAddr |= *p<<8;
	p++;
	lowAddr |= *p;

	addrType = ADDR_LONG;
}
