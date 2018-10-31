#include "webserver.h"

// must disable visual micro deep search for this to work
const char client_js_FileContent[] PROGMEM =
#include "..\..\wemos-d1-mini-rf24-door\wemos-d1-mini-webserver\public\client.js"
;

const char index_html_FileContent[] PROGMEM =
#include "..\..\wemos-d1-mini-rf24-door\wemos-d1-mini-webserver\public\index.html"
;

const char style_css_FileContent[] PROGMEM =
#include "..\..\wemos-d1-mini-rf24-door\wemos-d1-mini-webserver\public\style.css"
;

const char newLine[] PROGMEM = "\r\n";
const char http_status_200_OK[] PROGMEM = "HTTP/1.1 200 OK";
const char http_status_404_NOTFOUND[] PROGMEM = "HTTP/1.1 404 NOT FOUND";

const char content_type_js[] PROGMEM = "Content-Type: application/javascript";
const char content_type_css[] PROGMEM = "Content-Type: text/css";
const char content_type_html[] PROGMEM = "Content-Type: text/html";
const char content_type_json[] PROGMEM = "Content-Type: application/json";
const char content_type_plain[] PROGMEM = "Content-Type: text/plain";
const char content_length_zero[] PROGMEM = "Content-Length: 0";

Webserver::Webserver() : 
    mServer(80)
{

}


Webserver::~Webserver()
{
}

void Webserver::Begin(Print& print)
{
    mServer.begin();
    print.println(F("WEBSERVER_STARTED"));
}

void Webserver::Get_index_html(WiFiClient& client, Print& print)
{
    String s = FPSTR(http_status_200_OK);
    s += FPSTR(newLine);
    s += FPSTR(content_type_html);
    s += FPSTR(newLine);
    s += FPSTR(index_html_FileContent);
    const char* buffer = s.c_str();
    ClientWriteString(client, buffer);
}

int getMin(int a, int b)
{
    if (a < b)
    {
        return a;
    }

    return b;
}

void Webserver::ClientWriteString(WiFiClient& client, const char* buffer)
{
    int packetSize = 5000;
    int totLength = strlen(buffer);
    int index = 0;
    int remain = totLength - index;
    int sendLength = getMin(remain, packetSize);
    while (index < totLength)
    {
        client.write(&buffer[index], sendLength);
        index += packetSize;
        remain = totLength - index;
        sendLength = getMin(remain, packetSize);
    }

    if (remain > 0)
    {
        client.write(&buffer[index], remain);
    }
    client.flush();
}

void Webserver::Get_client_js(WiFiClient& client)
{
    String s = FPSTR(http_status_200_OK);
    s += FPSTR(newLine);
    s += FPSTR(content_type_js);
    s += FPSTR(newLine);
    s += FPSTR(newLine);
    s += FPSTR(client_js_FileContent);
    const char* buffer = s.c_str();
    ClientWriteString(client, buffer);
}

void Webserver::Get_style_css(WiFiClient& client)
{
    String s = FPSTR(http_status_200_OK);
    s += FPSTR(newLine);
    s += FPSTR(content_type_css);
    s += FPSTR(newLine);
    s += FPSTR(newLine);
    s += FPSTR(style_css_FileContent);
    const char* buffer = s.c_str();
    ClientWriteString(client, buffer);
}

void Webserver::Get_favicon(WiFiClient& client)
{
    String s = FPSTR(http_status_404_NOTFOUND);
    const char* buffer = s.c_str();
    ClientWriteString(client, buffer);
};


void Webserver::PrintClientRequest(String& req, Print& print)
{
    print.print(F("["));
    print.print(req);
    print.print(F("]"));
    print.println();
}


void Webserver::HandleClient(WiFiClient& client, Door& door, Print& print)
{
    // Read the first line of the request
    String req = client.readStringUntil('\r');
    client.read(); // \r
    client.read(); // \n

    if (req.indexOf(F("GET / ")) >= 0) // space after / to make sure it's root
    {
        Get_index_html(client, print);
    }
    else if (req.indexOf(F("GET /client.js")) >= 0)
    {
        Get_client_js(client);
    }
    else if (req.indexOf(F("GET /style.css")) >= 0)
    {
        Get_style_css(client);
    }
    else if (req.indexOf(F("GET /favicon.ico")) >= 0)
    {
        Get_favicon(client);
    }
    else if (req.indexOf(F("POST /api/v1/toggle")) >= 0)
    {
        print.println(F("WEBAPI TOGGLE REQUEST"));

        door.Toggle(print);

        // status
        String s = FPSTR(http_status_200_OK);
        s += FPSTR(newLine);
        // header
        s += FPSTR(content_type_plain);
        s += FPSTR(newLine);
        // blank line indicates data
        s += FPSTR(newLine);
        s += FPSTR(F("Ok"));
        const char* buffer = s.c_str();
        ClientWriteString(client, buffer);
    }
    else if (req.indexOf(F("GET /api/v1/status")) >= 0)
    {
        DoorCode result = door.GetStatus(print);
        // status
        String s = FPSTR(http_status_200_OK);
        s += FPSTR(newLine);
        // header
        s += FPSTR(content_type_plain);
        s += FPSTR(newLine);
        // blank line indicates data
        s += FPSTR(newLine);
        switch (result)
        {
            case DoorCode::Open:
                s += F("IsOpen");
                break;

            case DoorCode::Close:
                s += F("IsClosed");
                break;

            case DoorCode::Unknown:
                s += F("IsUnknown");
                break;
        }
        
        const char* buffer = s.c_str();
        ClientWriteString(client, buffer);
    }
    else
    {
        print.println(F("WEBAPI 404 NOTFOUND"));
        String s = FPSTR(http_status_404_NOTFOUND);
        const char* buffer = s.c_str();
        ClientWriteString(client, buffer);
    }
}

void Webserver::Loop(Door& door, Print& print)
{
    // Check if a client has connected
    WiFiClient client = mServer.available();
    if (!client)
    {
        return;
    }

    // Wait until the client sends some data
    while (!client.available())
    {
        delay(1);
    }

    HandleClient(client, door, print);
}