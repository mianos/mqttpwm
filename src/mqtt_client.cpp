#include <ArduinoJson.h>
#include <ESPDateTime.h>
#include <StringSplitter.h>
#include "mqtt_client.h"

MqttClient::MqttClient(std::unique_ptr<PWMControl> pwmControlPtr, const char* mqttServer, int port) :
    client(espClient),
    lastMsg(0),
    value(0),
    port(port),
    pwmControl(std::move(pwmControlPtr)) {
  this->mqttServer = mqttServer;
  client.setServer(mqttServer, port);
  client.setCallback([this](char* topic, byte* payload, unsigned int length) {
    callback(topic, payload, length);
  });
}


void MqttClient::loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  auto now = millis();
  if (now - lastMsg > statusFrequencyMs) {
    lastMsg = now;
    ++value;
    StaticJsonDocument<200> doc;
    doc["value"] = value;
    doc["time"] = DateTime.toISOString();
    doc["duty_cycle"] = pwmControl->dutyCycle;
    String status_topic = "tele/" + name + "/status";
    String output;
    serializeJson(doc, output);
    client.publish(status_topic.c_str(), output.c_str());
  }
}

void MqttClient::callback(char* topic_str, byte* payload, unsigned int length) {
  auto topic = String(topic_str);
  auto splitter = StringSplitter(topic, '/', 4);
  auto itemCount = splitter.getItemCount();
  if (itemCount < 3) {
    Serial.printf("Item count less than 3 %d '%s'\n", itemCount, topic_str);
    return;
  }
#if 1
  for (int i = 0; i < itemCount; i++) {
    String item = splitter.getItemAtIndex(i);
    Serial.println("Item @ index " + String(i) + ": " + String(item));
  }
  Serial.printf("command '%s'\n", splitter.getItemAtIndex(itemCount - 1).c_str());
#endif
  
  if (splitter.getItemAtIndex(0) == "cmnd") {
    DynamicJsonDocument jpl(1024);
    auto err = deserializeJson(jpl, payload, length);
    if (err) {
      Serial.printf("deserializeJson() failed: '%s'\n", err.c_str());
      return;
    }
    String output;
    serializeJson(jpl, output);
    auto dest = splitter.getItemAtIndex(itemCount - 1);
#if 1
    if (dest == "duty_cycle") {
      if (jpl.containsKey("value")) {
        auto duty_cycle = jpl["value"].as<int>();
        Serial.printf("Setting duty_cycle to %d\n", duty_cycle);
        pwmControl->setDutyCycle(duty_cycle);
      }
    }
#endif
  }
}

void MqttClient::reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "esp32s2client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      String cmnd_topic = String("cmnd/") + name + "/#";
      Serial.printf("connected topic '%s'\n", cmnd_topic.c_str());
      client.subscribe(cmnd_topic.c_str());

      StaticJsonDocument<200> doc;
      doc["version"] = 1;
      doc["time"] = DateTime.toISOString();
      doc["hostname"] = WiFi.getHostname();
      doc["ip"] = WiFi.localIP().toString();
      String status_topic = "tele/" + name + "/init";
      String output;
      serializeJson(doc, output);
      client.publish(status_topic.c_str(), output.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

