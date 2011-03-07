// ReceivePacket.cpp


#include "DTM_Xbee.h"



XBeeReceivePacket::XBeeReceivePacket( byte *_frameBuf, size_t _frameBufLen )
:	XBeePacket(),
	frameBuf( _frameBuf ),
	frameBufLen( _frameBufLen )
{
}


void XBeeReceivePacket::reset()
{
	addrType = ADDR_LOCAL;
	payloadSize = 0;
	checksumOK = false;
	overflow = 0;
}


/**
 * Called by the XBee class's receive method to fill in any cached fields in the receive packet.
 * This is called after the packet has been completely received.
**/
void XBeeReceivePacket::populateFields()
{
	// Not valid for all packets, but this makes the code smaller and gives invalid data for the types it
	// is invalid for.
	frameID = frameBuf[0];
	
	switch( apiID )
	{
		case XBee::API_STATUS:	
			type = STATUS;			
			status = frameBuf[0];
			break;
		
		case XBee::API_AT_RESP:
			type = AT_RESP;
			cmd[0] = frameBuf[1];
			cmd[1] = frameBuf[2];
			status = frameBuf[3];
			payloadSize -= 4;
			payloadPtr = &frameBuf[4];
			break;
		
		case XBee::API_AT_RESP_REMOTE:
			type = AT_RESP;
			parseLongAddr( &frameBuf[1] );		
			parseShortAddr( &frameBuf[9] );
			cmd[0] = frameBuf[11];
			cmd[1] = frameBuf[12];
			status = frameBuf[13];
			payloadSize -= 14;
			payloadPtr = &frameBuf[14];
			break;
		
		case XBee::API_TX_STATUS:		
			type = TX_STATUS;
			status = frameBuf[1];
			break;
	
		case XBee::API_RX_16:
			type = RX_DATA;	
			parseShortAddr( &frameBuf[0] );
			rssi = -(frameBuf[2]);			
			optionsmap = frameBuf[3];
			payloadSize -= 4;
			payloadPtr = &frameBuf[4];
			break;

		case XBee::API_RX_64:
			type = RX_DATA;	
			parseLongAddr( &frameBuf[0] );		
			rssi = -(frameBuf[8]);			
			optionsmap = frameBuf[9];		
			payloadSize -= 10;
			payloadPtr = &frameBuf[10];
			break;
		
		default:
		 	type = INVALID;
	 		break;
	}
}
