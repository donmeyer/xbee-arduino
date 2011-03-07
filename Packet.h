
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
	
	word shortAddr() const { return _shortAddr; }
	unsigned long highAddr() const { return _highAddr; }
	unsigned long lowAddr() const { return _lowAddr; }

protected:
	XBeePacket();
	XBeePacket( word _shortAddr );
	XBeePacket( unsigned long _highAddr, unsigned long _lowAddr );
		
	void parseShortAddr( byte *p );
	void parseLongAddr( byte *p );
	
protected:
	enum { ADDR_LOCAL, ADDR_SHORT, ADDR_LONG } addrType;
	
	word _shortAddr;
	unsigned long _highAddr;
	unsigned long _lowAddr;
};


#endif