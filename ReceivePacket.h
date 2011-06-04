// Receivepacket.h
// Copyright (c) 2011, Donald T. Meyer
// All rights reserved.
// This source code is licensed under the BSD 2-Clause License, full text available in license.txt.


#ifndef DTM_XBEE_RECEIVEPACKET_H
#define DTM_XBEE_RECEIVEPACKET_H

#include "WProgram.h"

#include "Packet.h"


class XBee;

/*  
                AT Response      Modem Status     TX Status     RX Data
                -----------      -----------     ---------     -------
 Frame ID             x                               x
 Address       (remote only)                                       x
 Cmd                  x 
 Status               x               x               x
 RSSI                                                              x
 Option bitmap                                                     x
 Data                 x                                            x
*/


class XBeeReceivePacket : public XBeePacket {
public:
	//friend bool XBee::receiveWait( XBeeReceivePacket*, int );
	friend class XBee;
	

	XBeeReceivePacket( byte *_frameBuf, size_t _frameBufLen );

	/** Returns true if the checksum was ok and there was no buffer overflow. */
	bool isOK() const { return ( checksumOK && (overflow == 0) ); }

	/** Valid for RX Data packets only */
	bool isAddressBroadcast() const { return optionsmap & 0x01 ? true : false; }

	/** Valid for RX Data packets only */
	bool isPANBroadcast() const { return optionsmap & 0x02 ? true : false; }
	
protected:
	void reset();
	void populateFields();
	
public:
	enum PacketType { INVALID, STATUS, AT_RESP, TX_STATUS, RX_DATA };

	PacketType type;	// All
	byte apiID;			// All
	byte rssi;			// RX Data
	byte frameID;		// AT Cmd Resp, TX Status
	byte status;		// AT Cmd resp, TX Status, Modem Status
	char cmd[2];		// AT Cmd resp
	int payloadSize;	// AT Response, RX Data
	byte *payloadPtr;	// AT Response, RX Data

	int overflow;		// All.  If greater than zero, this indicates how many characters did not fit in the frame buffer.
	
private:
	byte *frameBuf;
	size_t frameBufLen;

	byte optionsmap;

	bool checksumOK;
};



#endif
