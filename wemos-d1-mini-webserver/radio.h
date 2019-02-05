#pragma once

#include <Arduino.h>
#include <RF24.h>

#define RF_CEPIN 4
#define RF_CSPIN 15

static byte RFAddress[][6] = { "1Door" };

class Radio
{
public:
    Radio();
    ~Radio();

    void Setup(Print& print);
    void Read(void* buf, uint8_t len);
    bool Available();

private:
    RF24 mRadio;
};

