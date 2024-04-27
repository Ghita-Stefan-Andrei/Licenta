import serial
from COM_Protocol import InterpretPacket as InP

ser = serial.Serial('COM3', 115200)

decoder = InP()

while True:
    packData = ''
    header = ser.read(4).decode('utf-8')
    packet, status, length = decoder.checkHeader(header)
    if status == 1:
        packet += ser.read(length).decode('utf-8')
        if decoder.checkSumCheck(packet) == 1:
            print(decoder.decodePacket(packet))
        elif decoder.checkSumCheck(packet) == -1:
            print('Error: Invalid check sum')
    elif status == -1:
        print(f'Error: Invalid start byte, supposed to be \'AA\', but received {header[0:2]}')
