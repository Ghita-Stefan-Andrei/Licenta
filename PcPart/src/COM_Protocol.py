from functools import reduce
from defines import StatusLog
from defines import ByteDex
from defines import Format

class InterpretPacket:
    def __init__(self):
        self.individualBytes = []

    def findStartByte(self, ser):
        """
        Finds the start byte of a packet in the serial communication.
    
        Args:
            ser: The serial communication object.
    
        Returns:
            bool: True if the start byte is found, False otherwise.
        """
        found = False
        while True:
            byte = ser.read(1)
            if byte == b'':
                found = False
                break  
            if byte.decode('utf-8', errors='ignore') == 'A':  
                second_byte = ser.read(1)
                if second_byte.decode('utf-8', errors='ignore') == 'A':
                    found = True
                    break
                found = False
                break
        return found

    def checkHeader(self, header):
        """
        This function checks the header of a received packet.

        Args:
            header (str): The header of the received packet.

        Returns:
            tuple: A tuple containing the following:
                - The header.
                - The status of the check (RIGHT_START_BYTE, WRONG_START_BYTE).
                - The length of the packet, in bytes.
        """
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
        """
        This function performs a checksum check on the packet.
        And separates the bytes of the packet in a list of bytes. 

        Args:
            packet (str): The packet on which the checksum check needs to be performed.

        Returns:
            str: Returns StatusLog.INVALID_CHECK_SUM if the checksum is invalid, 
                 otherwise returns StatusLog.VALID_CHECK_SUM.
        """
        sliceBytes = 2
        self.individualBytes = [ByteDex.getByteFromStr(packet[i:i+sliceBytes]) for i in range(0, len(packet), sliceBytes)]
        if reduce(lambda x, y: x ^ y, self.individualBytes) != 0: return StatusLog.INVALID_CHECK_SUM #checkSum
        return StatusLog.VALID_CHECK_SUM

    def decodeEthPacket(self, packet):
        """
        Decodes the Ethernet packet that is sent when the board first connection to ethernet is tried and returns the status and IP address.

        Args:
            packet (str): The packet to be easly displayed.

        Returns:
            str: Returns a message indicating the internet connection status and the IP address if connected.
                 If not connected, returns a message indicating no internet connection found.
        """
        ethStatus = self.individualBytes[ByteDex.ETHERNET_STATUS_POS]

        if ethStatus == ByteDex.NOT_CONNECTED: return f"No ethernet connection found.\nPacket: {packet}\n"

        if ethStatus == ByteDex.CONNECTED:
            ipFirstByte  = self.individualBytes[ByteDex.IP_FIRST_BYTE]
            ipSecondByte = self.individualBytes[ByteDex.IP_SECOND_BYTE]
            ipThirdByte  = self.individualBytes[ByteDex.IP_THIRD_BYTE]
            ipForthByte  = self.individualBytes[ByteDex.IP_FORTH_BYTE]

            return f"Conectat la ethernet.\nIP:{ipFirstByte}.{ipSecondByte}.{ipThirdByte}.{ipForthByte}\nPachet: {packet}\n"


    def decodeTriggerPacket(self, packet, outputType):
        """
        Decodes the trigger packet and extracts relevant information such as timestamp and slope type.

        Args:
            packet (str): The packet to be easly displayed.

        Returns:
            str: A formatted string containing the timestamp and slope type.
        """
        year   = self.individualBytes[ByteDex.YEAR] + Format.YEAR_OFFSET;
        month  = self.individualBytes[ByteDex.MONTH]
        day    = self.individualBytes[ByteDex.DAY]
        hour   = self.individualBytes[ByteDex.HOUR]
        minute = self.individualBytes[ByteDex.MINUTE]
        second = self.individualBytes[ByteDex.SECOND]
        milsFH = self.individualBytes[ByteDex.MILS_FH] 
        milsSH = self.individualBytes[ByteDex.MILS_SH] 

        if outputType == "csv":
            slope = (
                    '1' if self.individualBytes[ByteDex.SLOPE_BYTE_POSITION] == ByteDex.RISING_SLOPE else
                    '0' if self.individualBytes[ByteDex.SLOPE_BYTE_POSITION] == ByteDex.FALLING_SLOPE else
                    "-1"
                    )
            return [
                f"{day:{Format.TIME_DISPLAY_FORMAT}}",
                f"{month:{Format.TIME_DISPLAY_FORMAT}}",
                f"{year}",
                f"{hour:{Format.TIME_DISPLAY_FORMAT}}",
                f"{minute:{Format.TIME_DISPLAY_FORMAT}}",
                f"{second:{Format.TIME_DISPLAY_FORMAT}}",
                f"{milsFH * 100 + milsSH:{Format.TIME_MS_DISPLAY_FORMAT}}",
                f"{slope}"
               ]
        else:
            slope = (
                    'Panta crescatoare' if self.individualBytes[ByteDex.SLOPE_BYTE_POSITION] == ByteDex.RISING_SLOPE else
                    'Panta descrescatoare' if self.individualBytes[ByteDex.SLOPE_BYTE_POSITION] == ByteDex.FALLING_SLOPE else
                    "Eroare: Octetul de panta lipseste/invalid."
                    )
            return (
                f"Schimbare detectata la {hour:{Format.TIME_DISPLAY_FORMAT}}:"
                f"{minute:{Format.TIME_DISPLAY_FORMAT}}:"
                f"{second:{Format.TIME_DISPLAY_FORMAT}}:"
                f"{milsFH * 100 + milsSH:{Format.TIME_MS_DISPLAY_FORMAT}} on "
                f"{day:{Format.TIME_DISPLAY_FORMAT}}/"
                f"{month:{Format.TIME_DISPLAY_FORMAT}}/"
                f"{year}\n"
                f"Tipul pantei: {slope}\n"
                f"Pachet: {packet}\n"
               )
      
    def decodeEthCheckPacket(self, packet):
        """
        Decodes the Ethernet check packet and determines the status of the Ethernet connection.

        Args:
            packet (str): The packet to be easily displayed.

        Returns:
            str: A message indicating the status of the Ethernet connection and the packet details.
                 If connected, returns 'Connected to ethernet' along with the packet details.
                 If disconnected, returns 'Disconnected from ethernet' along with the packet details.
                 If the status is unknown, returns 'Unknown' along with the packet details.
        """
        status = ''
        if self.individualBytes[ByteDex.ETHERNET_CHECK_POS] == ByteDex.CONNECTED:
            status = f'Conectat la ethernet\nPachet: {packet}\n'
        elif  self.individualBytes[ByteDex.ETHERNET_CHECK_POS] == ByteDex.NOT_CONNECTED:
            status = f'Deconectat de la ethernet\nPachet: {packet}\n'
        else:
            status = f'Necunoscut\nPachet: {packet}\n'
        
        return status
        
    def decodeNTPStatusPacket(self, packet):
        status = ''

        if self.individualBytes[ByteDex.NTP_STATUS_BYTE_POSITION] == ByteDex.NTP_STATUS_UPDATED:
            status = f'Clientul NTP este actualizat.\nPachet: {packet}\n'
        elif  self.individualBytes[ByteDex.NTP_STATUS_BYTE_POSITION] == ByteDex.NTP_STATUS_NOT_UPDATED:
            status = f'Clientul NTP nu a fost acutalizat cand a fost detectata o schimbare a semnalului.\nPachet: {packet}\n'

        return status

    def decodePacket(self, packet, outputType):
        """
        Decodes the packet based on its type and returns the decoded data.

        Args:
            packet (str): The packet to be decoded.

        Returns:
            str: A formatted string containing the decoded information based on the packet type.
                 If the packet type is a trigger byte, it returns the timestamp and slope type.
                 If the packet type is a boot byte, it returns a message indicating that the board booted up.
                 If the packet type is an Ethernet status byte, it returns the internet connection status and IP address.
                 If the packet type is an Ethernet status check, it returns the status of the Ethernet connection.
                 If the packet type is unknown, it returns a message indicating the unknown packet type.
        """
        decodedData = ''
        justTime = ''
        if self.individualBytes[ByteDex.TYPE_BYTE_POSITION] == ByteDex.TRIGGER_BYTE:
            if outputType == "csv":
                decodedData = ''
                justTime = self.decodeTriggerPacket(packet, outputType)
            else:
                decodedData = self.decodeTriggerPacket(packet, outputType)
                justTime = ''

        elif self.individualBytes[ByteDex.TYPE_BYTE_POSITION] == ByteDex.BOOT_BYTE:
            decodedData = f'Placa a intrat in functia setup, asteptati aprinderea ledului albastru\nPachet: {packet}\n'
        
        elif self.individualBytes[ByteDex.TYPE_BYTE_POSITION] == ByteDex.ETHERNET_STATUS_BYTE:
            decodedData = self.decodeEthPacket(packet)

        elif self.individualBytes[ByteDex.TYPE_BYTE_POSITION] == ByteDex.ETHERNET_STATUS_CHECK:
            decodedData = self.decodeEthCheckPacket(packet)

        elif self.individualBytes[ByteDex.TYPE_BYTE_POSITION] == ByteDex.NTP_STATUS_CHECK:
            decodedData = self.decodeNTPStatusPacket(packet)

        else:
            decodedData = f"Tip de pachet necunoscut: {hex(self.individualBytes[ByteDex.TYPE_BYTE_POSITION])[2:].upper()}"
        
        return decodedData, justTime

def getSerialSettings(consoleXfile, filePath):
    """
    Retrieves the serial settings based on the input method and file path.

    Args:
        consoleXfile (str): The method of input for serial settings ('keyboard' or 'file').
        filePath (str): The file path for serial settings if the method is 'file'.

    Returns:
        tuple: A tuple containing the following:
            - The COM port number in the format "COM{com}".
            - The bit rate for the serial communication.
    """
    com, bitRate = '', 0
    if consoleXfile == 'keyboard':
        while True:
            comIn = input("Enter com number: ")
            if comIn.isdigit():
                com = comIn
                break
            else:
                print("Input is not a valid number.")

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
    return f"COM{com}", bitRate