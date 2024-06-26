

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#ESP8266 CODE IN ARDUINO IDE:
 
const char* ssid = "SSN"; // Your WiFi SSID
const char* password = "Ssn1!Som2@Sase3#"; // Your WiFi password
 
ESP8266WebServer server(80);
 
int irPin = 2;  // IR sensor input pin
int ledPin = 15; // LED output pin
int value;      // Variable to store the sensor value
String statusMessage = "vacant"; // Initial status message
unsigned long previousUpdateTime = 0;
const unsigned long updateInterval = 5000; // Update every 5 seconds 
 
void setup() {
  Serial.begin(115200);
  pinMode(irPin, INPUT); // Initialize IR sensor pin as an input
  pinMode(ledPin, OUTPUT); // Initialize LED pin as an output
 
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
 
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop() {
  value = digitalRead(irPin); // Read value on pin 2
 
  if (value == 1) {
    digitalWrite(ledPin, LOW); // Turn off the LED (green when vacant)
    
    Serial.println("vacant");
    delay(5000);
    statusMessage = "vacant";
  } else {
    digitalWrite(ledPin, HIGH); // Turn on the LED (red when occupied)
    
    Serial.println("occupied");
    delay(5000);
    statusMessage = "occupied";
  }
 
  server.handleClient();
 
  unsigned long currentMillis = millis();
  if (currentMillis - previousUpdateTime >= updateInterval) {
    previousUpdateTime = currentMillis;
    Serial.println(statusMessage); // Print the current status to the serial monitor
  }
}
 
void handleRoot() {
  value = digitalRead(irPin); // Read value from IR sensor
 
  if (value == HIGH) { // Assuming HIGH means occupied
    digitalWrite(ledPin, HIGH); // LED ON
    statusMessage = "occupied";
  } else {
    digitalWrite(ledPin, LOW); // LED OFF
    statusMessage = "vacant";
  }
 
  server.send(200, "text/plain", statusMessage); // Send the status to the web server
}
