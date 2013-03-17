// DTM Xbee.h

/*
 * Copyright (c) 2011, Donald T. Meyer
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice, this list of conditions
 * and the following disclaimer.
 * 
 * - Redistributions in binary form must reproduce the above copyright notice, this list of
 *  conditions and the following disclaimer in the documentation and/or other materials provided
 *  with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DTM_XBEE_H
#define DTM_XBEE_H


#include "Arduino.h"

#include "Packet.h"
#include "ReceivePacket.h"
#include "TransmitPacket.h"


//#define DEBUG



class XBee {
public:
	XBee( Stream &_stream );
	
	bool receiveWait( XBeeReceivePacket *packet, int timeout );

	void send( const XBeeTransmitPacket *packet );
	
	void sendAT( const XBeeATCmdPacket *packet );
		
private:
	void emit( byte b );
	void emitShortAddr( const XBeeOutboundPacket *packet );
	void emitLongAddr( const XBeeOutboundPacket *packet );
	
public:
	static const byte START_DELIMITER = 0x7E;

	// Transmit Data packets sent to the modem
	static const byte API_TX_16 = 0x01;
	static const byte API_TX_64 = 0x00;

	static const byte API_AT_CMD = 0x08;
	static const byte API_AT_CMD_QUEUED = 0x09;
	static const byte API_AT_CMD_REMOTE = 0x17;

	// Packets received from the modem
	static const byte API_STATUS = 0x8A;
	static const byte API_AT_RESP = 0x88;
	static const byte API_AT_RESP_REMOTE = 0x97;
	static const byte API_TX_STATUS = 0x89;
	static const byte API_RX_16 = 0x81;
	static const byte API_RX_64 = 0x80;

public:
	/** If true, we don't return packets that overflow or fail the checksum. */
	bool onlyGoodPackets;
	
	// Some statistics.  These are maintained regardless of wether or not we retunr bad packes as well as good ones.
	int overflowCount;
	int badChecksumCount;
	
private:
	Stream &stream;			// Stream used to talk to the XBee modem.
	
	byte outboundCsum;		// This is an instance variable so we can share it with the emit() method.

	// These variables apply to the inbound packet.
	byte inboundCsum;		// Checksum of the inbound packet
	int inboundLen;			// Length value from the incoming packet.
	enum { S_EMPTY, S_GOT_START, S_GOT_HI_LEN, S_GOT_LO_LEN, S_GOT_API, S_COMPLETE } state;
};


#endif
