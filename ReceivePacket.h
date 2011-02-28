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

	enum PacketType { INVALID, STATUS, AT_RESP, AT_RESP_REMOTE, TX_STATUS, RX_DATA };
	

	XBeeReceivePacket( byte *_frameBuf, size_t _frameBufLen );
	
	bool isOK() const { return ( checksumOK && (overflow == 0) ); }

	PacketType getType() const;
	
	int getPayloadSize() const { return payloadSize; }
	
	// TX Status
	byte getFrameID() const;
	byte getStatus() const;
	
public:
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

	bool checksumOK;
	int overflow;	// If greater than zero, this indicates how many characters did not fit in the frame buffer.
};



#endif
