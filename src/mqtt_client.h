#pragma once
#include <memory>

#include <WiFi.h>
#include <PubSubClient.h>

#include "pwmc.h"

class MqttClient {
public:
  MqttClient(std::unique_ptr<PWMControl> pwmControlPtr, const char* mqttServer="mqtt2.mianos.com");
  void loop();
  std::unique_ptr<PWMControl> pwmControl;

private:
  WiFiClient espClient;
  PubSubClient client;
  unsigned long lastMsg;
  const char* mqttServer;
  static const int MSG_BUFFER_SIZE = 50;
  int value;
  String name = "pumpt";

  void callback(char* topic, byte* payload, unsigned int length);
  void reconnect();
};
