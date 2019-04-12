# Flock MSP Transport

The MSP transport sends flock over MSPv2 (MultiWii Serial Protocol,
version 2). For details about the MSP protocol, see the
[MSPv2 specification](https://github.com/iNavFlight/inav/wiki/MSP-V2).

# Communication

Flock messages are wrapped in a MSPv2 payload, using the MSP message
code `MSP2_COMMON_FLOCK = 0x100B`.

Host initiated messages are not possible with MSP, so all messages must be
initiated by the flock device.

For this reason, the `0x01 \(Device Info\)` command must be sent by the flock
device to the host during initialization, until it receives an MSP
response from the host.

Once communication has been established, the flock device should retrieve the
information it needs (GPS coordinates, altitude, speed, etc..) from the FC
using MSP. Please, refer to the MSP implementation(s) that you intend to target
for this purpose.

The flock device should forward all the flock commands defined in the FLOCK
specification as `Device initiated commands` received from the air to the host.

# Host initiated communication

Due to the way MSP is designed, is not possible for the flight controller to
initiate the `0x82` and `0x83` commands, which broadcast data from the host
to the air.

There are ongoing efforts to allow MSP to support this use case, so this
document will be updated as soon as host initiated communication via MSP is
possible.
