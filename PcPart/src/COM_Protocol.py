from functools import reduce
import re
from defines import StatusLog
from defines import ByteDex
from defines import Format

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

    def decodeEthPacket(self, packet):
        ethStatus = self.individualBytes[ByteDex.ETHERNET_STATUS_POS]

        if ethStatus == ByteDex.NOT_CONNECTED: return f"No internet connection found.\nPacket: {packet}\n"

        if ethStatus == ByteDex.CONNECTED:
            ipFirstByte  = self.individualBytes[ByteDex.IP_FIRST_BYTE]
            ipSecondByte = self.individualBytes[ByteDex.IP_SECOND_BYTE]
            ipThirdByte  = self.individualBytes[ByteDex.IP_THIRD_BYTE]
            ipForthByte  = self.individualBytes[ByteDex.IP_FORTH_BYTE]

            return f"Connected to internet.\nIP:{ipFirstByte}.{ipSecondByte}.{ipThirdByte}.{ipForthByte}\nPacket: {packet}\n"


    def decodeTriggerPacket(self, packet):
        year   = self.individualBytes[ByteDex.YEAR] + Format.YEAR_OFFSET;
        month  = self.individualBytes[ByteDex.MONTH]
        day    = self.individualBytes[ByteDex.DAY]
        hour   = self.individualBytes[ByteDex.HOUR]
        minute = self.individualBytes[ByteDex.MINUTE]
        second = self.individualBytes[ByteDex.SECOND]
        milsFH = self.individualBytes[ByteDex.MILS_FH] 
        milsSH = self.individualBytes[ByteDex.MILS_SH] 

        slope = (
                'Rising Slope' if self.individualBytes[ByteDex.SLOPE_BYTE_POSITION] == ByteDex.RISING_SLOPE else
                'Falling Slope' if self.individualBytes[ByteDex.SLOPE_BYTE_POSITION] == ByteDex.FALLING_SLOPE else
                "Error: Slope byte missing/wrong."
                )

        return (
                f"Change detected at {hour:{Format.TIME_DISPLAY_FORMAT}}:"
                f"{minute:{Format.TIME_DISPLAY_FORMAT}}:"
                f"{second:{Format.TIME_DISPLAY_FORMAT}}:"
                f"{milsFH * 100 + milsSH:{Format.TIME_MS_DISPLAY_FORMAT}} on "
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
        
        elif self.individualBytes[ByteDex.TYPE_BYTE_POSITION] == ByteDex.ETHERNET_STATUS_BYTE:
            decodedData = self.decodeEthPacket(packet)

        else:
            decodedData = f"Unknown packet type: {hex(self.individualBytes[ByteDex.TYPE_BYTE_POSITION])[2:].upper()}"
        
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
        try:
            with open(filePath, 'r') as file:
                line = file.readline().strip().split(',')
                com, bitRate = line
        except Exception as e:
            print("Error: Incorrect or missing serial settings file path")
    else:
        print('Invalid method.')
    return com, bitRate