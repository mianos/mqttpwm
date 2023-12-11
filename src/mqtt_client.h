#pragma once
#include <memory>

#include <WiFi.h>
#include <PubSubClient.h>

#include "pwmc.h"

class MqttClient {
public:
  MqttClient(std::unique_ptr<PWMControl> pwmControlPtr, const char* mqttServer="mqtt2.mianos.com");
//  RadarMqtt(std::unique_ptr<PWMControl> pwmControlPtr, const String& server="mqtt2.mianos.com", int port=1883);
  void loop();
  std::unique_ptr<PWMControl> pwmControl;

private:
  WiFiClient espClient;
  PubSubClient client;
  unsigned long lastMsg;
  const char* mqttServer;
  static const int MSG_BUFFER_SIZE = 50;
  int value;

  void callback(char* topic, byte* payload, unsigned int length);
  void reconnect();
};
