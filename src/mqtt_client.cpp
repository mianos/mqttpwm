
#include "mqtt_client.h"

MqttClient::MqttClient(std::unique_ptr<PWMControl> pwmControlPtr, const char* mqttServer) :
    client(espClient),
    lastMsg(0),
    value(0),
    pwmControl(std::move(pwmControlPtr)) {
  this->mqttServer = mqttServer;
  client.setServer(mqttServer, 1883);
  client.setCallback([this](char* topic, byte* payload, unsigned int length) {
    callback(topic, payload, length);
  });
}


void MqttClient::loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    char msg[MSG_BUFFER_SIZE];
    snprintf(msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("pumpt/test", msg);
  }
}

void MqttClient::callback(char* topic, byte* payload, unsigned int length) {
  // Implement your callback logic here
}

void MqttClient::reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish("outTopic", "hello world");
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

