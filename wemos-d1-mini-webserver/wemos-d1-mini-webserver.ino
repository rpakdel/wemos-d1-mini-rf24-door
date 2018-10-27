// To build in VS Micro, must disable Deep Search

#include <ArduinoHttpClient.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "myssid.h"
#include "webserver.h"
#include "adafruitio.h"

#define DEBUG_SERIAL Serial
#define DEBUG_SERIAL_BAUD_RATE 115200

Webserver server;
AdafruitIO aio;
Door door;

void setupWifi(Print& print)
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

void setup() 
{
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUD_RATE);

    pinMode(DOOR_STATUS_IN_PIN, INPUT_PULLUP);
    pinMode(DOOR_SIGNAL_OUT_PIN, OUTPUT);

    setupWifi(DEBUG_SERIAL);
    server.Begin(DEBUG_SERIAL);
}

void loop() 
{
    aio.PostDoorStatus(door, DEBUG_SERIAL);
    server.Loop(door, DEBUG_SERIAL);
}
