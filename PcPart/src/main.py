import serial

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

while True:
    header = ser.read(4).decode('utf-8')
    packet, status, length = checkHeader(header)
    if status == 1:
        packet += ser.read(length).decode('utf-8')
    
    