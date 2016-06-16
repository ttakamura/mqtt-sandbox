#include <MQTT.h>
#include <PubSubClient.h>

#include <ESP8266WiFi.h>
extern "C" {
#include "user_interface.h"
}

// アクセスポイント接続情報
const char *wifi_ssid  = "wifi_ssid";      // SSID
const char *wifi_pass  = "wifi_pass";  // パスワード

// MQTT接続情報
const char *mqtt_host  = "std1.mqtt.shiguredo.jp";    // 接続先のホスト名
const char *mqtt_user  = "mqtt_user";          // ユーザー名
const char *mqtt_pass  = "mqtt_pass";          // パスワード
const char *mqtt_topic = "mqtt_user/fumi/cds"; // アクセス先トピック

// ClientID
const char *mqtt_client = "ardiono01";

WiFiClient wclient;
PubSubClient client(wclient, mqtt_host);

void setup() {
  Serial.begin(115200);
  connectWifi();
}

void loop() {
  int ADC_Value = 0;
  ADC_Value = system_adc_read();
  Serial.println("=======ANALOG " + String(ADC_Value) + " ");
  publish(ADC_Value);
  delay(5000);
}

void publish(int val) {
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.println("Connecting to MQTT server");
      if (client.connect(MQTT::Connect(mqtt_client).set_auth(mqtt_user, mqtt_pass))) {
        Serial.println("Connected to MQTT server");
      } else {
        Serial.println("Could not connect to MQTT server");
      }
    }
    Serial.print("Publishing: ");
    String message = String("count ") + String(val);
    Serial.println(message);

    client.publish(MQTT::Publish(mqtt_topic, message).set_retain());

    if (client.connected()) {
      client.loop();
    }
  }
}

void connectWifi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(wifi_ssid);
    Serial.println("...");
    WiFi.begin(wifi_ssid, wifi_pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}
