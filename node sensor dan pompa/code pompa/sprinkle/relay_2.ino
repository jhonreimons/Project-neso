#include <WiFi.h>
#include <PubSubClient.h>

#define RELAY_PIN1 15
#define RELAY_PIN2 4
#define RELAY_PIN3 5
#define RELAY_PIN4 18
#define RELAY_PIN5 2


// Konfigurasi WiFi
const char* ssid = "vivo 1938";
const char* password = "123sin@1234";

// Konfigurasi MQTT
const char* mqtt_server = "192.168.43.19";
const char* mqtt_username = "";
const char* mqtt_password = "";
const char* mqtt_topic = "topic/perangkat3";



WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // Mulai Serial
  Serial.begin(9600);
  while (!Serial);


  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(RELAY_PIN3, OUTPUT);
  pinMode(RELAY_PIN4, OUTPUT);
  pinMode(RELAY_PIN5, OUTPUT);
  
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
    if (client.connect("perangkat3", mqtt_username, mqtt_password)) {
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

  if (String(topic) == "topic/perangkat3") {
    // Cek nilai pesan yang diterima
    if (data == "OFF3") {
      // Nyalakan relay
      Serial.println("Relay turned OFF");
      digitalWrite(RELAY_PIN1, HIGH);
      digitalWrite(RELAY_PIN4, LOW);
      digitalWrite(RELAY_PIN2, HIGH);
      digitalWrite(RELAY_PIN3, LOW);
      digitalWrite(RELAY_PIN5, HIGH);
    } else if (data == "ON3") {
      // Matikan relay
      Serial.println("Relay turned ON");
      digitalWrite(RELAY_PIN1, LOW);
      digitalWrite(RELAY_PIN4, HIGH);
      digitalWrite(RELAY_PIN2, HIGH);
      digitalWrite(RELAY_PIN3, LOW);
      digitalWrite(RELAY_PIN5, LOW);
    }else if (data == "OFF4") {
      // Nyalakan relay
      Serial.println("Relay turned OFF");
      digitalWrite(RELAY_PIN2, HIGH);
      digitalWrite(RELAY_PIN3, LOW);
      digitalWrite(RELAY_PIN1, HIGH);
      digitalWrite(RELAY_PIN4, LOW);
      digitalWrite(RELAY_PIN5, HIGH);
    } else if (data == "ON4") {
      // Matikan relay
      Serial.println("Relay turned ON");
      digitalWrite(RELAY_PIN2, LOW);
      digitalWrite(RELAY_PIN3, HIGH);
      digitalWrite(RELAY_PIN1, HIGH);
      digitalWrite(RELAY_PIN4, LOW);
      digitalWrite(RELAY_PIN5, LOW);
    }
    Serial.print(data);
   
}
}

