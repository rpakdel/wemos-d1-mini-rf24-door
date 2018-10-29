#include "controller.h"
#include <TimeLib.h>
#include <Time.h>


Controller::Controller(MyWifi& wifi, Webserver& server, Door& door, IfttWebhook& iftt, Ntp& ntp) :
    mWifi(wifi),
    mServer(server),
    mDoor(door),
    mIftt(iftt),
    mNtp(ntp)
{
}

Controller::~Controller()
{
}

void Controller::GetCurrentHourMinuteFromNTP(int& h, int& m, Print& print)
{
    time_t epoch_utc = mNtp.GetCurrentEpochTime(print);
    // convert to local time
    time_t epoch_pst = epoch_utc - (7 * 60 * 60);
    h = hour(epoch_pst);
    m = minute(epoch_pst);
}

void Controller::Setup(Print& print)
{
    mDoor.Setup(print);
    mWifi.Setup(print);
    mServer.Begin(print);

    int h;
    int m;
    GetCurrentHourMinuteFromNTP(h, m, print);
    mIftt.PostDoorOnline(h, m, print);
}

void Controller::PostDoorLeftOpenToIFTT(DoorCode doorStatus, Print& print)
{
    long mil = millis();
    long sec = mil / 1000;
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

void Controller::GetDoorOpenedToIFTT(DoorCode doorStatus, Print& print)
{
    int h;
    int m;
    if (mPrevDoorStatus != doorStatus)
    {
        mPrevDoorStatus = doorStatus;

        switch (doorStatus)
        {
        case DoorCode::Open:
            GetCurrentHourMinuteFromNTP(h, m, print);
            print.printf("DOOR OPENED %d:%d", h, m);
            print.println();
            mIftt.PostDoorOpenedWithTime(h, m, print);
            
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
    mServer.Loop(print);
}
