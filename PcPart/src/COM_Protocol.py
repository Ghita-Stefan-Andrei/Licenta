from functools import reduce
import re

class StatusLog:
    WRONG_START_BYTE = False
    RIGHT_START_BYTE = True

    INVALID_CHECK_SUM = False
    VALID_CHECK_SUM   = True

class ByteDex:
    def getByteFromStr(byte):
        return int(byte, 16)
    
    HEADER_LENGTH            = 4 #bytes read from serial
    TYPE_BYTE_POSITION       = 2
    TIME_FIRST_BYTE_POSITION = 3
    SLOPE_BYTE_POSITION      = TIME_FIRST_BYTE_POSITION + 6

    YEAR_OFFSET   = 0
    MONTH_OFFSET  = 1
    DAY_OFFSET    = 2
    HOUR_OFFSET   = 3
    MINUTE_OFFSET = 4
    SECOND_OFFSET = 5

    START_BYTE    = 0xAA
    TRIGGER_BYTE  = 0xAB
    BOOT_BYTE     = 0xAC
    RISING_SLOPE  = 0x01
    FALLING_SLOPE = 0x10

class Format:
    TIME_DISPLAY_FORMAT = '0>2'
    YEAR_OFFSET = 2000

class InterpretPacket:
    def __init__(self):
        self.individualBytes = []

    def checkHeader(self, header):
        startByte = header[0:2]
        dataLenByte = header[2:]

        retHeader, retStatus, length = 0, 0, 0
        if ByteDex.getByteFromStr(startByte) == ByteDex.START_BYTE:
            retHeader = header
            retStatus = StatusLog.RIGHT_START_BYTE
            length = 2 * (ByteDex.getByteFromStr(dataLenByte) + 1)
        else:
            retHeader, retStatus, length = "", StatusLog.WRONG_START_BYTE, 0

        return retHeader, retStatus, length
    
    def checkSumCheck(self, packet): 
        sliceBytes = 2
        self.individualBytes = [ByteDex.getByteFromStr(packet[i:i+sliceBytes]) for i in range(0, len(packet), sliceBytes)]
        if reduce(lambda x, y: x ^ y, self.individualBytes) != 0: return StatusLog.INVALID_CHECK_SUM #checkSum
        return StatusLog.VALID_CHECK_SUM

    def decodeTriggerPacket(self, packet):
        year   = self.individualBytes[ByteDex.TIME_FIRST_BYTE_POSITION + ByteDex.YEAR_OFFSET] + Format.YEAR_OFFSET;
        month  = self.individualBytes[ByteDex.TIME_FIRST_BYTE_POSITION + ByteDex.MONTH_OFFSET]
        day    = self.individualBytes[ByteDex.TIME_FIRST_BYTE_POSITION + ByteDex.DAY_OFFSET]
        hour   = self.individualBytes[ByteDex.TIME_FIRST_BYTE_POSITION + ByteDex.HOUR_OFFSET]
        minute = self.individualBytes[ByteDex.TIME_FIRST_BYTE_POSITION + ByteDex.MINUTE_OFFSET]
        second = self.individualBytes[ByteDex.TIME_FIRST_BYTE_POSITION + ByteDex.SECOND_OFFSET] 

        slope = (
                'Rising Slope' if self.individualBytes[ByteDex.SLOPE_BYTE_POSITION] == ByteDex.RISING_SLOPE else
                'Falling Slope' if self.individualBytes[ByteDex.SLOPE_BYTE_POSITION] == ByteDex.FALLING_SLOPE else
                "Error: Slope byte missing/wrong."
                )

        return (
                f"Change detected at {hour:{Format.TIME_DISPLAY_FORMAT}}:"
                f"{minute:{Format.TIME_DISPLAY_FORMAT}}:"
                f"{second:{Format.TIME_DISPLAY_FORMAT}} on "
                f"{day:{Format.TIME_DISPLAY_FORMAT}}/"
                f"{month:{Format.TIME_DISPLAY_FORMAT}}/"
                f"{year}\n"
                f"Slope type: {slope}\n"
                f"Packet: {packet}\n"
               )

    def decodePacket(self, packet):
        decodedData = ''
        if self.individualBytes[ByteDex.TYPE_BYTE_POSITION] == ByteDex.TRIGGER_BYTE:
            decodedData = self.decodeTriggerPacket(packet)

        elif self.individualBytes[ByteDex.TYPE_BYTE_POSITION] == ByteDex.BOOT_BYTE:
            decodedData = f'Board booted up\nPacket: {packet}\n'
        
        return decodedData

def getSerialSettings(consoleXfile, filePath):
    com, bitRate = '', 0
    if consoleXfile == 'keyboard':
        while True:
            comIn = input("Enter com number (COMx): ").upper()
            if re.match(r'^COM\d+$', comIn):
                com = comIn
                break
            else:
                print("Input doesn't respect the COMx format.")

        bitRate = int(input('Insert bit rate:'))
    elif consoleXfile == 'file':
        #basePath = os.path.dirname(__file__)
        #filePath = os.path.join(basePath[:-3], 'serialSettings.txt')
        try:
            with open(filePath, 'r') as file:
                line = file.readline().strip().split(',')
                com, bitRate = line
        except Exception as e:
            print("Error: Incorrect or missing serial settings file path")
    else:
        print('Invalid method.')
    return com, bitRate