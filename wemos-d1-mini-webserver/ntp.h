#pragma once

#include <Arduino.h>
#include <WiFiUdp.h>


class Ntp
{
public:
    Ntp();
    ~Ntp();

    // Gets time_t as seconds since 1970/1/1 (Unix Epoch)
    time_t GetCurrentEpochTime(Print& print);

    

private:
    WiFiUDP mItpClient;
    void SendNTPpacket(Print& print);
    time_t GetNTPPacket(Print& print);
};

