
#ifndef DTM_XBEE_PACKET_H
#define DTM_XBEE_PACKET_H


#include "WProgram.h"



/**
 * Virtual base class for all packets
**/
class XBeePacket {
public:
	bool isLocalAddr() const { return addrType == ADDR_LOCAL; }
	bool isShortAddr() const { return addrType == ADDR_SHORT; }
	bool isLongAddr() const { return addrType == ADDR_LONG; }
	
	word getShortAddr() const { return shortAddr; }
	unsigned long getHighAddr() const { return highAddr; }
	unsigned long getLowAddr() const { return lowAddr; }

protected:
	XBeePacket();
	XBeePacket( word _shortAddr );
	XBeePacket( unsigned long _highAddr, unsigned long _lowAddr );
		
protected:
	enum { ADDR_LOCAL, ADDR_SHORT, ADDR_LONG } addrType;
	
	word shortAddr;
	unsigned long highAddr;
	unsigned long lowAddr;
};


#endif