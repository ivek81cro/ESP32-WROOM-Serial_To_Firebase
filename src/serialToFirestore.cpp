#include "serialToFirestore.h"

struct EspNowMessage {
  char dateAndTime[20];
  float temperature;
};

SerialToFirestore::SerialToFirestore() : MySerial(1) {}

void SerialToFirestore::setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("ESP32 Device 2 - Receiver");

  initSerialDataStream();
  initWiFi();
  initFirestoreDatabase();
}

void SerialToFirestore::loop() {
  // Check if enough data is available to fill the struct
  if (MySerial.available() >= sizeof(EspNowMessage)) {
    // Read the struct data from Serial
    MySerial.readBytes((uint8_t*)&incomingReadings, sizeof(incomingReadings));

    // Print the received data
    Serial.print("Received Date and Time: ");
    Serial.println(incomingReadings.dateAndTime);
    Serial.print("Received Temperature: ");
    Serial.println(incomingReadings.temperature);

    // Send received data to Firestore
    sendToFirestore("temperature_data", incomingReadings.dateAndTime, incomingReadings.temperature);
  }
}

void SerialToFirestore::initFirestoreDatabase() {
  // Initialize Firebase
  config.database_url = DATABASE_URL;
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void SerialToFirestore::initWiFi() {
  // Initialize WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void SerialToFirestore::initSerialDataStream() {
  // Initialize UART1 (TX = GPIO17, RX = GPIO16 on ESP32-C3)
  MySerial.begin(9600, SERIAL_8N1, 16, 17); // RX = GPIO16, TX = GPIO17
}

void SerialToFirestore::sendToFirestore(String collection, String timestamp, float temperature) {
  FirebaseJson json;
  json.set("fields/timestamp/stringValue", timestamp);
  json.set("fields/temperature/doubleValue", temperature);

  String documentPath = collection + "/" + String((unsigned long)millis());

  Serial.print("Sending data to collection: ");
  Serial.println(collection);
  Serial.print("Document Path: ");
  Serial.println(documentPath);
  Serial.print("Timestamp: ");
  Serial.println(timestamp);
  Serial.print("Temperature: ");
  Serial.println(temperature);

  if (Firebase.Firestore.createDocument(&firebaseData, PROJECT_ID, "", documentPath.c_str(), json.raw())) {
    Serial.println("Data sent successfully");
  } else {
    Serial.println("Failed to send data");
    Serial.println(firebaseData.errorReason());
  }
}

