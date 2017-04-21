#ifndef APA_H
#define APA_H

//Define return types
typedef enum {APA_SUCCESS, APA_OCCUPIED, APA_INCOMPATIBLE_FORMATS, APA_OOM, APA_FAILURE} APAPortStatus;

//Struct object
typedef struct apa_type *APAPort;

//apaParseInputQueue
//Returns: a status flag
//takes: a pointer to a memory address
//       an integer denoting the message length
//Parse the input queue into the apa struct
APAPortStatus apaParseInputQueue(APAPort a, void *inputQueue,int length);

//
APAPortStatus apaSendToOutputQueue(*outputQueue);

APAPortStatus apaMovePacket(struct apaPortType *port0, struct apaPortType *port1);
APAPortStatus apaCopyPacket(struct apaPortType *port0, struct apaPortType *port1);
APAPortStatus apaRoutePacket(struct apaPortType *port);
APAPortStatus apaReversePath(struct apaPortType *port);
char apaGetPacket(struct apaPortType *port);
char apaPutPacket(struct apaPortType *port);

// (Start/End)Value helpers allow APA to encode more complex functionality into
// the start and end values of the packet, including checksums and format information
uint8_t genStartValue(APAPort a);
uint8_t parseStartValue(uint8_t startVal);

uint8_t genEndValue(APAPort a);
uint8_t parseEndValue(uint8_t endVal);

#endif
