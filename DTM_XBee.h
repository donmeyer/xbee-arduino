// DTM Xbee.h


#ifndef DTM_XBEE_H
#define DTM_XBEE_H



#include "WProgram.h"


#define DEBUG


class XBeeReceivePacket;
class XBeeTransmitPacket;
class XBeeOutboundPacket;
class XBeeATCmdPacket;



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
	
private:	
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


	// These variables apply to the inbound packet.
	byte inboundCsum;			// Checksum of the inbound packet
	byte outboundCsum;
	bool complete;		// True if the packet is complete
	enum { S_EMPTY, S_COMPLETE } state;
};




class XBeeReceivePacket {
public:
	XBeeReceivePacket( byte *_frameBuf, size_t _frameBufLen );
	
public:
	word shortAddr;
	unsigned long highAddr;
	unsigned long lowAddr;
	
	byte apiID;
	byte rssi;
	byte options;

private:
	byte *frameBuf;
	size_t frameBufLen;
	size_t bufferSize;
	bool overflow;
};



/**
 * Virtual base class for all outbound packets
**/
class XBeeOutboundPacket {
public:
	static const byte DEFAULT_FRAME_ID = 0x01;
	
protected:
	XBeeOutboundPacket();
	XBeeOutboundPacket( word _shortAddr );
	XBeeOutboundPacket( unsigned long _highAddr, unsigned long _lowAddr );
	
public:
	bool isLocalAddr() const { return addrType == ADDR_LOCAL; }
	bool isShortAddr() const { return addrType == ADDR_SHORT; }
	bool isLongAddr() const { return addrType == ADDR_LONG; }
	
	word getShortAddr() const { return shortAddr; }
	unsigned long getHighAddr() const { return highAddr; }
	unsigned long getLowAddr() const { return lowAddr; }
	
public:
	byte frameID;
	
private:
	enum { ADDR_LOCAL, ADDR_SHORT, ADDR_LONG } addrType;
	
	word shortAddr;
	unsigned long highAddr;
	unsigned long lowAddr;
};



class XBeeATCmdPacket : public XBeeOutboundPacket {
public:
	XBeeATCmdPacket();
	XBeeATCmdPacket( word _shortAddr );
	XBeeATCmdPacket( unsigned long _highAddr, unsigned long _lowAddr );
	
	void set( char _cmd1, char _cmd2, const char *arg );
	void set( char _cmd1, char _cmd2 );
	void set( const char *cmd );
	//void set( char cmd1, char cmd2, unsigned long arg );
	
	char getCmd1() const { return cmd1; }
	char getCmd2() const { return cmd2; }
	const char *getArg() const { return strArg; }
	
public:
	bool applyChanges;
private:
	char cmd1;
	char cmd2;
	//char argBuf[9];
	const char *strArg;
};



class XBeeTransmitPacket : public XBeeOutboundPacket {
public:
	XBeeTransmitPacket( word _shortAddr, const void *_payload, size_t _payloadSize );
	XBeeTransmitPacket( unsigned long _highAddr, unsigned long _lowAddr, const void *_payload, size_t _payloadSize );
	
	void setPayload( const void *_payload, size_t _payloadSize ) { payload = (const byte*)_payload; payloadSize = _payloadSize; }
	
	size_t getPayloadSize() const { return payloadSize; }
	const byte *getPayload() const { return payload; }
	
public:
	bool disableAck;
	bool broadcastPAN;
private:
	const byte *payload;
	size_t payloadSize;
};



#endif
