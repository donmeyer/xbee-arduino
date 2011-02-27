// ReceivePacket.cpp


#include "DTM_Xbee.h"



// If called without frame buf???  Arduino has no malloc(), right?

XBeeReceivePacket::XBeeReceivePacket( byte *_frameBuf, size_t _frameBufLen )
:	frameBuf( _frameBuf ),
	frameBufLen( _frameBufLen )
{
	
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
