# Wireless Garage Door Sensor and Opener (Wemos D1 version)

## Hardware
* Base-station with relay

![Wemos D1 mini + NRF24L01 Garage door sensor and opener + 315Mhz RF receiver](/wemos-d1-mini-webserver/wemos-d1-mini-rf24-door.JPG?raw=true)

* Wireless door sensor: 

![](https://media.giphy.com/media/3o752bfTEIFtwjCC2I/200w_d.gif)

https://giphy.com/gifs/3o752bfTEIFtwjCC2I/html5

* The door sensor uses a generic window reed switch used commonly for home security (not operational yet).
* The door sensor is a separate Arduino that talks to the Wemos D1 mini via NRF24L01.

* I found that shorting the two connections on the garage door opener toggles the door.
* A simple relay (Normally Open, Active LOW) is used to achieve the same from Arduino.
* The Wemos D1 is 3.3v and I found using the GPIO pins to keep the relay HIGH needed a 3.3V to 5V trigger converter.

## Software
* The web page and web API are hosted on the Wemos D1 including the HTML, client side JavaScript and CSS. 
* I'm using Basic HTTP auth + SSL to reduce risk of somebody getting a hold of the door.
* The door status is uploaded to Adafruit.IO once per minute by the ESP8266.
* This IFTT applet then sends me notifications if the door has been left open: 
  https://ifttt.com/applets/317604p-if-the-door-is-opened-send-me-a-notification
  
## High Level Data Flow

### Web -> Relay
web toggle button -> nginx -> Wemos D1 mini -> relay to short the garage door button

### Door status -> Web 
Door sensor -> Arduino (2) -> NRF24L01 -> Wemos D1 mini -> nginx -> web door status
