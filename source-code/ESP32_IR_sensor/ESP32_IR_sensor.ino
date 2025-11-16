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
const char* topic_ir_value = "kelompok1/ir";
const char* topic_alert    = "kelompok1/alert";

// === Pins ===
#define IR_PIN 15   // IR digital obstacle sensor

WiFiClientSecure espClient;
PubSubClient client(espClient);

// Timer non-blocking
unsigned long lastRead = 0;
const unsigned long interval = 5000;  // ms

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
    if (client.connect("ESP32IR", mqtt_user, mqtt_pass)) {
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

  pinMode(IR_PIN, INPUT);

  setup_wifi();

  espClient.setInsecure();  
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  // Pastikan MQTT connected
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();

  // Jangan baca sensor sebelum WiFi + MQTT stabil
  if (WiFi.status() != WL_CONNECTED || !client.connected()) {
    return;
  }

  // === Non-blocking read ===
  unsigned long now = millis();
  if (now - lastRead >= interval) {
    lastRead = now;

    int irValue = digitalRead(IR_PIN);
    Serial.print("IR Sensor: ");
    Serial.println(irValue);

    if (irValue == LOW) {
      client.publish(topic_alert, "Obstacle detected");
      client.publish(topic_ir_value, "1");
    } else {
      client.publish(topic_ir_value, "0");
    }
  }
}
