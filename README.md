# Air Watch 360

**Air Watch 360** is an IoT-based environmental monitoring device designed for heat-producing industries to monitor atmospheric conditions in real-time. It tracks temperature, humidity, and harmful gas levels (using MQ-2 and MQ-135 gas sensors), alerting personnel if the gas levels exceed hazardous thresholds with an audible buzzer. 

This device helps industries maintain safe working environments by monitoring potential hazards.

---

## Features

- **Temperature and Humidity Monitoring**: Uses a DHT11 sensor to measure real-time temperature and humidity.
- **Gas Monitoring**: Monitors harmful gas concentrations using the MQ-2 and MQ-135 gas sensors.
- **Hazard Alert System**: Activates a buzzer if gas levels exceed defined thresholds to warn personnel of unsafe conditions.
- **Wi-Fi Connectivity**: Connects to Wi-Fi to allow real-time monitoring over a web interface.
- **Real-Time Data Update**: Updates the data on the web interface every 0.5 seconds.

## Components

- ESP32 microcontroller
- DHT11 temperature and humidity sensor
- MQ-2 gas sensor
- MQ-135 gas sensor
- Buzzer

---

## Pin Configuration

| Component   | ESP32 GPIO Pin |
|-------------|----------------|
| DHT11       | GPIO 4         |
| MQ-2 Sensor | GPIO 34 (Analog)|
| MQ-135 Sensor | GPIO 35 (Analog)|
| Buzzer      | GPIO 5         |

## Hazard Thresholds

- **MQ-2 Hazardous Level**: 400
- **MQ-135 Hazardous Level**: 2500

---

## Setup Instructions

1. **Hardware Setup**:
   - Connect the DHT11, MQ-2, MQ-135, and the buzzer as per the pin configuration table above.
   
2. **Software Setup**:
   - Clone the repository and open the project in the Arduino IDE.
   - Install required libraries (WiFi, DHT).
   - Update the Wi-Fi credentials in the code:
     ```cpp
     const char* ssid = "Your_SSID";
     const char* password = "Your_PASSWORD";
     ```
   - Upload the code to the ESP32.

3. **Web Interface**:
   - After uploading, the ESP32 connects to Wi-Fi, and the IP address will be printed in the Serial Monitor.
   - Open a browser and navigate to `http://<ESP32_IP_ADDRESS>` to access the real-time monitoring interface.

---

## Usage

- **Real-Time Monitoring**: The web interface displays temperature, humidity, and gas levels. 
- **Buzzer Alert**: If gas levels exceed the thresholds, the buzzer sounds to alert nearby personnel.
- **Data Update**: Data refreshes every 0.5 seconds on the web interface, providing up-to-date environmental conditions.

## Web Interface Overview

- **Temperature**: Displays the current temperature in Celsius.
- **Humidity**: Displays the current humidity percentage.
- **MQ-2 Gas Sensor**: Displays the real-time reading from the MQ-2 gas sensor.
- **MQ-135 Gas Sensor**: Displays the real-time reading from the MQ-135 gas sensor.

---

## Contributor

- [Devakrishnan Gopal](https://github.com/gdevakrishnan)

## License

This project is licensed under the MIT License.

---

## Troubleshooting

- **Wi-Fi Connection Issues**: Ensure correct SSID and password are provided and check ESP32 proximity to the router.
- **Buzzer Not Activating**: Verify the gas sensor readings and compare them with the hazardous thresholds.
- **Sensor Readings Not Displaying**: Check connections and ensure sensors are correctly connected to the designated pins.

--- 

## Future Enhancements

- Adding more types of sensors for a broader range of hazardous gases.
- Storing historical data on a cloud server for long-term analysis.
- SMS or email alerts for critical gas level detection.

--- 

Enjoy monitoring with Air Watch 360!
