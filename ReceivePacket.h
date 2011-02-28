// Receivepacket.h

#ifndef DTM_XBEE_RECEIVEPACKET_H
#define DTM_XBEE_RECEIVEPACKET_H

#include "WProgram.h"

#include "Packet.h"


class XBee;



class XBeeReceivePacket : public XBeePacket {
public:
	//friend bool XBee::receiveWait( XBeeReceivePacket*, int );
	friend class XBee;
	
	XBeeReceivePacket( byte *_frameBuf, size_t _frameBufLen );
	
	int getPayloadSize() const { return payloadSize; }
	
	// TX Status
	byte getFrameID() const { return frameBuf[0]; }
	byte getStatus() const { return frameBuf[1]; }
	
public:
	enum { STATUS, AT_RESP, AT_RESP_REMOTE, TX_STATUS, RX_DATA } type;
	byte apiID;
	//byte rssi;
	//byte options;
	
	// Make these accessors that pluck the data from the frame buffer
	//byte frameID;	// AT Cmd Resp
	//byte status;	// AT Cmd resp
	//char cmd1;		// AT Cmd resp
	//char cmd2;		// AT Cmd resp

private:
	byte *frameBuf;
	size_t frameBufLen;
	int payloadSize;
	//size_t bufferSize;
	int overflow;	// If greater than zero, this indicates how many characters did not fit in the frame buffer.
};



#endif
