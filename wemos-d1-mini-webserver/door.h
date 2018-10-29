#pragma once

#include <Arduino.h>
#include <RF24.h>

enum DoorCode
{
    Unknown = -1,
    Close = 0,
    Open = 1,
};

#define DOOR_RELAY_PIN D0

#define RF_CEPIN D2
#define RF_CSPIN D8

#define MAX_DOOR_STATUS_WAIT_MILLIS 60000 // 60 seconds


#define DOOR_IS_CLOSED_CODE 11
#define DOOR_IS_OPEN_CODE 22

class Door
{
public:
    Door();
    
    ~Door();

    

    void Setup(Print& print);

    DoorCode GetStatus(Print& print);
    void Toggle(Print& print);

private:
    RF24 mRadio;
    DoorCode mStatus = DoorCode::Unknown;
    long mPrevDoorStatusCodeMillis = 0;
    
    void SetupRadio(Print& print);

    DoorCode GetStatusRF24(Print& print);
    DoorCode GetStatusTestOpenClose(Print& print);
    DoorCode GetStatusTestAlwaysOpen(Print & print);
    // for testing
    DoorCode mTestStatus = DoorCode::Close;
};

