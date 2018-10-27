#include "door.h"


Door::Door() : mStatus(DOOR_IS_CLOSED)
{
}

Door::~Door()
{
}

long pm = 0;
int Door::GetStatus()
{
    long m = millis();
    if ((m - pm) > (60 * 1000))
    {
        pm = m;
        if (mStatus == DOOR_IS_OPEN)
        {
            mStatus = DOOR_IS_CLOSED;
        }
        else
        {
            mStatus = DOOR_IS_OPEN;
        }
    }
    return mStatus;
}

void Door::Toggle(Print& print)
{
    print.println(F("TOGGLE"));
}

