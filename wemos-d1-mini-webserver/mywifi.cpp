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
    print.println();
    print.print(F("SSID "));
    print.println(MYSSID);
    print.println();
    WiFi.mode(WIFI_STA);
    WiFi.begin(MYSSID, MYPASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(F("."));
    }
    print.println();
    print.print(F("WiFi connected, IP: "));
    print.println(WiFi.localIP());
}