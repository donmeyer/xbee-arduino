// DTM Xbee.h


#ifndef DTM_XBEE_H
#define DTM_XBEE_H


#include "WProgram.h"

#include "Packet.h"
#include "ReceivePacket.h"
#include "TransmitPacket.h"


//#define DEBUG



class XBee {
public:
	XBee();
	
	void begin( unsigned long baud );
	
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
	
	// Some statistics
	int overflowCount;
	int badChecksumCount;
	
private:	
	byte outboundCsum;			// This is an instance variable so we can share it with the emit() method.

	// These variables apply to the inbound packet.
	byte inboundCsum;			// Checksum of the inbound packet
	int inboundLen;			// Length value from the incoming packet.
	enum { S_EMPTY, S_GOT_START, S_GOT_HI_LEN, S_GOT_LO_LEN, S_GOT_API, S_COMPLETE } state;
};


#endif
