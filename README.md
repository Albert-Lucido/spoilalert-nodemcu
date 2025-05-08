# Vegetable Spoilage Monitoring System

This project consists of a NodeMCU-based sensor system that monitors environmental conditions affecting vegetable spoilage and sends the data to a MongoDB database through a Node.js server.

## Hardware Requirements

### Components
1. NodeMCU ESP8266
2. DHT11 Temperature and Humidity Sensor
3. MQ-4 Gas Sensor (for CO2 and Ethylene detection)
4. Jumper wires
5. Breadboard
6. USB cable (Micro USB)
7. Power supply (5V)

### Pin Connections
- DHT11:
  - VCC → 3.3V
  - DATA → D4 (GPIO4)
  - GND → GND
- MQ-4:
  - VCC → 3.3V
  - AOUT → A0
  - GND → GND

## Software Setup

### 1. Driver Installation
1. Download CH340 driver from [sparks.gogo.co.nz](https://sparks.gogo.co.nz/ch340.html)
2. Install the driver:
   - For Windows: Run the installer from the unzipped folder
   - For Mac: Follow the PDF instructions in the zip file (different for macOS versions)
   - For Linux: Usually built into the kernel, no installation needed
3. Connect NodeMCU to computer
    - If NodeMCU is not detected or has connection errors in Arduino IDE:
     1. Run CH34x_Install_Windows_v3_4.EXE
     2. Click "UNINSTALL"
     3. Click "INSTALL"
     4. reconnect nodemcu to port
4. Verify installation in Device Manager (should appear as COM port)

### 2. Arduino IDE Setup
1. Download and install [Arduino IDE](https://www.arduino.cc/en/software)
2. Open Arduino IDE
3. Go to File → Preferences
4. Add this URL to Additional Boards Manager URLs:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
5. Go to Tools → Board → Boards Manager
6. Search for "esp8266" and install "ESP8266 by ESP8266 Community"

### 3. Required Libraries
Install these libraries through Arduino IDE (Tools → Manage Libraries):
- ESP8266WiFi (by ESP8266 Community)
- ESP8266HTTPClient (by ESP8266 Community)
- WiFiManager (by tzapu)
- DHT sensor library (by Adafruit)

### 4. Server Setup
1. Install [Node.js](https://nodejs.org/) (LTS version)
2. Install [MongoDB](https://www.mongodb.com/try/download/community)
3. Clone this repository
4. Navigate to project directory
5. Install dependencies:
   ```bash
   npm install
   ```
6. Start the server:
   ```bash
   node server.js
   ```

### 5. Arduino Code Setup
1. Open `nodemcu_spoilage_monitor.ino` in Arduino IDE
2. Select correct board:
   - Tools → Board → ESP8266 Boards → NodeMCU 1.0 (ESP-12E Module)
3. Select correct port:
   - Tools → Port → (your COM port)
4. Upload the code

## First-Time Setup
1. After uploading the code, the NodeMCU will create a WiFi access point named "NodeMCU-Setup"
2. Connect to this WiFi network
3. A configuration portal will open automatically
4. Enter your WiFi credentials
5. Enter the server URL (e.g., http://your-server-ip:3000/api/data)
6. Save the configuration

## Operation
- The system will automatically:
  - Read temperature and humidity from DHT11
  - Read gas levels from MQ-4
  - Determine vegetable type based on sensor readings
  - Send data to the server every 3 seconds
  - Store data in MongoDB database

## Troubleshooting
1. If NodeMCU doesn't connect to WiFi:
   - Press and hold the reset button for 5 seconds
   - The configuration portal will open again
2. If sensor readings are incorrect:
   - Check all connections
   - Verify power supply
   - Check sensor placement
3. If data isn't being sent to server:
   - Verify server is running
   - Check WiFi connection
   - Verify server URL in configuration
4. If Arduino IDE cannot detect NodeMCU:
   - Reinstall CH340 driver:
     1. Run CH34x_Install_Windows_v3_4.EXE
     2. Click "UNINSTALL"
     3. Click "INSTALL"
     4. Restart your computer
   - Check USB cable (try a different cable)
   - Try a different USB port
   - Verify the correct COM port is selected in Arduino IDE

## Security Notes
- Change the default MongoDB credentials in server.js
- Use HTTPS in production
- Implement proper authentication for the API endpoints
