from COM_Protocol import InterpretPacket as InP
from COM_Protocol import StatusLog
from COM_Protocol import ByteDex
from COM_Protocol import getSerialSettings
import serial
import CLArguments
import csv

args = CLArguments.createCLArguments()

com, bitRate = getSerialSettings(args.source, args.filePath)

decoder = InP()

ser = serial.Serial(com, bitRate)
print(f'Serial configured on {com} with {bitRate} bitrate.')
print('Listening...')

while True:
    #Reads characters one by one from the serial stream, and returns a header when byte 0xAA is found. 
    try:
        if decoder.findStartByte(ser):
            header = 'AA' + ser.read(ByteDex.HEADER_LENGTH - ByteDex.HEAD_BYTE).decode('utf-8')
        else: continue  

    except Exception as e:
        print(f'Error decoding header: {e}')
        continue
    
    #Validates header, creating a packet in which the rest of the message will be stored if the status is True ans a length variable that will be used to read the remaining bytes. 
    packet, status, length = decoder.checkHeader(header)

    if status == StatusLog.RIGHT_START_BYTE:
        #reads the remaining bytes and adds them to the packet
        try:
            packet += ser.read(length).decode('utf-8')
        except Exception as e:
            print(f'Error reading packet data: {e}')
            continue
        
        #if the checksum is valid, decode the packet and output the information sent by the board
        if decoder.checkSumCheck(packet) == StatusLog.VALID_CHECK_SUM:
            msg, justTime = decoder.decodePacket(packet)
            print(msg)
            with open("C:\\Users\\Asus\\Desktop\\ProiectLicenta\\Measurement\\triggerTime.csv", "a", newline="") as csvFile:
                csvWriter = csv.writer(csvFile)
                csvWriter.writerow(justTime)

        elif decoder.checkSumCheck(packet) == StatusLog.INVALID_CHECK_SUM:
            print('Error: Invalid check sum')

    elif status == StatusLog.WRONG_START_BYTE:
        print(f'Error: Invalid start byte, supposed to be \'AA\', but received {header[0:2]}')

    