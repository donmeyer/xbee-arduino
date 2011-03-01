// Receivepacket.h

#ifndef DTM_XBEE_RECEIVEPACKET_H
#define DTM_XBEE_RECEIVEPACKET_H

#include "WProgram.h"

#include "Packet.h"


class XBee;

/*  
                AT Response     Status     TX Status     RX Data
                -----------     ------     ---------     -------
 Frame ID             x                         x
 Address              x                                      x
 Cmd                  x
 Status               x            x            x
 RSSI                                                        x
 Option bitmap                                               x
 Data                 x                                      x
*/


class XBeeReceivePacket : public XBeePacket {
public:
	//friend bool XBee::receiveWait( XBeeReceivePacket*, int );
	friend class XBee;
	

	XBeeReceivePacket( byte *_frameBuf, size_t _frameBufLen );

	/** Returns true if the checksum was ok and there was no buffer overflow. */
	bool isOK() const { return ( checksumOK && (overflow == 0) ); }

	/** Valid for AT Response, RX Data */
	//const byte *getPayload() const { return frameBuf; }

	/** Valid for AT Response, RX Data */
	//int getPayloadSize() const { return payloadSize; }
	
	bool isAddressBroadcast() const { return optionsmap & 0x01 ? true : false; }
	bool isPANBroadcast() const { return optionsmap & 0x02 ? true : false; }
	
protected:
	void reset();
	void populateFields();
	
public:
	enum PacketType { INVALID, STATUS, AT_RESP, TX_STATUS, RX_DATA } type;
	byte apiID;
	byte rssi;
	byte frameID;	// AT Cmd Resp
	byte status;	// AT Cmd resp
	char cmd[2];		// AT Cmd resp
	int payloadSize;
	byte *payloadPtr;
	
private:
	byte *frameBuf;
	size_t frameBufLen;

	byte optionsmap;

	bool checksumOK;
	int overflow;	// If greater than zero, this indicates how many characters did not fit in the frame buffer.
};



#endif
