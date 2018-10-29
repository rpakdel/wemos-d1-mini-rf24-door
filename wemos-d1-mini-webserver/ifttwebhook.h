#pragma once

#include <ArduinoHttpClient.h>
#include <ESP8266WiFi.h>

class IfttWebhook
{
public:
    IfttWebhook();
    ~IfttWebhook();
    void PostDoorOpened(Print& print);
    void PostDoorOpenedWithTime(int hour, int min, Print& print);
    void PostDoorLeftOpen(int minutes, Print& print);
    void PostDoorOnline(int hour, int min, Print & print);
private:
    WiFiClient mClient;
    HttpClient mHttpClient;
};

