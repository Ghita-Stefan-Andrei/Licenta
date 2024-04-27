import serial
from COM_Protocol import InterpretPacket as InP
from COM_Protocol import StatusLog

decoder = InP(115200)

#ser = serial.Serial(, 115200)
ser = serial.Serial('COM3', 115200)

while True:
    packData = ''
    header = ser.read(4).decode('utf-8')
    packet, status, length = decoder.checkHeader(header)

    if status == StatusLog.RIGHT_START_BYTE:
        packet += ser.read(length).decode('utf-8')

        if decoder.checkSumCheck(packet) == StatusLog.VALID_CHECK_SUM:
            print(decoder.decodePacket(packet))

        elif decoder.checkSumCheck(packet) == StatusLog.INVALID_CHECK_SUM:
            print('Error: Invalid check sum')

    elif status == StatusLog.WRONG_START_BYTE:
        print(f'Error: Invalid start byte, supposed to be \'AA\', but received {header[0:2]}')
