#include <Arduino.h>
#include <memory>

#include <ESPDateTime.h>

#include "mqtt_client.h"
#include "pwmc.h"
#include "provision.h"

std::unique_ptr<PWMControl> pwmController;
std::unique_ptr<MqttClient> mqttClient;


void setup() {
  Serial.begin(115200);
  delay(2000);

  wifi_connect();

  pwmController = std::make_unique<PWMControl>();
  mqttClient = std::make_unique<MqttClient>(std::move(pwmController));
  DateTime.setTimeZone("AEST-10AEDT,M10.1.0,M4.1.0/3");
  DateTime.begin(/* timeout param */);
  if (!DateTime.isTimeValid()) {
    Serial.printf("Failed to get time from server\n");
  }
}

void loop() {
  mqttClient->loop();
}

