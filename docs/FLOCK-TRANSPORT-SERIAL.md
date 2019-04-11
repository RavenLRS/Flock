# Flock Serial Transport

This document defines the Flock Serial transport, which carries
Flock over an UART connection.

See [FLOCK] for the Flock protocol definition.

## UART parameters

Protocol is based on full duplex uart at 3.3v logic level, 8 bits,
no parity and 1 stop bit at 115200bps, also known as 115200/8-N-1.

## Packet structure

Each frame starts with the "\ffF" (0xff, 0x46) header
followed by:

- Total packet length as uint8_t, not counting the 2 byte header.
- A blob which includes both the the command and its payload.
- Finally, a CRC8 checksum covering all packet except the 2 byte header,
using the 0xD5 polynomial and 0 as the initial value (also known as CRC
DVB S2).

[FLOCK]: FLOCK.md
