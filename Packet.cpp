

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


