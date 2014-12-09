#include "SparkHue.h"
#include "http_client.h"

#define PORT 80
#define TIME_BETWEEN_REQUESTS 100 //max 10 per second for bridge

byte bridgeIP[] = { 10, 0, 1, 2 }; // Enter the IP address of your hue bridge
char bridgeUser[] = "newdeveloper"; // a registered user on your hue bridge
char response[1024];

HTTPClient client;

void setup() {
    Serial.begin(9600);
}

unsigned long lastRequest = 0;
boolean disconnected = false;

void loop() {
  if(!Serial.available()){
    return;
  }

  if(!disconnected) {
    Spark.disconnect();
    disconnected = true;
  }

  if (millis() - lastRequest > TIME_BETWEEN_REQUESTS) {
    increaseDecreaseBrightness();
  }
}

void setBrightness(int brightness) {
  //Convert int to char[]
  char smallBuffer[10];
  itoa(brightness, smallBuffer, 10);

  char requestBody[20];
  sprintf(requestBody, "%s", "{\"bri\":");
  strcat(requestBody, smallBuffer);
  strcat(requestBody, "}");

  char endpoint[200];
  sprintf(endpoint, "/api/");
  strcat(endpoint, bridgeUser);
  strcat(endpoint, "/lights/1/state");

  client.makeRequest(2, endpoint, bridgeIP, PORT, false, "application/json","", "", requestBody, response, 1024);

  lastRequest = millis();
}

boolean increaseBrightness = false;
int brightness = 255;

void increaseDecreaseBrightness() {
  if(increaseBrightness) {
      brightness += 20;
        if(brightness > 255) {
          brightness = 255;
          increaseBrightness = false;
        }
    }
    else {
      brightness -= 20;
      if(brightness < 0) {
        brightness = 0;
        increaseBrightness = true;
      }
    }
  setBrightness(brightness);
}
