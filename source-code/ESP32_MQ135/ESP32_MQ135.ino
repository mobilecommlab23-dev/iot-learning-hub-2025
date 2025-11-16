#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// === WiFi credentials ===
const char* ssid = "StrongHold";
const char* password = "satusampaisembilan";

// === MQTT HiveMQ Cloud (TLS 8883) ===
const char* mqtt_server = "48ed9182c70c423482b6229d8954803b.s1.eu.hivemq.cloud";
const int   mqtt_port   = 8883;

const char* mqtt_user = "didiSg02";
const char* mqtt_pass = "didiSg02";

// === MQTT Topics ===
const char* topic_mq_value = "kelompok2/mq135";
const char* topic_alert    = "kelompok2/alert";

// === MQ135 Analog Pin ===
#define MQ135_PIN 34

WiFiClientSecure espClient;
PubSubClient client(espClient);

// Timer non-blocking
unsigned long lastRead = 0;
const unsigned long interval = 2000;  

void setup_wifi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nWiFi connected!");
}

void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT... ");
    if (client.connect("ESP32_MQ135", mqtt_user, mqtt_pass)) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(1500);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(MQ135_PIN, INPUT);

  setup_wifi();

  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();

  // pastikan WiFi & MQTT stabil dulu
  if (WiFi.status() != WL_CONNECTED || !client.connected()) {
    return;
  }

  unsigned long now = millis();
  if (now - lastRead >= interval) {
    lastRead = now;

    int raw = analogRead(MQ135_PIN);   // nilai 0–4095
    Serial.print("MQ135 Raw: ");
    Serial.println(raw);

    char mq_str[16];
    snprintf(mq_str, sizeof(mq_str), "%d", raw);
    client.publish(topic_mq_value, mq_str);

    // optional alert
    if (raw > 3000) {
      client.publish(topic_alert, "Air quality bad (MQ135 high)");
    }
  }
}
