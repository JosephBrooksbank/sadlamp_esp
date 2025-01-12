#include <WiFi.h>
#include <HTTPClient.h>
#include <math.h>
#include <Arduino.h>
// #include<ArduinoJson.h>

#define BAUD 115200
// put function declarations here:


static const unsigned short ONBOARD_LED = 2;
static const unsigned short C_LED = 6;
static const unsigned short W_LED = 7;
static const unsigned short ONBOARD_BUTTON = 0;
static const unsigned short ONBOARD_TOUCH = 4;


void setup()
{
  Serial0.begin(115200);
  delay(1000);
  // put your setup code here, to run once:

  pinMode(ONBOARD_BUTTON, INPUT_PULLUP);
  pinMode(ONBOARD_LED, OUTPUT);
  
  WiFi.begin("generic network name", "MirrorWindowWall");
  Serial0.println("\nConnecting...");



  while (WiFi.status() != WL_CONNECTED)
  {
    Serial0.print(".");
  }

  Serial0.println("\nConnected to Wifi!");

  Serial0.print("Local IP: ");
  Serial0.print(WiFi.localIP());

}

void blinkLed(int msLength) {
  digitalWrite(ONBOARD_LED, HIGH);
  delay(msLength);
  digitalWrite(ONBOARD_LED, LOW);
  delay(msLength);
}


void loop()
{

  delay(5);
}





