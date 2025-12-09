#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL36B8BuJ9G"
#define BLYNK_TEMPLATE_NAME "gas value"
#define BLYNK_AUTH_TOKEN "tqc-KEN3vhNqt-RyzQPj2Nl_YGOnmzCo"

#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClient.h>

// ThingSpeak details
unsigned long ChannelNumber = 2486072;
const char* apiKey = "T81V1FR32LRG5KBB";

// WiFi + Blynk credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Prasanna";
char pass[] = "prasanna22";

WiFiClient client;
BlynkTimer timer;

// Pin definitions
#define Green D3
#define Yellow D4
#define Red D5
#define Buzzer D6
#define Relay D7
#define Sensor A0

int pinValue = 0;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, pass);
  Blynk.begin(auth, ssid, pass);
  ThingSpeak.begin(client);

  pinMode(Sensor, INPUT);
  pinMode(Green, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Relay, OUTPUT);

  // Run functions periodically
  timer.setInterval(2000L, checkGasLevel);
  timer.setInterval(5000L, sendToThingSpeak);
}

BLYNK_WRITE(V0) {
  pinValue = param.asInt();  // Switch ON/OFF from Blynk
}

void checkGasLevel() {
  int sensorRaw = analogRead(Sensor);
  int sensor = map(sensorRaw, 0, 1024, 0, 100);
  
  Serial.print("Gas Level: ");
  Serial.println(sensor);

  Blynk.virtualWrite(V1, sensor);

  if (pinValue == 1) {
    if (sensor <= 25) {
      digitalWrite(Green, HIGH);
      digitalWrite(Yellow, LOW);
      digitalWrite(Red, LOW);
      digitalWrite(Buzzer, LOW);
      digitalWrite(Relay, LOW);
    } 
    else if (sensor > 25 && sensor <= 70) {
      digitalWrite(Green, LOW);
      digitalWrite(Yellow, HIGH);
      digitalWrite(Red, LOW);
      digitalWrite(Buzzer, LOW);
      digitalWrite(Relay, HIGH);
      Blynk.logEvent("fire_alert", "Warning: Gas leakage detected!");
    } 
    else { // sensor > 70
      digitalWrite(Green, LOW);
      digitalWrite(Yellow, LOW);
      digitalWrite(Red, HIGH);
      digitalWrite(Buzzer, HIGH);
      digitalWrite(Relay, HIGH);
      Blynk.logEvent("fire_alert", "DANGER: High Gas Leakage!");
    }
  } else {
    digitalWrite(Green, LOW);
    digitalWrite(Yellow, LOW);
    digitalWrite(Red, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(Relay, LOW);
  }
}

void sendToThingSpeak() {
  int sensorRaw = analogRead(Sensor);
  int sensor = map(sensorRaw, 0, 1024, 0, 100);

  if (pinValue == 0) {
    ThingSpeak.writeField(ChannelNumber, 1, 0, apiKey);
  } else {
    ThingSpeak.writeField(ChannelNumber, 1, sensor, apiKey);
  }
}

void loop() {
  Blynk.run();
  timer.run();
}