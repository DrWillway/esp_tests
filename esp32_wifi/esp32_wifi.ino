#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"

AsyncWebServer server(80);
const char *ssid = "esp-equipa-x";
const char *password = "mestrado.meia";

void notFound(AsyncWebServerRequest *request)
{
 request->send(404, "application/json", "{\"message\":\"Not found\"}");
}
int pin1_status, pin2_status, pin3_status;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(27, OUTPUT);
  ledcAttach(12, 5000, 8);
  // ledcAttachPin(12, 0);
  ledcAttach(14, 5000, 8);
  // ledcAttachPin(14, 1);
  ledcAttach(27, 5000, 8);
  // ledcAttachPin(27, 2);
  pin1_status = 0;
  pin2_status = 0;
  pin3_status = 0;
  ledcWrite(12, pin1_status);
  ledcWrite(14, pin2_status);
  ledcWrite(27, pin3_status);
  Serial.begin(9600);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "application/json", String("{\"pin 1\":\"") + pin1_status + "\",\"pin 2\":\"" + pin2_status + "\",\"pin 3\":\"" + pin3_status + "\"}");
  });
  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/status", [](AsyncWebServerRequest * request, JsonVariant & json) {
    JsonObject jsonObj = json.as<JsonObject>();
    pin1_status = jsonObj["pin 1"].as<int>();
    pin2_status = jsonObj["pin 2"].as<int>();
    pin3_status = jsonObj["pin 3"].as<int>();
    ledcWrite(12, pin1_status);
    ledcWrite(14, pin2_status);
    ledcWrite(27, pin3_status);

    request->send(200);
  });
  server.addHandler(handler);
  server.onNotFound(notFound);
  server.begin();
}


// the loop function runs over and over again forever
void loop() {
  
}
