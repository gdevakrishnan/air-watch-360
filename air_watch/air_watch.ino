#include <WiFi.h>
#include <DHT.h>

// Wi-Fi credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Define the pin and DHT sensor type
#define DHTPIN 4            // DHT sensor connected to GPIO4
#define DHTTYPE DHT11       // DHT11 sensor type
#define BUZZER_PIN 5  // Connect the buzzer to GPIO5

// Define the analog pins for the gas sensors
#define MQ2_SENSOR_PIN 34   // MQ-2 gas sensor connected to GPIO34 (analog input)
#define MQ135_SENSOR_PIN 35 // MQ-135 gas sensor connected to GPIO35 (analog input)

#define MQ2_HAZARD_THRESHOLD 400     // Example threshold for MQ-2
#define MQ135_HAZARD_THRESHOLD 2500  // Example threshold for MQ-135

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Initialize Wi-Fi server on port 80
WiFiServer server(80);

void setup() {
    Serial.begin(115200);    // Start serial communication
    dht.begin();             // Initialize the DHT sensor
    WiFi.begin(ssid, password);

    // Initialize buzzer pin as output
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);  // Start with the buzzer off
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();
}

void loop() {
    // Check if a client has connected
    WiFiClient client = server.available();
    
    // Handle client requests
    if (client) {
        // Wait until the client sends some data
        while (!client.available()) {
            delay(1);
        }

        String request = client.readStringUntil('\r');
        client.flush();

        // If the request is for the root page
        if (request.indexOf("GET / ") >= 0) {
          // Serve the HTML page with the JavaScript for real-time updates
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head><title>air-watch-360</title>");
          client.println("<style>");
          client.println("body { font-family: Arial, sans-serif; background-color: #f4f4f4; margin: 0; padding: 20px; }");
          client.println("h2 { color: #333; }");
          client.println("p { font-size: 1.2em; color: #555; }");
          client.println(".sensor-data { background: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); margin-top: 20px; }");
          client.println("strong { color: #007BFF; }");
          client.println(".value { font-weight: bold; font-size: 1.5em; }");
          client.println("footer { margin-top: 20px; font-size: 0.9em; color: #777; }");
          client.println("</style>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h2>Air Watch <strong>360</strong></h2>");
          client.println("<div class='sensor-data'>");
          client.println("<p><strong>Temperature:</strong> <span id='temperature' class='value'></span> &deg;C</p>");
          client.println("<p><strong>Humidity:</strong> <span id='humidity' class='value'></span> %</p>");
          client.println("<p><strong>MQ-2 Gas Sensor:</strong> <span id='mq2' class='value'></span></p>");
          client.println("<p><strong>MQ-135 Gas Sensor:</strong> <span id='mq135' class='value'></span></p>");
          client.println("</div>");
          client.println("<footer><strong><h3>Hazardous levels</h3></strong>MQ-2 sensor is <strong>400</strong><br>MQ-135 sensor is <strong>2500</strong><br><br>Data updated every 0.5 seconds</footer>");
          client.println("<script>");
          client.println("function fetchData() {");
          client.println("  fetch('/data').then(response => response.json()).then(data => {");
          client.println("    document.getElementById('temperature').innerText = data.temperature;");
          client.println("    document.getElementById('humidity').innerText = data.humidity;");
          client.println("    document.getElementById('mq2').innerText = data.mq2;");
          client.println("    document.getElementById('mq135').innerText = data.mq135;");
          client.println("  });");
          client.println("  setTimeout(fetchData, 500);"); // Fetch data every 2 seconds
          client.println("}");
          client.println("fetchData();"); // Initial call to fetch data
          client.println("</script>");
          client.println("</body></html>");
        }


        // If the request is for sensor data
        else if (request.indexOf("GET /data") >= 0) {
            // Read temperature and humidity from DHT11
            float humidity = dht.readHumidity();
            float temperature = dht.readTemperature();

            // Read values from MQ-2 and MQ-135 gas sensors
            int mq2Value = analogRead(MQ2_SENSOR_PIN);
            int mq135Value = analogRead(MQ135_SENSOR_PIN);

            // Check if DHT sensor readings failed
            if (isnan(humidity) || isnan(temperature)) {
                Serial.println("Failed to read from DHT sensor!");
                return;
            }

            // Activate the buzzer if the gas levels exceed hazardous thresholds
            if (mq2Value > MQ2_HAZARD_THRESHOLD || mq135Value > MQ135_HAZARD_THRESHOLD) {
                digitalWrite(BUZZER_PIN, HIGH);  // Turn buzzer on
                delay(1200);
                digitalWrite(BUZZER_PIN, LOW);   // Turn buzzer off
            } else {
                digitalWrite(BUZZER_PIN, LOW);   // Turn buzzer off
            }

            // Create JSON response
            String jsonResponse = "{\"temperature\": " + String(temperature) +
                                  ", \"humidity\": " + String(humidity) +
                                  ", \"mq2\": " + String(mq2Value) +
                                  ", \"mq135\": " + String(mq135Value) + "}";

            // Send the JSON response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: application/json");
            client.println("Connection: close");
            client.println();
            client.println(jsonResponse);
        }

        client.stop();
        Serial.println("Client disconnected.");
    }
}
