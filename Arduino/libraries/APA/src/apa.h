//
//
// apa.h
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

//
// includes
//

#include <util/delay.h>

//
// defines
//

#define apa_max_packet 10 // maximum path and payload size

#define apa_start '{' // packet start
#define apa_pointer '^' // packet path pointer
#define apa_divider '|' // packet payload divider
#define apa_end '}' // packet end
#define apa_escape '\\' // packet end escape

#define apa_flood 'F' // flood destination
#define apa_here 'Z' // terminal destination

//
// structs
//

struct apa_port_type {
    unsigned char path_in[apa_max_packet];
    unsigned char path_out[apa_max_packet];
    unsigned char path_in_length, path_out_length;
    unsigned char payload_in[apa_max_packet];
    unsigned char payload_out[apa_max_packet];
    unsigned char payload_in_length, payload_out_length;
    char destination;
    char id;
    struct apa_port_type *next_port;
};

//
// prototypes
//

void apa_process_packet(struct apa_port_type *port);
void apa_get_packet(struct apa_port_type *port);
void apa_put_packet(struct apa_port_type *port);
void apa_move_packet(struct apa_port_type *port0, struct apa_port_type *port1);
void apa_copy_packet(struct apa_port_type *port0, struct apa_port_type *port1);
void apa_port_scan(struct apa_port_type *port);
void apa_route_packet(struct apa_port_type *port);
