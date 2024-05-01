from COM_Protocol import InterpretPacket as InP
from COM_Protocol import StatusLog
from COM_Protocol import ByteDex
from COM_Protocol import getSerialSettings
import serial
import CLArguments

args = CLArguments.createCLArguments()

com, bitRate = getSerialSettings(args.source, args.filePath)

decoder = InP()

ser = serial.Serial(com, bitRate)
print(f'Serial configured on {com} with {bitRate} bitrate.')
print('Listening...')

while True:
    header = ser.read(ByteDex.HEADER_LENGTH).decode('utf-8')
    packet, status, length = decoder.checkHeader(header)

    if status == StatusLog.RIGHT_START_BYTE:
        packet += ser.read(length).decode('utf-8')

    elif status == StatusLog.WRONG_START_BYTE:
        print(f'Error: Invalid start byte, supposed to be \'AA\', but received {header[0:2]}')

    if decoder.checkSumCheck(packet) == StatusLog.VALID_CHECK_SUM:
        print(decoder.decodePacket(packet))

    elif decoder.checkSumCheck(packet) == StatusLog.INVALID_CHECK_SUM:
        print('Error: Invalid check sum')