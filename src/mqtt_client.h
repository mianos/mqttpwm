#pragma once
#include <memory>

#include <WiFi.h>
#include <PubSubClient.h>

#include "pwmc.h"

class MqttClient {
public:
  MqttClient(std::unique_ptr<PWMControl> pwmControlPtr, const char* mqttServer="mqtt2.mianos.com", int port=1883);
  void loop();
  std::unique_ptr<PWMControl> pwmControl;

private:
  WiFiClient espClient;
  PubSubClient client;
  unsigned long lastMsg;
  const char* mqttServer;
  int value;
  String name = "pumpt";
  int statusFrequencyMs = 60000;
  int port = 1883;

  void callback(char* topic, byte* payload, unsigned int length);
  void reconnect();
  void PublishStatus();
};
