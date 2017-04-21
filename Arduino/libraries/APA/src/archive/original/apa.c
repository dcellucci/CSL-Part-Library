//
//
// apa.c
//
// asynchronous packet automata networking
//
// Neil Gershenfeld
//
// CBA MIT 12/4/11
//
// (c) Massachusetts Institute of Technology 2011
// Permission granted for experimental and personal use;
// license for commercial sale available from MIT.
//
// todo
//    flood route
//    rejected handshakes
//    bridge serial error checking
//    merge bridge port scan
//    adaptive bit timing
//
//
// includes
//

#include <apa.h>

//
// functions
//

void apa_put_char(struct apa_port_type *port, char c, unsigned char *return_value) {
   //
   // put synchronous char on port with timeouts
   //
   // set ready
   //
   set(*port->port_out, port->pin_out);
   //
   // wait for start clock
   //
   *return_value = apa_timeout;
   while (1) {
      if (0 != pin_test(*port->pins_in, port->pin_in))
         break;
      *return_value -= 1;
      if (*return_value == 0)
         return;
      }
   //
   // set start bit
   //
   clear(*port->port_out, port->pin_out);
   //
   // bit 0
   //
   *return_value = apa_timeout;
   while (1) {
      if (0 == pin_test(*port->pins_in, port->pin_in))
         break;
      *return_value -= 1;
      if (*return_value == 0)
         return;
      }
   if (((c >> 0) & 1) == 0)
      clear(*port->port_out, port->pin_out);
   else
      set(*port->port_out, port->pin_out);
   //
   // bit 1
   //
   *return_value = apa_timeout;
   while (1) {
      if (0 != pin_test(*port->pins_in, port->pin_in))
         break;
      *return_value -= 1;
      if (*return_value == 0)
         return;
      }
   if (((c >> 1) & 1) == 0)
      clear(*port->port_out, port->pin_out);
   else
      set(*port->port_out, port->pin_out);
   //
   // bit 2
   //
   *return_value = apa_timeout;
   while (1) {
      if (0 == pin_test(*port->pins_in, port->pin_in))
         break;
      *return_value -= 1;
      if (*return_value == 0)
         return;
      }
   if (((c >> 2) & 1) == 0)
      clear(*port->port_out, port->pin_out);
   else
      set(*port->port_out, port->pin_out);
   //
   // bit 3
   //
   *return_value = apa_timeout;
   while (1) {
      if (0 != pin_test(*port->pins_in, port->pin_in))
         break;
      *return_value -= 1;
      if (*return_value == 0)
         return;
      }
   if (((c >> 3) & 1) == 0)
      clear(*port->port_out, port->pin_out);
   else
      set(*port->port_out, port->pin_out);
   //
   // bit 4
   //
   *return_value = apa_timeout;
   while (1) {
      if (0 == pin_test(*port->pins_in, port->pin_in))
         break;
      *return_value -= 1;
      if (*return_value == 0)
         return;
      }
   if (((c >> 4) & 1) == 0)
      clear(*port->port_out, port->pin_out);
   else
      set(*port->port_out, port->pin_out);
   //
   // bit 5
   //
   *return_value = apa_timeout;
   while (1) {
      if (0 != pin_test(*port->pins_in, port->pin_in))
         break;
      *return_value -= 1;
      if (*return_value == 0)
         return;
      }
   if (((c >> 5) & 1) == 0)
      clear(*port->port_out, port->pin_out);
   else
      set(*port->port_out, port->pin_out);
   //
   // bit 6
   //
   *return_value = apa_timeout;
   while (1) {
      if (0 == pin_test(*port->pins_in, port->pin_in))
         break;
      *return_value -= 1;
      if (*return_value == 0)
         return;
      }
   if (((c >> 6) & 1) == 0)
      clear(*port->port_out, port->pin_out);
   else
      set(*port->port_out, port->pin_out);
   //
   // bit 7
   //
   *return_value = apa_timeout;
   while (1) {
      if (0 != pin_test(*port->pins_in, port->pin_in))
         break;
      *return_value -= 1;
      if (*return_value == 0)
         return;
      }
   if (((c >> 7) & 1) == 0)
      clear(*port->port_out, port->pin_out);
   else
      set(*port->port_out, port->pin_out);
   //
   // wait for stop clock
   //
   *return_value = apa_timeout;
   while (1) {
      if (0 == pin_test(*port->pins_in, port->pin_in))
         break;
      *return_value -= 1;
      if (*return_value == 0)
         return;
      }
   clear(*port->port_out, port->pin_out);
   bit_read_delay();
   }

