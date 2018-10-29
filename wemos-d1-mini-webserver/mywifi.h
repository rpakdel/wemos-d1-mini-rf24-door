#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

class MyWifi
{
public:
    MyWifi();
    ~MyWifi();
    void Setup(Print& print);
};

