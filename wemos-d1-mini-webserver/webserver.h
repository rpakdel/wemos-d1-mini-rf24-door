#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "door.h"

// Pin for current door status. HIGH = open, LOW = closed
#define DOOR_STATUS_IN_PIN 3
// Pin is toggled when door should be opened or closed
#define DOOR_SIGNAL_OUT_PIN 2

class Webserver
{
public:
    Webserver();
    ~Webserver();

    void Begin(Print& print);
    void Loop(Door& door, Print& print);
private:
    WiFiServer mServer;
    

    void HandleClient(WiFiClient& client, Door& doorStatus, Print & print);
    void Get_index_html(WiFiClient& client, Print& print);
    void ClientWriteString(WiFiClient & client, const char * buffer);
    void Get_client_js(WiFiClient & client);
    void Get_style_css(WiFiClient & client);
    void Get_favicon(WiFiClient & client);
    void PrintClientRequest(String & req, Print & print);
};

