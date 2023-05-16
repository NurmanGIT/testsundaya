#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPI.h>

const char* ssid = "RnD_Sundaya";
const char* password = "sundaya22";
const char* serverIP = "<ip_address_device>";

const int potentiometerPin = 34; // Pin A0 pada ESP32
const int serPin = 17; // Pin SER pada ESP32
const int rclkPin = 16; // Pin RCLK pada ESP32
const int srclkPin = 15; // Pin SRCLK pada ESP32

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Menghubungkan ke jaringan WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Mengatur pin sebagai OUTPUT
  pinMode(serPin, OUTPUT);
  pinMode(rclkPin, OUTPUT);
  pinMode(srclkPin, OUTPUT);

  // Mengatur server endpoint
  server.on("/get-data", HTTP_GET, [](AsyncWebServerRequest *request){
    int potValue = analogRead(potentiometerPin);
    String response = String("{\"potentiometer_value\": ") + String(potValue) + String("}");
    request->send(200, "application/json", response);
  });

  // Menghidupkan server
  server.begin();
}

void loop() {
  // Membaca nilai potensiometer
  int potValue = analogRead(potentiometerPin);

  // Mengirimkan nilai potensiometer melalui Serial
  Serial.print("Potentiometer Value: ");
  Serial.println(potValue);

  // Mengontrol output LED menggunakan IC 74HC595
  shiftOut(serPin, srclkPin, MSBFIRST, potValue);

  // Mengupdate output IC 74HC595
  digitalWrite(rclkPin, HIGH);
  digitalWrite(rclkPin, LOW);

  delay(500); // Jeda waktu
}
