#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Servo.h>
#include <ArduinoJson.h>

const char* ssid = "HUAWEI-AY498A";
const char* pass = "15764233";

IPAddress IP(10, 0, 0, 19);
IPAddress NETMASK(255, 0, 0, 0);
IPAddress NETWORK(10, 0, 0, 1);
IPAddress DNS(8, 8, 8, 8);

Servo servoCamera;
uint8_t servoCameraValue = 0;
static const uint8_t gpioPin[] = {16,  5,  4,  0,  2, 14, 12, 13, 15,  3,  1};  // Total 11 Pin GPIO, Index mulai dari 0
uint8_t gpioState[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

WebSocketsServer ws(81);

void setup() {
    pinMode(gpioPin[1], OUTPUT);
    pinMode(gpioPin[2], OUTPUT);
    pinMode(gpioPin[3], OUTPUT);
    pinMode(gpioPin[4], OUTPUT);
    pinMode(gpioPin[5], OUTPUT);
    pinMode(gpioPin[6], OUTPUT);
    pinMode(gpioPin[7], OUTPUT);
    pinMode(gpioPin[8], OUTPUT);
    pinMode(gpioPin[9], OUTPUT);
    pinMode(gpioPin[10], OUTPUT);
    servoCamera.attach(gpioPin[0]);

// Set serial untuk message debugging
    Serial.begin(9600);

// Menyambungkan WiFi
    Serial.print("Menyambungkan ke WiFi ");
    WiFi.begin(ssid, pass);
    WiFi.config(IP, NETWORK, NETMASK, DNS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    } 
    Serial.println();
    Serial.println("WiFi Terhubung...");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP Address Aktif: ");
    Serial.println(WiFi.localIP());

    ws.begin();
    ws.onEvent(wsEvent);
    Serial.println("WebSocket server aktif...");
}
void loop() {
    ws.loop();
//  test();
}
void test() {
    for (int led = 1; led <= 10; led++) {
        digitalWrite(gpioPin[led], HIGH);
    }
    for (int angle = 0; angle <= 180; angle++) {
        servoCamera.write(angle);
        delay(20);
    }
    for (int led = 1; led <= 10; led++) {
        digitalWrite(gpioPin[led], LOW);
    }
    digitalWrite(gpioPin[12], LOW);
    for (int angle = 180; angle >= 0; angle--) {
        servoCamera.write(angle);
        delay(20);
    }
}
void broadcastDigitalWrite(int clientID = -1) {
    StaticJsonDocument<205> jsonDocument;
    JsonArray stateArray = jsonDocument.createNestedArray("data");
    for (int i = 0; i <= 10; i++) {
        stateArray.add(gpioState[i]);
    }
    jsonDocument["mode"] = "digitalWrite";
    String jsonSend = "";
    serializeJson(jsonDocument, jsonSend);

    if (clientID == -1) ws.broadcastTXT(jsonSend);
    else ws.sendTXT(clientID, jsonSend); 
}
void broadcastServoWrite(int clientID = -1) {
    StaticJsonDocument<205> jsonDocument;
    jsonDocument["mode"] = "servoWrite";
    jsonDocument["angle"] = servoCameraValue;
    String jsonSend = "";
    serializeJson(jsonDocument, jsonSend);
    if (clientID == -1) ws.broadcastTXT(jsonSend);
    else ws.sendTXT(clientID, jsonSend); 
}
void wsEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED :
        Serial.print("Disconnected: ");
        Serial.println(num);
        break;
        case WStype_CONNECTED :
        Serial.print("Connected: ");
        Serial.println(num);
        break;
        case WStype_TEXT : 
        String text = String((char *) &payload[0]);

        if (text == "requestInitialCondition") {
            broadcastDigitalWrite(num);
            broadcastServoWrite(num);
        }
        else {
            StaticJsonDocument<128> jsonDocument;
            DeserializationError error = deserializeJson(jsonDocument, text);

            String operationMode = jsonDocument["operationMode"];
            Serial.println("-------------------------------");
            Serial.print("by: ");
            Serial.println(num);
            Serial.print("Message: ");
            Serial.println(text);
            Serial.print("operationMode: ");
            Serial.println(operationMode);
            if (operationMode == "digitalWrite") {
                uint8_t pin = jsonDocument["pin"];
                uint8_t state = jsonDocument["state"];  

                gpioState[pin] = state;
                digitalWrite(gpioPin[pin], gpioState[pin]);

                Serial.print("pin: ");
                Serial.println(pin);
                Serial.print("state: ");
                Serial.println(state);

                broadcastDigitalWrite();
            }
            else if (operationMode == "servoWrite") {
                uint8_t pin = jsonDocument["pin"];
                uint8_t angle = jsonDocument["angle"]; 

                servoCameraValue = angle;
                servoCamera.write(angle);

                Serial.print("pin: ");
                Serial.println(pin);
                Serial.print("angle: ");
                Serial.println(angle);
                broadcastServoWrite();
            }     

        }
        break;
    }
}
