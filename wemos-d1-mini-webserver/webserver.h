#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "door.h"

class Webserver
{
public:
    Webserver();
    ~Webserver();

    void Begin(Print& print);
    void Loop(Door& door, Print& print);
private:
    WiFiServer mServer;

    void HandleClient(WiFiClient& client, Door& door, Print & print);
    void Get_index_html(WiFiClient& client, Print& print);
    void ClientWriteString(WiFiClient & client, const char * buffer);
    void Get_client_js(WiFiClient & client);
    void Get_style_css(WiFiClient & client);
    void Get_favicon(WiFiClient & client);
    void PrintClientRequest(String & req, Print & print);
};

