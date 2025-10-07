import os
import asyncio
import serial_asyncio
from datetime import datetime, timezone, timedelta
import numpy as np
import struct
from questdb.ingress import Sender, TimestampNanos, IngressError


def bytes2Float(bytes_array):
    """
    Convert a sequence of 4 bytes to a float using little-endian format.

    Parameters:
        bytes_array (bytes): A sequence of 4 bytes representing a float.

    Returns:
        float: The converted floating-point number.
    """
    return struct.unpack('<f', bytes_array)[0]

def bytes2u16Int(bytes_array):
    """
    Convert a sequence of 2 bytes to an unsigned 16-bit integer using little-endian format.

    Parameters:
        bytes_array (bytes): A sequence of 2 bytes representing an unsigned 16-bit integer.

    Returns:
        int: The converted integer.
    """
    return struct.unpack('<H', bytes_array)[0]

def u16_to_i16(u):
    """Convert unsigned 16-bit to signed 16-bit."""
    return u - 65536 if u > 32767 else u

def calculateCRC8(opCode, dataLength, data):
    """
    Calculate the CRC8 for an incoming UART packet.

    Parameters:
        opCode (int): The operation code of the packet.
        dataLength (int): The length of the data payload.
        data (bytes): The data payload.

    Returns:
        int: The calculated CRC8 value.
    """
    crcTable = [
        0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31,
        0x24, 0x23, 0x2A, 0x2D, 0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
        0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D, 0xE0, 0xE7, 0xEE, 0xE9,
        0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
        0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1,
        0xB4, 0xB3, 0xBA, 0xBD, 0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
        0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA, 0xB7, 0xB0, 0xB9, 0xBE,
        0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
        0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16,
        0x03, 0x04, 0x0D, 0x0A, 0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
        0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A, 0x89, 0x8E, 0x87, 0x80,
        0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
        0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8,
        0xDD, 0xDA, 0xD3, 0xD4, 0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
        0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44, 0x19, 0x1E, 0x17, 0x10,
        0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
        0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F,
        0x6A, 0x6D, 0x64, 0x63, 0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
        0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13, 0xAE, 0xA9, 0xA0, 0xA7,
        0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
        0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF,
        0xFA, 0xFD, 0xF4, 0xF3
    ]

    crc = 0x00  # Initialize CRC value

    # Update CRC with the opcode
    crc = crcTable[crc ^ opCode]

    # Update CRC with the data length
    crc = crcTable[crc ^ dataLength]

    # Update CRC with each data byte
    for i in range(dataLength):
        crc = crcTable[crc ^ data[i]]

    return crc

class PsocProtocol(asyncio.Protocol):
    def connection_made(self, transport):
        self.transport = transport
        print("Port opened", transport)
        self.data = bytearray()
        self.lines = []

    def process_data(self):
        opCode = self.data[0]
        dataLength = self.data[1]

        if opCode == 0x0B:  # testSuccess: startup banner from PSoC
            print("UART started.")
            return

        if dataLength != 40:
            print(f"Warning: Unexpected data length {dataLength}, expecting 40 (6 float32 + 8 int16).")
            return
        
        if opCode == 0xF1:
            print("Encryption Driver Error: PSOC looped. Please power-cycle the PSOC.")
            return


        data = self.data[2:2 + dataLength]
        receivedCRC = self.data[2 + dataLength]
        
        # print("No encryption error")

        # Decrypt the received packet
        # ******************************** #
        # Note: for Oct 30 day sheep study, we disable Crypto
        # ******************************** #
        # decrypted_packet = bytearray()
        # for i in range(0, dataLength, 16):
        #     block = data[i:i + 16]
        #     if len(block) < 16:
        #         # Padding if necessary
        #         block += bytes(16 - len(block))
        #     decrypted_block = cipher.decrypt(bytes(block))
        #     decrypted_packet.extend(decrypted_block[:len(block)])

        # Calculate CRC8
        calculatedCRC = calculateCRC8(opCode, dataLength, data)
        # Check if the received CRC matches the calculated one
        if receivedCRC != calculatedCRC:
            print("CRC check failed.")
            return
        
        timestamp = datetime.now(timezone.utc)
        try:
            # 1) First 6 floats (24 bytes total)
            adc0, adc1, adc2, adc3, fake_adc0, fake_adc1 = struct.unpack('<6f', data[:24])

            # 2) Next 8 int16 (16 bytes)
            uart_u16 = [bytes2u16Int(data[24+i:24+i+2]) for i in range(0, 16, 2)]
            uart_i16 = [u16_to_i16(v) for v in uart_u16]

            self.lines.append([timestamp, opCode] + list(struct.unpack('<6f', data[:24])) + uart_i16)

            if opCode == 0xF2:
                print("WARNING: ADC out of range.")
            elif opCode == 0xF3:
                print("WARNING: ADC jumping.")
        
        except struct.error as e:
            print(f"Unpack error: {e}.")

    def data_received(self, data):
        """
        Process the received data packet.

        Returns:
            None
        """
        self.data.extend(data)
        buffer_len = len(self.data)
        if buffer_len < 1 or buffer_len < self.data[1] + 3:
            return
        self.process_data()
        self.data = bytearray()
    
    def pause_reading(self):
        self.transport.pause_reading()

    def resume_reading(self):
        self.transport.resume_reading()

    def get_lines(self):
        lines = self.lines[:]
        self.lines = []
        return lines

    def connection_lost(self, exc):
        print("Port closed")
        self.transport.loop.stop()

