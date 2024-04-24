import serial
from functools import reduce

ser = serial.Serial('COM3', 115200)

def checkHeader(header):
    startByte = header[0:2]
    dataLenByte = header[2:]
    
    retHeader, retStatus, length = 0, 0, 0
    if startByte == 'AA':
        retHeader = header
        retStatus = 1
        length = 2 * (int(dataLenByte, 16) + 1)
    else:
        retHeader, retStatus, length = "", 0, 0
    
    return retHeader, retStatus, length

def checAndParsePacket(packet): #TODO to be split in 2 functions check and parse once all this function will be added to a class
    sliceBytes = 2
    individualBytes = [int(packet[i:i+sliceBytes], 16) for i in range(0, len(packet), sliceBytes)]
    if reduce(lambda x, y: x ^ y, individualBytes) != 0: return -1 #checkSum

    year   = individualBytes[2] + 2000;
    month  = individualBytes[3]
    day    = individualBytes[4]
    hour   = individualBytes[5]
    minute = individualBytes[6]
    second = individualBytes[7] 

    slope  = 'Rising Slope' if individualBytes[8] == int('01', 16) else 'Falling Slope' if individualBytes[8] == int('10', 16) else ""
    
    return f"Change detected at {hour}:{minute}:{second} on {day}/{month}/{year}\nSlope type: {slope}"



while True:
    header = ser.read(4).decode('utf-8')
    packet, status, length = checkHeader(header)
    if status == 1:
        packet += ser.read(length).decode('utf-8')
    
    print(checAndParsePacket(packet))
