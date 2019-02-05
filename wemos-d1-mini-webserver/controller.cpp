#include "controller.h"
#include <TimeLib.h>
#include <Time.h>


Controller::Controller(MyWifi& wifi, Webserver& server, Door& door, IfttWebhook& iftt) :
    mWifi(wifi),
    mServer(server),
    mDoor(door),
    mIftt(iftt)
{
}

Controller::~Controller()
{
}

void Controller::Setup(Print& print)
{
    mDoor.Setup(print);
    mWifi.Setup(print);
    mServer.Begin(print);

    mIftt.PostDoorIsOnline(print);
}

void Controller::PostDoorLeftOpenToIFTT(DoorCode doorStatus, Print& print)
{
    long currentMillis = millis();
    if (doorStatus == DoorCode::Open)
    {
        long diff = currentMillis - mPrevDoorOpenMillis;

        long sec = diff / 1000;
        long min = sec / 60;
        // every 15 minutes
        int numMinutesOpenThreshold = mNum15minInc * 15;
        int r = min / numMinutesOpenThreshold;
        //print.printf("%d %d %d %d", mil, sec, min, r);
        //print.println();
        if (r > 0)
        {
            mNum15minInc++;
            // reset back to 1 after 24 hours (24 * 4)
            if (mNum15minInc >= 96)
            {
                mNum15minInc = 1;
            }
            mIftt.PostDoorLeftOpen(numMinutesOpenThreshold, print);
        }
    }
    else
    {
        mPrevDoorOpenMillis = currentMillis;
    }
}

void Controller::GetDoorOpenedToIFTT(DoorCode doorStatus, Print& print)
{
    if (mPrevDoorStatus != doorStatus)
    {
        mPrevDoorStatus = doorStatus;

        switch (doorStatus)
        {
        case DoorCode::Open:
            print.printf("DOOR OPENED");
            print.println();
            mIftt.PostDoorOpened(print);
            
            break;

        case DoorCode::Close:
            print.println("DOOR CLOSED");
            break;

        case DoorCode::Unknown:
            print.println("DOOR UNKNOWN");
            break;
        }
    }
}

void Controller::CheckDoorStatus(Print& print)
{
    DoorCode doorStatus = mDoor.GetStatus(print);

    GetDoorOpenedToIFTT(doorStatus, print);
    PostDoorLeftOpenToIFTT(doorStatus, print);
}

void Controller::Loop(Print& print)
{
    CheckDoorStatus(print);
    mServer.Loop(mDoor, print);
}
