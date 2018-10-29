#include "ifttwebhook.h"

// define the IFTT URLs in this file. Do not commit to repo.
#include "webhookurls.h"

#define CONTENT_TYPE "Content-Type"
#define CONTENT_TYPE_APP_JSON "application/json"
#define CONTENT_LENGTH "Content-Length"

IfttWebhook::IfttWebhook() : 
    mHttpClient(mClient, IFTT_SERVER)
{
}

IfttWebhook::~IfttWebhook()
{
}

void IfttWebhook::PostDoorOpened(Print& print)
{
    print.print(F("IFTT_DOOROPENED POST... "));

    int errCode = mHttpClient.post(DoorOpenedPostURL);
    if (errCode != 0)
    {
        print.print(F("ERR "));
        print.print(errCode);
        print.print(F(" "));
    }

    int statusCode = mHttpClient.responseStatusCode();
    mHttpClient.stop();
    print.println(statusCode);
}

void IfttWebhook::PostDoorOpenedWithTime(int hour, int min, Print& print)
{
    print.print(F("IFTT_DOOROPENED POST... "));

    char postBody[256];
    
    sprintf(postBody, "{ \"value1\": \"%d\", \"value2\": \"%d\" }\0", hour, min);

    mHttpClient.beginRequest();
    mHttpClient.post(DoorOpenedPostURL);

    mHttpClient.sendHeader(CONTENT_TYPE, CONTENT_TYPE_APP_JSON);
    mHttpClient.sendHeader(CONTENT_LENGTH, strlen(postBody));
    mHttpClient.beginBody();
    mHttpClient.print(postBody);
    mHttpClient.endRequest();

    int statusCode = mHttpClient.responseStatusCode();
    print.println(statusCode);
}

void IfttWebhook::PostDoorLeftOpen(int minutes, Print& print)
{
    print.print(F("IFTT_DOORLEFTOPEN POST... "));
    char postBody[256];
    sprintf(postBody, "{ \"value1\": \"%i\" }\0", minutes);

    mHttpClient.beginRequest();
    mHttpClient.post(DoorLeftOpenPostURL);
    
    mHttpClient.sendHeader(CONTENT_TYPE, CONTENT_TYPE_APP_JSON);
    mHttpClient.sendHeader(CONTENT_LENGTH, strlen(postBody));
    mHttpClient.beginBody();
    mHttpClient.print(postBody);
    mHttpClient.endRequest();

    int statusCode = mHttpClient.responseStatusCode();
    print.println(statusCode);
}

void IfttWebhook::PostDoorOnline(int hour, int min, Print& print)
{
    print.print(F("IFTT_DOORONLINE POST... "));

    char postBody[256];

    sprintf(postBody, "{ \"value1\": \"%d\", \"value2\": \"%d\" }\0", hour, min);

    mHttpClient.beginRequest();
    mHttpClient.post(DoorOnlinePostURL);

    mHttpClient.sendHeader(CONTENT_TYPE, CONTENT_TYPE_APP_JSON);
    mHttpClient.sendHeader(CONTENT_LENGTH, strlen(postBody));
    mHttpClient.beginBody();
    mHttpClient.print(postBody);
    mHttpClient.endRequest();

    int statusCode = mHttpClient.responseStatusCode();
    print.println(statusCode);
}