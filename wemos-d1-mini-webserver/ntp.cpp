
#include "ntp.h"

// time gap in seconds from 01.01.1900 (NTP time) to 01.01.1970 (UNIX time)
#define DIFF1900TO1970 2208988800UL
#define NTP_SERVER "pool.ntp.org"
#define NTP_PORT 123
#define NTP_PACKET_SIZE 48 // NTP time stamp is in the first 48 bytes of the message

Ntp::Ntp()
{
}

Ntp::~Ntp()
{
}

time_t Ntp::GetCurrentEpochTime(Print& print)
{
    mItpClient.begin(NTP_PORT);
    SendNTPpacket(print);
    time_t result = GetNTPPacket(print);
    mItpClient.stop();
    return result;
}

void Ntp::SendNTPpacket(Print& print)
{
    
    print.print("GET EPOCH TIME");
    byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packet
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
                             // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    mItpClient.beginPacket(NTP_SERVER, NTP_PORT); // NTP requests are to port 123
    mItpClient.write(packetBuffer, NTP_PACKET_SIZE);
    mItpClient.endPacket();
}

// Gets time_t as seconds since 1970/1/1 (Unix Epoch)
time_t Ntp::GetNTPPacket(Print &print)
{
    byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packet

    int tryCount = 0;
    while (tryCount < 3)
    {
        if (mItpClient.parsePacket())
        {
            // We've received a packet, read the data from it
            // read the packet into the buffer
            mItpClient.read(packetBuffer, NTP_PACKET_SIZE);

            // the timestamp starts at byte 40 of the received packet and is four bytes,
            // or two words, long. First, extract the two words:

            time_t highWord = word(packetBuffer[40], packetBuffer[41]);
            time_t lowWord = word(packetBuffer[42], packetBuffer[43]);
            // combine the four bytes (two words) into a long integer
            // this is NTP time (seconds since Jan 1 1900):
            time_t secsSince1900 = highWord << 16 | lowWord;
            mItpClient.stop();
            print.println(F("OK"));
            // the different between NTP and UNIX time
            return secsSince1900 - DIFF1900TO1970;
        }
        print.print(F("."));
        delay(1000);
        tryCount++;
    }
    
    print.println("FAIL");
    return 0;
}
