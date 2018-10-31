// To build in VS Micro, must disable Deep Search

#include <TimeLib.h>
#include <Time.h>
#include <ArduinoHttpClient.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#include "webserver.h"
#include "ifttwebhook.h"
#include "controller.h"
#include "mywifi.h"
#include "ntp.h"

#define DEBUG_SERIAL Serial
#define DEBUG_SERIAL_BAUD_RATE 115200

Door door;
MyWifi wifi;
Webserver server;
IfttWebhook iftt;
Ntp ntp;

Controller controller(wifi, server, door, iftt, ntp);

void setup() 
{
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUD_RATE);
    delay(1000);

    controller.Setup(DEBUG_SERIAL);
}

void loop() 
{
    controller.Loop(DEBUG_SERIAL);
}
