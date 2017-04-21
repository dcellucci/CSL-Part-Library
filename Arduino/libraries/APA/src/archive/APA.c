#include "APA.h"

#define APA_SIMPLE_STARTEND 0

#define APA_BITS_PER_PATHVAL 3 //2^N different options
//Path values- an enum corresponding to different options for the path
typedef enum {PORT0, PORT1, PORT2, FLOOD, CARAT, SEPARATOR} PathVal;

struct apa_type{
  PathVal *pathIn;
  PathVal *pathOut;
  uint8_t pathInLength, pathOutLength;
  uint8_t *payloadIn;
  uint8_t *payloadOut;
  uint8_t payloadInLength, payLoadOutLength;
  struct apa_type *nextport;
  uint8_t id;
  uint8_t (*queuePacket)(uint8_t );
};

//apaParseInputQueue
//Returns: a status flag
//takes: a pointer to a memory address
//       an integer denoting the message length
//Parse the input queue into the apa struct
APAPortStatus apaParseInputQueue(void *inputQueue, int length){
  inputQueue = (char*)inputQueue;
  if(a->pathInLength > 0 || a->payloadInLength > 0){
    //The port currently has a packet in its input space
    return APA_OCCUPIED;
  }
  else{
    //Allocate the max possible path array size and check if we're out of memory
    if(uint8_t *tpatharray = malloc(sizeof(uint8_t)*(length-2)) == NULL){
      //we're out of memory. Better deal with that.
      return APA_OOM;
    }
    //Allocate the max possible payload array size and check if we're out of memory
    if(uint8_t *tpayloadarray = malloc(sizeof(uint8_t)*(length-2)) == NULL){
      //now we're out of memory. Free up the path array we allocated, then deal with it.
      free(tpatharray);
      return APA_OOM;
    }

    //
    //parse the start character
    //
    //If we're expecting constant startvals but we get something more complicated,
    //Throw an incompatible formats error
    if(APA_SIMPLE_STARTEND && (char)inputQueue[0] != '{'){
      return APA_INCOMPATIBLE_FORMATS;
    }
    else if((char)inputQueue[0] == '{' || ){
      
    }

    //read in the path until it hits the separator
    //assign the path in length and path in array to the port
    //read in the rest of the message (as given in length)
    //the rest of the message - 1 byte is the payload
    //the endvalue is the last byte in the payload.

  }
  return APA_SUCCESS;
}

static uint8_t genAPAStartValue(APAPort a){
#if APA_SIMPLE_STARTEND
  return '{'
#else
  return 0xDC //dummy value for now
#endif
}

static bool parseAPAStartValue(char *inputQueue){
  //Take the input queue and perform a checksum against the
  //Ad
}



static uint8_t genAPAEndValue(APAPort a){
#if APA_SIMPLE_STARTEND
  return '}'
#else
  return 0xCD //dummy value for now
#endif
}

static uint8_t parseAPAStartValue(uint8_t startval){

}
