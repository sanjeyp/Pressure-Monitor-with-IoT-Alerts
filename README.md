markdown
# IoT Pressure Monitor System with MAX30100 & ESP8266
https://satower9thgen.blogspot.com/2026/03/iot-pressure-monitor-with-max30100.html
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform: ESP8266](https://img.shields.io/badge/Platform-ESP8266-blue.svg)](https://www.espressif.com/en/products/socs/esp8266)
[![Arduino Compatible](https://img.shields.io/badge/Arduino-Compatible-green.svg)](https://www.arduino.cc/)

A real-time IoT-based pressure monitoring system using MAX30100 sensor and ESP8266 with visual LED alerts and web interface for remote monitoring.

![Dashboard Screenshot](images/dashboard.png)

## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Circuit Diagram](#circuit-diagram)
- [Wiring Connections](#wiring-connections)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [LED Behavior](#led-behavior)
- [Web Interface](#web-interface)
- [API Endpoints](#api-endpoints)
- [Code Structure](#code-structure)
- [Process Flow](#process-flow)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## 🎯 Overview

This project implements a real-time pressure monitoring system using MAX30100 sensor with ESP8266 WiFi module. The system provides:
- Continuous pressure monitoring
- Visual LED alerts (Red LED for alerts, Green LED for normal monitoring)
- Web-based dashboard for remote monitoring
- Adjustable pressure threshold
- Real-time heart rate and SpO2 display

## ✨ Features

### LED Indicators
- 🟢 **Green LED**: Glows continuously during normal pressure monitoring
- 🔴 **Red LED**: Blinks rapidly when pressure exceeds threshold
- ⚫ **Both OFF**: When monitoring is stopped

### IoT Capabilities
- 🌐 Real-time web dashboard
- 📱 Remote monitoring from any device
- ⚙️ Adjustable pressure threshold via web interface
- 🎮 Start/Stop monitoring controls
- 📊 JSON API for data integration

### Sensor Readings
- 💓 Heart Rate (BPM)
- 💨 SpO2 (Blood Oxygen Level)
- 📈 Calculated Pressure Value

## 🔧 Hardware Requirements

| Component | Quantity | Description |
|-----------|----------|-------------|
| ESP8266 NodeMCU | 1 | Main microcontroller |
| MAX30100 Sensor | 1 | Heart rate & SpO2 sensor |
| Red LED | 1 | Alert indicator |
| Green LED | 1 | Monitoring status indicator |
| 220Ω Resistors | 2 | Current limiting for LEDs |
| Breadboard | 1 | For prototyping |
| Jumper Wires | Several | Connections |
| USB Cable | 1 | Power and programming |

## 🔌 Circuit Diagram
┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│ MAX30100 │ │ ESP8266 │ │ LEDs │
│ │ │ (NodeMCU) │ │ │
│ VCC ────────┼────┼─── 3.3V │ │ │
│ GND ────────┼────┼─── GND │ │ │
│ SCL ────────┼────┼─── D1 (GPIO5) │ │ │
│ SDA ────────┼────┼─── D2 (GPIO4) │ │ │
│ │ │ │ │ Red LED ───┼─── D5 (GPIO14)
└─────────────┘ │ │ │ Green LED ──┼─── D6 (GPIO12)
└─────────────┘ └─────────────┘

text

## 📡 Wiring Connections

### MAX30100 to ESP8266
| MAX30100 Pin | ESP8266 Pin | GPIO |
|--------------|-------------|------|
| VCC | 3.3V | - |
| GND | GND | - |
| SCL | D1 | GPIO5 |
| SDA | D2 | GPIO4 |

### LED Connections
| Component | Pin | Connection |
|-----------|-----|------------|
| Red LED | Anode (+) | D5 (GPIO14) via 220Ω resistor |
| Red LED | Cathode (-) | GND |
| Green LED | Anode (+) | D6 (GPIO12) via 220Ω resistor |
| Green LED | Cathode (-) | GND |

## 💻 Software Requirements

### Arduino Libraries
Install these libraries via Arduino Library Manager:
- `MAX30100_PulseOximeter` by OXullo Intersecans
- `ESP8266WiFi` (built-in with ESP8266 board package)
- `ESP8266WebServer` (built-in)
- `ArduinoJson` by Benoit Blanchon

### Board Manager URL
Add this to Arduino IDE Preferences:
http://arduino.esp8266.com/stable/package_esp8266com_index.json

text

## 🚀 Installation

### 1. Clone the Repository
```bash
git clone https://github.com/yourusername/iot-pressure-monitor.git
cd iot-pressure-monitor
2. Configure WiFi Credentials
Open the Arduino sketch and modify:

cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
3. Upload Code
Select board: Tools → Board → NodeMCU 1.0 (ESP-12E Module)

Select port: Tools → Port → [Your COM Port]

Click Upload button

4. Access Web Interface
Open Serial Monitor (115200 baud)

Note the IP address displayed

Open browser and navigate to http://[ESP_IP_ADDRESS]

💡 LED Behavior
State	Green LED	Red LED	Description
Monitoring Normal	🟢 ON (Continuous)	🔴 OFF	System active, pressure normal
Alert - High Pressure	🔴 OFF	🔴 BLINKING	Pressure exceeds threshold
Monitoring Stopped	⚫ OFF	⚫ OFF	System paused
Sensor Error	🟡 BLINKING	🟡 BLINKING	Sensor initialization failed
🌐 Web Interface
The web interface provides:

Real-time pressure readings

Heart rate and SpO2 display

Current threshold value

Start/Stop monitoring buttons

Visual status indicator

LED status display

Threshold adjustment control

Access the Interface
text
http://[ESP_IP_ADDRESS]
📡 API Endpoints
GET /data
Returns JSON with current sensor data:

json
{
  "pressure": 20.27,
  "heartRate": 75.5,
  "spO2": 98.0,
  "threshold": 25.00,
  "isMonitoring": true
}
GET /threshold?value=[value]
Updates pressure threshold:

text
Response: "Threshold updated to: 25.00"
GET /start
Starts pressure monitoring:

text
Response: "Monitoring started"
GET /stop
Stops pressure monitoring:

text
Response: "Monitoring stopped"
📁 Code Structure
text
iot-pressure-monitor/
├── iot_pressure_monitor.ino    # Main Arduino sketch
├── README.md                    # This file
├── LICENSE                      # MIT License
├── images/
│   ├── dashboard.png           # Dashboard screenshot
│   ├── circuit.png             # Circuit diagram
│   └── wiring.png              # Wiring diagram
└── docs/
    ├── setup_guide.md          # Detailed setup instructions
    └── api_documentation.md    # API reference
🔄 Process Flow













🛠️ Troubleshooting
Sensor Not Detected
Check I2C connections (SCL/SDA)

Verify 3.3V power supply

Check for library compatibility

WiFi Connection Issues
Verify SSID and password

Check network availability

Ensure ESP8266 is within range

LEDs Not Working
Check resistor values (220Ω recommended)

Verify LED polarity

Test with simple blink sketch

Web Interface Not Accessible
Check IP address in Serial Monitor

Ensure same network connection

Disable firewall temporarily

🤝 Contributing
Contributions are welcome! Please follow these steps:

Fork the repository

Create feature branch (git checkout -b feature/AmazingFeature)

Commit changes (git commit -m 'Add AmazingFeature')

Push to branch (git push origin feature/AmazingFeature)

Open Pull Request

📄 License
This project is licensed under the MIT License - see the LICENSE file for details.

🙏 Acknowledgments
MAX30100 library by OXullo Intersecans

ESP8266 community for excellent documentation

Arduino community for inspiration

📧 Contact
Your Name - @yourtwitter - email@example.com

Project Link: https://github.com/yourusername/iot-pressure-monitor

⭐ Star this repository if you found it helpful!

Made with ❤️ for IoT and Health Monitoring

text

## Additional Files to Include in Your Repository:

### 1. LICENSE File
Create a `LICENSE` file with MIT License text:

```txt
MIT License

Copyright (c) 2024 [Your Name]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
2. .gitignore File
txt
# Arduino
*.ino.preproc
*.ino.cpp
*.ino.elf
*.ino.hex
*.ino.eep
*.ino.with_bootloader.bin
*.ino.with_bootloader.hex

# IDE
.vscode/
.idea/
*.swp
*.swo
*~

# OS
.DS_Store
Thumbs.db

# Build
build/
tmp/
3. Create Images Folder Structure
text
images/
├── dashboard.png    # Add your screenshot
├── circuit.png      # Add circuit diagram
└── wiring.png       # Add wiring diagram
How to Push to GitHub:
bash
# Initialize git repository
git init

# Add all files
git add .

# Commit changes
git commit -m "Initial commit: IoT Pressure Monitor System"

# Add remote repository
git remote add origin https://github.com/yourusername/iot-pressure-monitor.git

# Push to GitHub
git push -u origin main
