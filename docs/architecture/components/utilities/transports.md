# CRC-R: Transports

## Responsibilities

TBD

## Requirements

TBD

## Collaborators

TBD

## Rationale

TBD

## Source Links

TBD

## Notes

Do transports and streams need to be split up?


Transport = the mechanism of moving data across the buses. Hardware level.

Stream - the data format as it's going across the transport
Bytestream
Datagram
UDPErasureEncoding

Buffered/Non-buffered transport concepts? or stream?

Transports:
* BytestreamTransport
    * UsbBytestream?
* DatagramTransport
* UDPErasureEncodingTransport
* SocketTransport
* BufferedStreamTransport
* BufferedTransport

Protocols:
* InputBufferedStreamProtocol



* Transport classes:
    *  UART
    *  USB
    *  SPI
    *  BT
    *  I2C
    *  NFC
    *  Wifi
    *  RFID
* Buffered Decorator
    *  Non-buffered decorator


* Stream abstract class with subclasses
    *  Memory stream
    *  File stream
    *  byte stream
    *  Decorators for streams:
        *  Compress
        *  Decompress
        *  Base64 encode / decode
        *  Encrypt/decrypt


Byte transport/stream (other transports/serializers?)
