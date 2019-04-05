# Flock Serial Protocol

This document describes version 1 of the protocol used by a
FLOCK device to communicate with its host (usually a flight
controller) via serial communication.

## UART parameters

Protocol is based on full duplex uart at 3.3v logic level, 8 bits,
no parity and 1 stop bit at 115200bps, also known as 115200/8-N-1.

## General command structure

Each frame starts with the "\ffF" (0xff, 0x46) header
followed by:

- uint8_t: Packet length, not counting the 2 byte header.
- uint8_t: Command identifier.
- A payload which depends on the command.
- Finally, a CRC8 checksum covering all packet except the using
the 0xD5 polynomial and 0 as the initial value (also known as CRC
DVB S2).

Most commands are initiated by the host. The host sends a command to the
device with an identifier X and, depending on the command,
the device will either do nothing of respond with a frame with the
same command identifier that the host used.

Device initiated commands must no generate any response from the
host.

All payload definitions that contain structs should be interpreted
as packed.

## Host initiated commands

These commands are sent by the host to the device. Host should be
ready to handle responses to the commands that have it.

### 0x01 - Device info

Request: `empty`

Response: ```struct {
    uint8_t flock_version;
    char flock_device_name[12];
    uint8_t flock_device_version[3];
    typedef struct {
            uint8_t flock_device_addr[6];
    } flock_device_addr_t;
    uint8_t flock_radio_type;
    uint64_t flock_radio_min_freq;
    uint64_t flock_radio_max_freq;
    uint64_t flock_radio_default_freq;
}```


Where:

- `flock_version`: Version of the FLOCK protocol implemented by the device
- `flock_device_name`: User readable name of the device, null terminated.
- `flock_device_version`: Flock device version as a triplet of (major, minor, patch). Version semantics are defined by
each device.
- `flock_device_addr`: Unique (or pseudo-unique) device identifier. Devices might use a permanent ID generated during fabrication (e.g. a Chip ID or a MAC address), but they can also use a PRNG to
generate this an address and store it in a eeprom.
- `flock_radio_type`: Defines the radio type. Valid values are:
    - 1: LoRa sub-GHz (i.e. SX127x or SX126X)
    - 2: LoRa 2G4 (SX128X)
- `flock_radio_min_freq`: Minimum frequency supported by the device, in Hz.
- `flock_radio_max_freq`: Maximum frequency supported by the device, in Hz.
- `flock_radio_default_freq`: Frequency used by the device at
startup.

### 0x02 - Get current frequency

Request: `empty`

Response: ```uint64_t flock_radio_current_freq```

Where:

- `flock_radio_current_freq`: Frequency used by the device for
listening to and broadcasting messages.

### 0x03 - Set current frequency

Request: ```uint64_t flock_radio_current_freq```

Where:

- `flock_radio_current_freq`: Frequency used by the device for
listening to and broadcasting messages.

Response: `empty`

### 0x04 - Get host info

Request: `empty`

Response: ```typedef struct {
    uint8_t type;
    char name[17];
} flock_hostinfo_t```

Where:

- `type`: Host type, where the following values are valid:
  - 1: Remote controlled quadcopter.
  - 2: Remote controlled airplane.
  - 3: Remote controlled car.
  - 4: Remote controlled boat.
- `name`: Null terminated arbitrary string representing the pilot
or craft name. Other FLOCK nodes will see this name. Note that the payload
length is always 17 and must be null terminated, resulting on a maximum name
length of 16 characters.

### 0x05 - Set host info

Request: ```struct flock_hostinfo_t hostinfo```

Where:

- `hostinfo`: Host information, as defined in `Get host info`.

Response: `empty`

### 0x06 - Set host posvel

This command should be called periodically by the host to tell
the device its current coordinates, altitude, heading and speed.

Request: ```typedef struct {
    int32_t latitude;
    int32_t longitude;
    uint16_t altitude;
    uint16_t ground_speed;
    int16_t vertical_speed;
    uint16_t heading;
} flock_posvel_t```

Where:

- `latitude`: Latitude mapped from [-90, 90] to [0, 2^32-1]
- `longitude`: Longitude mapped from [-180, 180] to [0, 2^32-1]
- `altitude`: Altitude above Mean Sea Level in meters, with a
+10,000m offset (e.g. 0m above MSL is represented as 10,000)
- `ground_speed`: Ground speed in m/s * 10
- `vertical_speed`: Vertical speed in m/s * 10. Positive values
represent speeds in the up direction.
- `heading`: Heading as radians * 10000, normalized to the [0, 2*pi) range.

Response: `empty`

### 0x07 - Broadcast data

Request: ```uint8_t data[L]```

Where:

`data`: Arbitrary data to be broadcasted. The maximum length of 
the payload is 55 bytes. Payloads larger than that will be
silently ignored.

Response: `empty`

### 0x08 - Send data

Request: ```struct {
    flock_device_addr_t dest_addr;
    uint8_t data[L];
}```

Where:

`dest_addr`: Destination address for the data. Devices not 
matching this address will ignore the data.
`data`: Arbitrary data to be broadcasted. The maximum length of 
the payload is 48 bytes. Payloads larger than that will be
silently ignored.

Response: `empty`

## Device initiated commands

### 0x80 - Received remote posvel

Request: ```struct {
    flock_device_addr_t from_addr;
    flock_posvel_t posvel;
}```

Where:

`from_addr`: Address of the device sending its posvel
`posvel`: flock_posvel_t for the sending device.

Response: `empty`

### 0x81 - Received broadcast data

Request: ```struct {
    flock_device_addr_t from_addr;
    uint8_t data[L];
}```

Where:

`from_addr`: Source address for device sending the data.
`data`: Arbitrary data sent by the device. Host devices should be ready to
handle payloads up to 64 bytes.

Response: `empty`

### 0x82 - Received sent data

Request: ```struct {
    flock_device_addr_t from_addr;
    uint8_t data[L];
}```

`from_addr`: Source address for device sending the data.
`data`: Arbitrary data sent by the device. Host devices should be ready to
handle payloads up to 64 bytes.

Response: `empty`


## Guidelines implementing a FLOCK host

Flock hosts should assume future versions of the FLOCK-SERIAL
protocol will preserve backwards compatibility. No breaking
changes to the protocol will be allowed.

Hosts must ensure forward compatibility by relying only on
version/feature checks and/or testing for minimum payload lengths
for the data they intend to read (as opposed to testing for their
exact lengths). This lets new protocol versions introduce
additional fields in existing commands while preserving
interoperability.

Flock hosts should never assume a device supports non-volatile
storage.