class ArduinoProtocol(asyncio.Protocol):
    def connection_made(self, transport):
        self.transport = transport
        print("Port opened", transport)
        self.data = None
        self.lines = []


    def data_received(self, data):
        if b'\n' in data:
            if self.data is None:
                self.data = b""
                return
        try:
            if self.data is not None:
                self.data += data
                loc = self.data.find(b'\r\n')
                while loc >= 0:
                    line = self.data[:loc].decode('utf-8').split(',')
                    line = [int(v) for v in line]
                    line = [datetime.now(timezone.utc)] + line
                    self.lines.append(line)
                    self.data = self.data[loc+2:]
                    loc = self.data.find(b'\r\n')
        except Exception as e:
            self.data = None
            print(e)

    
    def pause_reading(self):
        self.transport.pause_reading()

    def resume_reading(self):
        self.transport.resume_reading()

    def get_lines(self):
        lines = self.lines[:]
        self.lines = []
        return lines

    def connection_lost(self, exc):
        print("Port closed")
        self.transport.loop.stop()

async def reader(protocol_type, db_name, columns, prefix, log_directory, port, baudrate):
    current_log_path = None
    fileio = None
    transport, protocol = await serial_asyncio.create_serial_connection(
            loop,
            protocol_type,
            port,
            baudrate,
    )

    
    while True:
        await asyncio.sleep(1)
        filepath = generate_filename(prefix, log_directory)
        if current_log_path != filepath:
            print("Writing to" + filepath)
            if fileio is not None:
                fileio.close()
            fileio = open(filepath, 'a+')
            current_log_path = filepath
        lines = protocol.get_lines()
        sent = "Sent"
        try:
            with Sender.from_env() as sender:
                for line in lines:
                    sender.row(
                        db_name,
                        columns={col: line[i+1] for (i,col) in enumerate(columns)},
                        at=TimestampNanos.from_datetime(line[0]),
                    )
                sender.flush()
        except IngressError as e:
            print(e)
            sent = str(e.code)

        for line in lines:
            # print(line[0].strftime("%M:%S.%f"), line[1:])
            fileio.write(line[0].isoformat())
            for val in line[1:]:
                fileio.write("," + str(val))
            fileio.write("," + sent)
            fileio.write("\n")
                 
        protocol.resume_reading()
    
    fileio.close()

def generate_filename(prefix, log_directory):
    """
    Generate a filename based on the current date and hour.

    Returns:
        str: The full path to the log file.
    """
    # Ensure the directory exists; create it if it doesn't
    if not os.path.exists(log_directory):
        os.makedirs(log_directory)
        print(f"Created directory: {log_directory}")
    current_time = datetime.now(timezone.utc).isoformat()[:11]
    # current_time = time.strftime("%Y-%m-%d_%H")  # e.g., 2024-10-31_16
    filename = f"{prefix}_data_log_{current_time}.txt"
    return os.path.join(log_directory, filename)

if __name__ == "__main__":
    import argparse
    import json
    parser = argparse.ArgumentParser(description='Record to questdb and text file')
    parser.add_argument('-c', '--config', required=True, type=str, help='configuration json file')
    args = parser.parse_args()
    with open(args.config, 'r') as f:
        config = json.load(f)

    protocol_type = None
    if config["protocol"] == "arduino":
        protocol_type = ArduinoProtocol
    elif config["protocol"] == "psoc":
        protocol_type = PsocProtocol
    else:
        raise NotImplementedError(f"Protocol {config['protocol']} not defined")

    loop = asyncio.get_event_loop()
    transport, protocol = loop.run_until_complete(reader(
        protocol_type,
        config["db_name"],
        config["columns"],
        config["prefix"],
        config["log_directory"],
        config["sensor_port"],
        config["sensor_baudrate"],
    ))
    loop.close()
