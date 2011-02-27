// TransmitPacket.cpp


#include "DTM_XBee.h"


/**
 * Local packet ctor.  Should only be used for AT commands.
**/
XBeeOutboundPacket::XBeeOutboundPacket()
:	frameID( DEFAULT_FRAME_ID ),
	addrType( ADDR_LOCAL )
{
}



XBeeOutboundPacket::XBeeOutboundPacket( word _shortAddr )
:	frameID( DEFAULT_FRAME_ID ),
	addrType( ADDR_SHORT ),
	shortAddr( _shortAddr )
{
}


XBeeOutboundPacket::XBeeOutboundPacket( unsigned long _highAddr, unsigned long _lowAddr )
:	frameID( DEFAULT_FRAME_ID ),
	addrType( ADDR_LONG ),
	highAddr( _highAddr ),
	lowAddr( _lowAddr ),
	shortAddr( 0xFFFE )			// Needed for the remote AT command
{	
}



//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------



XBeeTransmitPacket::XBeeTransmitPacket( word _shortAddr, const void *_payload, size_t _payloadSize )
:	XBeeOutboundPacket( _shortAddr ),
	payload( (const byte *)_payload ),
	payloadSize( _payloadSize ),
	disableAck( false ),
	broadcastPAN( false )
{
}


XBeeTransmitPacket::XBeeTransmitPacket( unsigned long _highAddr, unsigned long _lowAddr, const void *_payload, size_t _payloadSize )
:	XBeeOutboundPacket( _highAddr, _lowAddr ),
	payload( (const byte *)_payload ),
	payloadSize( _payloadSize ),
	disableAck( false ),
	broadcastPAN( false )
{	
}



//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------


XBeeATCmdPacket::XBeeATCmdPacket()
:	XBeeOutboundPacket(),
	applyChanges( false ),
	cmd1( ' ' ),
	cmd2( ' ' ),
	strArg( NULL )
{
}


XBeeATCmdPacket::XBeeATCmdPacket( word _shortAddr )
:	XBeeOutboundPacket( _shortAddr ),
	applyChanges( false ),
	cmd1( ' ' ),
	cmd2( ' ' ),
	strArg( NULL )
{
}


XBeeATCmdPacket::XBeeATCmdPacket( unsigned long _highAddr, unsigned long _lowAddr )
:	XBeeOutboundPacket( _highAddr, _lowAddr ),
	applyChanges( false ),
	cmd1( ' ' ),
	cmd2( ' ' ),
	strArg( NULL )
{
}



void XBeeATCmdPacket::set( char _cmd1, char _cmd2, const char *arg )
{
	cmd1 = _cmd1;
	cmd2 = _cmd2;
	strArg = arg;
}


void XBeeATCmdPacket::set( char _cmd1, char _cmd2 )
{
	cmd1 = _cmd1;
	cmd2 = _cmd2;
	strArg = NULL;	
}


void XBeeATCmdPacket::set( const char *cmd )
{
	if( strlen(cmd) >= 2 )
	{
		cmd1 = cmd[0];
		cmd2 = cmd[1];
		strArg = cmd+2;			
	}
	else
	{
		cmd1 = ' ';
		cmd2 = ' ';
		strArg = NULL;			
	}
}


#if 0
// Implementing this requires a 3rd-pary library?

void XBeeATCmdPacket::set( char _cmd1, char _cmd2, unsigned long arg )
{
	cmd1 = _cmd1;
	cmd2 = _cmd2;

	
}
#endif
