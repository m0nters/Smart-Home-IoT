#include "mqtt.h"

const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
PubSubClient mqttClient(wifiClient);

void mqttSetup() {
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(websiteDataHandler);
  mqttClient.setKeepAlive(90);
}

void mqttConnect() {
  while (!mqttClient.connected()) {
    String clientId = "ESP32Client-" + String(random(0xffff), HEX); // e.g. ESP32Client-9c7b
    if (mqttClient.connect(clientId.c_str())) {
      // Subscribe to topics if needed
      mqttClient.subscribe("home-0PPKrXoRcgyppks/#");
    }
    else {
      delay(5000);
    }
  }
}

void websiteDataHandler(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  // user mute sound effect on website
  if (String(topic) == "home-0PPKrXoRcgyppks/isMutedEntryDetection") {
    isMutedEntryDetection = (message == "true");
  }
  else if (String(topic) == "home-0PPKrXoRcgyppks/isMutedDoorSystem") {
    isMutedDoorSystem = (message == "true");
  }
  // user unlock door that is in permanently locked state on website
  else if (String(topic) == "home-0PPKrXoRcgyppks/isDoorPermanentlyLocked") {
    isDoorPermanentlyLocked = (message == "true");
  }
  // user change password on website
  else if (String(topic) == "home-0PPKrXoRcgyppks/hashedPassword") {
    for (unsigned int i = 0; i < length; i++) {
      hashedPassword[i] = payload[i];
    }
  }
}

void taskMQTT(void* parameter) {
  while (true) {
    if (!mqttClient.connected()) {
      mqttConnect();
    }
    mqttClient.loop();
    vTaskDelay(100 / portTICK_PERIOD_MS); // Run every 100ms
  }
}