#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Sensor and LED pins
#define RED_LED D5
#define GREEN_LED D6

// Sensor variables
PulseOximeter pox;
uint32_t tsLastReport = 0;
float currentPressure = 0;
float pressureThreshold = 25.0; // Adjust this threshold as needed
bool isMonitoring = false;

// Web server
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  
  // Initialize LEDs
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW); // Both LEDs off initially
  
  // Initialize MAX30100
  Serial.print("Initializing MAX30100..");
  if (!pox.begin()) {
    Serial.println("FAILED");
    for(;;) {
      digitalWrite(RED_LED, HIGH);
      delay(100);
      digitalWrite(RED_LED, LOW);
      delay(100);
    }
  } else {
    Serial.println("SUCCESS");
    // Start monitoring
    isMonitoring = true;
    Serial.println("Pressure monitoring STARTED");
  }
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP address: ");
  Serial.println(WiFi.localIP());
  
  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/threshold", handleThreshold);
  server.on("/start", handleStartMonitoring);
  server.on("/stop", handleStopMonitoring);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  pox.update();
  server.handleClient();
  
  if (isMonitoring) {
    // Read sensor data every second
    if (millis() - tsLastReport > 1000) {
      // Simulate pressure reading from MAX30100
      // Using heart rate as base for pressure simulation
      float heartRate = pox.getHeartRate();
      float spO2 = pox.getSpO2();
      
      // Calculate pressure based on sensor readings
      // You can adjust this formula based on your needs
      currentPressure = (heartRate * 0.15) + (spO2 * 0.1);
      
      Serial.print("Monitoring - Heart rate: ");
      Serial.print(heartRate);
      Serial.print("bpm / SpO2: ");
      Serial.print(spO2);
      Serial.print("% / Pressure: ");
      Serial.println(currentPressure);
      
      // Control LEDs based on pressure
      if (currentPressure > pressureThreshold) {
        // Pressure high - blink red LED
        digitalWrite(RED_LED, !digitalRead(RED_LED));
        digitalWrite(GREEN_LED, LOW);
        Serial.println("ALERT: Pressure too high!");
      } else {
        // Pressure normal - green LED glows steadily (live monitoring)
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH); // Continuous glow during monitoring
        Serial.println("Pressure normal - Monitoring...");
      }
      
      tsLastReport = millis();
    }
  } else {
    // Monitoring stopped - turn off both LEDs
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
  }
}

