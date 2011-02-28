// DTM_XBee.cpp


#include "DTM_Xbee.h"



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


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------


XBee::XBee()
:	complete( false ),
	state( S_EMPTY )
{	
}



void XBee::begin( unsigned long baud )
{
	Serial.begin( baud );
}


bool clientAvailable();
int clientRead();
void debug( const char *name, byte b );
void debug( const char *name, int x );

/**
 * This returns true if a message packet has been completely received and is ready to be processed.  In this case you MUST handle the message
 * before calling this method again, as the next call will discard the current packet's data and start building a new one.
 * If the timeout is zero, this will return immediately if no incoming characters are available at the XBee serial port.
**/ 
bool XBee::receiveWait( XBeeReceivePacket *packet, int timeout )
{
	if( state == S_COMPLETE )
	{
		state = S_EMPTY;
	}
	
	while( clientAvailable() )
	{
		byte b = clientRead();
		
		debug( "Packet char", b );
		
		if( state == S_EMPTY )
		{
			// Looking for the start delimiter
			if( b == START_DELIMITER )
			{
				state = S_GOT_START;
			}
		}
		else if( state == S_GOT_START )
		{			
			inboundLen = b<<8;
			state = S_GOT_HI_LEN;
		}
		else if( state == S_GOT_HI_LEN )
		{
			inboundLen |= b;
			debug( "Inbund len", inboundLen );
			state = S_GOT_LO_LEN;
		}
		else if( state == S_GOT_LO_LEN )
		{
			packet->frameID = b;
			inboundCsum = b;
			inboundCount = 0;
			needCount = inboundLen - 1;
			state = S_GOT_API;
		}
		else if( state == S_GOT_API )
		{
			// Now we just accumulate characters into the frame buffer
			debug( "Need", needCount );
			if( needCount-- )
			{
				inboundCsum += b;
				packet->frameBuf[inboundCount++] = b;
			}
			else
			{
				// Don't need any more, so this is the checksum!
				debug( "Got csum", b );
				debug( "Calc csum", inboundCsum );
				debug( "Tot csum", (byte)(inboundCsum + b) );
				state = S_COMPLETE;
				return true;
			}
		}
	}
	
	return false;
}



void XBee::send( const XBeeTransmitPacket *packet )
{
	word len = packet->getPayloadSize() + 3;		// payload + API ID + Frame ID + Options
	len += packet->isShortAddr() ? 2 : 8;
		
	// Start
	emit( START_DELIMITER );
	
	// Length
	emit( len>>8 );
	emit( len & 0x00FF );
	
	// --- Checksum starts here ---
	outboundCsum = 0;
	
	// API ID
	emit( packet->isShortAddr() ? API_TX_16 : API_TX_64 );

	// Frame ID
	emit( packet->frameID );
	
	// Address
	if( packet->isShortAddr() )
	{
		emitShortAddr( packet );
	}
	else
	{
		emitLongAddr( packet );
	}

	// Option byte
	byte options = 0x00;
	if( packet->disableAck )
	{
		options |= 0x01;
	}
	if( packet->broadcastPAN )
	{
		options |= 0x04;
	}	
	emit( options );
	
	len = packet->getPayloadSize();
	const byte *p = packet->getPayload();
	while( len-- )
	{
		emit( *p++ );
	}
	
	emit( 0xFF - outboundCsum );

#ifdef DEBUG	
	Serial.println( "---" );
#endif
}



void XBee::sendAT( const XBeeATCmdPacket *packet )
{
	word len = 4;		// API ID + Frame ID + Cmd Bytes
	
	if( ! packet->isLocalAddr() )
	{
		len += 11;	// 64-bit addr + 16-bit addr + option
	}
		
	const char *arg = packet->getArg();
	if( arg )
	{
		len += strlen(arg);
	}
	
	// Start
	emit( START_DELIMITER );
	
	// Length
	emit( len>>8 );
	emit( len & 0x00FF );
	
	// --- Checksum starts here ---
	outboundCsum = 0;
	
	
	// API ID
	if( packet->isLocalAddr() )
	{
		emit( packet->applyChanges ? API_AT_CMD : API_AT_CMD_QUEUED );
	}
	else
	{
		emit( API_AT_CMD_REMOTE );		
	}

	// Frame ID
	emit( packet->frameID );
	
	// Address (if we are remote)
	if( ! packet->isLocalAddr() )
	{
		// Address
		emitLongAddr( packet );
		emitShortAddr( packet );

		// Option byte
		byte options = 0x00;
		if( packet->applyChanges )
		{
			options |= 0x02;
		}
		emit( options );
	}

	emit( packet->getCmd1() );
	emit( packet->getCmd2() );
	
	if( arg )
	{
		while( *arg )
		{
			emit( *arg++ );
		}
	}
	
	emit( 0xFF - outboundCsum );

#ifdef DEBUG	
	Serial.println( "---" );
#endif
}



void XBee::emitShortAddr( const XBeeOutboundPacket *packet )
{
	word addr = packet->getShortAddr();
	emit( addr>>8 );
	emit( addr & 0x00FF );
}


void XBee::emitLongAddr( const XBeeOutboundPacket *packet )
{
	unsigned long addr;
	
	addr = packet->getHighAddr();
	emit( addr>>24 );
	emit( addr>>16 );
	emit( addr>>8 );
	emit( addr & 0x000000FF );	

	addr = packet->getLowAddr();
	emit( addr>>24 );
	emit( addr>>16 );
	emit( addr>>8 );
	emit( addr & 0x000000FF );	
}

void clientWrite( byte b );

void XBee::emit( byte b )
{
#ifdef DEBUG
	Serial.print( b, HEX );
	Serial.print( " " );
#else
	//Serial.write( b );
	clientWrite( b );
#endif
	outboundCsum += b;	
}
