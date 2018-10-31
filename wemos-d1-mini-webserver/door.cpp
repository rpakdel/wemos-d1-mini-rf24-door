#include "door.h"

byte mRFAddress[][6] = { "1Door" };

Door::Door() : 
    mRadio(RF_CEPIN, RF_CSPIN)
{
    
}

void Door::Setup(Print& print)
{
    print.println(F("DOOR_SETUP"));
    pinMode(DOOR_RELAY_PIN, OUTPUT);
    digitalWrite(DOOR_RELAY_PIN, HIGH);

    SetupRadio(print);
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
    long delay = 10 * 1000; // 5s interval
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
    if (mRadio.available())
    {
        uint8_t buffer;
        mRadio.read((uint8_t*)&buffer, sizeof(uint8_t));
    
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

void Door::SetupRadio(Print& print)
{
    print.print(F("RADIO_SETUP..."));
    mRadio.begin();
    mRadio.setPALevel(RF24_PA_MIN);
    mRadio.setAutoAck(1);
    mRadio.setRetries(15, 15);
    mRadio.openReadingPipe(0, mRFAddress[0]);
    mRadio.startListening();
    // wait until initialized
    delay(1000);
    print.println(F("COMPLETE"));
}