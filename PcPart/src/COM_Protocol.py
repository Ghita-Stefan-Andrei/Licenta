from functools import reduce

class InterpretPacket:
    def __init__(self):
        self.individualBytes = []

    def checkHeader(self, header):
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
    
    def checkSumCheck(self, packet): 
        sliceBytes = 2
        self.individualBytes = [int(packet[i:i+sliceBytes], 16) for i in range(0, len(packet), sliceBytes)]
        if reduce(lambda x, y: x ^ y, self.individualBytes) != 0: return -1 #checkSum
        return 1

    def decodeTriggerPacket(self, packet):
        year   = self.individualBytes[3] + 2000;
        month  = self.individualBytes[4]
        day    = self.individualBytes[5]
        hour   = self.individualBytes[6]
        minute = self.individualBytes[7]
        second = self.individualBytes[8] 

        slope  = 'Rising Slope' if self.individualBytes[9] == int('01', 16) else 'Falling Slope' if self.individualBytes[9] == int('10', 16) else ""

        return f"Change detected at {hour:0>2}:{minute:0>2}:{second:0>2} on {day:0>2}/{month:0>2}/{year}\nSlope type: {slope}\nPacket: {packet}\n"

    def decodePacket(self, packet):
        decodedData = ''
        if self.individualBytes[2] == int('AB', 16):
            decodedData = self.decodeTriggerPacket(packet)
        elif self.individualBytes[2] == int('AC', 16):
            decodedData = 'Board booted up'
        
        return decodedData
        