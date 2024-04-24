import ntplib
import time
from datetime import datetime, timezone

def fetch_server_time(ntp_server):
    client = ntplib.NTPClient()
    response = client.request(ntp_server, version=3)
    # Convert the timestamp to a timezone-aware datetime object in UTC
    server_time = datetime.fromtimestamp(response.tx_time, timezone.utc)
    return server_time


#ntp_server = 'pool.ntp.org'
#try:
#    server_time = fetch_server_time(ntp_server)
#    formatted_time = server_time.strftime('%Y,%m,%d,%H,%M,%S,%f')[:-3]  # Including milliseconds
#    print(formatted_time)
#except Exception as e:
#    print(f"Failed to get NTP time: {e}")

class getTimeFromNtp:
    def __init__(self, ntpServer):
        self.ntpserver = ntpServer
        self.milisec, self.sec, self.min, self.hour, self.day, self.month, self.year = 0,0,0,0,0,0,0

    def getTime(self):
        try:
            server_time = fetch_server_time(self.ntpserver)
            formatted_time = server_time.strftime('%Y,%m,%d,%H,%M,%S,%f')[:-3]
            formatted_time = formatted_time.split(',')
            self.year, self.month, self.day, self.hour, self.min, self.sec, self.milisec = formatted_time
            #print(self.milisec, self.sec, self.min, self.hour, self.day, self.month, self.year)
            return 1
        except Exception as e:
            print(f"Failed to get NTP time: {e}")
            return 0


#t = Time('pool.ntp.org')
#for i in range(10):
#    t.getTime()
#    time.sleep(1)