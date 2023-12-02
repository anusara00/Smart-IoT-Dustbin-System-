# Smart-IoT-Dustbin-System

## Overview
This project implements a Smart Bin Monitoring System using an ESP8266 microcontroller, ultrasonic sensor, servo motor, and MQTT for communication. The system monitors the waste level in the bin and communicates with a Node-RED dashboard for visualization.

## Components
- ESP8266 (NodeMCU)
- Ultrasonic Sensor (HC-SR04)
- Servo Motor
- IR Sensor
- Node-RED Dashboard

## Arduino Sketch
The Arduino sketch (`Smart_IoT_Dustbin_System.ino`) contains the code for interacting with sensors, controlling the servo motor, and publishing data to an MQTT broker.

## Node-RED Flow
The Node-RED flow (`node_red_flow.json`) provides a dashboard interface for visualizing bin levels and receiving commands from the MQTT broker.

## Setup Instructions
1. Upload the Arduino sketch to your ESP8266.
2. Import the Node-RED flow into your Node-RED instance.
3. Configure the WiFi, MQTT, and other parameters in the Arduino sketch.
4. Connect the hardware components according to the wiring specified in the sketch.
5. Power on the system and monitor the bin levels on the Node-RED dashboard.

## MQTT Topics
- `iot/Filling`: Publishes the bin level data and receives commands about the bin status.
- (Optional) `iot/Lidding`: Publishes information about the lid status.

## Dependencies
- NewPing library
- Servo library
- LiquidCrystal_I2C library
- ESP8266WiFi library
- PubSubClient library


