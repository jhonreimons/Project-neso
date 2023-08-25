#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
const char* ssid = "Neso";
const char* password = "jesika123";

// Konfigurasi MQTT Broker
const char* mqttServer = "192.168.43.61";
const int basahValue = 800;  // Ubah sesuai dengan nilai Anda saat basah
const int keringValue = 4095;  
const int soilMoisturePin = 34; // Ganti dengan pin yang sesuai pada ESP32
const char* topic = "sensor/soilmoisture";
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqttServer, 1883);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (client.connect("NODE1")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
int readSoilMoisture() {
  // Baca nilai sensor soil moisture
  int soilMoistureValue = analogRead(soilMoisturePin);

  // Lakukan kalibrasi
  int kalibrasiValue = map(soilMoistureValue, keringValue, basahValue, 0, 100);
  kalibrasiValue = constrain(kalibrasiValue, 0, 100);

  return kalibrasiValue;
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Baca nilai sensor soil moisture
  int soilMoistureValue = readSoilMoisture(); //readSoilMoisture();
  //Kirim data kelembaban tanah ke topik MQTT
  char msg[20];
  // snprintf(msg, sizeof(msg), "%d", soilMoistureValue);
  // client.publish(topic, msg);

  StaticJsonDocument<200> jsonDoc;
  jsonDoc["sensor_id"] = "sensor1";
  // soilMoistureValue = abs(soilMoistureValue);
  jsonDoc["soil_moisture"] = soilMoistureValue;
  String jsonString; 
  serializeJson(jsonDoc, jsonString);
  if (client.publish(topic, jsonString.c_str())) {
    Serial.println("Data sent successfully");
  } else {
    Serial.println("Data sending failed");
  }

  
  delay(5000); // Kirim data setiap 5 detik (sesuaikan dengan kebutuhan Anda)
}
