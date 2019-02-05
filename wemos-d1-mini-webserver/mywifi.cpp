#include "mywifi.h"
#include "myssid.h"

MyWifi::MyWifi()
{
}


MyWifi::~MyWifi()
{
}

void MyWifi::Setup(Print& print)
{

    print.print(F("WIFI_SETUP..."));
    print.print(F("SSID '"));
    print.print(MYSSID);
    print.print(F("'..."));
    WiFi.mode(WIFI_STA);
    WiFi.begin(MYSSID, MYPASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(F("."));
    }
    print.print(F("COMPLETE, IP: "));
    print.println(WiFi.localIP());
}