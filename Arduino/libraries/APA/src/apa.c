//
//
// apa.c
//
// asynchronous packet automata networking
//
// Neil Gershenfeld
// CBA MIT 12/4/11
//
// Additions by Daniel Cellucci
// NASA Ames Research Center 4/22/17
//

//
// includes
//

#include <apa.h>

//
// functions
//

void apa_get_packet(struct apa_port_type *port) {
    //
    // get packet on port with timeouts
    //
    char c;
    unsigned char return_value,count;
    //
    // find start
    //
    count = apa_timeout;

    if(port->packet_in_length == 0){
        //nothing in the packet queue
        return;
    }
    //
    //find index of start character
    //
    char * start = strchr(port->packet_in,apa_start);
    if(start == NULL)
      return;

    //
    //Find index of divider
    //
    char * path = strchr(start+1,apa_divider);
    if(path == NULL)
      return;

    //
    //Find index of endpoint
    //
    char * payload = strchr(path+1,apa_end);
    if(payload == NULL)
      return;

    //allocate the memory for the port/payload arrays
    port->path_in = malloc(sizeof(char)*(path-start-1));
    port->path_in_length = (path-start-1);

    port->payload_in = malloc(sizeof(char)*(payload-path-1));
    port->payload_in_length = (payload-path-1));

    //copy the path from the packet
    strncpy(port->path_in,start+1,path-start-1);
    port->path_in[path-start-1] = '\0';

    //copy the payload from the packet
    strncpy (port->payload_in, path+1, payload-path-1 );
    port->payload_in_length[payload-path-1] = '\0';

    //We're done with the packet, so let's free those data.
    free(port->packet_in);
    port->packet_in_length = 0;
}

void apa_put_packet(struct apa_port_type *port) {
    //first check if the output queue is open
    if(port->packet_out_length > 0){
        return;
    }
    //if the packet has zero length, allocate the memory for it.
    port->packet_out_length = port->port_out_length + port->payload_out_length + 3;
    port->packet_out = malloc(sizeof(char)*(port->packet_out_length);

    //
    // Populate the packet out
    //
    port->packet_out[0] = apa_start;
    //Concatenate the out packet's path
    strcat(port->packet_out,port->path_out);
    //add the divider
    port->packet_out[strlen(port->packet_out)] = apa_divider;
    //then add the payload
    strcat(port->packet_out,port->payload_out);
    //close the packet up
    port->packet_out[strlen(port->packet_out)] = apa_end;

    //clean up data
    free(port->path_out);
    port->path_out = NULL; //point it to nothing, just in case
    port->path_out_length = 0;

    free(port->payload_out);
    port->payload_out = NULL;
    port->payload_out_length = 0;

}

void apa_move_packet(struct apa_port_type *port0, struct apa_port_type *port1) {
    //
    // move packet from port0 in to port1 out
    //
    unsigned char i;
    //
    // point port1 path out to port 0 path in.
    //
    port1->path_out = port0->path_in;
    //
    //make path_in point to nothing
    //
    port0->path_in = NULL;
    //
    // update the lengths to reflect this change
    //
    port1->path_out_length = port0->path_in_length;
    port0->path_in_length = 0;

    //
    // copy payload by doing the same pointer business
    //
    port1->payload_out = port0->payload_in;
    //
    //make payload_in point to nothing
    //
    port0->payload_in = NULL;
    //
    // update the lengths to reflect this change
    //
    port1->payload_out_length = port0->payload_in_length;
    port0->payload_in_length = 0;
}

void apa_copy_packet(struct apa_port_type *port0, struct apa_port_type *port1) {
    //
    // copy packet from port0 in to port1 out
    //

    //
    // copy path
    //
    strcpy(port1->path_out,port0->path_in);
    port1->path_out_length = port0->path_in_length;
    //
    // copy payload
    //
    strcpy(port1->payload_out,port0->payload_in);
    port1->payload_out_length = port0->payload_in_length;
}

void apa_port_scan(struct apa_port_type *port) {
    //
    // port scan
    //
    struct apa_port_type *current_port;
    //
    // is an incoming packet waiting?
    //
    if (port->packet_in_length > 0) {
        //
        // yes, is the input empty?
        //
        if (port->path_in_length == 0) {
            //
            // yes, accept the packet
            //
            apa_get_packet(port);
            //
            // was a packet received?
            //
            if (port->path_in_length != 0) {
                //
                // yes, route it
                //
                apa_route_packet(port);
            }
        }
        //
        // no, reject it
        //
    }
    //
    // is an input packet waiting?
    //
    if (port->path_in_length != 0) {
        //
        // yes, is the destination here?
        //
        if (port->destination == apa_here) {
            //
            // yes, is the output empty?
            //
            if (port->path_out_length == 0) {
                //
                // yes, move packet to output, reverse path, and process
                //
                apa_move_packet(port,port);
                apa_process_packet(port);
            }
        }
        //
        // destination not here, is it a flood?
        //
        else if (port->destination == apa_flood) {
            //
            // yes, copy to other ports
            //
//
// NB: it is possible that the flood fails to propagate b/c the port already
// has a packet in its input buffer.
// The current version results in the packet just not being copied.
//
            current_port = port->next_port;

            while (current_port->id != port->id) {
                apa_copy_packet(port,current_port);
                current_port = current_port->next_port;
            }
            //
            //clear the path and payload of the original port after flooding
            //
            free(port->path_in);
            port->path_in = NULL;
            port->path_in_length = 0;

            free(port->payload_in);
            port->payload_in = NULL;
            port->payload_in_length = 0;
        }
        //
        // destination not here or flood, loop over other ports
        //
        else {
            current_port = port->next_port;
            while (1) {
                //
                // is current port the destination?
                //
                if (port->destination == current_port->id) {
                    //
                    // yes, is current port empty?
                    //
                    if (current_port->path_out_length == 0) {
                        //
                        // yes, move packet to current port
                        //
                        apa_move_packet(port,current_port);
                        break;
                    }
                }
                //
                // no, back to same port?
                //
                if (port->id == current_port->id) {
                    //
                    // yes, discard invalid path
                    //
                    free(port->path_in);
                    port->path_in = NULL;
                    port->path_in_length = 0;
                    break;
                }
                //
                // no, go to next port
                //
                current_port = current_port->next_port;
            }
        }
    }
    //
    // is an output packet waiting?
    //
    if (port->path_out_length != 0) {
        //
        // yes, try to send it
        //
        apa_put_packet(port);
    }
}

void apa_route_packet(struct apa_port_type *port) {
    //
    // route packet path and set destination
    //

    //Check if the packet is at its final destination
    if(port->path_in[0] == apa_pointer){
      port->destination = apa_here;
    }
    else{
      //take the next value in the path as the id of the port this needs to go to
      port->destination = port->path_in[0];
      //shift all the values in the path_in array over by one
      memmove(&(port->path_in[0]),&(port->path_in[1]),sizeof(char)*(strlen(port->path_in)-1));
      //set the last value in the path to the current port ID
      port->path_in[strlen(port->path_in)-1] = port->id;
    }
}
