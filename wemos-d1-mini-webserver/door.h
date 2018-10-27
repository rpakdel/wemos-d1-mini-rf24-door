#pragma once

#include <Arduino.h>

#define DOOR_IS_OPEN HIGH
#define DOOR_IS_CLOSED LOW

class Door
{
public:
    Door();
    ~Door();

    int GetStatus();
    void Toggle(Print & print);

private:
    volatile int mStatus;
};

