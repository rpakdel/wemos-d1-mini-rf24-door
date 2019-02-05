#pragma once

#include "webserver.h"
#include "door.h"
#include "ifttwebhook.h"
#include "mywifi.h"

class Controller
{
public:
    Controller(MyWifi& wifi, Webserver& server, Door& door, IfttWebhook& iftt);
    ~Controller();

    

    void Setup(Print& print);

    
    
    void Loop(Print& print);

private:
    Webserver mServer;
    Door mDoor;
    IfttWebhook mIftt;
    MyWifi mWifi;
    DoorCode mPrevDoorStatus = DoorCode::Unknown;
    long mPrevDoorOpenMillis = 0;
    int mNum15minInc = 1;

    void CheckDoorStatus(Print& print);
    void PostDoorLeftOpenToIFTT(DoorCode doorStatus, Print& print);
    void GetDoorOpenedToIFTT(DoorCode doorStatus, Print& print);
    //void GetCurrentHourMinuteFromNTP(int & h, int & m, Print& print);
};

