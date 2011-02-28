// ReceivePacket.cpp


#include "DTM_Xbee.h"



// If called without frame buf???  Arduino has no malloc(), right?

XBeeReceivePacket::XBeeReceivePacket( byte *_frameBuf, size_t _frameBufLen )
:	XBeePacket(),
	frameBuf( _frameBuf ),
	frameBufLen( _frameBufLen )
{
}


XBeeReceivePacket::PacketType XBeeReceivePacket::getType() const
{
	switch( apiID )
	{
		case XBee::API_STATUS:			return STATUS;			break;
		case XBee::API_AT_RESP:			return AT_RESP;			break;
		case XBee::API_AT_RESP_REMOTE:	return AT_RESP_REMOTE;	break;
		case XBee::API_TX_STATUS:		return TX_STATUS;		break;
		case XBee::API_RX_16:			return RX_DATA;			break;
		case XBee::API_RX_64:			return RX_DATA;			break;
		
		default: 						return INVALID; 		break;
	}
}


byte XBeeReceivePacket::getFrameID() const
{
	return frameBuf[0];
}


byte XBeeReceivePacket::getStatus() const
{
	return frameBuf[1];
}



#if 0
/**
 * 
**/
const void *XBeeReceivePacket::getPayload()
{
	return NULL;
}


size_t getPayloadLen()
{
	return 0;
}

bool isShortAddress()
{
	return true;
}

bool isLongAddress()
{
	return true;
}
#endif
