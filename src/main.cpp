#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <math.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "index_html.h"
#include <WiFiUdp.h>
#include <NTPClient.h>


#define BAUD 115200
static const unsigned short ONBOARD_LED = 2;
static const unsigned short C_LED = 6;
static const unsigned short W_LED = 7;
static const unsigned short ONBOARD_BUTTON = 0;
static const unsigned short ONBOARD_TOUCH = 4;

AsyncWebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, 60*60*6*-1 );

String readLedState() {
  bool isOn = digitalRead(ONBOARD_LED);
  if (isOn) {
    return "checked";
  }
  return "";
}

String processor(const String& var) 
{
  if (var == "test") {
    return "changed!";
  }
  if (var == "isLedOn" ) {
    return readLedState();
  }

  return var;
}


void setup()
{
  Serial.begin(115200);
  delay(1000);
  // put your setup code here, to run once:

  pinMode(ONBOARD_BUTTON, INPUT_PULLUP);
  pinMode(ONBOARD_LED, OUTPUT);
  
  WiFi.begin("generic network name", "MirrorWindowWall");
  Serial.println("\nConnecting...");


  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
  }

  Serial.println("\nConnected to Wifi!");

  Serial.print("Local IP: ");
  Serial.print(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    // send_P = "send page"
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam("output") && request->hasParam("state")) {
      inputMessage = request->getParam("state")->value();
      digitalWrite(ONBOARD_LED, inputMessage.toInt());
    } else {
      inputMessage = "unknown message";
    }

    Serial.print("Received update message to ");
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  server.begin();

  // update a little over once an hour (but not exactly once an hour, so that the time of day changes and we don't hammer at the same time)
  timeClient.setUpdateInterval((1000 * 60 * 60) + 50000);
  timeClient.begin();

}

void blinkLed(int msLength) {
  digitalWrite(ONBOARD_LED, HIGH);
  delay(msLength);
  digitalWrite(ONBOARD_LED, LOW);
  delay(msLength);
}


void loop()
{
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  Serial.println(timeClient.getDay());
  delay(500);
}





