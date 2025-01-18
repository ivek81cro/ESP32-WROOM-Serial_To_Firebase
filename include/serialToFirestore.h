#ifndef SERIAL_TO_FIRESTORE_H
#define SERIAL_TO_FIRESTORE_H

#include "credentials.h"
#include <HardwareSerial.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>

// Provide the token generation process info.

class SerialToFirestore {
public:
    SerialToFirestore();
    void setup();
    void loop();

private:
    void initFirestoreDatabase();
    void initWiFi();
    void initSerialDataStream();
    void sendToFirestore(String path, String dateAndTime, float temperature);

    HardwareSerial MySerial;
    FirebaseData firebaseData;
    FirebaseAuth auth;
    FirebaseConfig config;

    // Structure example to send data
    // Must match the receiver structure
    typedef struct EspNowMessage {
        char dateAndTime[25];
        float temperature;    
    } EspNowMessage;

    // Create a struct_message to hold incoming sensor readings
    EspNowMessage incomingReadings;
};

#endif // SERIAL_TO_FIRESTORE_H