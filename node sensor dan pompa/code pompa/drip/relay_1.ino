#include <WiFi.h>
#include <PubSubClient.h>

#define RELAY_PIN 5
#define RELAY_PIN1 18

// Konfigurasi WiFi
const char* ssid = "vivo 1938";
const char* password = "123sin@1234";

// Konfigurasi MQTT
const char* mqtt_server = "192.168.43.19";
const char* mqtt_username = "";
const char* mqtt_password = "";
const char* mqtt_topic = "topic/perangkat1";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // Mulai Serial
  Serial.begin(9600);
  while (!Serial);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RELAY_PIN1, OUTPUT);

  // Hubungkan ke jaringan WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");

  // Hubungkan ke broker MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  reconnect();  // Panggil fungsi reconnect() untuk inisialisasi MQTT saat awal
}

void loop() {
  // Periksa koneksi WiFi dan MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

// Fungsi untuk menghubungkan kembali ke broker MQTT
void reconnect() {
  while (!client.connected()) {
    if (client.connect("Prangkat1", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// Fungsi untuk menangani pesan yang diterima
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  // Konversi payload menjadi string
  String data = "";
  for (int i = 0; i < length; i++) {
    data += (char)payload[i];
  }

  if (String(topic) == "topic/perangkat1") {
    // Cek nilai pesan yang diterima
    if (data == "OFF") {
      // Nyalakan relay
      Serial.println("Relay turned OFF");
      digitalWrite(RELAY_PIN, HIGH);
      digitalWrite(RELAY_PIN1, LOW);
    } else if (data == "ON") {
      // Matikan relay
      Serial.println("Relay turned ON");
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(RELAY_PIN1, HIGH);
    }
  }
}

