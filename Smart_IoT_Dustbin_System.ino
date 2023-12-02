#include <NewPing.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* WIFI_SSID = "Dialog 4G";
const char* WIFI_PASSWORD = "139A2NLA49G";
const char* MQTT_BROKER = "broker.emqx.io";

const char* MQTT_TOPIC1 = "iot/Filling";
// const char* MQTT_TOPIC2 = "iot/Lidding";

const char* MQTT_USERNAME = "emqx";
const char* MQTT_PASSWORD = "public";
const int MQTT_PORT = 1883;

#define bin_capacity 9  // Set the maximum bin capacity level in centimeters

Servo myservo;
const int trigPin = 12;
const int echoPin = 13;
const int irSensorPin = 2;
const int ledPin1 = 16;
const int ledPin2 = 0;
const int lcdColumns = 16;
const int lcdRows = 2;

NewPing sonar(trigPin, echoPin);
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(callback);

  while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += WiFi.macAddress();
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("Public EMQX MQTT broker connected");
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  client.subscribe(MQTT_TOPIC1);
  // client.subscribe(MQTT_TOPIC2);

  myservo.attach(15);
  myservo.write(0);
  pinMode(irSensorPin, INPUT_PULLUP);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  int distance = sonar.ping_cm();
  Serial.print("Current Waste level: ");
  Serial.println(distance);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bin Capacity:");
  lcd.setCursor(0, 1);
  lcd.print(distance);

  String message = String(distance) + "cm";

  if (bin_capacity >= distance) {
    lcd.setCursor(0, 1);
    lcd.print("Bin is Full!");
    client.publish(MQTT_TOPIC1, "Bin is Full!");
    digitalWrite(ledPin2, LOW);

    for (int i = 0; i < 5; i++) {
      digitalWrite(ledPin1, HIGH);
      delay(400);
      digitalWrite(ledPin1, LOW);
      delay(400);
    }
  } else {
    digitalWrite(ledPin2, HIGH);
    client.publish(MQTT_TOPIC1, message.c_str());
  }

  if (digitalRead(irSensorPin) == LOW) {
    myservo.write(0);
    // client.publish(MQTT_TOPIC2, "Lid is Open");
    delay(1000);
    myservo.write(180);
  }
  else {
    // client.publish(MQTT_TOPIC2, "Lid is Close");
  }
  
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}
