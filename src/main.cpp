#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <math.h>
// #include<ArduinoJson.h>

#define BAUD 115200
// put function declarations here:
int myFunction(int, int);
void sendMessageToServer(String message);
void setLed(bool isOn);
void blinkLed(int msLength);

enum METHOD_TYPE {
  GET,
  POST
};
String fetch(String path, String message, METHOD_TYPE methodType);

const String serverName = "http://192.168.0.90:5173";
const unsigned long timerDelay = 5000;
unsigned long lastTime = 0;
// static const unsigned short ONBOARD_LED = 2;
static const unsigned short C_LED = 6;
static const unsigned short W_LED = 7;
static const unsigned short ONBOARD_BUTTON = 0;
static const unsigned short ONBOARD_TOUCH = 4;
static const unsigned short MAX_BLINK = 500;
// static const unsigned short REED_SENSOR = 13;


void setup()
{
  Serial.begin(115200);
  delay(1000);
  // put your setup code here, to run once:

  pinMode(EXTERNAL_LED, OUTPUT);
  pinMode(ONBOARD_BUTTON, INPUT_PULLUP);
  // pinMode(REED_SENSOR, INPUT_PULLDOWN);
  
  WiFi.begin("generic network name", "MirrorWindowWall");
  Serial.println("\nConnecting...");



  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    // blinkLed(50);
  }

  Serial.println("\nConnected to Wifi!");
  // setLed(true);


  Serial.print("Local IP: ");
  Serial.print(WiFi.localIP());

}
const unsigned short numberOfPoints = 7;


// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}

int getStrength()
{
  long rssi = 0;

  for (int i = 0; i < numberOfPoints; i++)
  {
    rssi += WiFi.RSSI();
    delay(20);
  }
  return rssi / numberOfPoints;
}

void blinkLed(int msLength) {
  setLed(true);
  delay(msLength);
  setLed(false);
  delay(msLength);
}

void setLed(bool isOn) {
  digitalWrite(ONBOARD_LED, isOn ? HIGH : LOW );
  digitalWrite(EXTERNAL_LED, isOn ? HIGH : LOW );
}

void loop()
{

  auto response = fetch("/getStatus", "", GET);
  Serial.println("test");
  // DynamicJsonDocument doc(200);
  // deserializeJson(doc, response);
  // bool isOn = doc["isOn"];
  // setLed(isOn);
  delay(500);
}



void sendMessageToServer(String message) {
  HTTPClient client;

  String serverPath = serverName + "/message/" + message;
  Serial.println(serverPath);
  client.begin(serverPath.c_str());
  int httpResponseCode = client.GET();
  Serial.println(httpResponseCode);
  // client.end();
}


String fetch(String path, String message, METHOD_TYPE methodType) {
  HTTPClient client;
  
  String serverPath = serverName + + "/api" + path; 

  int httpResponseCode = 0;
  switch (methodType) {
    case GET : {
      if (message != "") {
        serverPath += "/" + message;
      }
      client.begin(serverPath.c_str());
      httpResponseCode = client.GET();
      break;
    }
    case POST : {
        client.addHeader("Content-Type", "application/json");
        httpResponseCode = client.POST(message);
        client.begin(serverPath.c_str());
        break;
    }
    default: {
      Serial.println("Unknown method type");
      return "";
    }
  }

  if (httpResponseCode < 0) {
      Serial.println("Response code invalid for http request");
      return "";
  }

  String payload = "{}";

  payload = client.getString();
  client.end();
  return payload;
}

