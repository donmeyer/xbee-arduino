// TransmitPacket.cpp
// Copyright (c) 2011, Donald T. Meyer
// All rights reserved.
// This source code is licensed under the BSD 2-Clause License, full text available in license.txt.


#include "DTM_XBee.h"


/**
 * Base class ctor.
 * 
 * Local packet ctor.  Should only be used for AT commands.
**/
XBeeOutboundPacket::XBeeOutboundPacket()
:	XBeePacket(),
	frameID( DEFAULT_FRAME_ID )
{
}



/**
 * Base class ctor.
**/
XBeeOutboundPacket::XBeeOutboundPacket( word _shortAddr )
:	XBeePacket( _shortAddr ),
	frameID( DEFAULT_FRAME_ID )
{
}


/**
 * Base class ctor.
**/
XBeeOutboundPacket::XBeeOutboundPacket( unsigned long _highAddr, unsigned long _lowAddr )
:	XBeePacket( _highAddr, _lowAddr ),
	frameID( DEFAULT_FRAME_ID )
{	
}



/**
 * If the frame ID isn't zero, increment it.  (if zero, we assume the user set it to zero to inhibit reponses)
 * This deals with wrapping and ensures that we go from 255 to 1 (skipping zero).
**/
void XBeeOutboundPacket::bumpID()
{
	if( frameID != 0 )
	{
		frameID++;
		if( frameID == 0 )
		{
			frameID == 1;
		}
	}
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------


/**
 * Ctor for an outbound data packet using a short (16-bit) destination address.
**/
XBeeTransmitPacket::XBeeTransmitPacket( word _shortAddr, const void *_payload, size_t _payloadSize )
:	XBeeOutboundPacket( _shortAddr ),
	payload( (const byte *)_payload ),
	payloadSize( _payloadSize ),
	disableAck( false ),
	broadcastPAN( false )
{
}


/**
 * Ctor for an outbound data packet using a long (64-bit) destination address.
**/
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

/**
 * This will create a local AT command packet.
**/
XBeeATCmdPacket::XBeeATCmdPacket()
:	XBeeOutboundPacket(),
	applyChanges( true ),
	cmd1( ' ' ),
	cmd2( ' ' ),
	strArg( NULL )
{
}


/**
 * This will create a remote AT command packet using the short address of the receiver.
**/
XBeeATCmdPacket::XBeeATCmdPacket( word _shortAddr )
:	XBeeOutboundPacket( _shortAddr ),
	applyChanges( true ),
	cmd1( ' ' ),
	cmd2( ' ' ),
	strArg( NULL )
{
}


/**
 * This will create a remote AT command packet using the long address of the receiver.
**/
XBeeATCmdPacket::XBeeATCmdPacket( unsigned long _highAddr, unsigned long _lowAddr )
:	XBeeOutboundPacket( _highAddr, _lowAddr ),
	applyChanges( true ),
	cmd1( ' ' ),
	cmd2( ' ' ),
	strArg( NULL )
{
}


/**
 * Set the command type and data based on discrete command characters and an optional string argument.
 * This will increment the packet ID (unless it is zero)
**/
void XBeeATCmdPacket::set( char _cmd1, char _cmd2, const char *arg )
{
	cmd1 = _cmd1;
	cmd2 = _cmd2;
	strArg = arg;
	
	bumpID();
}


/**
 * Set the command type and data from a string starting with the two character command code and a subsequent optional argument.
 * 
 * This will increment the packet ID (unless it is zero)
**/
void XBeeATCmdPacket::set( const char *cmd )
{
	//if( strlen(cmd) >= 2 )
	if( cmd[0] && cmd[1] )		// 6 bytes shorter than the strlen() way
	{
		cmd1 = cmd[0];
		cmd2 = cmd[1];
		if( cmd[2] )
		{
			strArg = cmd + 2;	// Pint at the argument part of the string.
		}
		else
		{
			strArg = NULL;
		}
	}
	else
	{
		cmd1 = ' ';
		cmd2 = ' ';
		strArg = NULL;			
	}

	bumpID();
}


#if 0
// Implementing this requires a 3rd-pary library?

/**
 * Set the comand type and data.
 * This will increment the packet ID (unless it is zero)
**/
void XBeeATCmdPacket::set( char _cmd1, char _cmd2, unsigned long arg )
{
	cmd1 = _cmd1;
	cmd2 = _cmd2;

	
	bumpID();
}
#endif