// Web server handlers
void handleRoot() {
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<title>MAX30100 Pressure Monitor</title>";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<style>";
  html += "body { font-family: Arial; text-align: center; margin: 40px; }";
  html += ".card { background: #f0f0f0; padding: 20px; margin: 10px; border-radius: 10px; }";
  html += ".alert { background: #ffcccc; color: red; font-weight: bold; }";
  html += ".normal { background: #ccffcc; color: green; }";
  html += ".monitoring { background: #ccccff; color: blue; }";
  html += ".stopped { background: #cccccc; color: gray; }";
  html += "button { padding: 10px 20px; margin: 5px; border: none; border-radius: 5px; cursor: pointer; }";
  html += ".start-btn { background: #4CAF50; color: white; }";
  html += ".stop-btn { background: #f44336; color: white; }";
  html += "</style>";
  html += "<script>";
  html += "function updateData() {";
  html += "  fetch('/data')";
  html += "    .then(response => response.json())";
  html += "    .then(data => {";
  html += "      document.getElementById('pressure').innerText = data.pressure.toFixed(2);";
  html += "      document.getElementById('heartRate').innerText = data.heartRate.toFixed(1);";
  html += "      document.getElementById('spO2').innerText = data.spO2.toFixed(1);";
  html += "      document.getElementById('threshold').innerText = data.threshold.toFixed(2);";
  html += "      ";
  html += "      const statusDiv = document.getElementById('status');";
  html += "      if (!data.isMonitoring) {";
  html += "        statusDiv.className = 'card stopped';";
  html += "        statusDiv.innerHTML = '⏸️ Monitoring Stopped';";
  html += "      } else if (data.pressure > data.threshold) {";
  html += "        statusDiv.className = 'card alert';";
  html += "        statusDiv.innerHTML = '🚨 ALERT: Pressure Too High!';";
  html += "      } else {";
  html += "        statusDiv.className = 'card normal';";
  html += "        statusDiv.innerHTML = '✅ Pressure Normal - Live Monitoring';";
  html += "      }";
  html += "      ";
  html += "      // Update LED status";
  html += "      const ledStatus = document.getElementById('ledStatus');";
  html += "      if (!data.isMonitoring) {";
  html += "        ledStatus.innerHTML = '🔴 Red LED: OFF<br>🟢 Green LED: OFF';";
  html += "      } else if (data.pressure > data.threshold) {";
  html += "        ledStatus.innerHTML = '🔴 Red LED: BLINKING<br>🟢 Green LED: OFF';";
  html += "      } else {";
  html += "        ledStatus.innerHTML = '🔴 Red LED: OFF<br>🟢 Green LED: GLOWING';";
  html += "      }";
  html += "    });";
  html += "}";
  html += "";
  html += "function setThreshold() {";
  html += "  const newThreshold = document.getElementById('newThreshold').value;";
  html += "  fetch('/threshold?value=' + newThreshold)";
  html += "    .then(response => response.text())";
  html += "    .then(data => {";
  html += "      alert('Threshold updated to: ' + newThreshold);";
  html += "      updateData();";
  html += "    });";
  html += "}";
  html += "";
  html += "function startMonitoring() {";
  html += "  fetch('/start')";
  html += "    .then(response => response.text())";
  html += "    .then(data => {";
  html += "      alert('Monitoring Started!');";
  html += "      updateData();";
  html += "    });";
  html += "}";
  html += "";
  html += "function stopMonitoring() {";
  html += "  fetch('/stop')";
  html += "    .then(response => response.text())";
  html += "    .then(data => {";
  html += "      alert('Monitoring Stopped!');";
  html += "      updateData();";
  html += "    });";
  html += "}";
  html += "";
  html += "setInterval(updateData, 2000);";
  html += "window.onload = updateData;";
  html += "</script>";
  html += "</head>";
  html += "<body>";
  html += "<h1>MAX30100 Pressure Monitor</h1>";
  html += "";
  html += "<div id=\"status\" class=\"card\">Loading...</div>";
  html += "";
  html += "<div class=\"card\">";
  html += "  <h3>Sensor Readings</h3>";
  html += "  <p>Pressure: <span id=\"pressure\">--</span> units</p>";
  html += "  <p>Heart Rate: <span id=\"heartRate\">--</span> bpm</p>";
  html += "  <p>SpO2: <span id=\"spO2\">--</span> %</p>";
  html += "  <div id=\"ledStatus\">🔴 Red LED: --<br>🟢 Green LED: --</div>";
  html += "</div>";
  html += "";
  html += "<div class=\"card\">";
  html += "  <h3>Control Panel</h3>";
  html += "  <button class=\"start-btn\" onclick=\"startMonitoring()\">Start Monitoring</button>";
  html += "  <button class=\"stop-btn\" onclick=\"stopMonitoring()\">Stop Monitoring</button>";
  html += "</div>";
  html += "";
  html += "<div class=\"card\">";
  html += "  <h3>Settings</h3>";
  html += "  <p>Current Threshold: <span id=\"threshold\">--</span> units</p>";
  html += "  <input type=\"number\" id=\"newThreshold\" step=\"0.1\" placeholder=\"New threshold\">";
  html += "  <button onclick=\"setThreshold()\">Set Threshold</button>";
  html += "</div>";
  html += "</body>";
  html += "</html>";
  
  server.send(200, "text/html", html);
}

void handleData() {
  StaticJsonDocument<300> doc;
  doc["pressure"] = currentPressure;
  doc["heartRate"] = pox.getHeartRate();
  doc["spO2"] = pox.getSpO2();
  doc["threshold"] = pressureThreshold;
  doc["isMonitoring"] = isMonitoring;
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleThreshold() {
  if (server.hasArg("value")) {
    pressureThreshold = server.arg("value").toFloat();
    server.send(200, "text/plain", "Threshold updated to: " + String(pressureThreshold));
  } else {
    server.send(400, "text/plain", "Missing value parameter");
  }
}

void handleStartMonitoring() {
  isMonitoring = true;
  Serial.println("Monitoring STARTED via web");
  server.send(200, "text/plain", "Monitoring started - Green LED will glow during normal pressure");
}

void handleStopMonitoring() {
  isMonitoring = false;
  Serial.println("Monitoring STOPPED via web");
  server.send(200, "text/plain", "Monitoring stopped - All LEDs turned off");
}