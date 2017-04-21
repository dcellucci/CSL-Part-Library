/*
 * APAProtocol.cpp
 * Created by Nick Cramer 8/2/2016
 * Based off of the APA tutorial form Daniel Celluci @ https://github.com/dcellucci/APA_Tutorial.git
 * and was origionally developed by Neil Gershenfeld from CBA MIT in 12/4/11
*/
// ******************************************************************************************
// INCLUDED LIBRARIES
#include "APAProtocol.h"

// apaProtocol class constructor
apaProtocol::apaProtocol(char id):apaPort(0){

	if (id != 0){
		apaPortType apaPort(id);
	}
	apaPort.id = id;
	apaPort.pathInLength = 0;
	apaPort.pathOutLength = 0;
	//apaPort.device(id);
}

//////////////////////////////////////////////////////////////////////
// apaProcessPacket function - this functions takes a given port and
//								processes the curent APA packet
// input - apaPortType
// output - 0 if everything worked and 1 if something failed
// Note: this probably could be done better with use of default string
//			parsing but in the spirit of the initial implementation
//			I am trying to keep it as bare bones as possible in order
//			to allow continued implemention on Tiny like micros
//////////////////////////////////////////////////////////////////////
unsigned char apaProtocol::apaProcessPacket(struct apaPortType *port){
	//Create and intialize counting variable
	char count = 0;
	
	//Checking for garbage before the message
	while(port->packet[count] != apa_start){
		count +=1;
		//If the count is higher than the maximum packet length return 1
		if (count > 2*apa_max_packet+3){
			port->pathInLength = 0;
			return 1;
		}
	}
	
	//Reintializing the input path length
	port->pathInLength = 0;
	
	//Get the input path by reading all characters until divider is seen
	while(port->packet[count] != apa_divider){
		//If the count is higher than the maximum packet length return 1
		if (count > 2*apa_max_packet+3){
			port->pathInLength = 0;
			return 1;
		}
		//Save the proccessed character in the path in aray
		port->pathIn[port->pathInLength] = port->packet[count];
		port->pathInLength += 1;
		count +=1;
	}
	
	//Remove the last incrimenation on the path in length
	port->pathInLength -= 1;
	
	//Reinitialize payload in length
	port->payloadInLength = 0;
	
	//Get the payload by reading all characters until end char is seen
	while(port->packet[count] != apa_end){
		//If the count is higher than the maximum packet length return 1
		if (count > 2*apa_max_packet+3){
			port->pathInLength = 0;
			return 1;
		}
		
		//Check for the escape symbol
		if(port->packet[count] == apa_escape){
			//If the count is higher than the maximum packet length return 1
			if (count+1 > 2*apa_max_packet+3){
				port->pathInLength = 0;
				return 1;
			}
			
			port->payloadIn[port->payloadInLength] = port->packet[count+1];
			port->payloadInLength += 1;
			count = count + 2;
			
			//If the count is higher than the maximum packet length return 1
			if (count > 2*apa_max_packet+3){
				port->pathInLength = 0;
				return 1;
			}
		}
		//Store the payload values
		port->payloadIn[port->payloadInLength] = port->packet[count];
		port->payloadInLength += 1;
		count += 1;
	}
	
	//Remove the last incrimenation on the payload in length
	port->payloadInLength -= 1;
}

//////////////////////////////////////////////////////////////////////
// apaMovePacket function - this functions moves the input of port 0
//							to the output of port 1. By reseting the
//							path lengths of port 0 it effectively 
//							deletes the previous packet
// input - apaPortType, apaPortType
//////////////////////////////////////////////////////////////////////
void apaProtocol::apaMovePacket(struct apaPortType *port0, struct apaPortType *port1){
	unsigned char i;
	
	// Copy the input path of port 0 to the output path of port 1
	for (i = 0; i < port0->pathInLength; ++i)
		port1->pathOut[i] = port0->pathIn[i];
	
	// Copy the path length
	port1->pathOutLength = port0->pathInLength;
	// Reset the payload length
	port0->pathInLength = 0;
	
	// Copy the input payload from port0 to the output of port 1
	for (i = 0; i < port0->payloadInLength; ++i)
		port1->payloadOut[i] = port0->payloadIn[i];
	
	// Copy the payload length from port 0 to port 1
	port1->payLoadOutLength = port0->payloadInLength;
	// Reset the payload length
	port0->payloadInLength = 0;
}

