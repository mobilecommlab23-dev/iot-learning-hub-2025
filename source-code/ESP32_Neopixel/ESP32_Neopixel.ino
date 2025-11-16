#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

// ===== NeoPixel =====
#define NEOPIXEL_PIN 4
#define NUM_PIXELS   8
Adafruit_NeoPixel strip(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// ===== Capacitive Touch Breakout (digital OUT) =====
#define CAP_TOUCH_PIN 27  // Ganti sesuai pin OUT modul piringan

// ===== WiFi Credentials =====
const char* ssid     = "StrongHold";
const char* password = "satusampaisembilan";

// ===== MQTT Credentials =====
const char* mqtt_server   = "48ed9182c70c423482b6229d8954803b.s1.eu.hivemq.cloud";
const int   mqtt_port     = 8883;
const char* mqtt_user     = "didiSg02";
const char* mqtt_password = "didiSg02";

WiFiClientSecure espClient;
PubSubClient client(espClient);

// ===== Timer =====
unsigned long prevMillis = 0;
unsigned long interval   = 1000;

// ===== WiFi connect =====
void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }
}

// ===== MQTT connect =====
void connectMQTT() {
  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);

  while (!client.connected()) {
    client.connect("esp32-cap-touch", mqtt_user, mqtt_password);
    delay(500);
  }
}

void setup() {
  Serial.begin(115200);

  strip.begin();
  strip.show();

  pinMode(CAP_TOUCH_PIN, INPUT);

  connectWiFi();
  connectMQTT();
}

void loop() {
  client.loop();

  unsigned long now = millis();
  if (now - prevMillis >= interval) {
    prevMillis = now;

    int touchState = digitalRead(CAP_TOUCH_PIN);
    bool touched = (touchState == LOW);  // kebanyakan modul aktif LOW

    if (touched) {
      for (int i = 0; i < NUM_PIXELS; i++) {
        strip.setPixelColor(i, strip.Color(0, 100, 0));
      }
      strip.show();
      client.publish("kelompok5/touch", "not touched");
    } else {
      for (int i = 0; i < NUM_PIXELS; i++) {
        strip.setPixelColor(i, strip.Color(100, 0, 0));
      }
      strip.show();
      client.publish("kelompok5/touch", "touched");
    }

    Serial.print("Touch state: ");
    Serial.println(touched ? "FREE" : "Touched");
  }
}
