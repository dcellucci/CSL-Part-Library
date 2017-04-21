/*

*/


// ******************************************************************************************
// DEFINITIONS

#define apa_max_packet 10 // maximum path and payload size
#define apa_timeout 255 // timeout loop count

#define apa_start '{' // packet start
#define apa_pointer '^' // packet path pointer
#define apa_divider '|' // packet payload divider
#define apa_end '}' // packet end
#define apa_escape '\\' // packet end escape

#define apa_flood 'F' // flood destination


// ******************************************************************************************
// STRCUT DEFINITIONS

typedef struct APAPort {
    uint8_t pathIn[apa_max_packet];
    uint8_t pathOut[apa_max_packet];
    uint8_t pathInLength, pathOutLength;
    uint8_t payloadIn[apa_max_packet];
    uint8_t payloadOut[apa_max_packet];
    uint8_t payloadInLength, payLoadOutLength;
    uint8_t destination;
    uint8_t id;
    struct APAPort *nextPort;
    uint8_t packet[2*apa_max_packet+3];
    uint8_t (*sendMessage)(char*payloadOut,char*pathOut)
    APAPort(char val):id(val),device(val){};
} APAPort;
// ******************************************************************************************
// CLASS DEFINITIONS

class APAProtocol {

	public:
		APAPort port;
		unsigned char apaProcessPacket(struct apaPortType *port);
		void apaMovePacket(struct apaPortType *port0, struct apaPortType *port1);
		void apaCopyPacket(struct apaPortType *port0, struct apaPortType *port1);
		void apaRoutePacket(struct apaPortType *port);
		void apaReversePath(struct apaPortType *port);
		char apaGetPacket(struct apaPortType *port);
		char apaPutPacket(struct apaPortType *port);
		void apaPortScan(struct apaPortType *port);
		apaProtocol(char);
};
