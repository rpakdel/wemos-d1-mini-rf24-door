#pragma once

#include <ArduinoHttpClient.h>
#include <ESP8266WiFi.h>
// define AIO_FEED and AIO_KEY in this file. Do not commit to repo
#include "aio.h"
#include "door.h"

#define AIO_SERVER "io.adafruit.com"
#define AIO_PORT 80
#define AIO_CONTENT_TYPE "Content-Type"
#define AIO_CONTENT_TYPE_APP_JSON "application/json"
#define AIO_POST_DOOR_WAIT_MILLIS 60000
#define AIO_CONTENT_LENGTH "Content-Length"
#define AIO_CONTENT_LENGTH_VALUE 16
#define AIO_DOOR_STATUS_HIGH "{ \"value\": \"1\" }"
#define AIO_DOOR_STATUS_LOW "{ \"value\": \"0\" }"



class AdafruitIO
{
public:
    AdafruitIO();
    ~AdafruitIO();

    void PostDoorStatus(Door& door, Print& print);

private:
    WiFiClient mClient;
    HttpClient mHttpClient;

    int mPrevDoorCode = DoorCode::Unknown;
    long prevPostMillis = 0;
    long openPostCount = 0;
    char postBody[256];

};

