class StatusLog:
    WRONG_START_BYTE = False  #Flags to mark the status of the first byte
    RIGHT_START_BYTE = True   #Flags to mark the status of the first byte

    INVALID_CHECK_SUM = False #Flags to mark the status of the check sum byte
    VALID_CHECK_SUM   = True  #Flags to mark the status of the check sum byte

class ByteDex:
    def getByteFromStr(byte):
        return int(byte, 16)
    
    HEADER_LENGTH            = 4                                           #bytes read from serial
    HEAD_BYTE                = 2                                           #to calculate how much bytes from the header needs to be read
    TYPE_BYTE_POSITION       = 2                                           #Position of the type byte in the pack
    TIME_FIRST_BYTE_POSITION = 3                                           #Pointer to the first position of the time data array in the pack
    TOTAL_TIME_BYTES         = 8                                           #Number of time data bytes in the pack
    SLOPE_BYTE_POSITION      = TIME_FIRST_BYTE_POSITION + TOTAL_TIME_BYTES #Position of the slope type byte

    YEAR    = TIME_FIRST_BYTE_POSITION + 0 #Position of the byte that tells the year in raport to TIME_FIRST_BYTE_POSITION
    MONTH   = TIME_FIRST_BYTE_POSITION + 1 #Position of the byte that tells the month in raport to TIME_FIRST_BYTE_POSITION
    DAY     = TIME_FIRST_BYTE_POSITION + 2 #Position of the byte that tells the day in raport to TIME_FIRST_BYTE_POSITION
    HOUR    = TIME_FIRST_BYTE_POSITION + 3 #Position of the byte that tells the hour in raport to TIME_FIRST_BYTE_POSITION
    MINUTE  = TIME_FIRST_BYTE_POSITION + 4 #Position of the byte that tells the minute in raport to TIME_FIRST_BYTE_POSITION
    SECOND  = TIME_FIRST_BYTE_POSITION + 5 #Position of the byte that tells the second in raport to TIME_FIRST_BYTE_POSITION
    MILS_FH = TIME_FIRST_BYTE_POSITION + 6 #FIRST  HALF OF THE NUMBER OF MILISECONDS in raport to TIME_FIRST_BYTE_POSITION
    MILS_SH = TIME_FIRST_BYTE_POSITION + 7 #SECOND HALF OF THE NUMBER OF MILISECONDS in raport to TIME_FIRST_BYTE_POSITION

    START_BYTE             = 0xAA #The byte that every pack must start with
    TRIGGER_BYTE           = 0xAB #Byte that tells that the pack is of type TRIGGER which includes time data and type of slope
    BOOT_BYTE              = 0xAC #Byte that tells that the pack is about the board starting to run
    ETHERNET_STATUS_BYTE   = 0xAD #Byte that tells that the pack is about the ethernet connection status of the board during set up
    ETHERNET_STATUS_CHECK  = 0xAE #Byte that tells that the pack is about the board connection to ethernet during program execution

    RISING_SLOPE           = 0x01 #Byte marking that the signal went from 0 to 1
    FALLING_SLOPE          = 0x10 #Byte marking that the signal went from 1 to 0

    NOT_CONNECTED          = 0    #Board is not connected to eth     
    CONNECTED              = 1    #Board is connected to eth
    IP_FIRST_BYTE_POSITION = 3    #Pointer to the fist byte of the IP adress
    TOTAL_IP_BYTES         = 4    #Number of bytes used by the IP of the board
    IP_FIRST_BYTE          = IP_FIRST_BYTE_POSITION + 0 
    IP_SECOND_BYTE         = IP_FIRST_BYTE_POSITION + 1
    IP_THIRD_BYTE          = IP_FIRST_BYTE_POSITION + 2
    IP_FORTH_BYTE          = IP_FIRST_BYTE_POSITION + 3
    ETHERNET_STATUS_POS    = IP_FIRST_BYTE_POSITION + TOTAL_IP_BYTES #Status of ethernet byte position in ETHERNET_STATUS_BYTE pack
    ETHERNET_CHECK_POS     = 3                                       #Status of ethernet byte position in ETHERNET_STATUS_CHECK pack

class Format:
    TIME_DISPLAY_FORMAT = '0>2'    #Format do display months, days, hours, minutes, seconds in a 2 digit format
    TIME_MS_DISPLAY_FORMAT = '0>3' #Format do display millisecs in a 3 digit format
    YEAR_OFFSET = 2000             #To be added to the value of the year sent by the board to display the correct year