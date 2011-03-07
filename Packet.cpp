

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
XBeePacket::XBeePacket( word shortAddr )
:	addrType( ADDR_SHORT ),
	_shortAddr( shortAddr )
{
}


/**
 * Base class ctor.
**/
XBeePacket::XBeePacket( unsigned long highAddr, unsigned long lowAddr )
:	addrType( ADDR_LONG ),
	_highAddr( highAddr ),
	_lowAddr( lowAddr ),
	_shortAddr( 0xFFFE )			// Needed for the remote AT command
{	
}



void XBeePacket::parseShortAddr( byte *p )
{
	_shortAddr = *p<<8;
	p++;
	_shortAddr |= *p;

	addrType = ADDR_SHORT;
}


void XBeePacket::parseLongAddr( byte *p )
{
	_highAddr = *p<<24;
	p++;
	_highAddr |= *p<<16;
	p++;
	_highAddr |= *p<<8;
	p++;
	_highAddr |= *p;
	
	_lowAddr = *p<<24;
	p++;
	_lowAddr |= *p<<16;
	p++;
	_lowAddr |= *p<<8;
	p++;
	_lowAddr |= *p;

	addrType = ADDR_LONG;
}