void apa_get_char(struct apa_port_type *port, char *c, unsigned char *return_value) {
   //
   // read synchronous character on port with timeouts
   //
   *c = 0;
   //
   // wait for ready
   //
   *return_value = apa_timeout;
   while (1) {
      if (0 != pin_test(*port->pins_in, port->pin_in))
         break;
      *return_value -= 1;
      if (*return_value == 0)
         return;
      }
   //
   // send start clock
   //
   set(*port->port_out, port->pin_out);
   //
   // wait for start bit
   //
   *return_value = apa_timeout;
   while (1) {
      if (0 == pin_test(*port->pins_in, port->pin_in))
         break;
      *return_value -= 1;
      if (*return_value == 0)
         return;
      }
   //
   // bit 0
   //
   clear(*port->port_out, port->pin_out);
   bit_read_delay();
   if (0 == pin_test(*port->pins_in, port->pin_in))
      *c |= (0 << 0);
   else
      *c |= (1 << 0);
   //
   // bit 1
   //
   set(*port->port_out, port->pin_out);
   bit_read_delay();
   if (0 == pin_test(*port->pins_in, port->pin_in))
      *c |= (0 << 1);
   else
      *c |= (1 << 1);
   //
   // bit 2
   //
   clear(*port->port_out, port->pin_out);
   bit_read_delay();
   if (0 == pin_test(*port->pins_in, port->pin_in))
      *c |= (0 << 2);
   else
      *c |= (1 << 2);
   //
   // bit 3
   //
   set(*port->port_out, port->pin_out);
   bit_read_delay();
   if (0 == pin_test(*port->pins_in, port->pin_in))
      *c |= (0 << 3);
   else
      *c |= (1 << 3);
   //
   // bit 4
   //
   clear(*port->port_out, port->pin_out);
   bit_read_delay();
   if (0 == pin_test(*port->pins_in, port->pin_in))
      *c |= (0 << 4);
   else
      *c |= (1 << 4);
   //
   // bit 5
   //
   set(*port->port_out, port->pin_out);
   bit_read_delay();
   if (0 == pin_test(*port->pins_in, port->pin_in))
      *c |= (0 << 5);
   else
      *c |= (1 << 5);
   //
   // bit 6
   //
   clear(*port->port_out, port->pin_out);
   bit_read_delay();
   if (0 == pin_test(*port->pins_in, port->pin_in))
      *c |= (0 << 6);
   else
      *c |= (1 << 6);
   //
   // bit 7
   //
   set(*port->port_out, port->pin_out);
   bit_read_delay();
   if (0 == pin_test(*port->pins_in, port->pin_in))
      *c |= (0 << 7);
   else
      *c |= (1 << 7);
   //
   // stop clock
   //
   clear(*port->port_out, port->pin_out);
   bit_read_delay();
   //
   // return
   //
   }

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
   do {
      apa_get_char(port, &c, &return_value);
      count -= 1;
      if ((return_value == 0) || (count == 0)) {
         port->path_in_length = 0;
         return;
         }
      } while (c != apa_start);
   //
   // get path
   //
   port->path_in_length = 0;
   count = apa_max_packet;
   do {
      apa_get_char(port, &c, &return_value);
      count -= 1;
      if ((return_value == 0) || (count == 0)) {
         port->path_in_length = 0;
         return;
         }
      port->path_in[port->path_in_length] = c;
      port->path_in_length += 1;
      } while (c != apa_divider);
   port->path_in_length -= 1;
   //
   // get payload
   //
   port->payload_in_length = 0;
   count = apa_max_packet;
   do {
      //
      // get next char
      //
      apa_get_char(port, &c, &return_value);
      count -= 1;
      if ((return_value == 0) || (count == 0)) {
         port->path_in_length = 0;
         return;
         }
      //
      // check for escape
      //
      if (c == apa_escape) {
         //
         // get escaped char
         //
         apa_get_char(port, &c, &return_value);
         count -= 1;
         if ((return_value == 0) || (count == 0)) {
            port->path_in_length = 0;
            return;
            }
         port->payload_in[port->payload_in_length] = c;
         port->payload_in_length += 1;
         //
         // get next char
         //
         apa_get_char(port, &c, &return_value);
         count -= 1;
         if ((return_value == 0) || (count == 0)) {
            port->path_in_length = 0;
            return;
            }
         }
      //
      // save char
      //
      port->payload_in[port->payload_in_length] = c;
      port->payload_in_length += 1;
      } while (c != apa_end);
   port->payload_in_length -= 1;
   }

