#include "door.h"



Door::Door(Radio& radio) :
    mRadio(radio)
{
    
}

void Door::Setup(Print& print)
{
    mRadio.Setup(print);
    print.print(F("DOOR_SETUP..."));
    pinMode(DOOR_RELAY_PIN, OUTPUT);
    digitalWrite(DOOR_RELAY_PIN, HIGH);
    print.println(F("COMPLETE"));
}

Door::~Door()
{
}

DoorCode Door::GetStatus(Print & print)
{
    return GetStatusRF24(print);
}

DoorCode Door::GetStatusTestAlwaysOpen(Print& print)
{
    return DoorCode::Open;
}

long prevTestMillis = 0;
DoorCode prevTestDoorCode = DoorCode::Unknown;

DoorCode Door::GetStatusTestOpenClose(Print& print)
{
    long currentMillis = millis();
    long delay = 10 * 1000; // 10s interval
    long diff = currentMillis - prevTestMillis;

    DoorCode status = DoorCode::Unknown;
    if (diff >= delay)
    {
        prevTestMillis = currentMillis;
        if (prevTestDoorCode != DoorCode::Open)
        {
            status = DoorCode::Open;
        }
        else
        {
            status = DoorCode::Close;
        }
    }
    else
    {
        status = prevTestDoorCode;
    }

    prevTestDoorCode = status;
    return status;
}

DoorCode Door::GetStatusRF24(Print& print)
{
    long currentMillis = millis();
    if (mRadio.Available())
    {
        uint8_t buffer;
        mRadio.Read((uint8_t*)&buffer, sizeof(uint8_t));
    
        //print.print(F("Got door status code "));
        //print.println(buffer);
        mPrevDoorStatusCodeMillis = currentMillis;
        if (buffer == DOOR_IS_CLOSED_CODE)
        {
            mStatus = DoorCode::Close;
        }
        else if (buffer == DOOR_IS_OPEN_CODE)
        {
            mStatus = DoorCode::Open;
        }
        else
        {
            mStatus = DoorCode::Unknown;
        }

        return mStatus;
    }
    else if ((currentMillis - mPrevDoorStatusCodeMillis) > MAX_DOOR_STATUS_WAIT_MILLIS)
    {
        print.println(F("Waited too long for door status code"));
        mStatus = DoorCode::Unknown;
        return mStatus;
    }

    // no change
    return mStatus;
}

void Door::Toggle(Print& print)
{
    print.print(F("TOGGLE_RELAY..."));
    digitalWrite(DOOR_RELAY_PIN, LOW);
    delay(125);
    digitalWrite(DOOR_RELAY_PIN, HIGH);
    print.println(F("DONE"));
}

