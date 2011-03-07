

#ifndef DTM_XBEE_TRANSMITPACKET_H
#define DTM_XBEE_TRANSMITPACKET_H


#include "WProgram.h"



/**
 * Virtual base class for all outbound packets
**/
class XBeeOutboundPacket : public XBeePacket {
public:
	static const byte DEFAULT_FRAME_ID = 0x01;
	
protected:
	XBeeOutboundPacket();
	XBeeOutboundPacket( word _shortAddr );
	XBeeOutboundPacket( unsigned long _highAddr, unsigned long _lowAddr );
	
protected:
	void bumpID();
	
public:
	byte frameID;
	
private:
};



/**
 * AT Command packet.
 * This is used for both local and remote AT commands.
**/
class XBeeATCmdPacket : public XBeeOutboundPacket {
public:
	XBeeATCmdPacket();
	XBeeATCmdPacket( word _shortAddr );
	XBeeATCmdPacket( unsigned long _highAddr, unsigned long _lowAddr );
	
	void set( char _cmd1, char _cmd2, const char *arg=NULL );
	void set( const char *cmd );
	
	char getCmd1() const { return cmd1; }
	char getCmd2() const { return cmd2; }
	const char *getArg() const { return strArg; }
	
public:
	/**
	 * For local commands, this controls wether or not the apply immediately or are queued.
	 * If true, they apply immediately (API ID 0x08).  If false they queue (API ID 0x09)
	 * 
	 * For remote commands this does the same thing essentially.
	 * 
	 * The default is true (changes apply immediately)
	**/
	bool applyChanges;

private:
	char cmd1;
	char cmd2;
	const char *strArg;
};



/**
 * Transmit Data Packet
**/
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