//////////////////////////////////////////////////////////////////////
// apaCopyPacket function - this functions copies with out deleting the
// 							input of port 0 to the output of port 1
// input - apaPortType, apaPortType
//////////////////////////////////////////////////////////////////////
void apaProtocol::apaCopyPacket(struct apaPortType *port0, struct apaPortType *port1){
	unsigned char i;
	
	// Copy the input path of port 0 to the output path of port 1
	for (i = 0; i < port0->pathInLength; ++i)
      port1->pathOut[i] = port0->pathIn[i];
	// Copy the path length
	port1->pathOutLength = port0->pathInLength;

	// Copy the input payload from port0 to the output of port 1
	for (i = 0; i < port0->payloadInLength; ++i)
		port1->payloadOut[i] = port0->payloadIn[i];
		
	// Copy the payload length
	port1->payLoadOutLength = port0->payloadInLength;
}

//////////////////////////////////////////////////////////////////////
// apaRoutePacket function - this function finds the pointer and
//							indicates either the completion of the 
//							pathing or the next port
// input - apaPortType
//////////////////////////////////////////////////////////////////////
void apaProtocol::apaRoutePacket(struct apaPortType *port){
	static unsigned char ptr;
	
	// Initialize APA pointer to zero
	ptr = 0;
	
	// Increment through the path
	while (port->pathIn[ptr] != apa_pointer)
      ++ptr;
	
	// Check if the pointer has reached the devider
	if ((ptr+1) == port->pathInLength) {
		//Inidcate that the destination is this port
		port->destination = apa_here;
	}
	else{
		// Set the destination to the next port after the pointer
		port->destination = port->pathIn[ptr+1];
		// Set the current pointer position to this port
		port->pathIn[ptr] = port->id;
		// Move the pointer to the next port/the destination
		port->pathIn[ptr+1] = apa_pointer;
	}
}

//////////////////////////////////////////////////////////////////////
// apaReversePath function - this function reverses the output path
//							allowing pathing back to the source
// input - apaPortType
//////////////////////////////////////////////////////////////////////
void apaProtocol::apaReversePath(struct apaPortType *port) {
   static unsigned char i;
   static unsigned char temp[apa_max_packet];
   
   // Reverse the path and save into a temporary file
	for (i = 0; i < port->pathOutLength; ++i)
		temp[i] = port->pathOut[port->pathOutLength-i-1];
	
	// Replace path with new reversed path
	for (i = 0; i < port->pathOutLength; ++i)
		port->pathOut[i] = temp[i];
}

//////////////////////////////////////////////////////////////////////
// apaGetPacket function - this function picks up a packet from the
//							physical layer and saves it to the port
//							packet
// input - apaPortType
//////////////////////////////////////////////////////////////////////
char apaProtocol::apaGetPacket(struct apaPortType *port){
	return port->device.apaGetPacket(port->packet);
}

//////////////////////////////////////////////////////////////////////
// apaPutPacket function - this function sends a packet over the
//							physical layer
// input - apaPortType
//////////////////////////////////////////////////////////////////////
char apaProtocol::apaPutPacket(struct apaPortType *port){
	return port->device.apaPutPacket(port->pathOut,port->payloadOut,port->pathOutLength,port->payLoadOutLength);
}

//////////////////////////////////////////////////////////////////////
// apaProtScan function - this function scans the port process and 
//							routes recieve packet 
// input - apaPortType
//////////////////////////////////////////////////////////////////////
void apaProtocol::apaProtScan(struct apaPortType *port){
	// Create current port
	struct apaPortType *currentPort;
	
	// Get packet on line if there
	if (apaGetPacket(port) > 0 ){
		// If there was a packet route it
		apaRoutePacket(port);
	}
	
	// Check to see if the current port has a path in
	if (port->pathInLength !=0 ){
		// If the current port is the destination then
		if (port->destination == apa_here){
			// Move the packet to the output
			apaMovePacket(port,port);
			// Reverset he path of the packed
			apaReversePath(port);
			//Send packet back????
			apaPutPacket(port);
		}
		// Check if the package is a flood/broadcast?
		else if (port->destination == apa_flood){
			// If it is a flood/broadcast copy it to the other ports
			// Set current port to next port
			currentPort = port->nextPort;
			// Send package on until it returns to this port
			while(currentPort->id != port->id){
				apaCopyPacket(port,currentPort);
				currentPort = currentPort->nextPort;
			}
			// Reset input path length to zero effectively deleting current message
			port->pathInLength = 0;
		}
		// The destination is not here and it is not flood loop through the rest of the path
		else{
			currentPort = port->nextPort;
			// While the loop has not returned
			while(port->id != currentPort->id){
				if (port->destination != currentPort->id){
					// Check if the port is empty
					if (currentPort->pathOutLength == 0){
						// Move the packet to the current port and exit loop
						apaMovePacket(port,currentPort);
						break;
					}
				}
				//Move on to next port
				currentPort = currentPort->nextPort;
			}
			// Discard path
			if (port->id == currentPort->id){
				port->pathInLength = 0;
			}
		}
	}
	// Check if there is an output packet waiting
	if (port->pathOutLength != 0){
		//Send packet
		apaPutPacket(port);
	}
}