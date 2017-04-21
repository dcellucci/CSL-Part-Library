# Asynchronous Packet Automaton

APA consists of a `PACKET` that consists of a `PATH` and a `PAYLOAD`, and an `AUTOMATON` that parses this packet and decides what to do with it.

## The APA Packet
The `PACKET` begins with a `START VALUE`, a `PATH`, then a `SEPARATOR`, followed by the `PAYLOAD` and the `END VALUE`.

For instance, the packet `{01120310^|Hello, World}` has the following format:  

    START VALUE : {
           PATH : 01120310^
      SEPARATOR : |
        PAYLOAD : Hello, World
      END VALUE : }  

### The `START VALUE`
The `START VALUE` is the first byte in the message. It can either be a fixed value, such as in the example above, or it can be a byte which contains information about the path variable, such as a checksum.

### The `PATH`
The PATH consists of a series of values. Each value can either be a port ID, a position carat, or any number of special flags. It ends with the`SEPARATOR` value.

Therefore, the maximum number of different states delineated by a single PATH value is equal to the sum of the maximum number of ports for any nodes in the APA array, plus one to denote the position carat, plus another state for each special flag, and one final value to denote the `SEPARATOR`.  

Each path value must then be represented by a string of N bits, where N is the smallest number such that 2^N is greater than or equal to the number of states.

The `START_VALUE`lists the value of N-1, encoded into the first two bits of the byte.

## The `PAYLOAD`

The payload is an array of values that then get parsed by the next higher level protocol. Typically, this value runs either until the end of the message, or until the `END VALUE` is reached.

## C Programming

**APA.h** consists of the structs
  1. an incomplete type APAProtocol
  2. an incomplete type APAPort

and the functions:
  3. `static uint8_t getStartValue()`
  4. `static uint8_t getEndValue()`

**APA.c**

APAProtocol is a struct consisting of:
  1. A function pointer to `getStartValue`
  2. an Enum definition for the `PORT` values, including the `SEPARATOR`
  3. A function pointer to `getEndValue()`

APAPort is a struct consisting of:
  1.
