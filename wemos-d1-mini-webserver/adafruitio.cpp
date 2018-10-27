#include "adafruitio.h"



AdafruitIO::AdafruitIO() : mHttpClient(mClient, AIO_SERVER, AIO_PORT)
{
}


AdafruitIO::~AdafruitIO()
{
}

void AdafruitIO::PostDoorStatus(Door& door, Print& print)
{
    int status = door.GetStatus();
    long m = millis();
    // if the status hasn't changed, check if enough time has gone by
    if (status == mPrevDoorStatus)
    {
        long diff = m - prevPostMillis;
        if (diff < AIO_POST_DOOR_WAIT_MILLIS)
        {
            return;
        }
        prevPostMillis = m;
    }
    else
    {
        // post status immediately
        prevPostMillis = m;
    }

    mPrevDoorStatus = status;
    print.println(F("POST_DOOR_AIO"));
    if (status == DOOR_IS_OPEN)
    {
        openPostCount++;
    }
    else
    {
        openPostCount = 0;
    }

    sprintf(postBody, "{ \"value\": \"%i\" }\0", openPostCount);

    mHttpClient.beginRequest();
    mHttpClient.post(AIO_DATA_URL);
    // defined in aio.h file
    mHttpClient.sendHeader(AIO_KEY_HEADER, AIO_KEY);
    mHttpClient.sendHeader(AIO_CONTENT_TYPE, AIO_CONTENT_TYPE_APP_JSON);
    mHttpClient.sendHeader(AIO_CONTENT_LENGTH, strlen(postBody));
    mHttpClient.beginBody();
    mHttpClient.print(postBody);
    mHttpClient.endRequest();

    int statusCode = mHttpClient.responseStatusCode();
    print.print(F("POST_DOOR_STATUS "));
    print.println(statusCode);
}