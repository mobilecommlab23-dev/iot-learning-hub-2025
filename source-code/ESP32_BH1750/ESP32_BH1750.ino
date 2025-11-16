#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <BH1750.h>

// === WiFi credentials ===
const char* ssid = "StrongHold";
const char* password = "satusampaisembilan";

// === MQTT HiveMQ Cloud (TLS 8883) ===
const char* mqtt_server = "48ed9182c70c423482b6229d8954803b.s1.eu.hivemq.cloud";
const int   mqtt_port   = 8883;

const char* mqtt_user = "didiSg02";
const char* mqtt_pass = "didiSg02";

// === MQTT Topics ===
const char* topic_lux_value = "kelompok3/lux";
const char* topic_alert     = "kelompok3/alert";

// === I2C Pins untuk BH1750 ===
#define SDA_PIN 21
#define SCL_PIN 22

BH1750 lightMeter;

WiFiClientSecure espClient;
PubSubClient client(espClient);

unsigned long lastRead = 0;
const unsigned long interval = 3000;  // baca tiap 500 ms

// === WiFi connect ===
void setup_wifi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nWiFi connected!");
}

// === MQTT reconnect ===
void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT... ");
    if (client.connect("ESP32_BH1750", mqtt_user, mqtt_pass)) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // I2C start
  Wire.begin(SDA_PIN, SCL_PIN);
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);

  setup_wifi();

  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();

  // Jangan baca sensor kalau WiFi atau MQTT belum stabil
  if (WiFi.status() != WL_CONNECTED || !client.connected()) {
    return;
  }

  unsigned long now = millis();
  if (now - lastRead >= interval) {
    lastRead = now;

    float lux = lightMeter.readLightLevel();
    Serial.print("Lux: ");
    Serial.println(lux);

    // Publish nilai lux
    char lux_str[16];
    dtostrf(lux, 6, 2, lux_str);
    client.publish(topic_lux_value, lux_str);

    // Optional alert
    if (lux < 10) {
      client.publish(topic_alert, "Too dark");
    }
  }
}