void apa_put_packet(struct apa_port_type *port) {
   //
   // put packet on port with timeouts
   //
   unsigned char i;
   char c;
   unsigned char return_value;
   //
   // put start
   //
   apa_put_char(port, apa_start, &return_value);
   if (return_value == 0)
      return;
   //
   // put path
   //
   for (i = 0; i < port->path_out_length; ++i)
      apa_put_char(port, port->path_out[i], &return_value);
      if (return_value == 0)
         return;
   port->path_out_length = 0;
   //
   // put divider
   //
   apa_put_char(port, apa_divider, &return_value);
   if (return_value == 0)
      return;
   //
   // put payload
   //
   for (i = 0; i < port->payload_out_length; ++i) {
      //
      // get next char
      //
      c = port->payload_out[i];
      //
      // check if escape needed
      //
      if ((c == apa_end) || (c == apa_escape))
         //
         // yes, put escape
         //
         apa_put_char(port, apa_escape, &return_value);
         if (return_value == 0)
            return;
      //
      //
      // put char
      //
      apa_put_char(port, c, &return_value);
      if (return_value == 0)
         return;
      }
   //
   // put end
   //
   apa_put_char(port, apa_end, &return_value);
   if (return_value == 0)
      return;
   //
   // success, clear packet and return
   //
   port->payload_out_length = 0;
   }

void apa_move_packet(struct apa_port_type *port0, struct apa_port_type *port1) {
   //
   // move packet from port0 in to port1 out
   //
   unsigned char i;
   //
   // copy path
   //
   for (i = 0; i < port0->path_in_length; ++i)
      port1->path_out[i] = port0->path_in[i];
   port1->path_out_length = port0->path_in_length;
   port0->path_in_length = 0;
   //
   // copy payload
   //
   for (i = 0; i < port0->payload_in_length; ++i)
      port1->payload_out[i] = port0->payload_in[i];
   port1->payload_out_length = port0->payload_in_length;
   port0->payload_in_length = 0;
   }

void apa_copy_packet(struct apa_port_type *port0, struct apa_port_type *port1) {
   //
   // copy packet from port0 in to port1 out
   //
   unsigned char i;
   //
   // copy path
   //
   for (i = 0; i < port0->path_in_length; ++i)
      port1->path_out[i] = port0->path_in[i];
   port1->path_out_length = port0->path_in_length;
   //
   // copy payload
   //
   for (i = 0; i < port0->payload_in_length; ++i)
      port1->payload_out[i] = port0->payload_in[i];
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
   if (0 != pin_test(*port->pins_in, port->pin_in)) {
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
            apa_reverse_path(port);
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
         current_port = port->next_port;
         while (current_port->id != port->id) {
            apa_copy_packet(port,current_port);
            current_port = current_port->next_port;
            }
         port->path_in_length = 0;
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
               port->path_in_length = 0;
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
   static unsigned char ptr;
   //
   // find pointer
   //
   ptr = 0;
   while (port->path_in[ptr] != apa_pointer)
      ++ptr;
   //
   // check if pointer reached divider
   //
   if ((ptr+1) == port->path_in_length) {
      //
      // yes, terminate
      //
      port->destination = apa_here;
      }
   else {
      //
      // no, transit
      //
      port->destination = port->path_in[ptr+1];
      port->path_in[ptr] = port->id;
      port->path_in[ptr+1] = apa_pointer;
      }
   }

void apa_reverse_path(struct apa_port_type *port) {
   //
   // reverse output packet path
   //
   static unsigned char i;
   static unsigned char temp[apa_max_packet];
   for (i = 0; i < port->path_out_length; ++i)
      temp[i] = port->path_out[port->path_out_length-i-1];
   for (i = 0; i < port->path_out_length; ++i)
      port->path_out[i] = temp[i];
   }

uint16_t apa_hex_int(unsigned char *ptr) {
   //
   // convert hex word to int
   //
   unsigned char hex_char(unsigned char arg) {
      if (('0' <= arg) && (arg <= '9'))
         return (arg - '0');
      else if (('a' <= arg) && (arg <= 'f'))
         return (10 + arg - 'a');
      else if (('A' <= arg) && (arg <= 'F'))
         return (10 + arg - 'A');
      else
         return 0;
      }
   return ((hex_char(ptr[0]) << 12) + (hex_char(ptr[1]) << 8) + (hex_char(ptr[2]) << 4) + hex_char(ptr[3]));
   }
