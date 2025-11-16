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
const char* topic_distance = "kelompok4/distance";
const char* topic_alert    = "kelompok4/alert";

// === Pins HC-SR04 ===
#define TRIG_PIN 5
#define ECHO_PIN 18

WiFiClientSecure espClient;
PubSubClient client(espClient);

// Timer non-blocking
unsigned long lastRead = 0;
const unsigned long interval = 4000;  // baca tiap 400 ms

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
    if (client.connect("ESP32_HCSR04", mqtt_user, mqtt_pass)) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(1500);
    }
  }
}

// === Non-blocking distance measurement ===
float getDistance() {
  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // durasi echo
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);  // timeout 30ms

  float distance = duration * 0.034 / 2;
  if (distance <= 0 || distance > 400) return 400; // biar gak ngawur

  return distance;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  setup_wifi();

  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();

  // Jangan baca sensor sebelum WiFi dan MQTT stabil
  if (WiFi.status() != WL_CONNECTED || !client.connected()) {
    return;
  }

  unsigned long now = millis();
  if (now - lastRead >= interval) {
    lastRead = now;

    float distance = getDistance();
    Serial.print("Distance: ");
    Serial.println(distance);

    // Publish distance
    char dist_str[16];
    dtostrf(distance, 6, 2, dist_str);
    client.publish(topic_distance, dist_str);

    // Optional alert
    if (distance < 10) {
      client.publish(topic_alert, "Object too close");
    }
  }
}
